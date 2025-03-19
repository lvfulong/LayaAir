#include <stdint.h>
#include <unistd.h>

#include "plugin_render.h"
#include "../../napi/plugin_manager.h"
#include "../modules/TouchesNapi.h"
#include "../../napi/NAPIFun.cpp"

#include <assert.h>
#include "utils/Log.h"
#include "native_window/external_window.h"
#include "native_buffer/native_buffer.h"


using namespace laya;

#ifdef __cplusplus
extern "C" {
#endif

PluginRender* PluginRender::instance_ = nullptr;
OH_NativeXComponent_Callback PluginRender::callback_;
uint64_t PluginRender::animationInterval_ = 1;//16;  默认跑满，现在限制FPS存在问题：设置60，结果30  
uint64_t PluginRender::lastTime = 0;


void OnSurfaceCreatedCB(OH_NativeXComponent* component, void* window)
{
    LOGD("OnSurfaceCreatedCB");
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_CREATED, component, window);
}

void OnSurfaceChangedCB(OH_NativeXComponent* component, void* window)
{
    LOGD("OnSurfaceChangedCB");
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_CHANGED, component, window);
}

void OnSurfaceDestroyedCB(OH_NativeXComponent* component, void* window)
{
    LOGD("OnSurfaceDestroyedCB");
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_DESTROY, component, window);
}
void OnSurfaceHideCB(OH_NativeXComponent* component, void* window) {
    LOGD("OnSurfaceHideCB");
    
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_HIDE,component, window);
}

void OnSurfaceShowCB(OH_NativeXComponent* component, void* window) {
    LOGD("OnSurfaceShowCB");
    
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_SHOW,component, window);
}

void DispatchTouchEventCB(OH_NativeXComponent* component, void* window)
{
    OH_NativeXComponent_TouchEvent* touchEvent = new(std::nothrow) OH_NativeXComponent_TouchEvent();
    if (!touchEvent) {
        LOGE("DispatchTouchEventCB::touchEvent alloc failed");
        return;
    }
    int32_t ret = OH_NativeXComponent_GetTouchEvent(component, window, touchEvent);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_TOUCH_EVENT, component, window, touchEvent);
    } else {
        delete touchEvent;
    }
}

PluginRender::PluginRender() : component_(nullptr)
{
    auto renderCallback = PluginRender::GetNXComponentCallback();
    renderCallback->OnSurfaceCreated = OnSurfaceCreatedCB;
    renderCallback->OnSurfaceChanged = OnSurfaceChangedCB;
    renderCallback->OnSurfaceDestroyed = OnSurfaceDestroyedCB;
    renderCallback->DispatchTouchEvent = DispatchTouchEventCB;
}

PluginRender* PluginRender::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new PluginRender();
    }
    return instance_;
}

OH_NativeXComponent_Callback* PluginRender::GetNXComponentCallback()
{
    return &PluginRender::callback_;
}

// static
void PluginRender::onMessageCallback(const uv_async_t* /* req */) {
    void* window = nullptr;
    WorkerMessageData msgData;
    PluginRender* render = PluginRender::GetInstance();

    while (true) {
        //loop until all msg dispatch
        if (!render->dequeue(reinterpret_cast<WorkerMessageData*>(&msgData))) {
            // Queue has no data
            break;
        }

        if ((msgData.type >= MessageType::WM_XCOMPONENT_SURFACE_CREATED) && (msgData.type <= MessageType::WM_XCOMPONENT_SURFACE_DESTROY)) {
            OH_NativeXComponent* nativexcomponet = reinterpret_cast<OH_NativeXComponent*>(msgData.data);
            // CC_ASSERT(nativexcomponet != nullptr);

            if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_CREATED) {
                render->OnSurfaceCreated(nativexcomponet, msgData.window);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_TOUCH_EVENT) {
                render->DispatchTouchEvent(nativexcomponet, msgData.window, msgData.touchEvent);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_CHANGED) {
                render->OnSurfaceChanged(nativexcomponet, msgData.window);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_HIDE) {
                render->OnSurfaceHide();
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_SHOW) {
                render->OnSurfaceShow(msgData.window);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_DESTROY) {
                render->OnSurfaceDestroyed(nativexcomponet, msgData.window);
            } else {
                // CC_ASSERT(false);
            }
            continue;
        }

        if (msgData.type == MessageType::WM_APP_SHOW) {
            render->OnShowNative();
        } else if (msgData.type == MessageType::WM_APP_HIDE) {
            render->OnHideNative();
        } else if (msgData.type == MessageType::WM_APP_DESTROY) {
            render->OnDestroyNative();
        }
        if(msgData.type == MessageType::WM_VSYNC) {
            // render->runTask();
        }
    }
}

static uint64_t getCurrentMillSecond() {
    struct timeval stCurrentTime;

    gettimeofday(&stCurrentTime,NULL);
    return stCurrentTime.tv_sec * 1000 + stCurrentTime.tv_usec / 1000; //millseconds
}

// static
void PluginRender::timerCb(uv_timer_t* handle)
{
    laya::JCConch::s_pConch->update();
}

void PluginRender::SetNativeXComponent(OH_NativeXComponent* component)
{
    component_ = component;
    OH_NativeXComponent_RegisterCallback(component_, &PluginRender::callback_);
    OH_NativeXComponent_RegisterSurfaceHideCallback(component_, OnSurfaceHideCB);
    OH_NativeXComponent_RegisterSurfaceShowCallback(component_, OnSurfaceShowCB);
}

void PluginRender::workerInit(napi_env env, uv_loop_t* loop) {
    LOGD("PluginRender::workerInit");
    workerLoop_ = loop;
    if (workerLoop_) {
        laya::JCConch::s_pConch.reset(new laya::JCConch());
        uv_async_init(workerLoop_, &messageSignal_, reinterpret_cast<uv_async_cb>(PluginRender::onMessageCallback));
        if (!messageQueue_.empty()) {
            triggerMessageSignal(); // trigger the signal to handle the pending message
        }
    }
}


void PluginRender::sendMsgToWorker(const MessageType& type, OH_NativeXComponent* component, void* window) {
    WorkerMessageData data{type, static_cast<void*>(component), window};
    enqueue(data);
}

void PluginRender::sendMsgToWorker(const MessageType& type, OH_NativeXComponent* component, void* window, OH_NativeXComponent_TouchEvent* touchEvent) {
    WorkerMessageData data{type, static_cast<void*>(component), window, touchEvent};
    enqueue(data);
}

void PluginRender::enqueue(const WorkerMessageData& msg) {
    messageQueue_.enqueue(msg);
    triggerMessageSignal();
}

bool PluginRender::dequeue(WorkerMessageData* msg) {
    return messageQueue_.dequeue(msg);
}

void PluginRender::triggerMessageSignal() {
    if(workerLoop_ != nullptr) {
        // It is possible that when the message is sent, the worker thread has not yet started.
        uv_async_send(&messageSignal_);
    }
}

void PluginRender::run() {
    LOGD("PluginRender::run");
    if (workerLoop_) {
        uv_timer_init(workerLoop_, &timerHandle_);
        timerInited_ = true;
    }
}

void PluginRender::changeFPS(uint64_t animationIntervalMs) {
    LOGD("PluginRender::changeFPS, animationInterval from %lu to %lu", animationInterval_, animationIntervalMs);
    if (timerInited_ && animationIntervalMs != animationInterval_) {
        uv_timer_set_repeat(&timerHandle_, animationIntervalMs);
    }
    animationInterval_ = animationIntervalMs;
}

void PluginRender::OnSurfaceCreated(OH_NativeXComponent* component, void* window)
{
    LOGD("PluginRender::OnSurfaceCreated");
    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        int32_t code = SET_USAGE;
        OHNativeWindow *oHNativeWindow = static_cast<OHNativeWindow *>(window);
        int32_t ret = OH_NativeWindow_NativeWindowHandleOpt(oHNativeWindow, code, NATIVEBUFFER_USAGE_MEM_DMA);

        laya::BackendOptions options;
        laya::JCConch::s_pConchRender->createBackend(options);
        laya::JCConch::s_pConchRender->createScreenSurface(window);
	    laya::JCConch::s_pConch->onAppStart();

        laya::JCConch::s_pConchRender->onScreenSurfaceResize(width_, height_);
    }
}

void PluginRender::OnSurfaceChanged(OH_NativeXComponent* component, void* window)
{
    LOGD("PluginRender::OnSurfaceChanged");
    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        laya::JCConch::s_pConchRender->onScreenSurfaceResize(width_, height_);
    }
}

void PluginRender::OnSurfaceDestroyed(OH_NativeXComponent* component, void* window)
{
    LOGD("PluginRender::OnSurfaceDestroyed");
}
void PluginRender::OnSurfaceHide()
{
    LOGD("PluginRender::OnSurfaceHide");
    laya::JCConch::s_pConchRender->destroyScreenSurface();
}

void PluginRender::OnSurfaceShow(void* window)
{
    LOGD("PluginRender::OnSurfaceShow");
    laya::JCConch::s_pConchRender->createScreenSurface(window);
}
void PluginRender::DispatchTouchEvent(OH_NativeXComponent* component, void* window, OH_NativeXComponent_TouchEvent* touchEvent)
{
    intptr_t ids[touchEvent->numPoints];
    float xs[touchEvent->numPoints];
    float ys[touchEvent->numPoints];
    for (int i = 0; i < touchEvent->numPoints; i++) {
        ids[i] = touchEvent->touchPoints[i].id;
        xs[i] = touchEvent->touchPoints[i].x;
        ys[i] = touchEvent->touchPoints[i].y;
        LOGD("Touch Info : x = %f, y = %f", xs[i], ys[i]);
    }
    switch (touchEvent -> type) {
        case OH_NATIVEXCOMPONENT_DOWN:
            nativeHandleTouchDown(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGD("Touch Info : OH_NATIVEXCOMPONENT_DOWN");
            break;
        case OH_NATIVEXCOMPONENT_UP:
            nativeHandleTouchUp(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGD("Touch Info : OH_NATIVEXCOMPONENT_UP");
            break;
        case OH_NATIVEXCOMPONENT_MOVE:
            nativeHandleTouchMove(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGD("Touch Info : OH_NATIVEXCOMPONENT_MOVE");
            break;
        case OH_NATIVEXCOMPONENT_CANCEL:
            nativeHandleTouchCancel(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGD("Touch Info : OH_NATIVEXCOMPONENT_CANCEL");
            break;
        case OH_NATIVEXCOMPONENT_UNKNOWN:
            LOGD("Touch Info : OH_NATIVEXCOMPONENT_UNKNOWN");
            break;
        default:
            LOGD("Touch Info : default");
            break;
    }
    delete touchEvent;
}

void PluginRender::OnCreateNative(napi_env env, uv_loop_t* loop) {
    LOGD("PluginRender::OnCreateNative");
}

void PluginRender::OnShowNative() {
    LOGD("PluginRender::OnShowNative");
    if (timerInited_) {
        uv_timer_start(&timerHandle_, &PluginRender::timerCb, 0, animationInterval_);
    }
    laya::JCConch::s_pConch->onAppResume();
}

void PluginRender::OnHideNative() {
    LOGD("PluginRender::OnHideNative");

    laya::JCConch::s_pConch->onAppPause();

    if (timerInited_) {
        uv_timer_stop(&timerHandle_);
    }
}

void PluginRender::OnDestroyNative() {
    LOGD("PluginRender::OnDestoryNative");
    if (timerInited_) {
        uv_timer_stop(&timerHandle_);
    }
    JCAudioManager::GetInstance()->stopMp3();
	laya::JCConch::s_pConch->onAppDestroy();
	laya::JCConch::s_pConch.reset();
}

napi_value PluginRender::Export(napi_env env, napi_value exports)
{
    LOGD("PluginRender::Export");
    // Register JS API
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("changeShape", PluginRender::NapiChangeShape),
        DECLARE_NAPI_FUNCTION("drawTriangle", PluginRender::NapiDrawTriangle),
        DECLARE_NAPI_FUNCTION("changeColor", PluginRender::NapiChangeColor),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    return exports;
}

napi_value PluginRender::NapiChangeShape(napi_env env, napi_callback_info info)
{
    LOGD("PluginRender::NapiChangeShape");
    PluginRender* instance = PluginRender::GetInstance();
    if (instance) {
        //instance->eglCore_->Update();
    }
    return nullptr;
}

napi_value PluginRender::NapiDrawTriangle(napi_env env, napi_callback_info info)
{
    LOGD("NapiDrawTriangle");
    return nullptr;
}

napi_value PluginRender::NapiChangeColor(napi_env env, napi_callback_info info)
{
    LOGD("NapiChangeColor");
    return nullptr;
}

#ifdef __cplusplus
}
#endif
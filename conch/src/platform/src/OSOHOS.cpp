#include "OSOHOS.h"
#include <JCConch.h>
#include <aki/jsbind.h>
#include <utils/Log.h>
namespace laya
{

OSOHOS::~OSOHOS()
{
}
float OSOHOS::getTotalMem()
{
    return 0; // todo
}
int OSOHOS::getAvalidMem()
{
    if (auto getAvalidMem = aki::JSBind::GetJSFunction("DeviceUtils.getAvalidMem"))
    {
        return getAvalidMem->Invoke<int>();
    }
    return 0;
}
int OSOHOS::getMemoryUsageInByte()
{
    return 0; // todo
}
void OSOHOS::exit()
{
    if (auto exit = aki::JSBind::GetJSFunction("ApplicationManager.exit"))
    {
        exit->Invoke<void>();
    }
}
int OSOHOS::getNetworkType()
{
    if (auto getNetworkType = aki::JSBind::GetJSFunction("NetworkUtils.getNetworkType"))
    {
        networkType = getNetworkType->Invoke<int>();
    }
    return networkType;
}
void OSOHOS::setScreenWakeLock(bool bWakeLock)
{
    if (auto setKeepScreenOn = aki::JSBind::GetJSFunction("DeviceUtils.setKeepScreenOn"))
    {
        setKeepScreenOn->Invoke<void>(bWakeLock);
    }
}
void OSOHOS::setSensorAble(bool bSensorAble)
{
    if (bSensorAble)
    {
        if (auto accelerometerEnable = aki::JSBind::GetJSFunction("Accelerometer.enable"))
        {
            accelerometerEnable->Invoke<void>();
        }
        if (auto orientationEnable = aki::JSBind::GetJSFunction("Orientation.enable"))
        {
            orientationEnable->Invoke<void>();
        }
    }
    else
    {
        if (auto accelerometerDisable = aki::JSBind::GetJSFunction("Accelerometer.disable"))
        {
            accelerometerDisable->Invoke<void>();
        }
        if (auto orientationDisable = aki::JSBind::GetJSFunction("Orientation.disable"))
        {
            orientationDisable->Invoke<void>();
        }
    }
}
int OSOHOS::getSafeInsetTop()
{
    return 0;
}
int OSOHOS::getSafeInsetLeft()
{
    return 0;
}
int OSOHOS::getSafeInsetBottom()
{
    return 0;
}
int OSOHOS::getSafeInsetRight()
{
    return 0;
}
JsValue OSOHOS::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    napi_deferred deferred;
    napi_value promise;

    napi_create_promise(context, &deferred, &promise);

    std::function<void(std::string)> cb = [deferred, cbref](std::string message) {
        postToJS([deferred, message, cbref]() {
            if (!cbref.lock())
                return;
            auto isolate = v8::Isolate::GetCurrent();
            auto context = isolate->GetCurrentContext();
            napi_value v = JsValueFromV8LocalValue(Converter<const char *>::ToJs(message));
            napi_resolve_deferred(context, deferred, v);
        });
    };
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleAsyncMessage"))
    {
        post->Invoke<void>(eventName, data, cb);
    }
    return V8LocalValueFromJsValue(promise);
}
std::string OSOHOS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    // handleSyncMessage is called in ohos ui thread
    std::string eventResult;
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleSyncMessage"))
    {
        eventResult = post->Invoke<std::string>(eventName, data);
    }
    return eventResult;
}
} // namespace laya
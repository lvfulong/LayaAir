#include "OSLinux.h"
#ifdef BUILDING_CONCH_FROME_SOURCE
#include "Exports.h"
#else
#include "ExportsShared.h"
#endif
#include <JCConch.h>
#include <future>
#include <utils/Log.h>

extern handleSyncMessageCallback g_handleSyncMessageCb;
extern handleAsyncMessageCallback g_handleAsyncMessageCb;
extern void conchRegisterHandleMessageHandler(const char *eventName, std::function<void(const char *)> cb);

namespace laya
{
int OSLinux::getUsedMem()
{
    return 0; // todo
}
OSLinux::~OSLinux()
{
}
float OSLinux::getTotalMem()
{
    return 0; // todo
}
int OSLinux::getAvalidMem()
{
    return 0; // todo
}
int OSLinux::getMemoryUsageInByte()
{
    return 0; // todo
}
void OSLinux::exit()
{
    // todo
}
int OSLinux::getNetworkType()
{
    return 1; // todo
}
void OSLinux::setScreenWakeLock(bool bWakeLock)
{
    // todo
}
void OSLinux::setSensorAble(bool bSensorAble)
{
    // todo
}
int OSLinux::getSafeInsetTop()
{
    return 0;
}
int OSLinux::getSafeInsetLeft()
{
    return 0;
}
int OSLinux::getSafeInsetBottom()
{
    return 0;
}
int OSLinux::getSafeInsetRight()
{
    return 0;
}
jsvm_value OSLinux::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    //auto isolate = v8::Isolate::GetCurrent();
    //auto context = isolate->GetCurrentContext();

    //napi_deferred deferred;
    //napi_value promise;

    //napi_create_promise(context, &deferred, &promise);
    auto promise = jsbind::Promise::Make();
    std::function<void(std::string)> cb = [promise, cbref](std::string message) {
        postToJS([promise, message, cbref]() {
            if (!cbref.lock())
                return;
            //auto isolate = v8::Isolate::GetCurrent();
            //auto context = isolate->GetCurrentContext();
            //napi_value v = jsvm_valueFromV8LocalValue(MakeJSValue<const char *>(message));
            //napi_resolve_deferred(context, deferred, v);
            promise.resolve(message);
        });
    };
    if (g_handleAsyncMessageCb)
    {
        // handleAsyncMessage is called in platform os ui thread
        postToPlatform([eventName, data]() { g_handleAsyncMessageCb(eventName.c_str(), data.c_str()); });
    }
    //return V8LocalValueFromjsvm_value(promise);
    return promise.getHandle();
}
std::string OSLinux::postSyncMessage(const std::string &eventName, const std::string &data)
{
    // handleSyncMessage is called in platform os ui thread
    std::string eventResult;
    std::promise<std::string> promise;
    if (g_handleSyncMessageCb)
    {
        conchRegisterHandleMessageHandler(eventName.c_str(),
                                          [&promise](const char *message) { promise.set_value(message); });
        postToPlatform([eventName, data]() { g_handleSyncMessageCb(eventName.c_str(), data.c_str()); });
    }
    eventResult = promise.get_future().get();

    return eventResult;
}
void OSLinux::setPreferredFramesPerSecond(uint64_t fps)
{

}
} // namespace laya

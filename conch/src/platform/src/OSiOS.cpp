#include "OSiOS.h"
#include "CToObjectC.h"
#include <JCConch.h>
#include <utils/Log.h>
namespace laya
{

OSiOS::~OSiOS()
{
}
float OSiOS::getTotalMem()
{
    return CToObjectCGetTotalMem();
}
int OSiOS::getAvalidMem()
{
    return CToObjectCGetAvalidMem();
}
int OSiOS::getMemoryUsageInByte()
{
    return CToObjectCGetMemoryUsageInByte();
}
void OSiOS::exit()
{
}
int OSiOS::getNetworkType()
{
    return CToObjectCGetNetworkType();
}
void OSiOS::setScreenWakeLock(bool bWakeLock)
{
    CToObjectCSetScreenWakeLock(bWakeLock);
}
void OSiOS::setSensorAble(bool bSensorAble)
{
    CToObjectCSetSensorAble(bSensorAble);
}
JsValue OSiOS::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
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
    CToObjectCPostAsyncMessage(eventName, data, cb);
    return V8LocalValueFromJsValue(promise);
}
std::string OSiOS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    // handleSyncMessage is called in platform os ui thread
    std::string eventResult = CToObjectCPostSyncMessage(eventName, data);
    return eventResult;
}
} // namespace laya
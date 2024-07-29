#include "OSiOS.h"
#include "CToObjectC.h"
#include <JCConch.h>
#include <utils/Log.h>
namespace laya
{

OSiOS::~OSiOS()
{
}
int OSiOS::getUsedMem()
{
    return CToObjectCGetUsedMem();
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
int OSiOS::getSafeInsetTop()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
    return safeInsetTop;
}
int OSiOS::getSafeInsetLeft()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
    return safeInsetLeft;
}
int OSiOS::getSafeInsetBottom()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
    return safeInsetBottom;
}
int OSiOS::GetSafeInsetRight()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
    return safeInsetRight;
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
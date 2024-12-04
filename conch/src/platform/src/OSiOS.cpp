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
int OSiOS::getSafeInsetRight()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
    return safeInsetRight;
}
jsvm_value OSiOS::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    auto promise = jsbind::Promise::Make();
    std::function<void(std::string)> cb = [promise, cbref](std::string message) {
        postToJS([promise, message, cbref]() {
            if (!cbref.lock())
                return;
            promise.resolve(message);
        });
    };
    CToObjectCPostAsyncMessage(eventName, data, cb);
    return promise.getHandle();
}
std::string OSiOS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    // handleSyncMessage is called in platform os ui thread
    std::string eventResult = CToObjectCPostSyncMessage(eventName, data);
    return eventResult;
}
void OSiOS::setPreferredFramesPerSecond(uint64_t fps)
{
    CToObjectCSetPreferredFramesPerSecond(fps);
}
} // namespace laya

#include <platform/OS.h>
#include "CToObjectC.h"
#include <JCConch.h>
#include <utils/Log.h>
#include <utils/JCFileSystem.h>

namespace laya
{
int OS::getUsedMem()
{
    return CToObjectCGetUsedMem();
}
float OS::getTotalMem()
{
    return CToObjectCGetTotalMem();
}
int OS::getAvalidMem()
{
    return CToObjectCGetAvalidMem();
}
int OS::getMemoryUsageInByte()
{
    return CToObjectCGetMemoryUsageInByte();
}
void OS::exit()
{
    abort();
}
int OS::getNetworkType()
{
    return CToObjectCGetNetworkType();
}
void OS::setScreenWakeLock(bool bWakeLock)
{
    CToObjectCSetScreenWakeLock(bWakeLock);
}
void OS::setSensorAble(bool bSensorAble)
{
    CToObjectCSetSensorAble(bSensorAble);
}
int OS::getSafeInsetTop()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
    return safeInsetTop;
}
int OS::getSafeInsetLeft()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
    return safeInsetLeft;
}
int OS::getSafeInsetBottom()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
    return safeInsetBottom;
}
int OS::getSafeInsetRight()
{
    int safeInsetTop = 0;
    int safeInsetLeft = 0;
    int safeInsetBottom = 0;
    int safeInsetRight = 0;
    CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
    return safeInsetRight;
}
jsvm_value OS::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
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
std::string OS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    // handleSyncMessage is called in platform os ui thread
    std::string eventResult = CToObjectCPostSyncMessage(eventName, data);
    return eventResult;
}
void OS::setPreferredFramesPerSecond(uint64_t fps)
{
    CToObjectCSetPreferredFramesPerSecond(fps);
}
std::string OS::getExecutablePath()
{
    return CToObjectCGetExecutablePath();
}
std::string OS::getAssetFullPath(const std::string &assetRelativePath)
{
    static std::string rootAssetsPath = CToObjectCGetRootAssetsPath();
    return rootAssetsPath + "/" + assetRelativePath;
}
std::string OS::getAssetRootPath()
{
    std::string exePath = laya::OS::getExecutablePath();
    std::string assetRootPath = laya::FileSystem::parent_path(exePath);
    return assetRootPath;
}
std::string OS::getPersistentDataPath()
{
    return CToObjectCGetPersistentDataPath();
}
std::string OS::getTemporaryCachePath()
{
    return CToObjectCGetTemporaryCachePath();
}
} // namespace laya

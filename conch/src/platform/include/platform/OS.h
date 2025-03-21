#ifndef __OS_H__
#define __OS_H__
#include <jsbind/JSBind.h>
#include <memory>
#include <string>
namespace laya
{

class OS
{
  public:
    static int getUsedMem();
    static float getTotalMem();
    static int getAvalidMem();
    static int getMemoryUsageInByte();
    static void exit();
    static int getNetworkType();
    static void setScreenWakeLock(bool bWakeLock);
    static void setSensorAble(bool bSensorAble);
    static int getSafeInsetTop();
    static int getSafeInsetLeft();
    static int getSafeInsetBottom();
    static int getSafeInsetRight();
    static jsvm_value postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName,
                                        const std::string &data);
    static std::string postSyncMessage(const std::string &eventName, const std::string &data);
    static void setPreferredFramesPerSecond(uint64_t fps);
    static std::string getExecutablePath();
    static std::string getAssetFullPath(const std::string &assetRelativePath);
    static std::string getAssetRootPath();
    static std::string getPersistentDataPath();
    static std::string getTemporaryCachePath();
    #if defined(OS_ANDROID) || defined(OS_OHOS)
    static void setPersistentDataPath(const std::string &path);
    static void setTemporaryCachePath(const std::string &path);
    #endif
};
} // namespace laya
#endif

#ifndef __OSLinux_H__
#define __OSLinux_H__
#include <platform/OS.h>
namespace laya
{
class OSLinux : public OS
{
  public:
    ~OSLinux();
    int getUsedMem() override;
    float getTotalMem() override;
    int getAvalidMem() override;
    int getMemoryUsageInByte() override;
    void exit() override;
    int getNetworkType() override;
    void setScreenWakeLock(bool bWakeLock) override;
    void setSensorAble(bool bSensorAble) override;
    int getSafeInsetTop() override;
    int getSafeInsetLeft() override;
    int getSafeInsetBottom() override;
    int getSafeInsetRight() override;
    jsvm_value postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data) override;
    std::string postSyncMessage(const std::string &eventName, const std::string &data) override;
    void setPreferredFramesPerSecond(uint64_t fps) override;
};
} // namespace laya
#endif
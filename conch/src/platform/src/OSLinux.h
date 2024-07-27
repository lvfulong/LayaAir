#ifndef __OSLinux_H__
#define __OSLinux_H__
#include <platform/OS.h>
namespace laya
{
class OSLinux : public OS
{
  public:
    ~OSLinux();
    float getTotalMem() override;
    int getAvalidMem() override;
    int getMemoryUsageInByte() override;
    void exit() override;
    int getNetworkType() override;
    void setScreenWakeLock(bool bWakeLock) override;
    JsValue postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data) override;
    std::string postSyncMessage(const std::string &eventName, const std::string &data) override;
};
} // namespace laya
#endif
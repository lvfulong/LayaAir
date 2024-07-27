#ifndef __OSAndroid_H__
#define __OSAndroid_H__
#include <platform/OS.h>
namespace laya
{
class OSAndroid : public OS
{
  public:
    ~OSAndroid();
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
    JsValue postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data) override;
    std::string postSyncMessage(const std::string &eventName, const std::string &data) override;
};
} // namespace laya
#endif
#ifndef __OSWin_H__
#define __OSWin_H__
#include <platform/OS.h>
namespace laya
{
class OSWin : public OS
{
  public:
    ~OSWin();
    float getTotalMem() override;
    int getAvalidMem() override;
    int getMemoryUsageInByte() override;
    void exit() override;
    int getNetworkType() override;
    JsValue postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data) override;
    std::string postSyncMessage(const std::string &eventName, const std::string &data) override;
};
} // namespace laya
#endif

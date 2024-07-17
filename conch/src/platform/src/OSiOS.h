#ifndef __OSiOS_H__
#define __OSiOS_H__
#include <platform/OS.h>
namespace laya
{
class OSiOS : public OS
{
  public:
    ~OSiOS();
    float getTotalMem() override;
    int getAvalidMem() override;
    int getMemoryUsageInByte() override;
    void exit() override;
    JsValue postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data) override;
    std::string postSyncMessage(const std::string &eventName, const std::string &data) override;
};
} // namespace laya
#endif
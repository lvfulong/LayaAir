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
    std::string postAsyncMessage(const std::string &eventName, const std::string &data) override;
    std::string postSyncMessage(const std::string &eventName, const std::string &data) override;
};
} // namespace laya
#endif
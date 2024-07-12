#ifndef __OS_H__
#define __OS_H__
#include <memory>
#include <string>
namespace laya
{

class OS
{
  public:
    virtual ~OS()
    {
    }
    virtual float getTotalMem() = 0;
    virtual int getAvalidMem() = 0;
    virtual int getMemoryUsageInByte() = 0;
    virtual void exit() = 0;
    virtual std::string postAsyncMessage(const std::string &eventName, const std::string &data) = 0;
    virtual std::string postSyncMessage(const std::string &eventName, const std::string &data) = 0;
};

std::unique_ptr<OS> createOS();
} // namespace laya
#endif

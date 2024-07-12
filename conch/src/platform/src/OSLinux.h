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
};
} // namespace laya
#endif
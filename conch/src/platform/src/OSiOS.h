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
};
} // namespace laya
#endif
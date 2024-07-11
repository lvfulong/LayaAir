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
};
} // namespace laya
#endif
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
    int getMemoryUsageInByte() override;
};
} // namespace laya
#endif

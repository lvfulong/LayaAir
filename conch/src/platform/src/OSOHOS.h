#ifndef __OSOHOS_H__
#define __OSOHOS_H__
#include <platform/OS.h>
namespace laya
{
class OSOHOS : public OS
{
  public:
    ~OSOHOS();
    float getTotalMem() override;
};
} // namespace laya
#endif
#ifndef __BaseCameraProperty_H__
#define __BaseCameraProperty_H__

#include "render/LayaGL.h"

namespace laya
{
class BaseCameraProperty
{
  public:
    static uint32_t VIEWMATRIX;
    static uint32_t PROJECTMATRIX;
    static uint32_t VIEWPROJECTMATRIX;
    static void __init__();
};

} // namespace laya
#endif
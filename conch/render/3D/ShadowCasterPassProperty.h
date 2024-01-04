#ifndef __ShadowCasterPassProperty_H__
#define __ShadowCasterPassProperty_H__

#include "render/driver/gles/LayaGL.h"

namespace laya
{
class ShadowCasterPassProperty
{
  public:
    static uint32_t SHADOW_BIAS;
    static uint32_t SHADOW_LIGHT_DIRECTION;
    static void __init__();
};

} // namespace laya
#endif
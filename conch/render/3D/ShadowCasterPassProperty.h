#ifndef __ShadowCasterPassProperty_H__
#define __ShadowCasterPassProperty_H__

#include "render/LayaGL.h"

namespace laya
{
class ShadowCasterPassProperty
{
  public:
    static uint32_t SHADOW_BIAS;
    static uint32_t SHADOW_LIGHT_DIRECTION;
    static uint32_t SHADOW_PARAMS;
    static uint32_t SHADOW_MAP_SIZE;
    static uint32_t SHADOW_SPOTMAP_SIZE;
    static uint32_t SHADOW_SPOTMATRICES;
    static uint32_t SHADOW_MATRICES;
    static uint32_t SHADOW_SPLIT_SPHERES;
    static void __init__();
};

} // namespace laya
#endif
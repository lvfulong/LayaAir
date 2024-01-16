#ifndef __DepthPassProperty_H__
#define __DepthPassProperty_H__

#include <core/math/Vector4.h>
#include <render/3D/temp/ShaderDefine.h>

namespace laya
{

class DepthPassProperty
{
  public:
    static ShaderDefine *DEPTHPASS;
    static uint32_t DEFINE_SHADOW_BIAS;
    static Vector4 SHADOW_BIAS;
    static uint32_t DEPTHZBUFFERPARAMS;
    static void __init__();
};
} // namespace laya
#endif
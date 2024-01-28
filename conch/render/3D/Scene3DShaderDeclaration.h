#ifndef __Scene3DShaderDeclaration_H__
#define __Scene3DShaderDeclaration_H__

#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTShaderDefine.h>

namespace laya
{

class Scene3DShaderDeclaration
{
  public:
    /**@internal */
    // static SHADERDEFINE_FOG : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_FOG_LINEAR : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_FOG_EXP : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_FOG_EXP2 : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_DIRECTIONLIGHT : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_POINTLIGHT : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_SPOTLIGHT : ShaderDefine;
    /**@internal */
    // static SHADERDEFINE_SHADOW : ShaderDefine;

    static ShaderDefine *SHADERDEFINE_SHADOW_CASCADE;
    static ShaderDefine *SHADERDEFINE_SHADOW_SOFT_SHADOW_LOW;
    static ShaderDefine *SHADERDEFINE_SHADOW_SOFT_SHADOW_HIGH;
    /**@internal */
    // static SHADERDEFINE_SHADOW_SPOT : ShaderDefine;
    /**@internal */
    static ShaderDefine *SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_LOW;
    static ShaderDefine *SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_HIGH;
    static void __init__();
};
} // namespace laya
#endif
#ifndef __Sprite3DRenderDeclarationProperty_H__
#define __Sprite3DRenderDeclarationProperty_H__

#include "render/driver/gles/LayaGL.h"

namespace laya
{
class Sprite3DRenderDeclarationProperty
{
  public:
    static ShaderDefine* SHADERDEFINE_SPECCUBE_BOX_PROJECTION;
    static ShaderDefine* SHADERDEFINE_GI_LEGACYIBL;
    static ShaderDefine* SHADERDEFINE_GI_IBL;
    static ShaderDefine* SHADERDEFINE_IBL_RGBD;
    static ShaderDefine* SHADERDEFINE_VOLUMETRICGI;
    static void __init__();
};

} // namespace laya
#endif
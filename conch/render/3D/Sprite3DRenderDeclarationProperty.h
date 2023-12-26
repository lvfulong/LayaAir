#ifndef __Sprite3DRenderDeclarationProperty_H__
#define __Sprite3DRenderDeclarationProperty_H__

#include "render/driver/gles/LayaGL.h"

namespace laya
{
class Sprite3DRenderDeclarationProperty
{
  public:
    static uint32_t SHADERDEFINE_SPECCUBE_BOX_PROJECTION;
    static uint32_t SHADERDEFINE_GI_LEGACYIBL;
    static uint32_t SHADERDEFINE_GI_IBL;
    static uint32_t SHADERDEFINE_IBL_RGBD;
    static uint32_t SHADERDEFINE_VOLUMETRICGI;
    static void __init__();
};

} // namespace laya
#endif
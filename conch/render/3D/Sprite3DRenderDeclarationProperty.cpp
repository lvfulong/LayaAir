#include "Sprite3DRenderDeclarationProperty.h"
namespace laya
{
ShaderDefine* Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION = nullptr;
ShaderDefine* Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL = nullptr;
ShaderDefine* Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL = nullptr;
ShaderDefine* Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD = nullptr;
ShaderDefine* Sprite3DRenderDeclarationProperty::SHADERDEFINE_VOLUMETRICGI = nullptr;

void Sprite3DRenderDeclarationProperty::__init__()
{
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION = LayaGL::m_pWebglEngine->getDefineByName("SPECCUBE_BOX_PROJECTION");
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL = LayaGL::m_pWebglEngine->getDefineByName("GI_LEGACYIBL");
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL = LayaGL::m_pWebglEngine->getDefineByName("GI_IBL");
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD = LayaGL::m_pWebglEngine->getDefineByName("IBL_RGBD");
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_VOLUMETRICGI = LayaGL::m_pWebglEngine->getDefineByName("VOLUMETRICGI");
}
}; // namespace laya
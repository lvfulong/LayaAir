#include "Sprite3DRenderDeclarationProperty.h"
namespace laya
{
uint32_t Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION = 0;
uint32_t Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL = 0;
uint32_t Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL = 0;
uint32_t Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD = 0;
uint32_t Sprite3DRenderDeclarationProperty::SHADERDEFINE_VOLUMETRICGI = 0;

void Sprite3DRenderDeclarationProperty::__init__()
{
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_SPECCUBE_BOX_PROJECTION = LayaGL::m_pWebglEngine->getDefineByName("SPECCUBE_BOX_PROJECTION");
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_LEGACYIBL = LayaGL::m_pWebglEngine->getDefineByName("GI_LEGACYIBL");
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_GI_IBL = LayaGL::m_pWebglEngine->getDefineByName("GI_IBL");
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_IBL_RGBD = LayaGL::m_pWebglEngine->getDefineByName("IBL_RGBD");
    Sprite3DRenderDeclarationProperty::SHADERDEFINE_VOLUMETRICGI = LayaGL::m_pWebglEngine->getDefineByName("VOLUMETRICGI");
}
}; // namespace laya
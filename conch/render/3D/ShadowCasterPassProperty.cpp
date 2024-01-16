#include "ShadowCasterPassProperty.h"
namespace laya
{
uint32_t ShadowCasterPassProperty::SHADOW_BIAS = 0;
uint32_t ShadowCasterPassProperty::SHADOW_LIGHT_DIRECTION = 0;
uint32_t ShadowCasterPassProperty::SHADOW_PARAMS = 0;
uint32_t ShadowCasterPassProperty::SHADOW_MAP_SIZE = 0;
uint32_t ShadowCasterPassProperty::SHADOW_SPOTMATRICES = 0; 
uint32_t ShadowCasterPassProperty::SHADOW_SPOTMAP_SIZE = 0; 
void ShadowCasterPassProperty::__init__()
{
    ShadowCasterPassProperty::SHADOW_BIAS = LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowBias");
    ShadowCasterPassProperty::SHADOW_LIGHT_DIRECTION =
        LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowLightDirection");

    ShadowCasterPassProperty::SHADOW_PARAMS =
        LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowParams");

    ShadowCasterPassProperty::SHADOW_MAP_SIZE =
        LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowMapSize");

    ShadowCasterPassProperty::SHADOW_SPOTMATRICES =
        LayaGL::m_pWebglEngine->propertyNameToID("u_SpotViewProjectMatrix");
    ShadowCasterPassProperty::SHADOW_SPOTMAP_SIZE =
        LayaGL::m_pWebglEngine->propertyNameToID("u_SpotShadowMapSize");
}
}; // namespace laya
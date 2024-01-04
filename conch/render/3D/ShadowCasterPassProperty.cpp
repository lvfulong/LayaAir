#include "ShadowCasterPassProperty.h"
namespace laya
{
uint32_t ShadowCasterPassProperty::SHADOW_BIAS = 0;
uint32_t ShadowCasterPassProperty::SHADOW_LIGHT_DIRECTION = 0;
void ShadowCasterPassProperty::__init__()
{
    ShadowCasterPassProperty::SHADOW_BIAS = LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowBias");
    ShadowCasterPassProperty::SHADOW_LIGHT_DIRECTION =
        LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowLightDirection");
}
}; // namespace laya
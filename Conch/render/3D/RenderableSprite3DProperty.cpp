#include "RenderableSprite3DProperty.h"
namespace laya
{
uint32_t RenderableSprite3DProperty::SAHDERDEFINE_LIGHTMAP = 0;
uint32_t RenderableSprite3DProperty::LIGHTMAP_DIRECTION = 0;
uint32_t RenderableSprite3DProperty::SHADERDEFINE_LIGHTMAP_DIRECTIONAL = 0;
uint32_t RenderableSprite3DProperty::REFLECTIONCUBE_PROBEPOSITION = 0;
uint32_t RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMAX = 0;
uint32_t RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMIN = 0;
uint32_t RenderableSprite3DProperty::AMBIENTCOLOR = 0;
uint32_t RenderableSprite3DProperty::IBLTEX = 0;
uint32_t RenderableSprite3DProperty::IBLROUGHNESSLEVEL = 0;
uint32_t RenderableSprite3DProperty::AMBIENTSH = 0;
uint32_t RenderableSprite3DProperty::REFLECTIONTEXTURE = 0;
uint32_t RenderableSprite3DProperty::REFLECTIONCUBE_HDR_PARAMS = 0;
uint32_t RenderableSprite3DProperty::AMBIENTSHAR = 0;
uint32_t RenderableSprite3DProperty::AMBIENTSHAG = 0;
uint32_t RenderableSprite3DProperty::AMBIENTSHAB = 0;
uint32_t RenderableSprite3DProperty::AMBIENTSHBR = 0;
uint32_t RenderableSprite3DProperty::AMBIENTSHBG = 0;
uint32_t RenderableSprite3DProperty::AMBIENTSHBB = 0;
uint32_t RenderableSprite3DProperty::AMBIENTSHC = 0;
uint32_t RenderableSprite3DProperty::AMBIENTINTENSITY = 0;
uint32_t RenderableSprite3DProperty::REFLECTIONINTENSITY = 0;
uint32_t RenderableSprite3DProperty::VOLUMETRICGI_PROBECOUNTS = 0;
uint32_t RenderableSprite3DProperty::VOLUMETRICGI_PROBESTEPS = 0;
uint32_t RenderableSprite3DProperty::VOLUMETRICGI_PROBESTARTPOS = 0;
uint32_t RenderableSprite3DProperty::VOLUMETRICGI_PROBEPARAMS = 0;
uint32_t RenderableSprite3DProperty::VOLUMETRICGI_IRRADIANCE = 0;
uint32_t RenderableSprite3DProperty::VOLUMETRICGI_DISTANCE = 0;
void RenderableSprite3DProperty::__init__()
{
    RenderableSprite3DProperty::SAHDERDEFINE_LIGHTMAP = LayaGL::m_pWebglEngine->propertyNameToID("LIGHTMAP");
    RenderableSprite3DProperty::LIGHTMAP_DIRECTION = LayaGL::m_pWebglEngine->propertyNameToID("u_LightMapDirection");
    RenderableSprite3DProperty::SHADERDEFINE_LIGHTMAP_DIRECTIONAL = LayaGL::m_pWebglEngine->propertyNameToID("LIGHTMAP_DIRECTIONAL");
    RenderableSprite3DProperty::REFLECTIONCUBE_PROBEPOSITION = LayaGL::m_pWebglEngine->propertyNameToID("u_SpecCubeProbePosition");
    RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMAX = LayaGL::m_pWebglEngine->propertyNameToID("u_SpecCubeBoxMax");
    RenderableSprite3DProperty::REFLECTIONCUBE_PROBEBOXMIN = LayaGL::m_pWebglEngine->propertyNameToID("u_SpecCubeBoxMin");
    RenderableSprite3DProperty::AMBIENTCOLOR = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientColor");
    RenderableSprite3DProperty::IBLTEX = LayaGL::m_pWebglEngine->propertyNameToID("u_IBLTex");
    RenderableSprite3DProperty::IBLROUGHNESSLEVEL = LayaGL::m_pWebglEngine->propertyNameToID("u_IBLRoughnessLevel");
    RenderableSprite3DProperty::AMBIENTSH = LayaGL::m_pWebglEngine->propertyNameToID("u_IblSH");
    RenderableSprite3DProperty::REFLECTIONTEXTURE = LayaGL::m_pWebglEngine->propertyNameToID("u_ReflectTexture");
    RenderableSprite3DProperty::REFLECTIONCUBE_HDR_PARAMS = LayaGL::m_pWebglEngine->propertyNameToID("u_ReflectCubeHDRParams");
    RenderableSprite3DProperty::AMBIENTSHAR = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHAr");
    RenderableSprite3DProperty::AMBIENTSHAG = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHAg");
    RenderableSprite3DProperty::AMBIENTSHAB = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHAb");
    RenderableSprite3DProperty::AMBIENTSHBR = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHBr");
    RenderableSprite3DProperty::AMBIENTSHBG = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHBg");
    RenderableSprite3DProperty::AMBIENTSHBB = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHBb");
    RenderableSprite3DProperty::AMBIENTSHC = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientSHC");
    RenderableSprite3DProperty::AMBIENTINTENSITY = LayaGL::m_pWebglEngine->propertyNameToID("u_AmbientIntensity");
    RenderableSprite3DProperty::REFLECTIONINTENSITY = LayaGL::m_pWebglEngine->propertyNameToID("u_ReflectionIntensity");
    RenderableSprite3DProperty::VOLUMETRICGI_PROBECOUNTS = LayaGL::m_pWebglEngine->propertyNameToID("u_VolumetricGI.probeCounts");
    RenderableSprite3DProperty::VOLUMETRICGI_PROBESTEPS = LayaGL::m_pWebglEngine->propertyNameToID("u_VolumetricGI.probeStep");
    RenderableSprite3DProperty::VOLUMETRICGI_PROBESTARTPOS = LayaGL::m_pWebglEngine->propertyNameToID("u_VolumetricGI.probeStartPosition");
    RenderableSprite3DProperty::VOLUMETRICGI_PROBEPARAMS = LayaGL::m_pWebglEngine->propertyNameToID("u_VolumetricGI.probeParams");
    RenderableSprite3DProperty::VOLUMETRICGI_IRRADIANCE = LayaGL::m_pWebglEngine->propertyNameToID("u_ProbeIrradiance");
    RenderableSprite3DProperty::VOLUMETRICGI_DISTANCE = LayaGL::m_pWebglEngine->propertyNameToID("u_ProbeDistance");
}
}; // namespace laya
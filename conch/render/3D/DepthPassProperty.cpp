#include "DepthPassProperty.h"
#include "render/LayaGL.h"
namespace laya
{
ShaderDefine *DepthPassProperty::DEPTHPASS = nullptr;
uint32_t DepthPassProperty::DEFINE_SHADOW_BIAS = 0;
uint32_t DepthPassProperty::DEPTHZBUFFERPARAMS = 0;
Vector4 DepthPassProperty::SHADOW_BIAS;
void DepthPassProperty::__init__()
{
    DepthPassProperty::DEPTHPASS = LayaGL::m_pWebglEngine->getDefineByName("DEPTHPASS");
    DepthPassProperty::DEFINE_SHADOW_BIAS =
        LayaGL::m_pWebglEngine->propertyNameToID("u_ShadowBias");
    DepthPassProperty::DEPTHZBUFFERPARAMS =
        LayaGL::m_pWebglEngine->propertyNameToID("u_ZBufferParams");
}
}; // namespace laya
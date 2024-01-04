#include "Scene3DShaderDeclaration.h"
#include "render/driver/gles/LayaGL.h"
namespace laya
{
ShaderDefine* Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_CASCADE = nullptr;
ShaderDefine* Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_LOW = nullptr;
ShaderDefine* Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_HIGH = nullptr;
void Scene3DShaderDeclaration::__init__()
{
    Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_CASCADE = LayaGL::m_pWebglEngine->getDefineByName("SHADOW_CASCADE");
    Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_LOW = LayaGL::m_pWebglEngine->getDefineByName("SHADOW_SOFT_SHADOW_LOW");
    Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_HIGH = LayaGL::m_pWebglEngine->getDefineByName("SHADOW_SOFT_SHADOW_HIGH");
}
}; // namespace laya
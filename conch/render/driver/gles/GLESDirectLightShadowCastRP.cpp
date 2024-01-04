#include "GLESDirectLightShadowCastRP.h"
#include "render/3D/BaseCameraProperty.h"
#include "render/3D/ShadowCasterPassProperty.h"
#include <render/3D/temp/ShaderData.h>

namespace laya
{

// GLESDirectLightShadowCastRP
GLESDirectLightShadowCastRP::GLESDirectLightShadowCastRP()
{
    _cascadesSplitDistance.resize(_maxCascades + 1);
}

GLESDirectLightShadowCastRP::~GLESDirectLightShadowCastRP()
{
}

void GLESDirectLightShadowCastRP::update(RenderContext3D *context)
{
    // 根据cameraInfo的数据和mode还有directlight的数据,确认裁剪数据,确认shaderData的值
}
void GLESDirectLightShadowCastRP::render(RenderContext3D *context, std::vector<BaseRenderNode *> renderNodeList,
                                         uint32_t count)
{
    // for循环  裁剪完 更新渲染数据，再渲染
}

void GLESDirectLightShadowCastRP::applyRenderData(uint32_t sceneShaderData, uint32_t cameraShaderData)
{
    // 将渲染结果 写入scene和camera的shaderData中
}

void GLESDirectLightShadowCastRP::set_lightUp(const Vector3 &value)
{
}
void GLESDirectLightShadowCastRP::set_lightSide(const Vector3 &value)
{
}
void GLESDirectLightShadowCastRP::set_lightForward(const Vector3 &value)
{
}

void GLESDirectLightShadowCastRP::set_shadowCascadeMode(ShadowCascadesMode value)
{
}
void GLESDirectLightShadowCastRP::set_cameraInfo(CameraInfo value)
{
    // TODO
}
void GLESDirectLightShadowCastRP::set_destTarget(uint32_t value)
{
    // TODO
}
void GLESDirectLightShadowCastRP::_setupShadowCasterShaderValues(ShaderData *shaderValues,
                                                                 const ShadowSliceData &shadowSliceData,
                                                                 const Vector3 &LightParam, const Vector4 &shadowBias)
{
    shaderValues->setVector(ShadowCasterPassProperty::SHADOW_BIAS, shadowBias);
    shaderValues->setVector3(ShadowCasterPassProperty::SHADOW_LIGHT_DIRECTION, LightParam);
    auto cameraSV = shadowSliceData.cameraShaderValue; // TODO:should optimization with shader upload.
    cameraSV->setMatrix4x4(BaseCameraProperty::VIEWMATRIX, shadowSliceData.viewMatrix);
    cameraSV->setMatrix4x4(BaseCameraProperty::PROJECTMATRIX, shadowSliceData.projectionMatrix);
    cameraSV->setMatrix4x4(BaseCameraProperty::VIEWPROJECTMATRIX, shadowSliceData.viewProjectMatrix);
    shaderValues->setMatrix4x4(BaseCameraProperty::VIEWPROJECTMATRIX, shadowSliceData.viewProjectMatrix);
}
} // namespace laya
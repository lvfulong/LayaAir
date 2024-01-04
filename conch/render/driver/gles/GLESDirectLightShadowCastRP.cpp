#include "GLESDirectLightShadowCastRP.h"
#include "render/3D/BaseCameraProperty.h"
#include "render/3D/ShadowCasterPassProperty.h"
#include <render/3D/temp/ShaderData.h>
#include <render/3D/ShadowMode.h>
#include "render/3D/Scene3DShaderDeclaration.h"
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
void GLESDirectLightShadowCastRP::_applyRenderData(ShaderData* scene, ShaderData* camera)
{
    const GLESDirectLight& light = this->_light;
    if (light.shadowCascadesMode != ShadowCascadesMode::NoCascades)
        scene->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_CASCADE);
    else
        scene->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_CASCADE);

    switch (light.shadowMode)
    {
    case ShadowMode::Hard:
        scene->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_LOW);
        scene->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_HIGH);
        break;
    case ShadowMode::SoftLow:
        scene->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_LOW);
        scene->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_HIGH);
        break;
    case ShadowMode::SoftHigh:
        scene->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_HIGH);
        scene->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SOFT_SHADOW_LOW);
        break;
    }
    //scene.setTexture(ShadowCasterPass.SHADOW_MAP, this.destTarget); todo
    //scene.setBuffer(ShadowCasterPass.SHADOW_MATRICES, this._shadowMatrices); todo
    scene->setVector(ShadowCasterPassProperty::SHADOW_MAP_SIZE, this->_shadowMapSize);
    scene->setVector(ShadowCasterPassProperty::SHADOW_PARAMS, this->_shadowParams);
    //scene.setBuffer(ShadowCasterPass.SHADOW_SPLIT_SPHERES, this._splitBoundSpheres); todo
}
void GLESDirectLightShadowCastRP::getShadowBias(const Matrix4x4& shadowProjectionMatrix, double shadowResolution, Vector4& out)
{
    double frustumSize;

    // Frustum size is guaranteed to be a cube as we wrap shadow frustum around a sphere
    // elements[0] = 2.0 / (right - left)
    frustumSize = 2.0 / shadowProjectionMatrix.elements[0];


    // depth and normal bias scale is in shadowmap texel size in world space
    double texelSize = frustumSize / shadowResolution;
    double depthBias = -this->_light.shadowDepthBias * texelSize;
    double normalBias = -this->_light.shadowNormalBias * texelSize;

    if (this->_light.shadowMode == ShadowMode::SoftHigh) {
        // TODO: depth and normal bias assume sample is no more than 1 texel away from shadowmap
        // This is not true with PCF. Ideally we need to do either
        // cone base bias (based on distance to center sample)
        // or receiver place bias based on derivatives.
        // For now we scale it by the PCF kernel size (5x5)
        const double kernelRadius = 2.5;
        depthBias *= kernelRadius;
        normalBias *= kernelRadius;
    }
    out.setValue(depthBias, normalBias, 0.0, 0.0);
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
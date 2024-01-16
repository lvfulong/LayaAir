#include "GLESSpotLightShadowRP.h"
#include "render/3D/Scene3DShaderDeclaration.h"
#include "render/3D/ShadowCasterPassProperty.h"
#include "render/3D/BaseCameraProperty.h"
#include <render/3D/temp/ShaderData.h>
#include "GLESRenderContext3D.h"
#include "GLESCullUtil.h"

namespace laya
{
// SpotLightShadowRenderPass
GLESSpotLightShadowRP::GLESSpotLightShadowRP() : _renderQueue(false)
{
}

GLESSpotLightShadowRP::~GLESSpotLightShadowRP()
{
}

void GLESSpotLightShadowRP::update(GLESRenderContext3D* context)
{
    ShadowSpotData& shadowSpotData = this->_shadowSpotData;
    this->_getSpotLightShadowData(shadowSpotData, this->_shadowResolution, this->_shadowParams, this->_shadowSpotMatrices, this->_shadowSpotMapSize);
}

void GLESSpotLightShadowRP::render(GLESRenderContext3D* context, std::vector<GLESBaseRenderNode*>& list, uint32_t count) 
{
    ShaderData* shaderValues = context->sceneData;
    context->pipelineMode = "ShadowCaster";
    // TODOcontext->setRenderTarget(destTarget);

    ShadowSpotData& shadowSpotData = this->_shadowSpotData;
    this->_getShadowBias(shadowSpotData.resolution, this->_shadowBias);
    this->_setupShadowCasterShaderValues(shaderValues, &shadowSpotData, this->_shadowParams, this->_shadowBias);

    // Culling
    GLESCullUtil::cullingSpotShadow(shadowSpotData.cameraCullInfo, list, count, this->_renderQueue, *context);
    context->cameraData = shadowSpotData.cameraShaderValue;
    // TODOCamera::_updateMark++;
    // TODOcontext->cameraUpdateMask = Camera::_updateMark;

    if (_renderQueue._elements.getLength() > 0) {
        Viewport _tempViewport(shadowSpotData.offsetX, shadowSpotData.offsetY, shadowSpotData.resolution, shadowSpotData.resolution);
        Vector4 tempVec4(shadowSpotData.offsetX, shadowSpotData.offsetY, shadowSpotData.resolution, shadowSpotData.resolution);
    }
    else {
        Viewport _tempViewport(shadowSpotData.offsetX, shadowSpotData.offsetY, shadowSpotData.resolution, shadowSpotData.resolution);
        Vector4 tempVec4(shadowSpotData.offsetX, shadowSpotData.offsetY, shadowSpotData.resolution, shadowSpotData.resolution);
    }

    context->setClearData(static_cast<RenderClearFlagBits>(RenderClearFlag::Depth), Color::BLACK, 1.0f, 0);
    _renderQueue.renderQueue(context);
    // TODOthis->_applyCasterPassCommandBuffer(context);
    this->_applyRenderData(context->sceneData, context->cameraData);
}

void GLESSpotLightShadowRP::_getShadowBias(Real shadowResolution, Vector4& out)
{

    Real frustumSize = std::tan(_spotAngle * 0.5 * MathUtils3D::Deg2Rad) * _spotRange;
    Real texelSize = frustumSize / shadowResolution;
    Real depthBias = -_shadowDepthBias * texelSize;
    Real normalBias = -_shadowNormalBias * texelSize;

    if (_shadowMode == ShadowMode::SoftHigh) {
        // TODO: Handle the bias more accurately
        const Real kernelRadius = 2.5;
        depthBias *= kernelRadius;
        normalBias *= kernelRadius;
    }

    out.setValue(depthBias, normalBias, 0.0f, 0.0f);
}
void GLESSpotLightShadowRP::_getSpotLightShadowData(ShadowSpotData& shadowSpotData, Real resolution, Vector4& shadowParams, Matrix4x4& shadowSpotMatrices, Vector4& shadowMapSize) {
    Vector3& out = shadowSpotData.position = _lightPos;
    shadowSpotData.resolution = resolution;
    shadowMapSize.setValue(1.0f / resolution, 1.0f / resolution, static_cast<Real>(resolution), static_cast<Real>(resolution));
    shadowSpotData.offsetX = 0;
    shadowSpotData.offsetY = 0;

    Matrix4x4& spotWorldMatrix = _lightWorldMatrix;
    Matrix4x4& viewMatrix = shadowSpotData.viewMatrix;
    Matrix4x4& projectMatrix = shadowSpotData.projectionMatrix;
    Matrix4x4& viewProjectMatrix = shadowSpotData.viewProjectMatrix;
    BoundFrustum& boundFrustum = shadowSpotData.cameraCullInfo._boundFrustum;

    spotWorldMatrix.invert(viewMatrix);
    Matrix4x4::createPerspective(3.1416 * _spotAngle / 180.0f, 1.0f, 0.1f, _spotRange, projectMatrix);
    shadowParams.y = _shadowStrength;
    Matrix4x4::multiply(projectMatrix, viewMatrix, viewProjectMatrix);
    boundFrustum._matrix = viewProjectMatrix;
    viewProjectMatrix.cloneTo(shadowSpotMatrices);
    shadowSpotData.cameraCullInfo._position = out;
}

void GLESSpotLightShadowRP::_setupShadowCasterShaderValues(ShaderData* shaderValues, ShadowSpotData* shadowSliceData, const Vector4& shadowparams, const Vector4& shadowBias) {
    shaderValues->setVector(ShadowCasterPassProperty::SHADOW_BIAS, shadowBias);
    shaderValues->setVector(ShadowCasterPassProperty::SHADOW_PARAMS, shadowparams);

    // 由于 shadowSliceData 现在是一个指针，我们需要使用 -> 运算符来访问成员
    ShaderData* cameraSV = shadowSliceData->cameraShaderValue; // 如果 cameraShaderValue 是对象的话，用引用来避免拷贝
    cameraSV->setMatrix4x4(BaseCameraProperty::VIEWMATRIX, shadowSliceData->viewMatrix);
    cameraSV->setMatrix4x4(BaseCameraProperty::PROJECTMATRIX, shadowSliceData->projectionMatrix);
    cameraSV->setMatrix4x4(BaseCameraProperty::VIEWPROJECTMATRIX, shadowSliceData->viewProjectMatrix);

    shaderValues->setMatrix4x4(BaseCameraProperty::VIEWPROJECTMATRIX, shadowSliceData->viewProjectMatrix);
}
void GLESSpotLightShadowRP::_applyRenderData(ShaderData* sceneData, ShaderData* cameraData)
{
    const GLESSpotLight& spotLight = this->_light;
    switch (static_cast<uint32_t>(spotLight.shadowMode)) {
    case ShadowMode::Hard:
        sceneData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_HIGH);
        sceneData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_LOW);
        break;
    case ShadowMode::SoftLow:
        sceneData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_LOW);
        sceneData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_HIGH);
        break;
    case ShadowMode::SoftHigh:
        sceneData->addDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_HIGH);
        sceneData->removeDefine(Scene3DShaderDeclaration::SHADERDEFINE_SHADOW_SPOT_SOFT_SHADOW_LOW);
        break;
    }
    //todo sceneData.setTexture(ShadowCasterPass.SHADOW_SPOTMAP, this.destTarget);
    sceneData->setMatrix4x4(ShadowCasterPassProperty::SHADOW_SPOTMATRICES, this->_shadowSpotMatrices);
    sceneData->setVector(ShadowCasterPassProperty::SHADOW_SPOTMAP_SIZE, this->_shadowSpotMapSize);
    sceneData->setVector(ShadowCasterPassProperty::SHADOW_PARAMS, this->_shadowParams);
}

void GLESSpotLightShadowRP::set_position(Vector3 value)
{
}

void GLESSpotLightShadowRP::set_lightforward(Vector3 value)
{
}

void GLESSpotLightShadowRP::set_destTarget(uint32_t value)
{
}

void GLESSpotLightShadowRP::set_offsetx(uint32_t value)
{
}

void GLESSpotLightShadowRP::set_offsety(uint32_t value)
{
}

void GLESSpotLightShadowRP::set_resolution(uint32_t value)
{
}

} // namespace laya
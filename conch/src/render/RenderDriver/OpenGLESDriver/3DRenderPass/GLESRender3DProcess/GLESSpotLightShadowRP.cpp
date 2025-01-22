#include "GLESSpotLightShadowRP.h"
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h"
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESCullUtil.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.h"
namespace laya
{

GLESSpotLightShadowRP::GLESSpotLightShadowRP() : _renderQueue(false)
{
}

GLESSpotLightShadowRP::~GLESSpotLightShadowRP()
{
}
void GLESSpotLightShadowRP::setLight(RTSpotLight* value) 
{ 
    this->light = value;
    this->_shadowResolution = this->light->shadowResolution;
    this->_lightWorldMatrix = this->light->getWorldMatrix(this->_lightWorldMatrix);
    this->_lightPos = this->light->transform->getPosition();
    this->_spotAngle = this->light->spotAngle;
    this->_spotRange = this->light->spotRange;
    this->_shadowStrength = this->light->shadowStrength;
    // this.destTarget && RenderTexture.recoverToPool(this.destTarget);// TODO 优化
    //this.destTarget = ShadowUtils.getTemporaryShadowTexture(this._shadowResolution, this._shadowResolution, ShadowMapFormat.bit16);
}
void GLESSpotLightShadowRP::update(GLESRenderContext3D* context)
{
    ShadowSpotData& shadowSpotData = this->_shadowSpotData;
    this->_getSpotLightShadowData(shadowSpotData, this->_shadowResolution, this->_shadowParams, this->_shadowSpotMatrices, this->_shadowSpotMapSize);
}

void GLESSpotLightShadowRP::render(GLESRenderContext3D* context, std::vector<RTBaseRenderNode*>& list, uint32_t count)
{
    GLESShaderData* originCameraData = context->cameraData;
    GLESShaderData* shaderValues = context->sceneData;
    context->pipelineMode = "ShadowCaster";
    context->setRenderTarget(destTarget, static_cast<uint32_t>(RenderClearFlag::Depth));

    ShadowSpotData& shadowSpotData = this->_shadowSpotData;
    this->_getShadowBias(shadowSpotData.resolution, this->_shadowBias);
    this->_setupShadowCasterShaderValues(shaderValues, &shadowSpotData, this->_shadowParams, this->_shadowBias);

    // Culling
    GLESCullUtil::cullingSpotShadow(shadowSpotData.cameraCullInfo, list, count, this->_renderQueue, context);
    context->cameraData = shadowSpotData.cameraShaderValue;
    context->_cameraUpdateMask++;

        Viewport _tempViewport(shadowSpotData.offsetX, shadowSpotData.offsetY, shadowSpotData.resolution, shadowSpotData.resolution);
        Vector4 tempVec4(shadowSpotData.offsetX, shadowSpotData.offsetY, shadowSpotData.resolution, shadowSpotData.resolution);

    context->setViewport(_tempViewport);
    context->setScissor(tempVec4);

    if (LayaGL::m_pWebglEngine->enableUniformBufferObject) {
        _shadowSpotData.cameraShaderValue->updateUBOBuffer(BaseCameraProperty::UBONAME_CAMERA);
        shaderValues->updateUBOBuffer(Scene3DShaderDeclaration::UBONAME_SHADOW);
    }

    context->setClearData(static_cast<RenderClearFlagBits>(RenderClearFlag::Depth), Color::BLACK, 1.0f, 0);
    _renderQueue.renderQueue(context);

    this->_applyRenderData(context->sceneData, context->cameraData);
    this->_renderQueue._batch.recoverData();
    context->cameraData = originCameraData;
    context->_cameraUpdateMask++;
}

void GLESSpotLightShadowRP::_getShadowBias(float shadowResolution, Vector4& out)
{


    float frustumSize = std::tan(_spotAngle * 0.5 * MathUtils3D::Deg2Rad) * _spotRange;
    float texelSize = frustumSize / shadowResolution;
    float depthBias = -light->shadowDepthBias * texelSize;
    float normalBias = -light->shadowNormalBias * texelSize;

    if (_shadowMode == ShadowMode::SoftHigh) {
        // TODO: Handle the bias more accurately
        const float kernelRadius = 2.5;
        depthBias *= kernelRadius;
        normalBias *= kernelRadius;
    }

    out.setValue(depthBias, normalBias, 0.0f, 0.0f);
}
void GLESSpotLightShadowRP::_getSpotLightShadowData(ShadowSpotData& shadowSpotData, float resolution, Vector4& shadowParams, Matrix4x4& shadowSpotMatrices, Vector4& shadowMapSize) {
    Vector3& out = shadowSpotData.position = _lightPos;
    shadowSpotData.resolution = resolution;
    shadowMapSize.setValue(1.0f / resolution, 1.0f / resolution, static_cast<float>(resolution), static_cast<float>(resolution));
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

void GLESSpotLightShadowRP::_setupShadowCasterShaderValues(GLESShaderData* shaderValues, ShadowSpotData* shadowSliceData, const Vector4& shadowparams, const Vector4& shadowBias) {
    shaderValues->setVector(ShadowCasterPassProperty::SHADOW_BIAS, shadowBias);
    shaderValues->setVector(ShadowCasterPassProperty::SHADOW_PARAMS, shadowparams);

    // 由于 shadowSliceData 现在是一个指针，我们需要使用 -> 运算符来访问成员
    GLESShaderData* cameraSV = shadowSliceData->cameraShaderValue; // 如果 cameraShaderValue 是对象的话，用引用来避免拷贝
    cameraSV->setMatrix4x4(BaseCameraProperty::VIEWMATRIX, shadowSliceData->viewMatrix);
    cameraSV->setMatrix4x4(BaseCameraProperty::PROJECTMATRIX, shadowSliceData->projectionMatrix);
    cameraSV->setMatrix4x4(BaseCameraProperty::VIEWPROJECTMATRIX, shadowSliceData->viewProjectMatrix);

    shaderValues->setMatrix4x4(BaseCameraProperty::VIEWPROJECTMATRIX, shadowSliceData->viewProjectMatrix);
}
void GLESSpotLightShadowRP::_applyRenderData(GLESShaderData* sceneData, GLESShaderData* cameraData)
{
    const RTSpotLight* spotLight = this->light;
    switch (spotLight->shadowMode) {
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
    sceneData->setMatrix4x4(ShadowCasterPassProperty::SHADOW_SPOTMATRICES, this->_shadowSpotMatrices);
    sceneData->setVector(ShadowCasterPassProperty::SHADOW_SPOTMAP_SIZE, this->_shadowSpotMapSize);
    sceneData->setVector(ShadowCasterPassProperty::SHADOW_PARAMS, this->_shadowParams);
}


ShadowSpotData::ShadowSpotData()
{
    cameraShaderValue = new GLESShaderData(new RTDefineDatas());
    if (LayaGL::m_pWebglEngine->enableUniformBufferObject) {
        cameraShaderValue->createUniformBuffer(BaseCameraProperty::UBONAME_CAMERA, GLESCommandUniformMap::createGlobalUniformMap(BaseCameraProperty::UBONAME_CAMERA.c_str()));
    }
}

ShadowSpotData::~ShadowSpotData()
{
    delete cameraShaderValue;
}

} // namespace laya
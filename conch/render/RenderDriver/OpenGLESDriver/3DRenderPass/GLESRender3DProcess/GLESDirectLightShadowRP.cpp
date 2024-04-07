#include "GLESDirectLightShadowRP.h"
#include "Bindings/LayaAir/3D/JSTransform.h"
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESCullUtil.h"
#include <render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTModuleData.h>
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <render/ShadowMode.h>
#include <render/ShadowUtils.h>

namespace laya
{
GLESDirectLightShadowRP::GLESDirectLightShadowRP() : _renderQueue(false)
{
    _cascadesSplitDistance.resize(_maxCascades + 1);
}

GLESDirectLightShadowRP::~GLESDirectLightShadowRP()
{
}
void GLESDirectLightShadowRP::setLight(RTDirectLight *light)
{
    this->_light = light;
    Matrix4x4 lightWorld;
    float *lightWorldE = lightWorld.elements;
    Vector3 &lightUp = this->_lightUp;
    Vector3 &lightSide = this->_lightSide;
    Vector3 &lightForward = this->_lightForward;
    Matrix4x4::createFromQuaternion(this->_light->transform->getRotation(), lightWorld);
    lightSide.setValue(lightWorldE[0], lightWorldE[1], lightWorldE[2]);
    lightUp.setValue(lightWorldE[4], lightWorldE[5], lightWorldE[6]);
    lightForward.setValue(-lightWorldE[8], -lightWorldE[9], -lightWorldE[10]);
    // 设置分辨率
    auto atlasResolution = this->_light->shadowResolution;
    auto cascadesMode = this->shadowCastMode = this->_light->shadowCascadesMode;

    if (cascadesMode == ShadowCascadesMode::NoCascades)
    {
        this->_cascadeCount = 1;
        this->_shadowTileResolution = atlasResolution;
        this->_shadowMapWidth = atlasResolution;
        this->_shadowMapHeight = atlasResolution;
    }
    else
    {
        this->_cascadeCount = cascadesMode == ShadowCascadesMode::TwoCascades ? 2 : 4;
        Real shadowTileResolution =
            ShadowUtils::getMaxTileResolutionInAtlas(atlasResolution, atlasResolution, this->_cascadeCount);
        this->_shadowTileResolution = shadowTileResolution;
        this->_shadowMapWidth = shadowTileResolution * 2;
        this->_shadowMapHeight =
            cascadesMode == ShadowCascadesMode::TwoCascades ? shadowTileResolution : shadowTileResolution * 2;
    }
}

void GLESDirectLightShadowRP::update(GLESRenderContext3D *context)
{
    std::vector<F32> &splitDistance = this->_cascadesSplitDistance;
    std::vector<Plane> &frustumPlanes = this->_frustumPlanes;
    auto cameraNear = this->camera->nearplane;
    auto shadowFar = std::min(this->camera->farplane, this->_light->shadowDistance);
    // var shadowMatrices : Float32Array = this->_shadowMatrices;
    // var boundSpheres : Float32Array = this->_splitBoundSpheres;
    ShadowUtils::getCascadesSplitDistance(this->_light->shadowTwoCascadeSplits, this->_light->_shadowFourCascadeSplits,
                                          cameraNear, shadowFar, this->camera->fieldOfView * MathUtils3D::Deg2Rad,
                                          this->camera->aspectRatio, this->shadowCastMode, splitDistance);
    ShadowUtils::getCameraFrustumPlanes(this->camera->projectViewMatrix, frustumPlanes);
    Vector3 forward;
    this->camera->transform->getForward(forward);
    Vector3::normalize(forward, forward);
    for (int i = 0; i < this->_cascadeCount; i++)
    {
        ShadowSliceData &sliceData = this->_shadowSliceDatas[i];
        sliceData.sphereCenterZ = ShadowUtils::getBoundSphereByFrustum(
            splitDistance[i], splitDistance[i + 1], this->camera->fieldOfView * MathUtils3D::Deg2Rad,
            this->camera->aspectRatio, this->camera->transform->getPosition(), forward, sliceData.splitBoundSphere);
        ShadowUtils::getDirectionLightShadowCullPlanes(&frustumPlanes[0], i, &splitDistance[0], cameraNear,
                                                       this->_lightForward, sliceData);
        ShadowUtils::getDirectionalLightMatrices(this->_lightUp, this->_lightSide, this->_lightForward, i,
                                                 this->_light->shadowNearPlane, this->_shadowTileResolution, sliceData,
                                                 this->_shadowMatrices.data());
        if (this->_cascadeCount > 1)
            ShadowUtils::applySliceTransform(sliceData, this->_shadowMapWidth, this->_shadowMapHeight, i,
                                             this->_shadowMatrices.data());
    }
    ShadowUtils::prepareShadowReceiverShaderValues(this->_light->shadowStrength, this->_shadowMapWidth,
                                                   this->_shadowMapHeight, this->_shadowSliceDatas.data(),
                                                   this->_cascadeCount, this->_shadowMapSize, this->_shadowParams,
                                                   this->_shadowMatrices.data(), this->_splitBoundSpheres.data());
}

void GLESDirectLightShadowRP::render(GLESRenderContext3D *context, std::vector<RTBaseRenderNode *> &list, uint32_t count)

{
    GLESShaderData *shaderValues = context->sceneData;
    context->pipelineMode = "ShadowCaster";
    auto shadowMap = this->destTarget;
    context->setRenderTarget(shadowMap);
    context->setClearData(static_cast<uint32_t>(RenderClearFlag::Depth), Color::BLACK, 1, 0);
    // 需要把shadowmap clear Depth;
    for (int i = 0, n = this->_cascadeCount; i < n; i++)
    {
        ShadowSliceData &sliceData = this->_shadowSliceDatas[i];
        this->getShadowBias(sliceData.projectionMatrix, sliceData.resolution, this->_shadowBias);
        this->_setupShadowCasterShaderValues(shaderValues, sliceData, this->_lightForward, this->_shadowBias);
        ShadowCullInfo &shadowCullInfo = this->_shadowCullInfo;
        shadowCullInfo.position = sliceData.position;
        shadowCullInfo.cullPlanes = sliceData.cullPlanes;
        shadowCullInfo.cullPlaneCount = sliceData.cullPlaneCount;
        shadowCullInfo.cullSphere = sliceData.splitBoundSphere;
        shadowCullInfo.direction = this->_lightForward;
        // cull
        GLESCullUtil::culldirectLightShadow(shadowCullInfo, list, count, this->_renderQueue,
                                            (GLESRenderContext3D *)context);

        context->cameraData = sliceData.cameraShaderValue;
        context->_cameraUpdateMask++;

        auto resolution = sliceData.resolution;
        auto offsetX = sliceData.offsetX;
        auto offsetY = sliceData.offsetY;

        if (this->_renderQueue._elements.getLength() > 0)
        { // if one cascade have anything to render.
            Viewport _tempViewport(offsetX, offsetY, resolution, resolution);
            Vector4 tempVec4(offsetX + 1, offsetY + 1, resolution - 2, resolution - 2);
            context->setViewport(_tempViewport);
            context->setScissor(tempVec4);
        }
        else
        {
            Viewport _tempViewport(offsetX, offsetY, resolution, resolution);
            context->setViewport(_tempViewport);
            Vector4 tempVec4(offsetX, offsetY, resolution, resolution);
            context->setScissor(tempVec4);
        }
        context->setClearData((RenderClearFlagBits)RenderClearFlag::Depth, Color::BLACK, 1, 0);
        this->_renderQueue.renderQueue((GLESRenderContext3D *)context);
        GLESRenderCMD::applyCommandBuffers(context, _shadowCastCMDS);
    }
    this->_applyRenderData(context->sceneData, context->cameraData);
}

void GLESDirectLightShadowRP::_applyRenderData(GLESShaderData *scene, GLESShaderData *camera)
{
    const RTDirectLight &light = *this->_light;
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
    scene->setBuffer(ShadowCasterPassProperty::SHADOW_MATRICES, (uint8_t *)this->_shadowMatrices.data(),
                     _shadowMatrices.size() * sizeof(F32));
    scene->setVector(ShadowCasterPassProperty::SHADOW_MAP_SIZE, this->_shadowMapSize);
    scene->setVector(ShadowCasterPassProperty::SHADOW_PARAMS, this->_shadowParams);
    scene->setBuffer(ShadowCasterPassProperty::SHADOW_SPLIT_SPHERES, (uint8_t *)this->_splitBoundSpheres.data(),
                     this->_splitBoundSpheres.size() * sizeof(F32));
}

void GLESDirectLightShadowRP::getShadowBias(const Matrix4x4 &shadowProjectionMatrix, double shadowResolution,
                                          Vector4 &out)
{
    double frustumSize;

    // Frustum size is guaranteed to be a cube as we wrap shadow frustum around a sphere
    // elements[0] = 2.0 / (right - left)
    frustumSize = 2.0 / shadowProjectionMatrix.elements[0];

    // depth and normal bias scale is in shadowmap texel size in world space
    double texelSize = frustumSize / shadowResolution;
    double depthBias = -this->_light->shadowDepthBias * texelSize;
    double normalBias = -this->_light->shadowNormalBias * texelSize;

    if (this->_light->shadowMode == ShadowMode::SoftHigh)
    {
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
void GLESDirectLightShadowRP::_setupShadowCasterShaderValues(GLESShaderData *shaderValues,
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


void GLESDirectLightShadowRP::clearShadowCasterCommandBuffer() {
    _shadowCastCMDS.clear();
}

void GLESDirectLightShadowRP::addShadowCasterCommandBuffers(const std::vector<GLESRenderCMD*>& cmds) {
    _shadowCastCMDS.push_back(cmds);
}

} // namespace laya
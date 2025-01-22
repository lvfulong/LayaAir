#ifndef __GLESSpotLightShadowRP_H_
#define __GLESSpotLightShadowRP_H_

#include "render/3D/design/Render3DProcess.h"
#include <vector>
#include "render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTSpotLight.h"
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESRenderListQueue.h"
#include "render/ShadowSliceData.h"

namespace laya
{
    class GLESRenderContext3D;
    class RTBaseRenderNode;
    class GLESInternalRT;
    class ShadowSpotData
    {
    public:
        ShadowSpotData();
        ~ShadowSpotData();
        GLESShaderData* cameraShaderValue;
        Vector3 position;
        float offsetX;
        float offsetY;
        float resolution;
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
        Matrix4x4 viewProjectMatrix;
        CameraCullInfo cameraCullInfo;
    };
class GLESSpotLightShadowRP
{
  public:
      GLESSpotLightShadowRP();
      ~GLESSpotLightShadowRP();
    void update(GLESRenderContext3D*context);
    void render(GLESRenderContext3D* context, std::vector<RTBaseRenderNode*>& list, uint32_t count);
    void _applyRenderData(GLESShaderData* sceneData, GLESShaderData* cameraData);
    void _setupShadowCasterShaderValues(GLESShaderData* shaderValues, ShadowSpotData* shadowSliceData, const Vector4& shadowparams, const Vector4& shadowBias);
    void setRenderTarget(GLESInternalRT* value) { destTarget = value; }
    void setLight(RTSpotLight* value);
    void _getShadowBias(float shadowResolution, Vector4& out);
    void _getSpotLightShadowData(ShadowSpotData& shadowSpotData, float resolution, Vector4& shadowParams, Matrix4x4& shadowSpotMatrices, Vector4& shadowMapSize);
  public:

    RTSpotLight* light;
    Vector3 _lightPos;
    Matrix4x4 _lightWorldMatrix;
    float _shadowResolution;
    Matrix4x4 _shadowSpotMatrices;
    Vector4 _shadowSpotMapSize;
    Vector4 _shadowParams;
    float _spotAngle;
    float _spotRange;
    float _shadowDepthBias;
    float _shadowNormalBias;
    Vector4 _shadowBias;
    ShadowMode _shadowMode;
    float _shadowStrength;
    ShadowSpotData _shadowSpotData;
    GLESRenderListQueue _renderQueue;
    GLESInternalRT* destTarget;
};
} // namespace laya
#endif
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
    /*void set_position(Vector3 value);
    void set_lightforward(Vector3 value);
    void set_offsetx(uint32_t value);
    void set_offsety(uint32_t value);
    void set_resolution(uint32_t value);*/
    void _getShadowBias(float shadowResolution, Vector4& out);
    void _getSpotLightShadowData(ShadowSpotData& shadowSpotData, float resolution, Vector4& shadowParams, Matrix4x4& shadowSpotMatrices, Vector4& shadowMapSize);
  public:
    //Vector3 position;
    //Vector3 lightforward;
    //uint32_t offsetx;
    //uint32_t offsety;
   // uint32_t resolution;

    /*struct CameraFrustumCullInfo
    {
        Vector3 _position;
        bool _useOcclusionCulling;
        BoundFrustum _boundFrustum;
        uint32_t _cullingMask;
        uint32_t _staticMask = 0;
    } spotlightCullInfo;*/

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
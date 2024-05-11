#ifndef __GLESDirectLightShadowRP_H_
#define __GLESDirectLightShadowRP_H_

#include "render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTDirectLight.h"
#include "render/ShadowSliceData.h"
#include "render/3D/design/Render3DProcess.h"
#include <vector>
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESRenderListQueue.h"
#include <array>
#include <core/math/Types.h>
#include <render/3D/RenderObjs/RuntimeOBJ/GLESRenderCMD.h>

namespace laya
{
constexpr size_t _maxCascades = 4;
class GLESShaderData;


class GLESInternalRT;
class GLESRenderContext3D;
class RTBaseRenderNode;
class RTCameraNodeData;
class GLESDirectLightShadowRP : public IDirectLightShadowRP
{
  public:
      GLESDirectLightShadowRP();
    ~GLESDirectLightShadowRP();

    void update(GLESRenderContext3D* context);
    void render(GLESRenderContext3D* context, std::vector<RTBaseRenderNode*>& list, uint32_t count);

public:
    void setRenderTarget(GLESInternalRT* value) { destTarget = value; }
    void setLight(RTDirectLight* light);
    void setCameraNodeData(RTCameraNodeData* value) { camera = value; }
    //cmds
    void clearShadowCasterCommandBuffer();
    void addShadowCasterCommandBuffers(const std::vector<GLESRenderCMD*>& cmds);
  private:
    void _setupShadowCasterShaderValues(GLESShaderData *shaderValues, const ShadowSliceData &shadowSliceData,
                                        const Vector3 &LightParam, const Vector4 &shadowBias);
    void getShadowBias(const Matrix4x4 &shadowProjectionMatrix, double shadowResolution, Vector4 &out);
    void _applyRenderData(GLESShaderData *scene, GLESShaderData *camera);
  public:
    Vector3 _lightUp;
    Vector3 _lightSide;
    Vector3 _lightForward;
    ShadowCascadesMode shadowCastMode;
    RTCameraNodeData* camera = nullptr;
    GLESInternalRT* destTarget;
    std::array<F32, 16 * _maxCascades> _shadowMatrices;
    std::array<F32, 4 * _maxCascades> _splitBoundSpheres;
    std::vector<float> _cascadesSplitDistance;
    ShadowCullInfo _shadowCullInfo;
    uint32_t pipelineMode;
    RTDirectLight* _light;
    Vector4 _shadowMapSize;
    Vector4 _shadowParams;
    Vector4 _shadowBias;
    uint32_t _cascadeCount = 0;
    GLESRenderListQueue _renderQueue;
    std::array<ShadowSliceData, 4> _shadowSliceDatas;
    std::vector<Plane> _frustumPlanes;
    uint32_t _shadowMapWidth = 0;
    uint32_t _shadowMapHeight = 0;
    float _shadowTileResolution = 0;

    //cmd
    std::vector<std::vector<GLESRenderCMD*>> _shadowCastCMDS;
};
} // namespace laya
#endif
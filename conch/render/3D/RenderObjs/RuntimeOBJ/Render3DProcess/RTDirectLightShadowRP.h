#ifndef __RTDirectLightShadowRP_H_
#define __RTDirectLightShadowRP_H_

#include "render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTDirectLight.h"
#include "render/3D/ShadowSliceData.h"
#include "render/3D/design/Render3DProcess.h"
#include <vector>
#include "render/driver/gles/GLESRenderQueueList.h"
#include <array>
#include <core/math/Types.h>

namespace laya
{
constexpr size_t _maxCascades = 4;
class ShaderData;


class WebGLInternalRT;
class GLESRenderContext3D;
class RTBaseRenderNode;
class RTCameraNodeData;
class RTDirectLightShadowRP : public IDirectLightShadowRP
{
  public:
      RTDirectLightShadowRP();
    ~RTDirectLightShadowRP();

    void update(RTRenderContext3D* context);
    void render(RTRenderContext3D* context, std::vector<RTBaseRenderNode*>& list, uint32_t count);

public:
    //js bindings
    void setRenderTarget(WebGLInternalRT* value) { destTarget = value; }
    void setLight(RTDirectLight* light);
    void setCameraNodeData(RTCameraNodeData* value) { camera = value; }
  private:
    void _setupShadowCasterShaderValues(ShaderData *shaderValues, const ShadowSliceData &shadowSliceData,
                                        const Vector3 &LightParam, const Vector4 &shadowBias);
    void getShadowBias(const Matrix4x4 &shadowProjectionMatrix, double shadowResolution, Vector4 &out);
    void _applyRenderData(ShaderData *scene, ShaderData *camera);

  public:
    Vector3 _lightUp;
    Vector3 _lightSide;
    Vector3 _lightForward;
    ShadowCascadesMode shadowCastMode;
    RTCameraNodeData* camera = nullptr;
    WebGLInternalRT* destTarget;
    std::array<F32, 16 * _maxCascades> _shadowMatrices;
    std::array<F32, 4 * _maxCascades> _splitBoundSpheres;
    std::vector<Real> _cascadesSplitDistance;
    ShadowCullInfo _shadowCullInfo;
    uint32_t pipelineMode;
    RTDirectLight* _light;
    Vector4 _shadowMapSize;
    Vector4 _shadowParams;
    Vector4 _shadowBias;
    uint32_t _cascadeCount = 0;
    GLESRenderQueueList _renderQueue;
    std::array<ShadowSliceData, 4> _shadowSliceDatas;
    std::vector<Plane> _frustumPlanes;
    uint32_t _shadowMapWidth = 0;
    uint32_t _shadowMapHeight = 0;
    Real _shadowTileResolution = 0;
};
} // namespace laya
#endif
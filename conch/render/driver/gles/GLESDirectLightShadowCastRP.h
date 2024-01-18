#ifndef __GLESDirectLightShadowCastRP_H_
#define __GLESDirectLightShadowCastRP_H_

#include "RTDirectLight.h"
#include "render/3D/ShadowSliceData.h"
#include "render/3D/design/Render3DProcess.h"
#include <vector>
#include "GLESRenderQueueList.h"
#include <array>
#include <core/math/Types.h>

namespace laya
{
constexpr size_t _maxCascades = 4;
class ShaderData;



class GLESDirectLightShadowCastRP : public IDirectLightShadowRP
{
  public:
    struct CameraInfo
    {
        Real farPlane;
        Real nearPlane;
        Real fieldOfView;
        Real aspectRatio;
        Vector3 position;
        Vector3 forward;
        Matrix4x4 projectionViewMatrix;
    };

  public:
    GLESDirectLightShadowCastRP();
    ~GLESDirectLightShadowCastRP();

    void update(RenderContext3D *context) override;
    void render(RenderContext3D *context, std::vector<GLESBaseRenderNode*>& list, uint32_t count) override;

    void set_lightUp(const Vector3 &value);
    void set_lightSide(const Vector3 &value);
    void set_lightForward(const Vector3 &value);
    void set_shadowCascadeMode(ShadowCascadesMode value);
    void set_cameraInfo(CameraInfo value);
    void set_destTarget(uint32_t value);

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
    CameraInfo camera;
    uint32_t destTarget;
    std::array<F32, 16 * _maxCascades> _shadowMatrices;
    std::array<F32, 4 * _maxCascades> _splitBoundSpheres;
    std::vector<Real> _cascadesSplitDistance;
    // std::vector<DirectLightFrustumCullInfo> cullInfos;
    ShadowCullInfo _shadowCullInfo;
    uint32_t pipelineMode;
    RTDirectLight _light;
    Vector4 _shadowMapSize;
    Vector4 _shadowParams;
    Vector4 _shadowBias;
    //ShadowSpotData _shadowSpotData;
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
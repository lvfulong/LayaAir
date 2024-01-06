#ifndef __GLESDirectLightShadowCastRP_H_
#define __GLESDirectLightShadowCastRP_H_

#include "GLESDirectLight.h"
#include "render/3D/ShadowSliceData.h"
#include "render/3D/design/Render3DProcess.h"
#include <vector>
#include "GLESRenderQueueList.h"
#include <array>

namespace laya
{
constexpr size_t _maxCascades = 4;
class ShaderData;

class ShadowSpotData
{
  public:
    ShaderData *cameraShaderValue;
    Vector3 position;
    double offsetX;    // todo type
    double offsetY;    // todo type
    double resolution; // todo type
    Matrix4x4 viewMatrix;
    Matrix4x4 projectionMatrix;
    Matrix4x4 viewProjectMatrix;
    CameraCullInfo cameraCullInfo;
};

class GLESDirectLightShadowCastRP : public IDirectLightShadowRP
{
  public:
    struct CameraInfo
    {
        float _far;
        float _near;
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
    ShadowCascadesMode mode;
    CameraInfo cameraInfo;
    uint32_t destTarget;
    std::vector<uint32_t> _cascadesSplitDistance;
    // std::vector<DirectLightFrustumCullInfo> cullInfos;
    ShadowCullInfo _shadowCullInfo;
    uint32_t pipelineMode;
    GLESDirectLight _light;
    Vector4 _shadowMapSize;
    Vector4 _shadowParams;
    Vector4 _shadowBias;
    ShadowSpotData _shadowSpotData;
    uint32_t _cascadeCount = 0;
    GLESRenderQueueList _renderQueue;
    std::array<ShadowSliceData, 4> _shadowSliceDatas;
};
} // namespace laya
#endif
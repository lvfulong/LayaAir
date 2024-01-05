#ifndef __GLESDirectLightShadowCastRP_H_
#define __GLESDirectLightShadowCastRP_H_

#include "render/3D/ShadowSliceData.h"
#include "render/3D/design/Render3DProcess.h"
#include "GLESDirectLight.h"
#include <vector>

namespace laya
{
constexpr size_t _maxCascades = 4;
class ShaderData;


struct ShadowCullInfo
{
    Vector3							_position;
    std::vector<Plane>				_cullPlanes;
    BoundSphere						_cullSphere;
    int								_cullPlaneCount;
    Vector3							_direction;
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
    void render(RenderContext3D *context, std::vector<BaseRenderNode *> renderNodeList, uint32_t count) override;
    void applyRenderData(uint32_t sceneShaderData, uint32_t cameraShaderData);

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
    void _applyRenderData(ShaderData* scene, ShaderData* camera);
  public:
    Vector3 _lightUp;
    Vector3 _lightSide;
    Vector3 _lightForward;
    ShadowCascadesMode mode;
    CameraInfo cameraInfo;
    uint32_t destTarget;
    std::vector<uint32_t> _cascadesSplitDistance;
    //std::vector<DirectLightFrustumCullInfo> cullInfos;
    uint32_t pipelineMode;
    GLESDirectLight _light;
    Vector4 _shadowMapSize;
    Vector4 _shadowParams;
};
} // namespace laya
#endif
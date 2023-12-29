#ifndef __GLESDirectLightShadowCastRP_H_
#define __GLESDirectLightShadowCastRP_H_

#include <vector>
#include "render/3D/design/Render3DProcess.h"

namespace laya
{
class GLESDirectLightShadowCastRP : public IDirectLightShadowRP
{
  public:
    enum class ShadowCascadesMode
    {
        NoCascades,
        TwoCascades,
        FourCascades,
    };

    struct DirectLightFrustumCullInfo
    {
        Vector3 _position;
        std::vector<Plane> _cullPlanes;
        BoundSphere _cullSphere;
        int _cullPlaneCount;
        Vector3 _direction;
    };
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

  public:
    Vector3 lightUp;
    Vector3 lightSide;
    Vector3 lightForward;
    ShadowCascadesMode mode;
    CameraInfo cameraInfo;
    uint32_t destTarget;
    std::vector<uint32_t> shadowSliceShaderData;
    std::vector<DirectLightFrustumCullInfo> cullInfos;
    uint32_t pipelineMode;
};
} // namespace laya
#endif
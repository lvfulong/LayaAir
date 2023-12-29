#ifndef __GLESSpotLightShadowRP_H_
#define __GLESSpotLightShadowRP_H_

#include "render/3D/design/Render3DProcess.h"
#include <vector>

namespace laya
{
class GLESSpotLightShadowRP
{
  public:
    GLESSpotLightShadowRP();
    ~GLESSpotLightShadowRP();
    void update(RenderContext3D *context);
    void render(RenderContext3D *context, std::vector<BaseRenderNode *> renderNodeList, uint32_t count);
    void applyRenderData(uint32_t sceneShaderData, uint32_t cameraShaderData);

    void set_position(Vector3 value);
    void set_lightforward(Vector3 value);
    void set_destTarget(uint32_t value);
    void set_offsetx(uint32_t value);
    void set_offsety(uint32_t value);
    void set_resolution(uint32_t value);

  public:
    Vector3 position;
    Vector3 lightforward;
    uint32_t destTarget;
    uint32_t offsetx;
    uint32_t offsety;
    uint32_t resolution;

    struct CameraFrustumCullInfo
    {
        Vector3 _position;
        bool _useOcclusionCulling;
        BoundFrustum _boundFrustum;
        uint32_t _cullingMask;
        uint32_t _staticMask = 0;
    } spotlightCullInfo;
};
} // namespace laya
#endif
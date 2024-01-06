#ifndef __GLESForwardAddClusterRP_H_
#define __GLESForwardAddClusterRP_H_

#include "render/3D/design/Render3DProcess.h"
#include <vector>

namespace laya
{

class GLESForwardAddClusterRP
{
  public:
    enum class DepthTextureMode
    {
        None = 0,
        Depth = 1,
        DepthNormals = 2,
        DepthAndDepthNormals = 3,
    };

    struct CameraFrustumCullInfo
    {
        Vector3 _position;
        bool _useOcclusionCulling;
        BoundFrustum _boundFrustum;
        uint32_t _cullingMask;
        uint32_t _staticMask = 0;
    };

  public:
    GLESForwardAddClusterRP();
    ~GLESForwardAddClusterRP();
    void render(RenderContext3D *context, std::vector<GLESBaseRenderNode*> renderNodeList, uint32_t count);
    void renderDepthPass(RenderContext3D *context, JCSingletonList<GLESBaseRenderNode> &renderNodeList);
    void renderDepthNormalPass(RenderContext3D *context, JCSingletonList<GLESBaseRenderNode> &renderNodeList);
    void set_cameraCullInfo(CameraFrustumCullInfo value);
    void set_beforeForwardCmds(std::vector<uint32_t> value);
    void set_beforeSkybox(std::vector<uint32_t> value);
    void set_beforeTransparent(std::vector<uint32_t> value);
    void set_destTarget(uint32_t value);
    void set_skyRenderNode(GLESBaseRenderNode*value);
    void set_depthTextureMode(DepthTextureMode value);

  public:
    CameraFrustumCullInfo CameraCullInfo;
    std::vector<uint32_t> beforeForwardCmds;
    std::vector<uint32_t> beforeSkyboxCmds;
    std::vector<uint32_t> beforeTransparentCmds;
    uint32_t destTarget;
    uint32_t depthTarget;
    uint32_t depthNormalTarget;
    GLESBaseRenderNode*skyRenderNode;
    DepthTextureMode renderpassNode;
};
} // namespace laya
#endif
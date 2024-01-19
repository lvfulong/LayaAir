#ifndef __GLESForwardAddClusterRP_H_
#define __GLESForwardAddClusterRP_H_

#include "render/3D/design/Render3DProcess.h"
#include <vector>
#include "render/driver/gles/GLESRenderQueueList.h"
#include <core/math/Types.h>
#include <core/math/Vector4.h>
#include "render/tempbase.h"

namespace laya
{
    class RTRenderContext3D;
class GLESForwardAddClusterRP
{
  public:
    enum class DepthTextureMode
    {
        None = 0,
        Depth = 1,
        DepthNormals = 2,
        DepthAndDepthNormals = 3,
        MotionVectors = 4,
    };

    struct CameraInfo
    {
        Real farPlane;
        Real nearPlane;
      
    };

  public:
    GLESForwardAddClusterRP();
    ~GLESForwardAddClusterRP();
    void render(RTRenderContext3D* context, std::vector<GLESBaseRenderNode*> renderNodeList, uint32_t count);
  
    void set_cameraCullInfo(CameraCullInfo value);
    void set_beforeForwardCmds(std::vector<uint32_t> value);
    void set_beforeSkybox(std::vector<uint32_t> value);
    void set_beforeTransparent(std::vector<uint32_t> value);
    void set_destTarget(uint32_t value);
    void set_skyRenderNode(GLESBaseRenderNode*value);
    void set_depthTextureMode(DepthTextureMode value);
private:
    void _recoverRenderContext3D(RTRenderContext3D* context);
    void _mainPass(RTRenderContext3D* context);
    void opaqueTexturePass();
    void _renderDepthPass(RTRenderContext3D* context);
    void _renderDepthNormalPass(RTRenderContext3D* context);
  public:
    CameraCullInfo cameraCullInfo;
    std::vector<uint32_t> beforeForwardCmds;
    std::vector<uint32_t> beforeSkyboxCmds;
    std::vector<uint32_t> beforeTransparentCmds;
    uint32_t destTarget;
    uint32_t depthTarget;
    uint32_t depthNormalTarget;
    GLESBaseRenderNode*skyRenderNode;
    DepthTextureMode renderpassNode;
    static Viewport _context3DViewPortCatch;
    static Vector4 _contextScissorPortCatch;
    std::string pipelineMode;
    Color clearColor;
    uint32_t clearFlag;
    bool enableOpaque;
    GLESRenderQueueList opaqueList;
    GLESRenderQueueList transparent;
    Viewport _viewPort;
    std::string depthNormalPipelineMode;
    Color _defaultNormalDepthColor;
    std::string depthPipelineMode;
    Vector4 _zBufferParams;
    CameraInfo camera;
    Vector4 _scissor;
    DepthTextureMode depthTextureMode;
};
} // namespace laya
#endif
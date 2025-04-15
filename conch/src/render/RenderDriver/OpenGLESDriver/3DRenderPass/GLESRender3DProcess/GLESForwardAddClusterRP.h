#ifndef __GLESForwardAddClusterRP_H_
#define __GLESForwardAddClusterRP_H_

#include "render/ShadowSliceData.h"
#include "render/3D/design/Render3DProcess.h"
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/OpenGLESRenderUtil/GLESRenderListQueue.h"
#include "render/ShadowCullInfo.h"
#include <core/math/Types.h>
#include <core/math/Vector4.h>
#include <vector>

namespace laya
{
class RTCameraNodeData;
class GLESRenderContext3D;
class RTBaseRenderNode;
class GLESForwardAddClusterRP
{
  public:
    static Viewport _context3DViewPortCatch;
    static Vector4 _contextScissorPortCatch;
  public:
    enum class DepthTextureMode
    {
        None = 0,
        Depth = 1,
        DepthNormals = 2,
        DepthAndDepthNormals = 3,
        MotionVectors = 4,
    };

  public:
    GLESForwardAddClusterRP();
    ~GLESForwardAddClusterRP();
    void render(GLESRenderContext3D *context, std::vector<RTBaseRenderNode *> renderNodeList, uint32_t count);

    void setCameraCullInfo(const CameraCullInfo &value)
    {
        cameraCullInfo = value;
    }
    void setSkyRenderNode(RTBaseRenderNode *value)
    {
        skyRenderNode = value;
    }
    void setClearColor(Color value)
    {
        value.cloneTo(this->clearColor);
    }
    void setScissor(Vector4 value)
    {
        value.cloneTo(this->scissor);
    }
    void setViewport(Viewport value)
    {
        value.cloneTo(this->viewPort);
    }
    void setOpaqueTexture(GLESInternalRT *value)
    {
        opaqueTexture = value;
    }
    void setDepthNormalTarget(GLESInternalRT *value)
    {
        depthNormalTarget = value;
    }
    void setDepthTarget(GLESInternalRT *value)
    {
        depthTarget = value;
    }
    void setDestTarget(GLESInternalRT *value)
    {
        destTarget = value;
    }
    void setCameraNodeData(RTCameraNodeData *value)
    {
        camera = value;
    }

    void setOpaqueCMD(const std::vector<GLESRenderCMD*>& cmds);

    void clearBeforeForwardCmds();
    void addBeforeForwardCmds(const std::vector<GLESRenderCMD*>& cmds);
    void clearBeforeSkyboxCmds();
    void addBeforeSkyboxCmds(const std::vector<GLESRenderCMD*>& cmds);
    void clearBeforeTransparentCmds();
    void addBeforeTransparentCmds(const std::vector<GLESRenderCMD*>& cmds);
  private:
    void _recoverRenderContext3D(GLESRenderContext3D *context);
    void _mainPass(GLESRenderContext3D *context);
    void opaqueTexturePass(GLESRenderContext3D* context);
    void _renderDepthPass(GLESRenderContext3D *context);
    void _renderDepthNormalPass(GLESRenderContext3D *context);

  public:
    CameraCullInfo cameraCullInfo;
    RTCameraNodeData *camera = nullptr;
    GLESInternalRT *opaqueTexture;
    GLESInternalRT *destTarget;
    GLESInternalRT *depthTarget;
    GLESInternalRT *depthNormalTarget;
    RTBaseRenderNode *skyRenderNode;
    DepthTextureMode renderpassNode;
   
    std::string pipelineMode;
    Color clearColor;
    uint32_t clearFlag;
    bool enableOpaque;
    GLESRenderListQueue opaqueList;
    GLESRenderListQueue transparent;
    Viewport viewPort;
    std::string depthNormalPipelineMode;
    Color _defaultNormalDepthColor;
    std::string depthPipelineMode;
    Vector4 _zBufferParams;
    Vector4 scissor;
    DepthTextureMode depthTextureMode;
    bool enableCMD;
    bool enableTransparent;
    bool enableOpaqueTexture;
    //cmd
    std::vector<std::vector<GLESRenderCMD*>> _beforeForwardCmds;
    std::vector<std::vector<GLESRenderCMD*>> _beforeSkyboxCmds;
    std::vector<std::vector<GLESRenderCMD*>> _beforeTransparentCmds;

    std::vector<GLESRenderCMD*> opaquePassCmd;
};
} // namespace laya
#endif
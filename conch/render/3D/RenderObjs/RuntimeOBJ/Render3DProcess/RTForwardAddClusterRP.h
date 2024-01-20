#ifndef __RTForwardAddClusterRP_H_
#define __RTForwardAddClusterRP_H_

#include "render/3D/design/Render3DProcess.h"
#include <vector>
#include "render/driver/gles/GLESRenderQueueList.h"
#include <core/math/Types.h>
#include <core/math/Vector4.h>
#include "render/tempbase.h"
#include "render/3D/ShadowSliceData.h"

namespace laya
{
    class RTCameraNodeData;
    class RTRenderContext3D;
    class RTBaseRenderNode;
class RTForwardAddClusterRP
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
  public:
      RTForwardAddClusterRP();
    ~RTForwardAddClusterRP();
    void render(RTRenderContext3D* context, std::vector<RTBaseRenderNode*> renderNodeList, uint32_t count);

    //void set_beforeForwardCmds(std::vector<uint32_t> value);
    //void set_beforeSkybox(std::vector<uint32_t> value);
    //void set_beforeTransparent(std::vector<uint32_t> value);
    void setCameraCullInfo(const CameraCullInfo& value) { cameraCullInfo = value; }
    void setSkyRenderNode(RTBaseRenderNode* value) { skyRenderNode = value; }
    void setClearColor(Color& value) { value.cloneTo(this->clearColor); }
    void setScissor(Vector4& value) { value.cloneTo(this->scissor); }
    void setViewport(Viewport& value) { value.cloneTo(this->viewPort); }
    void setOpaqueTexture(WebGLInternalRT* value) { opaqueTexture = value; }
    void setDepthNormalTarget(WebGLInternalRT* value) { depthNormalTarget = value; }
    void setDepthTarget(WebGLInternalRT* value) { depthTarget = value; }
    void setDestTarget(WebGLInternalRT* value) { destTarget = value; }
    void setCameraNodeData(RTCameraNodeData* value) { camera = value; }
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
    RTCameraNodeData* camera;
    WebGLInternalRT* opaqueTexture;
    WebGLInternalRT* destTarget;
    WebGLInternalRT* depthTarget;
    WebGLInternalRT* depthNormalTarget;
    RTBaseRenderNode*skyRenderNode;
    DepthTextureMode renderpassNode;
    static Viewport _context3DViewPortCatch;
    static Vector4 _contextScissorPortCatch;
    std::string pipelineMode;
    Color clearColor;
    uint32_t clearFlag;
    bool enableOpaque;
    GLESRenderQueueList opaqueList;
    GLESRenderQueueList transparent;
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
    
};
} // namespace laya
#endif
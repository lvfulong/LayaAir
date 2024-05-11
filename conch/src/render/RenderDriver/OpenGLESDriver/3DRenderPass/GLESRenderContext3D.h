#ifndef __GLESRenderContext3D_H_
#define __GLESRenderContext3D_H_

#include "render/ShadowCullInfo.h"
#include <core/math/Color.h>
#include <core/math/Vector4.h>
#include <core/math/Viewport.h>
#include <functional>
#include <render/3D/design/renderEnum/RenderClearFlag.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h>
#include <utils/JCSingletonList.h>
#include <render/3D/RenderObjs/RuntimeOBJ/GLESRenderCMD.h>

namespace laya
{
class GLESRenderElement3D;
class GLESShaderData;
class GLESInternalRT;
class RTSceneNodeData;
class RTCameraModuleData;
class GLESRenderCMD;
class GLESRenderContext3D
{
  public:
    GLESRenderContext3D();
    ~GLESRenderContext3D();
    uint32_t drawRenderElementList(const JCSingletonList<GLESRenderElement3D *> &list);
    uint32_t drawRenderElementOne(GLESRenderElement3D *node);
    void runOneCMD(GLESRenderCMD* cmd);
    void runCMDList(const std::vector<GLESRenderCMD*>& cmds);
    void setRenderTarget(GLESInternalRT* renderTarget, RenderClearFlagBits flag = 0)
    {
        _clearFlag = flag;
        if (_renderTarget == renderTarget) {
            return;
        }
        _renderTarget = renderTarget;
        _needStart = true;
      ;
    }
    void setCameraData(GLESShaderData *shaderData)
    {
        this->cameraData = shaderData;
    }
    void setSceneData(GLESShaderData *sceneData)
    {
        this->sceneData = sceneData;
    }
    void setViewport(const Viewport &value)
    {
        this->viewPort = value;
        _needStart = true;
    }
    void setScissor(const Vector4 &value)
    {
        this->scissor = value;
        _needStart = true;
    }
    uint32_t setClearData(uint32_t flag, const Color &color, float depthValue, uint8_t stencilValue)
    {
        _clearFlag = flag;
        clearColor = color;
        clearDepth = depthValue;
        clearStencil = stencilValue;
        return 0;
    }
    void setSceneNodeData(RTSceneNodeData *value)
    {
        sceneNodeData = value;
    }
    void setCameraNodeData(RTCameraModuleData *value)
    {
        cameraNodeData = value;
    }
    void setGlobalShaderData(GLESShaderData *value)
    {
        this->globalShaderData = value;
    }
    void setGlobalConfigShaderData(RTDefineDatas*value)
    {
        globalConfigShaderData = value;
    }

  private:
    void _bindRenderTarget();
    void _start();
    void _end();

  public:
    GLESShaderData *globalShaderData = nullptr;
    RTDefineDatas*globalConfigShaderData = nullptr;
    GLESInternalRT *_renderTarget = nullptr;
    Viewport viewPort;
    Vector4 scissor;
    bool invertY;
    // pipelineMode
    std::string pipelineMode;
    uint32_t _clearFlag{0};
    float clearDepth;
    uint8_t clearStencil;
    Color clearColor;

    // data
    GLESShaderData *cameraData = nullptr;
    GLESShaderData *sceneData = nullptr;

    // upload flag
    uint32_t _sceneUpdateMask = 0;
    uint32_t _cameraUpdateMask = 0;
    RTSceneNodeData *sceneNodeData = nullptr;
    RTCameraModuleData *cameraNodeData = nullptr;
private:
    bool _needStart = true;
};
} // namespace laya
#endif

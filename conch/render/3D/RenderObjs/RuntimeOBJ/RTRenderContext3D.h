#ifndef __RTRenderContext3D_H_
#define __RTRenderContext3D_H_

#include "render/ShadowCullInfo.h"
#include <core/math/Color.h>
#include <core/math/Vector4.h>
#include <core/math/Viewport.h>
#include <functional>
#include <render/3D/design/renderEnum/RenderClearFlag.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h>
#include <utils/JCSingletonList.h>

namespace laya
{
class GLESRenderElement3D;
class GLESShaderData;
class GLESInternalRT;
class RTSceneNodeData;
class RTCameraModuleData;
class RTRenderContext3D
{
  public:
    RTRenderContext3D();
    ~RTRenderContext3D();
    uint32_t drawRenderElementList(const JCSingletonList<GLESRenderElement3D *> &list);
    uint32_t drawRenderElementOne(GLESRenderElement3D *node);
    void setRenderTarget(GLESInternalRT *renderTarget)
    {
        this->_renderTarget = renderTarget;
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
    }
    void setScissor(const Vector4 &value)
    {
        this->scissor = value;
    }
    uint32_t setClearData(RenderClearFlagBits flag, Color color, float depthValue, uint8_t stencilValue)
    {
        clearFlag = flag;
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
    void setGlobalConfigShaderData(DefineDatas *value)
    {
        globalConfigShaderData = value;
    }

  private:
    void _bindRenderTarget();
    void _start();
    void _end();

  public:
    GLESShaderData *globalShaderData = nullptr;
    DefineDatas *globalConfigShaderData = nullptr;
    GLESInternalRT *_renderTarget = nullptr;
    Viewport viewPort;
    Vector4 scissor;
    bool invertY;
    // pipelineMode
    std::string pipelineMode;
    RenderClearFlagBits clearFlag{0};
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
};
} // namespace laya
#endif

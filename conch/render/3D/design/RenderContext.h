#ifndef __RenderContext_H_
#define __RenderContext_H_
#include <functional>
#include "render/3D/design/RenderElement.h"
#include <core/math/Viewport.h>
#include <core/math/Vector4.h>
#include <core/math/Color.h>
#include "render/tempbase.h"
#include <utils/JCSingletonList.h>
#include <render/3D/design/renderEnum/RenderClearFlag.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h>



namespace laya{
    class RenderElementOBJ;
    class ShaderData;
    class WebGLInternalRT;
    class RTSceneNodeData;
    class RTCameraModuleData;
    //typedef std::function<void(JCSingletonList<RenderElementOBJ*>&) > extendCall;
 class IRenderContext3D
 {
 public:
     virtual ~IRenderContext3D();
     IRenderContext3D();
     virtual uint32_t drawRenderElementList(const JCSingletonList<RenderElementOBJ*>& list) { return 0; }
     virtual uint32_t drawRenderElementOne(RenderElementOBJ* one) { return 0; }
     virtual void setRenderTarget(WebGLInternalRT* renderTarget);
     virtual void setCameraData(ShaderData* shaderData);
     virtual void setSceneData(ShaderData* sceneData);
     virtual void setViewport(const Viewport& value);
     virtual void setScissor(const Vector4& value);
     virtual uint32_t setClearData(RenderClearFlagBits flag, Color color, float depthValue, uint8_t stencilValue);
     void setSceneNodeData(RTSceneNodeData* value) { sceneNodeData = value; }
     void setCameraNodeData(RTCameraModuleData* value) { cameraNodeData = value; }
     void setGlobalShaderData(ShaderData* value);
     void setglobalConfigShaderData(DefineDatas* value);
 public:
     ShaderData* globalShaderData = nullptr;
     DefineDatas* globalConfigShaderData = nullptr;
     WebGLInternalRT* _renderTarget = nullptr;
    Viewport viewPort;
    Vector4 scissor;
    bool invertY;
    //pipelineMode
    std::string pipelineMode;
    RenderClearFlagBits clearFlag{ 0 };
    float clearDepth;
    uint8_t clearStencil;
    Color clearColor;

    //data
    ShaderData* cameraData = nullptr;
    ShaderData* sceneData = nullptr;

    //upload flag
    uint32_t _sceneUpdateMask = 0;
    uint32_t _cameraUpdateMask = 0;
    RTSceneNodeData* sceneNodeData = nullptr;
    RTCameraModuleData* cameraNodeData = nullptr;
 };

}

#endif
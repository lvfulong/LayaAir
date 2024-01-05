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



namespace laya{
    class RenderElementOBJ;
    typedef std::function<void(JCSingletonList<RenderElementOBJ*>&) > extendCall;
 class RenderContext3D
 {
 public:
     static uint32_t globalShaderData;
     virtual ~RenderContext3D();
     RenderContext3D();
     virtual uint32_t drawRenderElementList(const JCSingletonList<RenderElementOBJ*>& list) = 0;
     virtual void drawRenderElementOne(RenderElementOBJ* one) = 0;
     virtual void setDestTarget(uint32_t renderTarget);
     virtual void setCameraData(uint32_t shaderData);
     virtual void setSceneData(uint32_t sceneData);
     virtual void setViewport(const Viewport& value);
     virtual void setScissor(const Vector4& value);
     virtual void clearData(RenderClearFlagBits flag, Color color, float depthValue, uint8_t stencilValue);
     virtual void setSceneUpdateMask(uint32_t mask);
     virtual void setCameraUpdateMask(uint32_t mask);
     
 private:
    
 public:
    uint32_t renderTarget;
    Viewport viewPort;
    Vector4 scissor;
    bool invertY;
    //pipelineMode
    std::string pipelineMode;
    RenderClearFlagBits clearflag{ 0 };
    float clearDepth;
    uint8_t clearStencil;
    Color clearColor;

    //data
    uint32_t cameraData;
    uint32_t sceneData;

    //upload flag
    uint32_t _sceneUpdataMask;
    uint32_t _cameraUpdateMask;
    
 };

 class NodeContext3DData
 {
 public:
     //��¼һЩ��Ⱦ����ʱ������ݣ�����ű��е�scene����
     NodeContext3DData();
     ~NodeContext3DData();
 public:
     uint32_t sceneLightmapDirtyFlag;
 };
}

#endif
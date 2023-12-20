
#ifndef __RenderContext_H_
#define __RenderContext_H_
#include <functional>
#include <render/tempbase.h>
#include <core/math/Viewport.h>
#include <core/math/Vector4.h>
#include <core/math/Color.h>
namespace layaRender{
    typedef std::function<void(SingleList<RenderElement*>&) > extendCall;
 class RenderContext3D
 {
 public:
     static uint32_t gloableShaderData;
     virtual ~RenderContext3D() {};
     RenderContext3D() ;
     virtual uint32_t drawRenderElementList(SingleList<RenderElement*>& list) = 0;
     virtual void drawRenderElementOne(RenderElement* one) = 0;
     virtual void setDestTarget(uint32_t rendertarget) = 0;
     virtual void setCameraInfo(CameraInfo* camerainfo) = 0;
     virtual void setCameraData(uint32_t shaderdata) = 0;
     virtual void setSceneData(uint32_t sceneData) = 0;
     virtual void setViewport(laya::Viewport viewport) = 0;
     virtual void setScissor(uint32_t x,uint32_t y,uint32_t width,uint32_t height) = 0;
     virtual void clearData(ClearFlag flag, laya::Color color, float depthValue, uint8_t stencilValue);
     virtual void setSceneUpdateMask(uint32_t mask);
     virtual void setCameraUpdateMask(uint32_t mask);
 private:
    
 public:
    CameraInfo* cameraInfo;
    uint32_t renderTarget;
    laya::Viewport viewPort;
    laya::Vector4 scissor;
    bool invertY;
    //pipelineMode
    uint32_t pipelineMode;

    ClearFlag clearflag;
    float clearDepth;
    uint8_t clearStencil;
    laya::Color clearColor;

    //data
    uint32_t cameraData;
    uint32_t sceneData;

    //upload flag
    uint32_t _sceneUpdataMask;
    uint32_t _cameraUpdateMask;

    extendCall preUpdate;
    extendCall afterUpdate;
    extendCall preRender;
    extendCall afterRender;
    
 };

}

#endif
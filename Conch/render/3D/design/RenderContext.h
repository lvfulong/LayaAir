
#ifndef __RenderContext_H_
#define __RenderContext_H_

#include <render/tempbase.h>
#include <core/math/Viewport.h>
#include <core/math/Vector4.h>

namespace layaRender{
 class RenderContext
 {
 public:
     static uint32_t gloableShaderData;

     using p_context = std::shared_ptr<RenderContext>;
     static p_context create() {
         return std::make_shared<RenderContext>();
     }
     virtual ~RenderContext() {};
     RenderContext() ;
     virtual uint32_t drawRenderElementList(SingleList<RenderElement*>& list) = 0;
     virtual void drawRenderElementOne(RenderElement* one) = 0;
     virtual void setDestTarget(uint32_t rendertarget) = 0;
     virtual void setCameraInfo(CameraInfo* camerainfo) = 0;
     virtual void setCameraData(uint32_t shaderdata) = 0;
     virtual void setSceneData(uint32_t sceneData) = 0;
     virtual void setViewport(laya::Viewport viewport) = 0;
     virtual void setScissor(float x,float y,float width,float height) = 0;

 private:
    
 public:
    CameraInfo* cameraInfo;
    uint32_t IRenderTarget;
    //viewPort
    laya::Viewport viewPort;
    //scissor
    laya::Vector4 scissor;

    bool invertY;

    pipelineMode:
 };

}

#endif
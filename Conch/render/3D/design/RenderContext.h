
#ifndef __RenderContext_H_
#define __RenderContext_H_

#include "render/tempbase.h"

namespace layaRender{
 class RenderContext
 {
 public:
    static gloableShaderData

     using p_context = std::shared_ptr<RenderContext>;
     static p_context create() {
         return std::make_shared<RenderContext>();
     }
     virtual ~RenderContext();
     RenderContext();
     virtual uint32_t drawRenderElementList(SingleList<RenderElement*>& list);
     virtual void drawRenderElementOne(RenderElement* one);
     virtual void setDestTarget(uint32_t rendertarget);
     virtual void setCameraInfo(CameraInfo* camerainfo);
     virtual void setCameraData(uint32_t shaderdata);
     virtual void setSceneData(uint32_t sceneData);
     virtual void setViewport(Viewport viewport);
     virtual void setScissor(float x,float y,float width,float height);

 private:
    
 public:
    CameraInfo* cameraInfo;
    destTarget: IRenderTarget;
    //viewPort
    Viewport viewPort;
    //scissor
    Vector4 scissor: ;
 };

}

#endif

#ifndef __RenderContext_H_
#define __RenderContext_H_

#include "render/tempbase.h"

namespace layaRender{
 class RenderContext
 {
 public:
     using p_context = std::shared_ptr<RenderContext>;
     static p_context create() {
         return std::make_shared<RenderContext>();
     }
     ~RenderContext();
     RenderContext();
     uint32_t drawRenderElementList(SingleList<RenderElement*>& list);
     void drawRenderElementOne(RenderElement* one);
 private:
    
 public:
     CameraInfo* cameraInfo;
 };

}

#endif
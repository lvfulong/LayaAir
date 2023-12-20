#ifndef __GLESRenderInstance_H_
#define __GLESRenderInstance_H_

#include "render/3D/design/RenderElement"

namespace laya{
    class GLESRenderInstance:public RenderInstanceData {
        void render(RenderContext3D* context) override;
    }
}
#endif
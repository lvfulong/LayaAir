#ifndef __GLESRenderInstanceData_H_
#define __GLESRenderInstanceData_H_

#include <render/3D/design/RenderElement.h>

namespace laya
{
    class GLESRenderContext3D;
    class GLESRenderInstanceData : public RenderInstanceData
    {
    public:
        void render(IRenderContext3D* context) override;
        void preRender(IRenderContext3D* context) override;
    };
} // namespace laya
#endif
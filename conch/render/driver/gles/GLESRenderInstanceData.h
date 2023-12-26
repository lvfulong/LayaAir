#ifndef __GLESRenderInstanceData_H_
#define __GLESRenderInstanceData_H_

#include <render/3D/design/RenderElement.h>

namespace laya
{
    class GLESRenderInstanceData : public RenderInstanceData
    {
    public:
        void render(RenderContext3D* context) override;
        void preRender(RenderContext3D* context) override;
    };
} // namespace laya
#endif
#ifndef __GLESRenderContext3D_H_
#define __GLESRenderContext3D_H_

#include <render/3D/design/RenderContext.h>
#include <render/3D/design/RenderElement.h>
#include <utils/JCSingletonList.h>
#include <render/3D/design/renderEnum/RenderClearFlag.h>
#include <binder/JSInterface.h>

namespace laya{
    class GLESRenderContext3D : public RenderContext3D {
    public:
        GLESRenderContext3D();
        ~GLESRenderContext3D();
        uint32_t drawRenderElementList(JCSingletonList<RenderElement3D*>& list) override;
        void drawRenderElementOne(RenderElement3D* one) override;
        static void exportJS(Context& context);
    private:
        void _bindRenderTarget();
        void _start();
        void _end();
    public:
    
    };
}
#endif
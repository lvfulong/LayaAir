
#ifndef __GLESRenderContext3D_H_
#define __GLESRenderContext3D_H_

#include <render/3D/design/RenderContext.h>
#include <render/3D/design/RenderElement.h>
namespace laya{
    class GLESRenderContext3D : public RenderContext3D {
    public:
        GLESRenderContext3D();
        ~GLESRenderContext3D();
        uint32_t drawRenderElementList(SingleList<RenderElement3D*>& list) override;
        void drawRenderElementOne(RenderElement3D* one) override;
        void setDestTarget(uint32_t rendertarget) override;
        void setCameraData(uint32_t shaderdata) override;
        void setSceneData(uint32_t sceneData) override;
        void setViewport(laya::Viewport viewport) override;
        void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void clearData(ClearFlag flag, laya::Color color, float depthValue, uint8_t stencilValue) override;
        void setSceneUpdateMask(uint32_t mask) override;
        void setCameraUpdateMask(uint32_t mask) override;
    private:
        void _bindRenderTarget();
        void _start();
        void _end();
    public:
    
    };
}
#endif
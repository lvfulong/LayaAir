
#include "RenderContext.h"
namespace laya{
    RenderContext3D::RenderContext3D(){
        //TODO
    }

    RenderContext3D::~RenderContext3D(){
        //TODO
    }

    void RenderContext3D::setDestTarget(uint32_t rendertarget) {
        this->renderTarget = rendertarget;
    }

    void RenderContext3D::setCameraData(uint32_t shaderdata) {
        this->cameraData = shaderdata;
    }

    void RenderContext3D::setSceneData(uint32_t sceneData) {
        this->sceneData = sceneData;
    }

    void RenderContext3D::setViewport(Viewport value) {
        this->viewPort = viewport;
    }

    void RenderContext3D::setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        scissor.setValue(x, y, width, height);
    }

    void RenderContext3D::clearData(RenderClearFlagBits flag, Color color, float depthValue, uint8_t stencilValue) {
        clearflag = flag;
        clearColor = color;
        clearDepth = depthValue;
        clearStencil = stencilValue;
    };

    void RenderContext3D::setSceneUpdateMask(uint32_t mask) {
        _sceneUpdataMask = mask;
    }

    void RenderContext3D::setCameraUpdateMask(uint32_t mask) {
        _cameraUpdateMask = mask;
    }

}

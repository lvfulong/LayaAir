#include "RenderContext.h"
namespace laya{
uint32_t RenderContext3D::globalShaderData = 0;
    RenderContext3D::RenderContext3D() {

    }
    RenderContext3D::~RenderContext3D(){
        //TODO
    }

    void RenderContext3D::setDestTarget(uint32_t renderTarget) {
        this->renderTarget = renderTarget;
    }

    void RenderContext3D::setCameraData(ShaderData* shaderData) {
        this->cameraData = shaderData;
    }

    void RenderContext3D::setSceneData(ShaderData* sceneData) {
        this->sceneData = sceneData;
    }

    void RenderContext3D::setViewport(const Viewport& value) {
        this->viewPort = value;
    }

    void RenderContext3D::setScissor(const Vector4& value) {
        this->scissor = value;
    }

    void RenderContext3D::setClearData(RenderClearFlagBits flag, Color color, float depthValue, uint8_t stencilValue) {
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

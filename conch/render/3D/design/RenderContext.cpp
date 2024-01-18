#include "RenderContext.h"
namespace laya{
uint32_t IRenderContext3D::globalShaderData = 0;
IRenderContext3D::IRenderContext3D() {

    }
    IRenderContext3D::~IRenderContext3D(){
        //TODO
    }

    void IRenderContext3D::setDestTarget(uint32_t renderTarget) {
        this->renderTarget = renderTarget;
    }

    void IRenderContext3D::setCameraData(ShaderData* shaderData) {
        this->cameraData = shaderData;
    }

    void IRenderContext3D::setSceneData(ShaderData* sceneData) {
        this->sceneData = sceneData;
    }

    void IRenderContext3D::setViewport(const Viewport& value) {
        this->viewPort = value;
    }

    void IRenderContext3D::setScissor(const Vector4& value) {
        this->scissor = value;
    }

    void IRenderContext3D::setClearData(RenderClearFlagBits flag, Color color, float depthValue, uint8_t stencilValue) {
        clearflag = flag;
        clearColor = color;
        clearDepth = depthValue;
        clearStencil = stencilValue;
    };

    void IRenderContext3D::setSceneUpdateMask(uint32_t mask) {
        _sceneUpdataMask = mask;
    }

    void IRenderContext3D::setCameraUpdateMask(uint32_t mask) {
        _cameraUpdateMask = mask;
    }

}

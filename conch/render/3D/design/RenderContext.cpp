#include "RenderContext.h"
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h>
namespace laya{

IRenderContext3D::IRenderContext3D() {

    }
    IRenderContext3D::~IRenderContext3D(){
        //TODO
    }
    void IRenderContext3D::setGlobalShaderData(ShaderData* value)
    {
        this->globalShaderData = value;
    }
    void IRenderContext3D::setglobalConfigShaderData(DefineDatas* value)
    {
        globalConfigShaderData = value;
    }
    void IRenderContext3D::setRenderTarget(WebGLInternalRT* renderTarget) {
        this->_renderTarget = renderTarget;
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

    uint32_t IRenderContext3D::setClearData(RenderClearFlagBits flag, Color color, float depthValue, uint8_t stencilValue) {
        clearFlag = flag;
        clearColor = color;
        clearDepth = depthValue;
        clearStencil = stencilValue;
        return 0;
    };
}

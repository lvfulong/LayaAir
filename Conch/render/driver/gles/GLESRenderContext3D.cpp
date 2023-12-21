#include "GLESRenderContext3D.h"
namespace laya{
    GLESRenderContext3D::GLESRenderContext3D() {

    };

    GLESRenderContext3D::~GLESRenderContext3D() {
    
    };
    uint32_t GLESRenderContext3D::drawRenderElementList(JCSingletonList<RenderElement3D*>& list) {
       _bindRenderTarget();
       _start();
       if (preUpdate) preUpdate(list);
       //update
       for (uint32_t i = 0, n = list.getLength(); i < n; i++) {
           list.m_vElements[i]->renderData.preRender(this);
       }
       if (afterUpdate)  afterUpdate(list);
       if (preRender) preRender(list);
       //render
       for (uint32_t i = 0, n = list.getLength(); i < n; i++) {
           list.m_vElements[i]->renderData.render(this);
       }
       if (preRender) afterRender(list);
        _end();
    }

    void GLESRenderContext3D::drawRenderElementOne(RenderElement3D* one) {
        _bindRenderTarget();
        _start();
        //update
        one->renderData.preRender(this);
        //render
        one->renderData.render(this);
        _end();
    }

    void GLESRenderContext3D::setDestTarget(uint32_t rendertarget) {
       this->renderTarget = rendertarget;
    }
    
    void GLESRenderContext3D::setCameraData(uint32_t shaderdata) {
        this->cameraData = shaderdata;
    }
    
    void GLESRenderContext3D::setSceneData(uint32_t sceneData) {
        this->sceneData = sceneData;
    }

    void GLESRenderContext3D::setViewport(laya::Viewport viewport) {
        this->viewPort = viewport;
    }

    void GLESRenderContext3D::setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        scissor.setValue(x, y, width, height);
    }
    
    void GLESRenderContext3D::clearData(ClearFlag flag, laya::Color color, float depthValue, uint8_t stencilValue) {
        clearflag = flag;
        clearDepth = depthValue;
        clearStencil = stencilValue;
        clearColor = color;
    }

    void GLESRenderContext3D::_bindRenderTarget(){
        //bind rendertarget
    }
    
    void GLESRenderContext3D::_start() {
        //set viewport
        //set scissor
        //clear color depth stencil data
    }
    void GLESRenderContext3D::_end() {
    
    }

    void GLESRenderContext3D::setSceneUpdateMask(uint32_t mask) {
        _sceneUpdataMask = mask;
    }

    void GLESRenderContext3D::setCameraUpdateMask(uint32_t mask) {
        _cameraUpdateMask = mask;
    }
}
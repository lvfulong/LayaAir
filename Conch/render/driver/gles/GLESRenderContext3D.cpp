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
}
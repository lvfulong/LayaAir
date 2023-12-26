#include "GLESRenderContext3D.h"
#include "LayaGL.h"

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
        //bind rendertarget todo
    }
    
    void GLESRenderContext3D::_start() {
        LayaGL::m_pWebglEngine->viewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
		LayaGL::m_pWebglEngine->scissor(scissor.x, scissor.y, scissor.z, scissor.w);
        LayaGL::m_pWebglEngine->clearRenderTexture(clearflag, &clearColor, clearDepth);

    }
    void GLESRenderContext3D::_end() {
    
    }
    void GLESRenderContext3D::exportJS(Context& context)
    {
        class_<GLESRenderContext3D> class_binding;
        //todo class_binding.function("drawRenderElementList", &GLESRenderContext3D::drawRenderElementList);
        class_binding.function("drawRenderElementOne", &GLESRenderContext3D::drawRenderElementOne);
        class_binding.function("setDestTarget", &GLESRenderContext3D::setDestTarget);
        class_binding.function("setViewport", &GLESRenderContext3D::setViewport);
        class_binding.function("setScissor", &GLESRenderContext3D::setScissor);
        class_binding.function("setScissor", &GLESRenderContext3D::setScissor);
        class_binding.function("setScissor", &GLESRenderContext3D::setScissor);
        class_binding.function("setSceneUpdateMask", &GLESRenderContext3D::setSceneUpdateMask);
        class_binding.function("setCameraUpdateMask", &GLESRenderContext3D::setCameraUpdateMask);
        class_binding.property("invertY", &GLESRenderContext3D::invertY);
        class_binding.property("sceneID", &GLESRenderContext3D::sceneData);
        class_binding.property("pipelineMode", &GLESRenderContext3D::pipelineMode);
        class_binding.constructor<>();
        context.class_("ConchGLESRenderContext3D", class_binding);
    }
}
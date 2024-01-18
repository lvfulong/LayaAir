#include "GLESRenderContext3D.h"
#include "LayaGL.h"
#include <render/3D/temp/RenderElementOBJ.h>
namespace laya
{
GLESRenderContext3D::GLESRenderContext3D(){

};

GLESRenderContext3D::~GLESRenderContext3D(){

};
uint32_t GLESRenderContext3D::drawRenderElementList(const JCSingletonList<RenderElementOBJ *> &list)
{
    /*_bindRenderTarget();
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
     _end();*/

    return 0;
}

void GLESRenderContext3D::drawRenderElementOne(RenderElementOBJ *one) 
{
    _bindRenderTarget();
    _start();
    // update
    one->renderData.preRender(static_cast<IRenderContext3D*>(this));
    // render
    one->renderData.render(static_cast<IRenderContext3D*>(this));
    _end();
}

void GLESRenderContext3D::_bindRenderTarget()
{
    // bind rendertarget todo
}

void GLESRenderContext3D::_start()
{
    LayaGL::m_pWebglEngine->viewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
    LayaGL::m_pWebglEngine->scissor(scissor.x, scissor.y, scissor.z, scissor.w);
    LayaGL::m_pWebglEngine->clearRenderTexture(clearflag, &clearColor, clearDepth);
}
void GLESRenderContext3D::_end()
{
}
} // namespace laya

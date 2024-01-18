#include "GLESRenderContext3D.h"
#include "LayaGL.h"
#include <render/3D/temp/RenderElementOBJ.h>
#include "render/driver/gles/LayaGL.h"
namespace laya
{
GLESRenderContext3D::GLESRenderContext3D(){

};

GLESRenderContext3D::~GLESRenderContext3D(){

};
uint32_t GLESRenderContext3D::drawRenderElementList(const JCSingletonList<RenderElementOBJ *> &list)
{
    _bindRenderTarget();
    _start();
    //if (preUpdate) preUpdate(list);
    for (uint32_t i = 0, n = list.getLength(); i < n; i++) {
        list.m_vElements[i]->_preUpdatePre(this);
    }
    //if (afterUpdate)  afterUpdate(list);
    //if (preRender) preRender(list);
    for (uint32_t i = 0, n = list.getLength(); i < n; i++) {
        list.m_vElements[i]->_render(this);
    }
    //if (preRender) afterRender(list);
     _end();

    return 0;
}

uint32_t GLESRenderContext3D::drawRenderElementOne(RenderElementOBJ *node)
{
    _bindRenderTarget();
    _start();
    node->_preUpdatePre(static_cast<IRenderContext3D*>(this));
    node->_render(static_cast<IRenderContext3D*>(this));
    _end();
    return 0;
}

void GLESRenderContext3D::_bindRenderTarget()
{
    if (this->_renderTarget)
    {
        LayaGL::m_pWebglEngine->getTextureContext()->bindRenderTarget(this->_renderTarget, 0);
    }
    else 
    {
        LayaGL::m_pWebglEngine->getTextureContext()->bindoutScreenTarget();
    }
}

void GLESRenderContext3D::_start()
{
    LayaGL::m_pWebglEngine->viewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
    LayaGL::m_pWebglEngine->scissor(scissor.x, scissor.y, scissor.z, scissor.w);
    if (this->clearFlag != static_cast<RenderClearFlagBits>(RenderClearFlag::Nothing)) {
        LayaGL::m_pWebglEngine->clearRenderTexture(clearFlag, &clearColor, clearDepth);
    }
}
void GLESRenderContext3D::_end()
{
}
} // namespace laya

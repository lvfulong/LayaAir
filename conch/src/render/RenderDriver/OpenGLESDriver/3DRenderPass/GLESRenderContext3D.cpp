#include "GLESRenderContext3D.h"
#include "render/LayaGL.h"
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderCMD.h"
namespace laya
{
GLESRenderContext3D::GLESRenderContext3D(){

};

GLESRenderContext3D::~GLESRenderContext3D(){

};
uint32_t GLESRenderContext3D::drawRenderElementList(const JCSingletonList<GLESRenderElement3D*> &list)
{
    if (_needStart) {
        _bindRenderTarget();
        _start();
        _needStart = false;
    }

    for (uint32_t i = 0, n = list.getLength(); i < n; i++)
    {
        list.m_vElements[i]->_preUpdatePre(this);
    }
    for (uint32_t i = 0, n = list.getLength(); i < n; i++)
    {
        list.m_vElements[i]->_render(this);
    }

    return 0;
}

uint32_t GLESRenderContext3D::drawRenderElementOne(GLESRenderElement3D*node)
{
    if (_needStart) {
        _bindRenderTarget();
        _start();
        _needStart = false;
    }
    node->_preUpdatePre(this);
    node->_render(this);
    return 0;
}

void GLESRenderContext3D::runOneCMD(GLESRenderCMD* cmd) {
    cmd->apply(this);
}

void GLESRenderContext3D::runCMDList(const std::vector<GLESRenderCMD*>& cmds) {
    for (GLESRenderCMD* i : cmds) {
        i->apply(this);
    }
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
    LayaGL::m_pWebglEngine->scissorTest(true);
    LayaGL::m_pWebglEngine->viewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
    LayaGL::m_pWebglEngine->scissor(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
    if (this->_clearFlag != static_cast<RenderClearFlagBits>(RenderClearFlag::Nothing))
    {
        LayaGL::m_pWebglEngine->clearRenderTexture(_clearFlag, &clearColor, clearDepth);
    }
    LayaGL::m_pWebglEngine->scissor(scissor.x, scissor.y, scissor.z, scissor.w);
}
void GLESRenderContext3D::_end()
{
}
} // namespace laya

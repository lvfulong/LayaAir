#include "GLESRenderGeometryElement.h"
#include "JCConch.h"
#include "JCConchRender.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h"
namespace laya
{
GLESRenderGeometryElement::GLESRenderGeometryElement(MeshTopology mode, DrawType drawType)
    : /*ResourceBase(JCConch::s_pConchRender->m_pRenderGeometryElementManager), */m_nRenderMode(mode), m_nDrawType(drawType)
{
    m_pDrawParams = new laya::JCSingletonList<int>(false);
}
GLESRenderGeometryElement::~GLESRenderGeometryElement()
{
    destroy();
}
void GLESRenderGeometryElement::setDrawArrayParams(int first, int count)
{
    m_pDrawParams->add(first);
    m_pDrawParams->add(count);
}
void GLESRenderGeometryElement::setDrawElementParams(int count, int offset)
{
    m_pDrawParams->add(offset);
    m_pDrawParams->add(count);
}
void GLESRenderGeometryElement::clearRenderParams()
{
    m_pDrawParams->resetLength();
}
void GLESRenderGeometryElement::destroy()
{
    _bufferState = nullptr;
    if (m_pDrawParams)
    {
        delete m_pDrawParams;
        m_pDrawParams = NULL;
    }
}
/*GLESRenderGeometryElement *GLESRenderGeometryElement::getRenderGeometryElement(uint32_t id)
{
    return JCConch::s_pConchRender->m_pRenderGeometryElementManager->getResource(id);
}*/
} // namespace laya

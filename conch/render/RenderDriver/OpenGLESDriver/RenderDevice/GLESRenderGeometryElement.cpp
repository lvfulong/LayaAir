#include "GLESRenderGeometryElement.h"
#include "JCConch.h"
#include "JCConchRender.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h"
namespace laya
{
uint32_t GLESRenderGeometryElement::_idCounter = 0;
GLESRenderGeometryElement::GLESRenderGeometryElement()
    /*: ResourceBase(JCConch::s_pConchRender->m_pRenderGeometryElementManager), */
{
    m_pDrawParams = new laya::JCSingletonList<int>(false);
    _id = GLESRenderGeometryElement::_idCounter++;
    
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
void GLESRenderGeometryElement::cloneTo(GLESRenderGeometryElement* geometry)
{
    geometry->setMeshTopology(getMeshTopology());
    geometry->setDrawType(getDrawType());
    geometry->setIndexFormat(getIndexFormat());
    geometry->setInstanceCount(getInstanceCount());
    geometry->m_pDrawParams->m_vElements = m_pDrawParams->m_vElements;
    geometry->m_pDrawParams->setLength(m_pDrawParams->getLength());
  
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

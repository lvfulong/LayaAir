#ifndef __GLESRenderGeometryElement_H__
#define __GLESRenderGeometryElement_H__
#include "render/LayaGL.h"
#include <render/3D/design/renderEnum/DrawType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/3D/design/renderEnum/RenderPologyMode.h>
#include <stdio.h>
#include <utils/JCSingletonList.h>
#include <utils/Preprocessor.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderDrawContext.h"
namespace laya
{
class GLESBufferState;

class GLESRenderGeometryElement // : public ResourceBase<GLESRenderGeometryElement>
{
public:
    static uint32_t _idCounter;
  public:
    GLESRenderGeometryElement();
    ~GLESRenderGeometryElement();
    void setDrawArrayParams(int first, int count);
    void setDrawElementParams(int count, int offset);
    void clearRenderParams();
    void cloneTo(GLESRenderGeometryElement* geometry);
    void destroy();
    // static GLESRenderGeometryElement *getRenderGeometryElement(uint32_t id);
    void setIndexFormat(IndexFormat indexFormat)
    {
        _indexFormat = indexFormat;
        this->_glindexFormat = LayaGL::m_pWebglEngine->getDrawContext()->getIndexType(this->_indexFormat);
    }
    IndexFormat getIndexFormat()
    {
        return _indexFormat;
    }
    void setMeshTopology(MeshTopology nMode)
    {
        _mode = nMode;
        this->_glmode = LayaGL::m_pWebglEngine->getDrawContext()->getMeshTopology(this->_mode);
    }
    MeshTopology getMeshTopology()
    {
        return _mode;
    }
    void setDrawType(DrawType nDrawType)
    {
        m_nDrawType = nDrawType;
    }
    DrawType getDrawType()
    {
        return m_nDrawType;
    }
    void setInstanceCount(int number)
    {
        m_nInstanceCount = number;
    }
    int getInstanceCount()
    {
        return m_nInstanceCount;
    }
    void setBufferState(GLESBufferState *value)
    {
        _bufferState = value;
    }



  public:
    GLESBufferState *_bufferState = nullptr;
    MeshTopology _mode;
    DrawType m_nDrawType;
    IndexFormat _indexFormat;
    int m_nInstanceCount;
    laya::JCSingletonList<int> *m_pDrawParams;
    GLenum _glindexFormat;
    GLenum _glmode;
    uint32_t _id;
};
} // namespace laya
#endif //__GLESRenderGeometryElement_H__

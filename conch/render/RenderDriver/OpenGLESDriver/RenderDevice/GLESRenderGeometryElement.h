#ifndef __GLESRenderGeometryElement_H__
#define __GLESRenderGeometryElement_H__
#include <render/3D/design/renderEnum/DrawType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/3D/design/renderEnum/RenderPologyMode.h>
#include <stdio.h>
#include <utils/JCSingletonList.h>
#include <utils/Preprocessor.h>

namespace laya
{
class GLESBufferState;
class GLESRenderGeometryElement// : public ResourceBase<GLESRenderGeometryElement>
{
  public:
    GLESRenderGeometryElement(MeshTopology mode, DrawType drawType);
    ~GLESRenderGeometryElement();
    void setDrawArrayParams(int first, int count);
    void setDrawElementParams(int count, int offset);
    void clearRenderParams();
    void destroy();
    // static GLESRenderGeometryElement *getRenderGeometryElement(uint32_t id);
    void setIndexFormat(IndexFormat indexFormat)
    {
        m_nIndexFormat = indexFormat;
        //TODO this->_glindexFormat = (LayaGL.renderDrawContext as GLRenderDrawContext).getIndexType(this._indexFormat);
    }
    IndexFormat getIndexFormat()
    {
        return m_nIndexFormat;
    }
    void setMeshTopology(MeshTopology nMode)
    {
        m_nRenderMode = nMode;
        //TODO this->_glmode = (LayaGL.renderDrawContext as GLRenderDrawContext).getMeshTopology(this._mode);
    }
    MeshTopology getMeshTopology()
    {
        return m_nRenderMode;
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
    GLESBufferState *_bufferState;
    MeshTopology m_nRenderMode;
    DrawType m_nDrawType;
    IndexFormat m_nIndexFormat;
    int m_nInstanceCount;
    laya::JCSingletonList<int> *m_pDrawParams;
    int _glindexFormat;
    int _glmode;
};
} // namespace laya
#endif //__GLESRenderGeometryElement_H__

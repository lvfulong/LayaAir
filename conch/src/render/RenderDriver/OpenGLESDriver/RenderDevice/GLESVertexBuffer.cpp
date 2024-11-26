#include "GLESVertexBuffer.h"
#include "render/LayaGL.h"

namespace laya
{
GLESVertexBuffer::GLESVertexBuffer(BufferTargetType targetType, BufferUsage bufferUsageType)
{
    _glBuffer = (GLBuffer *)LayaGL::m_pWebglEngine->createBuffer(BufferTargetType::ARRAY_BUFFER, bufferUsageType);
    LayaGL::m_pWebglEngine->_addStatisticsInfo(GPUEngineStatisticsInfo::RC_VertexBuffer, 1);
}
GLESVertexBuffer::~GLESVertexBuffer()
{
    if (this->_glBuffer != nullptr)
    {
        delete this->_glBuffer;
        this->_glBuffer = nullptr;
    }
}

void GLESVertexBuffer::_changeMemory(int bytelength) {
    LayaGL::m_pWebglEngine->_addStatisticsInfo(GPUEngineStatisticsInfo::M_VertexBuffer, -_glBuffer->m_byteLength + bytelength);
}


void GLESVertexBuffer::destroy()
{
    _glBuffer->destroy();
    _changeMemory(0);
    if (LayaGL::m_pWebglEngine != nullptr)
    {
        LayaGL::m_pWebglEngine->_addStatisticsInfo(GPUEngineStatisticsInfo::RC_VertexBuffer, -1);
    }
}
void GLESVertexBuffer::setDataLength(int byteLength)
{
    _changeMemory(byteLength);
    this->_glBuffer->setDataLength(byteLength);
}

void GLESVertexBuffer::setData(const char *buffer, int bufferBytes, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
    double dataCount /*= Number.MAX_SAFE_INTEGER*/)
{
    bind();
    bool needSubData = dataStartIndex != 0 || static_cast<int64_t>(dataCount) != 9007199254740991; /* || dataCount != Number.MAX_SAFE_INTEGER*/;

    
    if (needSubData)
    {
        if (static_cast<int64_t>(dataCount) == 9007199254740991)
        {
            dataCount = bufferBytes;
        }
        this->_glBuffer->setData((const char *)(buffer + dataStartIndex), dataCount, bufferOffset);
    }
    else
    {
        this->_glBuffer->setData((const char *)(buffer), bufferBytes, bufferOffset);
    }
}
void GLESVertexBuffer::bind()
{
    _glBuffer->bindBuffer();
}
void GLESVertexBuffer::unbind()
{
    _glBuffer->unbindBuffer();
}
void GLESVertexBuffer::orphanStorage()
{
    bind();
    this->_glBuffer->setDataLength(this->_glBuffer->m_byteLength);
}
void GLESVertexBuffer::setDataJS(jsbind::ArrayBuffer arrayBuffer, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
                                 double dataCount /*= Number.MAX_SAFE_INTEGER*/)
{
    DEBUG_CHECK(arrayBuffer.isValid());
    setData(reinterpret_cast<const char*>(arrayBuffer.getData()), arrayBuffer.getByteLength(), bufferOffset, dataStartIndex, dataCount);
}
} // namespace laya
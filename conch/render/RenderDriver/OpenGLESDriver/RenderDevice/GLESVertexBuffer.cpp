#include "GLESVertexBuffer.h"
#include "render/LayaGL.h"

namespace laya
{
GLESVertexBuffer::GLESVertexBuffer(BufferTargetType targetType, BufferUsage bufferUsageType)
{
    _glBuffer = (GLBuffer *)LayaGL::m_pWebglEngine->createBuffer(BufferTargetType::ARRAY_BUFFER, bufferUsageType);
}
GLESVertexBuffer::~GLESVertexBuffer()
{
    if (this->_glBuffer != nullptr)
    {
        delete this->_glBuffer;
        this->_glBuffer = nullptr;
    }
}
void GLESVertexBuffer::destroy()
{
    this->_glBuffer->destroy();
}
void GLESVertexBuffer::setDataLength(int byteLength)
{
    this->_glBuffer->setDataLength(byteLength);
}

void GLESVertexBuffer::setData(const char *buffer, int bufferBytes, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
    double dataCount /*= Number.MAX_SAFE_INTEGER*/)
{
    bind();
    bool needSubData = dataStartIndex != 0 || static_cast<int64_t>(dataCount) != 9007199254740991; /* || dataCount != Number.MAX_SAFE_INTEGER*/;

    if (static_cast<int64_t>(dataCount) == 9007199254740991)
    {
        dataCount = bufferBytes;
    }
    if (needSubData)
    {
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
void GLESVertexBuffer::setDataJS(JSValueAsParam data, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
                                 double dataCount /*= Number.MAX_SAFE_INTEGER*/)
{
    char *pArrayBufferPtr = NULL;
    int nABLen = 0;
    bool bIsArrayBuffer = extractJSAB(data, pArrayBufferPtr, nABLen);
    if (bIsArrayBuffer)
    {
        setData(pArrayBufferPtr, nABLen, bufferOffset, dataStartIndex, dataCount);
    }
}
} // namespace laya
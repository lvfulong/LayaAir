#include "GLESIndexBuffer.h"
#include "render/LayaGL.h"
#include <cassert>
#include <render/3D/design/renderEnum/BufferTargetType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h>

namespace laya
{
GLESIndexBuffer::GLESIndexBuffer(BufferTargetType targetType, BufferUsage bufferUsageType)
{
    this->_glBuffer = (GLBuffer *)LayaGL::m_pWebglEngine->createBuffer(targetType, bufferUsageType);
}
GLESIndexBuffer::~GLESIndexBuffer()
{
    destroy();
}
void GLESIndexBuffer::_setIndexDataLength(uint32_t data)
{
    GLESBufferState *curBufSta = GLESBufferState::_curBindedBufferState;

    if (curBufSta)
    {
        if (curBufSta->_bindedIndexBuffer == this)
        {
            this->_glBuffer->setDataLength(0);
        }
        else
        {
            curBufSta->unBind();
            this->_glBuffer->bindBuffer();
            this->_glBuffer->setDataLength(data);
            curBufSta->bind();
        }
    }
    else
    {
        this->_glBuffer->bindBuffer();
        this->_glBuffer->setDataLength(data);
    }
}
void GLESIndexBuffer::_setIndexData(char *data, uint32_t byteLength, uint32_t bufferOffset)
{

    GLESBufferState *curBufSta = GLESBufferState::_curBindedBufferState;

    if (curBufSta)
    {
        if (curBufSta->_bindedIndexBuffer == this)
        {
            this->_glBuffer->setDataLength(0);
        }
        else
        {
            curBufSta->unBind();
            this->_glBuffer->bindBuffer();
            this->_glBuffer->setData(data, byteLength, bufferOffset);
            curBufSta->bind();
        }
    }
    else
    {
        this->_glBuffer->bindBuffer();
        this->_glBuffer->setData(data, byteLength, bufferOffset);
    }
}
void GLESIndexBuffer::_setIndexDataJS(JSValueAsParam buffer, uint32_t bufferOffset)
{
    char *data = NULL;
    int dataLength = 0;
    bool bIsArrayBuffer = extractJSAB(buffer, data, dataLength);
    if (bIsArrayBuffer)
    {
        this->_setIndexData(data, dataLength, bufferOffset);
    }
}
void GLESIndexBuffer::destroy()
{
    if (_glBuffer != nullptr)
    {
        this->_glBuffer->destroy();
        delete _glBuffer;
        _glBuffer = nullptr;
    }
}
} // namespace laya

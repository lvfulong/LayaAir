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
    if (LayaGL::m_pWebglEngine != nullptr)
    {
        LayaGL::m_pWebglEngine->_addStatisticsInfo(GPUEngineStatisticsInfo::RC_IndexBuffer, 1);
    }
}

GLESIndexBuffer::~GLESIndexBuffer()
{
    destroy();
}

void GLESIndexBuffer::_changeMemory(int bytelength) {
    if (LayaGL::m_pWebglEngine != nullptr)
    {
        LayaGL::m_pWebglEngine->_addStatisticsInfo(GPUEngineStatisticsInfo::M_IndexBuffer, -_glBuffer->m_byteLength + bytelength);
    }
}

void GLESIndexBuffer::_setIndexDataLength(uint32_t data)
{
    _changeMemory(data);
    GLESBufferState *curBufSta = GLESBufferState::_curBindedBufferState;
    if (curBufSta)
    {
        curBufSta->unBind();
        this->_glBuffer->bindBuffer();
        this->_glBuffer->setDataLength(data);
        curBufSta->bind();
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
        curBufSta->unBind();
        this->_glBuffer->bindBuffer();
        this->_glBuffer->setData(data, byteLength, bufferOffset);
        curBufSta->bind();
    }
    else
    {
        this->_glBuffer->bindBuffer();
        this->_glBuffer->setData(data, byteLength, bufferOffset);
    }
}

void GLESIndexBuffer::_setIndexDataJS(jsbind::ArrayBuffer arrayBuffer, uint32_t bufferOffset)
{
    DEBUG_CHECK(arrayBuffer.isValid());
    this->_setIndexData(reinterpret_cast<char*>(arrayBuffer.getData()), arrayBuffer.getByteLength(), bufferOffset);
}

void GLESIndexBuffer::destroy()
{
    if (_glBuffer != nullptr)
    {
        if (LayaGL::m_pWebglEngine != nullptr)
        {
            LayaGL::m_pWebglEngine->_addStatisticsInfo(GPUEngineStatisticsInfo::RC_IndexBuffer, -1);
        }
        _changeMemory(0);
        this->_glBuffer->destroy();
        delete _glBuffer;
        _glBuffer = nullptr;
       
    }
}
} // namespace laya

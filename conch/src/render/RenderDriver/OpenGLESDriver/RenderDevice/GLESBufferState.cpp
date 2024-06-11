#include "GLESBufferState.h"
#include "GLESIndexBuffer.h"
#include "GLESVertexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLVertexState.h"
#include "render/LayaGL.h"

namespace laya
{
GLESBufferState *GLESBufferState::_curBindedBufferState = nullptr;
GLESBufferState::GLESBufferState()
{
    this->_glVertexState = LayaGL::m_pWebglEngine->createVertexState();
}

GLESBufferState::~GLESBufferState()
{
}
void GLESBufferState::applyVertexBuffers()
{
    this->_glVertexState->applyVertexBuffer(this->_vertexBuffers);
}

void GLESBufferState::applyIndexBuffers()
{
    this->_glVertexState->applyIndexBuffer(this->_bindedIndexBuffer);
}
void GLESBufferState::bind()
{
    this->_glVertexState->bindVertexArray();
    GLESBufferState::_curBindedBufferState = this;
}
void GLESBufferState::applyState(const std::vector<GLESVertexBuffer *> &vertexBuffers, GLESIndexBuffer *indexBuffer)
{
    this->_vertexBuffers = vertexBuffers;
    this->_bindedIndexBuffer = indexBuffer;
    if (indexBuffer != nullptr)
    {
        indexBuffer->_glBuffer->unbindBuffer();
    }
    this->bind();
    this->applyVertexBuffers();
    this->applyIndexBuffers();
    this->unBind();
    if (indexBuffer != nullptr)
    {
        indexBuffer->_glBuffer->unbindBuffer();
    }
}
void GLESBufferState::unBind()
{
    if (GLESBufferState::_curBindedBufferState == this)
    {
        this->_glVertexState->unbindVertexArray();
        GLESBufferState::_curBindedBufferState = nullptr;
    }
    else
    {
        assert("BufferState: must call bind() function first.");
    }
}
bool GLESBufferState::isBind()
{
    return (GLESBufferState::_curBindedBufferState == this);
}
void GLESBufferState::destroy()
{
    if (GLESBufferState::_curBindedBufferState == this) {
        _glVertexState->unbindVertexArray();
        GLESBufferState::_curBindedBufferState = nullptr;
    }
    this->_glVertexState->destroy();
    this->_vertexBuffers.clear();
    this->_bindedIndexBuffer = nullptr;
}
} // namespace laya

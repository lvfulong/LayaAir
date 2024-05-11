#include "GLVertexState.h"
#include <cassert>

namespace laya
{
GLVertexState::GLVertexState(GLESEngine *engine) : GLObject(engine)
{
    if (!engine->isWebGL2())
        m_vaoExt = (OESVertexArrayObjectExt *)engine->getExtension(WebGLExtension::OES_vertex_array_object);
    createVertexArray();
    m_angleInstancedArrays = (ANGLEInstancedArraysExt *)engine->getExtension(WebGLExtension::ANGLE_instanced_arrays);
}
GLVertexState::~GLVertexState()
{
    destroy();
}
void GLVertexState::createVertexArray()
{
    if (m_engine->isWebGL2())
        glGenVertexArrays(1, &m_vao);
    else
        m_vao = m_vaoExt->createVertexArrayOES();
}
void GLVertexState::deleteVertexArray()
{
    if (m_engine->isWebGL2())
        glDeleteVertexArrays(1, &m_vao);
    else
        m_vaoExt->deleteVertexArrayOES(m_vao);
}

void GLVertexState::bindVertexArray()
{
    if (m_engine->m_GLBindVertexArray == this)
        return;
    if (m_engine->isWebGL2())
        glBindVertexArray(m_vao);
    else
        m_vaoExt->bindVertexArrayOES(m_vao);
    m_engine->m_GLBindVertexArray = this;
}

void GLVertexState::unbindVertexArray()
{
    if (m_engine->isWebGL2())
        glBindVertexArray(0);
    else
        m_vaoExt->bindVertexArrayOES(0);
    m_engine->m_GLBindVertexArray = nullptr;
}

void GLVertexState::applyVertexBuffer(const std::vector<GLESVertexBuffer *> &vertexBuffers)
{
    // Clear front VAO
    this->clearVAO();
    // this->_vertexBuffers = vertexBuffer;
    if (m_engine->m_GLBindVertexArray == this)
    {
        _vertexDeclaration.resize(vertexBuffers.size());
        for (int i = 0, size = vertexBuffers.size(); i < size; i++)
        {
            GLESVertexBuffer *element = vertexBuffers[i];
            std::unordered_map<int32_t, VertexStateContext> &verDec = element->_shaderValues;
            _vertexDeclaration[i] = verDec;
            element->bind();
            for (std::unordered_map<int32_t, VertexStateContext>::iterator it = verDec.begin(); it != verDec.end(); it++)
            {
                GLuint loc = it->first;
                VertexStateContext &attribute = it->second;
                glEnableVertexAttribArray(loc);
                glVertexAttribPointer(loc, attribute.elementCount, attribute.elementType,
                                      (attribute.normalized > 0) ? GL_TRUE : GL_FALSE, attribute.vertexStride,
                                      ((const void *)attribute.elementOffset));
                if (element->_instanceBuffer)
                    this->vertexAttribDivisor(loc, 1);
            }
        }
    }
    else
    {
        assert("BufferState: must call bind() function first.");
    }
}
void GLVertexState::clearVAO()
{
    for (int i = 0, n = this->_vertexDeclaration.size(); i < n; i++)
    {
        std::unordered_map<int32_t, VertexStateContext> &verDec = this->_vertexDeclaration[i];
        for (std::unordered_map<int32_t, VertexStateContext>::iterator it = verDec.begin(); it != verDec.end(); it++)
        {
            glDisableVertexAttribArray(it->first);
        }
    }
}
void GLVertexState::applyIndexBuffer(GLESIndexBuffer *indexBuffer)
{
    // 需要强制更新IndexBuffer

    if (indexBuffer == nullptr)
    {
        return;
    }
    if (m_engine->m_GLBindVertexArray == this)
    {
        if (this->_bindedIndexBuffer != indexBuffer)
        {
            indexBuffer->_glBuffer->bindBuffer(); // TODO:可和vao合并bind
            this->_bindedIndexBuffer = indexBuffer;
        }
    }
    else
    {
        assert("BufferState: must call bind() function first.");
    }
}
void GLVertexState::applyVertexBufferTemp2d(int stride, const std::vector<AttribInfo> &attribInfo,
                                            VertexBuffer2D *vertexBuffers)
{
    // this._vertexBuffers = vertexBuffer;
    if (m_engine->m_GLBindVertexArray == this)
    {
        VertexBuffer2D *element = vertexBuffers;
        element->bind();
        int attribNum = attribInfo.size();
        for (int i = 0; i < attribNum; i++)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(
                i, attribInfo[i]._size, attribInfo[i]._type, GL_FALSE, stride,
                (const void *)attribInfo[i]._off); // 注意 normalize都设置为false了，想必没人要用这个功能把。
        }
    }
    else
    {
        assert("BufferState: must call bind() function first.");
    }
}
void GLVertexState::applyIndexBufferTemp2d(IndexBuffer2D *indexBuffer)
{
    // 需要强制更新IndexBuffer

    if (indexBuffer == nullptr)
    {
        return;
    }
    if (m_engine->m_GLBindVertexArray == this)
    {
        // if (m_bindedIndexBuffer2D != indexBuffer)
        {
            indexBuffer->bind(); // TODO:可和vao合并bind
            // m_bindedIndexBuffer2D = indexBuffer;
        }
    }
    else
    {
        assert("BufferState: must call bind() function first.");
    }
}
void GLVertexState::vertexAttribDivisor(int index, int divisor)
{
    if (m_engine->isWebGL2())
        glVertexAttribDivisor(index, divisor);
    else
        m_angleInstancedArrays->vertexAttribDivisorANGLE(index, divisor);
}
void GLVertexState::destroy()
{
    if (!m_destroyed)
    {
        deleteVertexArray();
        m_engine = nullptr;
        m_destroyed = true;
    }
}
} // namespace laya
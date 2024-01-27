#include "GLVertexState.h"
#include <cassert>

namespace laya
{
GLVertexState::GLVertexState(WebGLEngine *engine) : GLObject(engine)
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
    this.clearVAO();
    // this->_vertexBuffers = vertexBuffer;
    if (m_engine->m_GLBindVertexArray == this)
    {
        for (int i = 0, size = vertexBuffers.size(); i < size; i++)
        {
            GLESVertexBuffer *element = vertexBuffers[i];
            int n = element->m_nVertextDeclarationNum;
            element->bind();
            for (int j = 0; j < n; j++)
            {
                VertexDeclaration *verDec = &(element->m_pVertextDeclaration[j]);
                glEnableVertexAttribArray(verDec->location);
                glVertexAttribPointer(verDec->location, verDec->size, verDec->type,
                                      (verDec->normalize > 0) ? GL_TRUE : GL_FALSE, verDec->stride,
                                      ((const void *)verDec->offset));
                if (element->m_bInstanceBuffer)
                    this->vertexAttribDivisor(verDec->location, 1);
            }
        }
    }
    else
    {
        assert("BufferState: must call bind() function first.");
    }
}
clearVAO()
{
    for (let i = 0, n = this._vertexDeclaration.length; i < n; i++)
    {
        var verDec : VertexDeclaration = this._vertexDeclaration[i];
        var valueData : any = verDec._shaderValues;
        for (var k in valueData)
        {
            var loc : number = parseInt(k);
            this._gl.disableVertexAttribArray(loc);
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
            indexBuffer->bind(); // TODO:可和vao合并bind
            this->_bindedIndexBuffer = indexBuffer;
        }
    }
    else
    {
        assert("BufferState: must call bind() function first.");
    }
}
/*void GLVertexState::applyVertexBuffer(int stride, const std::vector<AttribInfo>& attribInfo, VertexBuffer2D*
vertexBuffers)
{
    //this._vertexBuffers = vertexBuffer;
    if (m_engine->m_GLBindVertexArray == this)
    {
        VertexBuffer2D* element = vertexBuffers;
        element->bind();
        int attribNum = attribInfo.size();
        for (int i = 0; i < attribNum; i++)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, attribInfo[i]._size, attribInfo[i]._type, GL_FALSE, stride, (const
void*)attribInfo[i]._off); //注意 normalize都设置为false了，想必没人要用这个功能把。
        }
    }
    else
    {
        assert("BufferState: must call bind() function first.");
    }
}*/
/*void GLVertexState::applyIndexBuffer(IndexBuffer2D* indexBuffer)
{
    //需要强制更新IndexBuffer

    if (indexBuffer == nullptr)
    {
        return;
    }
    if (m_engine->m_GLBindVertexArray == this)
    {
        //if (m_bindedIndexBuffer2D != indexBuffer)
        {
            indexBuffer->bind();//TODO:可和vao合并bind
            //m_bindedIndexBuffer2D = indexBuffer;
        }
    }
    else
    {
        assert("BufferState: must call bind() function first.");
    }
}*/
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
}
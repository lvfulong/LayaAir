#include "GLESVertexBuffer.h"
#include "render/driver/gles/LayaGL.h"

namespace laya
{
GLESVertexBuffer::GLESVertexBuffer(BufferTargetType targetType, BufferUsage bufferUsageType)
{
    _glBuffer = (GLBuffer *)LayaGL::m_pWebglEngine->createBuffer(BufferTargetType::ARRAY_BUFFER, bufferUsageType);
}
GLESVertexBuffer::~GLESVertexBuffer()
{
    if (m_pVertextDeclaration)
    {
        delete[] m_pVertextDeclaration;
        m_pVertextDeclaration = NULL;
    }

    if (this->_glBuffer != nullptr)
    {
        delete this->_glBuffer;
        this->_glBuffer = nullptr;
    }
}
void GLESVertexBuffer::destory()
{
    this->_glBuffer->destroy();
    // this._vertexDeclaration = null
}
void GLESVertexBuffer::setDataLength(int byteLength)
{
    this->_glBuffer->setDataLength(byteLength);
}

void GLESVertexBuffer::setData(const char *buffer, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
                               int dataCount /*= Number.MAX_SAFE_INTEGER*/)
{
    bind();
    bool needSubData = dataStartIndex != 0 /* || dataCount != Number.MAX_SAFE_INTEGER*/;
    if (needSubData)
    {
        this->_glBuffer->setData((const char *)(buffer + dataStartIndex), dataCount, bufferOffset);
    }
    else
    {
        this->_glBuffer->setData((const char *)(buffer), dataCount, bufferOffset);
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
    this->_glBuffer->setDataLength(this->_glBuffer->_byteLength);
}

void GLESVertexBuffer::setVertexDeclaration(int *declaration, int intLength)
{
    if (m_pVertextDeclaration != NULL)
    {
        delete[] m_pVertextDeclaration;
        m_pVertextDeclaration = NULL;
    }
    // TEST
    int n = m_nVertextDeclarationNum = intLength / sizeof(VertexDeclaration);
    m_pVertextDeclaration = new VertexDeclaration[n];
    int num = 0;
    for (int i = 0; i < n; i++)
    {
        m_pVertextDeclaration[i].location = declaration[num];
        num++;
        m_pVertextDeclaration[i].size = declaration[num];
        num++;
        m_pVertextDeclaration[i].type = declaration[num];
        num++;
        m_pVertextDeclaration[i].normalize = declaration[num];
        num++;
        m_pVertextDeclaration[i].stride = declaration[num];
        num++;
        m_pVertextDeclaration[i].offset = declaration[num];
        num++;
    }
}
} // namespace laya
//------------------------------------------------------------------------------

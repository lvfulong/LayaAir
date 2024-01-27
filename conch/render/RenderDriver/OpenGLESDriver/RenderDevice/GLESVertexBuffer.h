#ifndef __GLESVertexBuffer_H__
#define __GLESVertexBuffer_H__

#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.h>
#include <stdio.h>
#include <utils/Preprocessor.h>

namespace laya
{
struct VertexDeclaration
{
    int location;
    int size;
    int type;
    int normalize;
    int stride;
    int offset;
};
class GLESVertexBuffer // : public VertexBuffer
{
  public:
    GLESVertexBuffer(BufferTargetType targetType, BufferUsage bufferUsageType);
    ~GLESVertexBuffer();
    void GLESVertexBuffer::destory();
    void GLESVertexBuffer::setDataLength(int byteLength);

    void GLESVertexBuffer::setData(const char *buffer, int bufferOffset /* = 0*/, int dataStartIndex /* = 0*/,
                                   int dataCount /*= Number.MAX_SAFE_INTEGER*/);
    void GLESVertexBuffer::bind();
    void GLESVertexBuffer::unbind();
    void GLESVertexBuffer::orphanStorage();

    void GLESVertexBuffer::setVertexDeclaration(int *declaration, int intLength);

  public:
    GLBuffer *_glBuffer;
    VertexDeclaration *m_pVertextDeclaration = nullptr;
    int m_nVertextDeclarationNum = 0;
    ;
    bool _instanceBuffer = false;
};
} // namespace laya
#endif //__GLESVertexBuffer_H__

#ifndef __GLVertexState_H__
#define __GLVertexState_H__

#include "GLCapable.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h"
#include "LayaAir/2D/IndexBuffer2D.h"
#include "LayaAir/2D/VertexBuffer2D.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESIndexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.h"
#include <string>
#include <utils/Preprocessor.h>
namespace laya
{
struct AttribInfo
{
    uint32_t _type;
    uint32_t _size;
    uint32_t _off;
};

class GLVertexState : public GLObject
{
  public:
    GLVertexState(GLESEngine *engine);
    virtual ~GLVertexState();
    void applyVertexBuffer(const std::vector<GLESVertexBuffer *> &vertexBuffers);

    void applyIndexBuffer(GLESIndexBuffer *indexBuffer);

    void applyVertexBufferTemp2d(int stride, const std::vector<AttribInfo> &attribInfo,
                                 VertexBuffer2D *vertexBuffers); // TODO

    void applyIndexBufferTemp2d(IndexBuffer2D *indexBuffer); // TODO
    void destroy();

  private:
    friend class BufferStateBase;
    friend class GLESBufferState;
    void createVertexArray();

    void deleteVertexArray();

    void bindVertexArray();
    void unbindVertexArray();

    /*isVertexArray(): void {
        if (this._engine.isWebGL2)
            (<WebGL2RenderingContext>this._gl).isVertexArray(this._vao);
        else
            this._vaoExt.isVertexArrayOES(this._vao);
    }*/

    void vertexAttribDivisor(int index, int divisor);
    void clearVAO();

  private:
    ANGLEInstancedArraysExt *m_angleInstancedArrays;
    OESVertexArrayObjectExt *m_vaoExt;
    GLenum m_vao;
    std::vector<std::unordered_map<int32_t, VertexStateContext>> _vertexDeclaration;
    // IndexBuffer* m_bindedIndexBuffer;
    //_vertexBuffers: VertexBuffer[];
    GLESIndexBuffer *_bindedIndexBuffer = nullptr;
};
} // namespace laya
#endif
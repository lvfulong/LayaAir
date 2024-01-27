#ifndef __GLVertexState_H__
#define __GLVertexState_H__

#include "GLCapable.h"
#include "GLObject.h"
#include <string>
#include <utils/Preprocessor.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESIndexBuffer.h"
namespace laya
{
   /* struct AttribInfo
    {
        uint32_t _type;
        uint32_t _size;
        uint32_t _off;
    };*/

class GLVertexState : public GLObject
{
  public:
    GLVertexState(WebGLEngine *engine);
    ~GLVertexState();
    void applyVertexBuffer(const std::vector<GLESVertexBuffer *> &vertexBuffers) ;

    void applyIndexBuffer(GLESIndexBuffer *indexBuffer);

    // void applyVertexBuffer(int stride, const std::vector<AttribInfo>& attribInfo, VertexBuffer2D* vertexBuffers);

    // void applyIndexBuffer(IndexBuffer2D* indexBuffer);
    void destroy();

  private:
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

  private:
    ANGLEInstancedArraysExt *m_angleInstancedArrays;
    OESVertexArrayObjectExt *m_vaoExt;
    GLenum m_vao;
    std::vector<VertexDeclaration> _vertexDeclaration_vertexDeclaration;
    // IndexBuffer* m_bindedIndexBuffer;
    //_vertexBuffers: VertexBuffer[];
    GLESIndexBuffer *_bindedIndexBuffer = nullptr;
};
} // namespace laya
#endif
#ifndef __GLESBufferState_H__
#define __GLESBufferState_H__

#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLBuffer.h>
#include <stdio.h>
#include <utils/Preprocessor.h>

namespace laya
{
class GLESIndexBuffer;
class GLESVertexBuffer;
class GLVertexState;
class GLESBufferState //: public IBufferState
{
  public:
    GLESBufferState();
    ~GLESBufferState();
    void bind();
    void applyState(const std::vector<GLESVertexBuffer *> &vertexBuffers, GLESIndexBuffer *indexBuffer);
    void unBind();
    bool isBind();
    void destroy();

  private:
    void applyVertexBuffers();
    void applyIndexBuffers();

  public:
    GLESIndexBuffer *_bindedIndexBuffer;
    std::vector<GLESVertexBuffer *> _vertexBuffers;
    GLVertexState *_glVertexState;
    static GLESBufferState *_curBindedBufferState;
};
} // namespace laya
#endif
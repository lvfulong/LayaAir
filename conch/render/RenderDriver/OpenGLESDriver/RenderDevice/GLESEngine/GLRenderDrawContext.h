#ifndef __GLRenderDrawContext_H__
#define __GLRenderDrawContext_H__

#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h"
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <render/3D/design/renderEnum/RenderPologyMode.h>

namespace laya
{
class ANGLEInstancedArraysExt;
class WebGLEngine;
class GLESRenderGeometryElement;
class GLRenderDrawContext : public GLObject
{
  public:
    GLRenderDrawContext(WebGLEngine *engine);
    void drawElementsInstanced(int glmode, int count, int gltype, int offset, int instanceCount);
    void drawArraysInstanced(int glmode, int first, int count, int instanceCount);
    void drawArrays(int glmode, int first, int count);
    void drawElements(int glmode, int count, int gltype, int offset);
    void drawGeometryElement(GLESRenderGeometryElement *geometryElement);
    void drawElements2DTemp(MeshTopology mode, int count, IndexFormat type, int offset);
    GLenum getMeshTopology(MeshTopology mode);
    GLenum getIndexType(IndexFormat type);

  private:
    ANGLEInstancedArraysExt *m_angleInstancedArrays;
};
} // namespace laya
#endif //__GLRenderDrawContext_H__

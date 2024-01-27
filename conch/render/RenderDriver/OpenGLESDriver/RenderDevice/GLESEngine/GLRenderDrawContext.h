#ifndef __GLRenderDrawContext_H__
#define __GLRenderDrawContext_H__

#include "GLObject.h"
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
    void drawElementsInstanced(int glmode, int count, IndexFormat type, int offset, int instanceCount);
    void drawArraysInstanced(int glmode, int first, int count, int instanceCount);
    void drawArrays(int glmode, int first, int count);
    void drawElements(int glmode, int count, IndexFormat type, int offset);
    void drawGeometryElement(GLESRenderGeometryElement *geometryElement);
    void drawElements2DTemp(MeshTopology mode, int count, IndexFormat type, int offset);

  private:
    ANGLEInstancedArraysExt *m_angleInstancedArrays;
};
} // namespace laya
#endif //__GLRenderDrawContext_H__

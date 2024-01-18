#ifndef __LayaGL_H__
#define __LayaGL_H__

#include <render/driver/gles/WebGLEngine.h>
#include <render/driver/gles/GLTextureContext.h>
namespace laya
{
class LayaGL
{
  public:
    static WebGLEngine *m_pWebglEngine;
};
} // namespace laya
#endif
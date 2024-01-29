#ifndef __LayaGL_H__
#define __LayaGL_H__

#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLTextureContext.h>
namespace laya
{
class LayaGL
{
  public:
    static GLESEngine *m_pWebglEngine;
};
} // namespace laya
#endif
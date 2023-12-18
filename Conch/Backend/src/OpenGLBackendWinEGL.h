#ifndef __OPENGL_BACKEND_WIN_EGL_H__
#define __OPENGL_BACKEND_WIN_EGL_H__

#include "Backend.h"
#include <memory>
#include "esUtil.h"

namespace laya
{

class OpenGLBackendWinEGL
{
  public:
    OpenGLBackendWinEGL();
    ~OpenGLBackendWinEGL();
    void create(const BackendOptions &options);
    void makeCurrent();
    void swapBuffer();
    void createScreenSurface(void *nativeHandle);
    void destroyScreenSurface();
    void onScreenSurfaceResize(int width, int height);
    void preserveBackBuffer();
    void resetRenderStates();

  private:
    ESContext m_esContext;
    ;
};
} // namespace laya
#endif

#ifndef __OPENGL_BACKEND_X11_EGL_H__
#define __OPENGL_BACKEND_X11_EGL_H__

#include "Backend.h"
#include <memory>

namespace laya
{
class OpenGLBackendLinuxEGLX11Impl;
class OpenGLBackendLinuxEGLX11
{
  public:
    OpenGLBackendLinuxEGLX11();
    ~OpenGLBackendLinuxEGLX11();
    void create(const BackendOptions &options);
    void makeCurrent();
    void swapBuffer();
    void createScreenSurface(void *nativeHandle);
    void destroyScreenSurface();
    void onScreenSurfaceResize(int width, int height);
    void preserveBackBuffer();
    void resetRenderStates();

  private:
    std::unique_ptr<OpenGLBackendLinuxEGLX11Impl> m_impl;
};
} // namespace laya
#endif

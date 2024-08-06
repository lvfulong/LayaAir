#ifndef __OPENGL_BACKEND_X11_EGL_H__
#define __OPENGL_BACKEND_X11_EGL_H__

#include "backend/IBackend.h"
#include <memory>

namespace laya
{
class OpenGLBackendLinuxEGLX11Impl;
class OpenGLBackendLinuxEGLX11 : public IBackend
{
  public:
    OpenGLBackendLinuxEGLX11();
    ~OpenGLBackendLinuxEGLX11();
    void create(const BackendOptions &options) override;
    void makeCurrent() override;
    void swapBuffer() override;
    void createScreenSurface(void *nativeHandle) override;
    void destroyScreenSurface() override;
    void onScreenSurfaceResize(int width, int height) override;
    void preserveBackBuffer();
    void resetRenderStates();

  private:
    std::unique_ptr<OpenGLBackendLinuxEGLX11Impl> m_impl;
};
} // namespace laya
#endif

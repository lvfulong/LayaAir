#ifndef __OPENGL_BACKEND_ANDROID_EGL_H__
#define __OPENGL_BACKEND_ANDROID_EGL_H__

#include "backend/IBackend.h"
#include <memory>

namespace laya
{
class OpenGLBackendAndroidEGLImpl;
class OpenGLBackendAndroidEGL : public IBackend
{
  public:
    OpenGLBackendAndroidEGL();
    ~OpenGLBackendAndroidEGL();
    void create(const BackendOptions &options) override;
    void makeCurrent() override;
    void swapBuffer() override;
    void createScreenSurface(void *nativeHandle) override;
    void destroyScreenSurface() override;
    void onScreenSurfaceResize(int width, int height) override;
    void preserveBackBuffer();
    void resetRenderStates();

  private:
    void chooseConfig(const BackendOptions &options);
    std::unique_ptr<OpenGLBackendAndroidEGLImpl> m_impl;
};
} // namespace laya
#endif

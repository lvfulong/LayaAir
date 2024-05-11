#ifndef __OPENGL_BACKEND_ANDROID_EGL_H__
#define __OPENGL_BACKEND_ANDROID_EGL_H__

#include "Backend.h"
#include <memory>

namespace laya {
class OpenGLBackendAndroidEGLImpl;
class OpenGLBackendAndroidEGL {
public:
  OpenGLBackendAndroidEGL();
  ~OpenGLBackendAndroidEGL();
  void create(const BackendOptions &options);
  void makeCurrent();
  void swapBuffer();
  void createScreenSurface(void *nativeHandle);
  void destroyScreenSurface();
  void onScreenSurfaceResize(int width, int height);
  void preserveBackBuffer();
  void resetRenderStates();
private:
  void chooseConfig(const BackendOptions &options);
  std::unique_ptr<OpenGLBackendAndroidEGLImpl> m_impl;
};
} // namespace laya
#endif

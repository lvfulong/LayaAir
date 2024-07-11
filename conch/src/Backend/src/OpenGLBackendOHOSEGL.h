#ifndef __OPENGL_BACKEND_OHOS_EGL_H__
#define __OPENGL_BACKEND_OHOS_EGL_H__

#include "Backend.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <memory>
namespace laya
{

class OpenGLBackendOHOSEGL
{
  public:
    OpenGLBackendOHOSEGL();
    ~OpenGLBackendOHOSEGL();
    void create(const BackendOptions &options);
    void makeCurrent();
    void swapBuffer();
    void createScreenSurface(void *nativeHandle);
    void destroyScreenSurface();
    void onScreenSurfaceResize(int width, int height);
    void preserveBackBuffer();
    void resetRenderStates();

  public:
    int width_;
    int height_;

  private:
    EGLNativeWindowType mEglWindow;
    EGLDisplay mEGLDisplay = EGL_NO_DISPLAY;
    EGLConfig mEGLConfig = nullptr;
    EGLContext mEGLContext = EGL_NO_CONTEXT;
    EGLContext mSharedEGLContext = EGL_NO_CONTEXT;
    EGLSurface mEGLSurface = nullptr;
};
} // namespace laya
#endif

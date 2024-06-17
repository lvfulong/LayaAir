#ifndef __OPENGL_BACKEND_OHOS_EGL_H__
#define __OPENGL_BACKEND_OHOS_EGL_H__

#include "Backend.h"
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

  private:
    ;
};
} // namespace laya
#endif

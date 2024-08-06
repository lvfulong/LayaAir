#ifndef __OPENGL_BACKEND_WIN_EGL_H__
#define __OPENGL_BACKEND_WIN_EGL_H__

#include "backend/IBackend.h"
#include "esUtil.h"
#include <memory>

namespace laya
{

class OpenGLBackendWinEGL : public IBackend
{
  public:
    OpenGLBackendWinEGL();
    ~OpenGLBackendWinEGL();
    void create(const BackendOptions &options) override;
    void makeCurrent() override;
    void swapBuffer() override;
    void createScreenSurface(void *nativeHandle) override;
    void destroyScreenSurface() override;
    void onScreenSurfaceResize(int width, int height) override;
    void getScreenSurfaceSize(int *width, int *height);
    void preserveBackBuffer();
    void resetRenderStates();

  private:
    ESContext m_esContext;
    ;
};
} // namespace laya
#endif

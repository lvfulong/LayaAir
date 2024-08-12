#ifndef __OPENGL_BACKEND_IOS_H__
#define __OPENGL_BACKEND_IOS_H__

#include "backend/IBackend.h"
#include <memory>

namespace laya
{
class OpenGLBackendiOSImpl;
class OpenGLBackendiOS : public IBackend
{
  public:
    OpenGLBackendiOS();
    ~OpenGLBackendiOS();
    void create(const BackendOptions &options) override;
    void makeCurrent() override;
    void swapBuffer() override;

    void createScreenSurface(void *nativeHandle) override;
    void destroyScreenSurface() override;
    void onScreenSurfaceResize(int width, int height) override;
    void resetRenderStates();

  private:
    void bindScreenSurface();
    class std::unique_ptr<OpenGLBackendiOSImpl> m_impl;
};
} // namespace laya
#endif

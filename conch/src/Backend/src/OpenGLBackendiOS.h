#ifndef __OPENGL_BACKEND_IOS_H__
#define __OPENGL_BACKEND_IOS_H__

#include <memory>
#include "Backend.h"

namespace laya {
class OpenGLBackendiOSImpl;
class OpenGLBackendiOS {
public:
  OpenGLBackendiOS();
  ~OpenGLBackendiOS();
  void create(const BackendOptions &options);
  void makeCurrent();
  void swapBuffer();
  void bindScreenSurface();
  void createScreenSurface(void *nativeHandle);
  void destroyScreenSurface();
  void onScreenSurfaceResize(int width, int height);
  void resetRenderStates();
private:
  class std::unique_ptr<OpenGLBackendiOSImpl> m_impl;
};
} // namespace laya
#endif

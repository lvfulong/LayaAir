#include "OpenGLBackendWinEGL.h"

#include <utils/Log.h>
#include <vector>

namespace laya
{

OpenGLBackendWinEGL::OpenGLBackendWinEGL()
{
}
OpenGLBackendWinEGL::~OpenGLBackendWinEGL()
{
}

void OpenGLBackendWinEGL::create(const BackendOptions &options)
{
    m_esContext.hWnd = (HWND)options.nativeLayer;
   
}

void OpenGLBackendWinEGL::makeCurrent()
{
}

void OpenGLBackendWinEGL::swapBuffer()
{

    if (!eglSwapBuffers(m_esContext.eglDisplay, m_esContext.eglSurface))
    {
        LOGE("eglSwapBuffers failed %d", eglGetError());
    }
}
void OpenGLBackendWinEGL::createScreenSurface(void *nativeHandle)
{
    esCreateContext(&m_esContext, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
    // p_Windows.on(winWindows::Evt_WndResize,std::bind(gles_onEvent_WndResize,this,
    // std::placeholders::_1),m_pMyThread);
}
void OpenGLBackendWinEGL::destroyScreenSurface()
{
}
void OpenGLBackendWinEGL::onScreenSurfaceResize(int width, int height)
{
}

void OpenGLBackendWinEGL::getScreenSurfaceSize(int* width, int* height) {
    eglQuerySurface(m_esContext.eglDisplay, m_esContext.eglSurface, EGL_WIDTH, width);
    eglQuerySurface(m_esContext.eglDisplay, m_esContext.eglSurface, EGL_HEIGHT, height);
}

void OpenGLBackendWinEGL::preserveBackBuffer()
{
}
void OpenGLBackendWinEGL::resetRenderStates()
{
}
} // namespace laya

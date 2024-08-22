#include "OpenGLBackendWinEGL.h"
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <utils/Log.h>
#include <vector>


/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flag - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8
/// esCreateWindow flag - EGL_POST_SUB_BUFFER_NV supported.
#define ES_WINDOW_POST_SUB_BUFFER_SUPPORTED 16

namespace laya
{
struct OpenGLBackendWinEGLImpl
{
    GLint m_width = {0};
    GLint m_height = {0};
    EGLNativeWindowType m_hWnd;
    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
    EGLContext m_eglContext = EGL_NO_CONTEXT;
    EGLSurface m_eglSurface = EGL_NO_SURFACE;
    EGLConfig m_eglConfig = EGL_NO_CONFIG_KHR;
    bool m_preserveBackBuffer = true;
    GLuint m_flags = ES_WINDOW_RGB | ES_WINDOW_DEPTH;
};
OpenGLBackendWinEGL::OpenGLBackendWinEGL() : m_impl(new OpenGLBackendWinEGLImpl())
{
}

OpenGLBackendWinEGL::~OpenGLBackendWinEGL()
{
}

void OpenGLBackendWinEGL::create(const BackendOptions &options)
{
    m_impl->m_hWnd = (HWND)options.nativeLayer;

    GLuint flags = m_impl->m_flags;
    EGLint configAttribList[] = {EGL_RED_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_BLUE_SIZE, 5, EGL_ALPHA_SIZE,
                                 (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE, EGL_DEPTH_SIZE,
                                 (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE, EGL_STENCIL_SIZE,
                                 (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE, EGL_SAMPLE_BUFFERS,
                                 (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
                                 // EGL_SAMPLES,			2,
                                 EGL_NONE};

    RECT wndRect;
    GetClientRect(m_impl->m_hWnd, &wndRect);

    m_impl->m_width = wndRect.right - wndRect.left;
    m_impl->m_height = wndRect.bottom - wndRect.top;

    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE, EGL_NONE};
    m_impl->m_eglDisplay = eglGetDisplay(GetDC(m_impl->m_hWnd));
    if (m_impl->m_eglDisplay == EGL_NO_DISPLAY)
    {
        return;
    }
    if (!eglInitialize(m_impl->m_eglDisplay, &majorVersion, &minorVersion))
    {
        return;
    }

    // Bind to extensions
    /*eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
    eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)eglGetProcAddress("eglDestroyImageKHR");

    eglPostSubBufferNV = (PFNEGLPOSTSUBBUFFERNVPROC)eglGetProcAddress("eglPostSubBufferNV");

    glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)
    eglGetProcAddress("glEGLImageTargetTexture2DOES");

    glDeleteFencesNV = (PFNGLDELETEFENCESNVPROC) eglGetProcAddress("glDeleteFencesNV");
    glGenFencesNV = (PFNGLGENFENCESNVPROC) eglGetProcAddress("glGenFencesNV");
    glGetFenceivNV = (PFNGLGETFENCEIVNVPROC) eglGetProcAddress("glGetFenceivNV");
    glIsFenceNV = (PFNGLISFENCENVPROC) eglGetProcAddress("glIsFenceNV");
    glFinishFenceNV = (PFNGLFINISHFENCENVPROC) eglGetProcAddress("glFinishFenceNV");
    glSetFenceNV = (PFNGLSETFENCENVPROC) eglGetProcAddress("glSetFenceNV");
    glTestFenceNV = (PFNGLTESTFENCENVPROC) eglGetProcAddress("glTestFenceNV");
    */

    if (!eglGetConfigs(m_impl->m_eglDisplay, NULL, 0, &numConfigs))
    {
        return;
    }
    if (!eglChooseConfig(m_impl->m_eglDisplay, configAttribList, &m_impl->m_eglConfig, 1, &numConfigs))
    {
        return;
    }
    m_impl->m_eglContext = eglCreateContext(m_impl->m_eglDisplay, m_impl->m_eglConfig, EGL_NO_CONTEXT, contextAttribs);
    if (m_impl->m_eglContext == EGL_NO_CONTEXT)
    {
        return;
    }
}

void OpenGLBackendWinEGL::makeCurrent()
{
    if (!eglMakeCurrent(m_impl->m_eglDisplay, m_impl->m_eglSurface, m_impl->m_eglSurface, m_impl->m_eglContext))
    {
    }
}

void OpenGLBackendWinEGL::swapBuffer()
{

    if (!eglSwapBuffers(m_impl->m_eglDisplay, m_impl->m_eglSurface))
    {
        LOGE("eglSwapBuffers failed %d", eglGetError());
    }
}
void OpenGLBackendWinEGL::createScreenSurface(void *nativeHandle)
{
    GLuint flags = m_impl->m_flags;
    EGLint surfaceAttribList[] = {EGL_POST_SUB_BUFFER_SUPPORTED_NV,
                                  flags & (ES_WINDOW_POST_SUB_BUFFER_SUPPORTED) ? EGL_TRUE : EGL_FALSE, EGL_NONE,
                                  EGL_NONE};
    m_impl->m_eglSurface = eglCreateWindowSurface(m_impl->m_eglDisplay, m_impl->m_eglConfig,
                                                  (EGLNativeWindowType)m_impl->m_hWnd, surfaceAttribList);
    if (m_impl->m_eglSurface == EGL_NO_SURFACE)
    {
        return;
    }
}
void OpenGLBackendWinEGL::destroyScreenSurface()
{
}
void OpenGLBackendWinEGL::onScreenSurfaceResize(int width, int height)
{
}

void OpenGLBackendWinEGL::getScreenSurfaceSize(int *width, int *height)
{
    eglQuerySurface(m_impl->m_eglDisplay, m_impl->m_eglSurface, EGL_WIDTH, width);
    eglQuerySurface(m_impl->m_eglDisplay, m_impl->m_eglSurface, EGL_HEIGHT, height);
}

void OpenGLBackendWinEGL::preserveBackBuffer()
{
}
void OpenGLBackendWinEGL::resetRenderStates()
{
}
} // namespace laya

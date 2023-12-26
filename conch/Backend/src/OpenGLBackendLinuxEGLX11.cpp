#include "OpenGLBackendLinuxEGLX11.h"
#include <glad/egl.h>
#include <glad/gles2.h>
#include <vector>
// #define GLES_SILENCE_DEPRECATION
#include <utils/Log.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cassert>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

namespace laya
{
static void printConfig(EGLDisplay eglDisplay, EGLConfig config)
{
    LOGI("Config: %p", config);
    EGLint value = 0;
    eglGetConfigAttrib(eglDisplay, config, EGL_RENDERABLE_TYPE, &value);
    LOGI("  EGL_RENDERABLE_TYPE %i", value);
    eglGetConfigAttrib(eglDisplay, config, EGL_SURFACE_TYPE, &value);
    LOGI("  EGL_SURFACE_TYPE    %i", value);
    eglGetConfigAttrib(eglDisplay, config, EGL_RED_SIZE, &value);
    LOGI("  EGL_RED_SIZE        %i", value);
    eglGetConfigAttrib(eglDisplay, config, EGL_GREEN_SIZE, &value);
    LOGI("  EGL_GREEN_SIZE      %i", value);
    eglGetConfigAttrib(eglDisplay, config, EGL_BLUE_SIZE, &value);
    LOGI("  EGL_BLUE_SIZE       %i", value);
    eglGetConfigAttrib(eglDisplay, config, EGL_ALPHA_SIZE, &value);
    LOGI("  EGL_ALPHA_SIZE      %i", value);
    eglGetConfigAttrib(eglDisplay, config, EGL_DEPTH_SIZE, &value);
    LOGI("  EGL_DEPTH_SIZE      %i", value);
    eglGetConfigAttrib(eglDisplay, config, EGL_STENCIL_SIZE, &value);
    LOGI("  EGL_STENCIL_SIZE    %i", value);
    eglGetConfigAttrib(eglDisplay, config, EGL_SAMPLE_BUFFERS, &value);
    LOGI("  EGL_SAMPLE_BUFFERS  %i", value);
    eglGetConfigAttrib(eglDisplay, config, EGL_SAMPLES, &value);
    LOGI("  EGL_SAMPLES         %i", value);
}
EGLConfig GetConfig(EGLDisplay eglDisplay, bool *out_use_es3)
{
    static EGLConfig config = nullptr;
    static bool use_es3 = false;
    if (config)
    {
        *out_use_es3 = use_es3;
        return config;
    }

    for (bool try_es3 : std::vector<bool>{true, false})
    {
        EGLint config_attribs[] = {EGL_BUFFER_SIZE,
                                   32,
                                   EGL_ALPHA_SIZE,
                                   8,
                                   EGL_BLUE_SIZE,
                                   8,
                                   EGL_GREEN_SIZE,
                                   8,
                                   EGL_RED_SIZE,
                                   8,
                                   EGL_SAMPLES,
                                   -1,
                                   EGL_DEPTH_SIZE,
                                   -1,
                                   EGL_STENCIL_SIZE,
                                   -1,
                                   EGL_RENDERABLE_TYPE,
                                   try_es3 ? EGL_OPENGL_ES3_BIT : EGL_OPENGL_ES2_BIT,
                                   EGL_SURFACE_TYPE,
                                   EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
                                   EGL_NONE};
        EGLint num_configs = 0;
        if (!eglChooseConfig(eglDisplay, config_attribs, nullptr, 0, &num_configs) || num_configs == 0)
        {
            continue;
        }

        /*CHECK(*/ eglChooseConfig(eglDisplay, config_attribs, &config, 1, &num_configs /*)*/);
        use_es3 = try_es3;
        break;
    }

    // CHECK(config);
    *out_use_es3 = use_es3;
    return config;
}
struct OpenGLBackendLinuxEGLX11Impl
{
    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
    SDL_Window *m_sdlWindow;
    EGLConfig m_EGLConfig = EGL_NO_CONFIG_KHR;
    EGLContext m_EGLContext = EGL_NO_CONTEXT;
    EGLSurface m_EGLSurface = EGL_NO_SURFACE;
    int32_t m_width = {0};
    int32_t m_height = {0};
    bool m_preserveBackBuffer = true;
};
OpenGLBackendLinuxEGLX11::OpenGLBackendLinuxEGLX11() : m_impl(new OpenGLBackendLinuxEGLX11Impl())
{
}
OpenGLBackendLinuxEGLX11::~OpenGLBackendLinuxEGLX11()
{

    eglDestroyContext(m_impl->m_eglDisplay, m_impl->m_EGLContext);
    if (m_impl->m_EGLSurface != EGL_NO_SURFACE)
    {
        eglDestroySurface(m_impl->m_eglDisplay, m_impl->m_EGLSurface);
        m_impl->m_EGLSurface = EGL_NO_SURFACE;
    }
    EGLBoolean result = eglTerminate(m_impl->m_eglDisplay);
    assert(result != EGL_FALSE);
    m_impl->m_eglDisplay = EGL_NO_DISPLAY;
}

void OpenGLBackendLinuxEGLX11::create(const BackendOptions &options)
{
    m_impl->m_sdlWindow = (SDL_Window *)options.nativeLayer;
    SDL_SysWMinfo sys;
    SDL_VERSION(&sys.version);
    if (SDL_FALSE != SDL_GetWindowWMInfo(m_impl->m_sdlWindow, &sys))
    {
        // g_hWnd = sys.info.win.window;
        //  HINSTANCE hInstance = sys.info.win.hinstance;
    }
    else
    {
        assert(true && "get hWnd failed");
    }
    int egl_version = gladLoaderLoadEGL(NULL);
    assert(egl_version != 0 && "Unable to load EGL.\n");
     LOGE("egl_version  %d", egl_version);
    //EGLBoolean result = eglBindAPI(EGL_OPENGL_API);
    //assert(result != EGL_FALSE && "eglBindAPI failed");
    m_impl->m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    //m_impl->m_eglDisplay = eglGetDisplay((EGLNativeDisplayType)sys.info.x11.display);
    // m_impl->m_eglDisplay = sys.info.x11.display;
    assert(m_impl->m_eglDisplay != EGL_NO_DISPLAY);


    egl_version = gladLoaderLoadEGL(m_impl->m_eglDisplay);
    assert(egl_version != 0 && "Unable to load EGL.\n");
    LOGE("egl_version  %d", egl_version);

    m_impl->m_preserveBackBuffer = options.preserveBackBuffer;
    EGLint majorVersion = 0;
    EGLint minorVersion = 0;
    EGLBoolean  result = eglInitialize(m_impl->m_eglDisplay, &majorVersion, &minorVersion);
    assert(result != EGL_FALSE);

    bool use_es3 = false;//true;
    // chooseConfig(options);
    m_impl->m_EGLConfig = GetConfig(m_impl->m_eglDisplay, &use_es3);
    printConfig(m_impl->m_eglDisplay, m_impl->m_EGLConfig);
    std::vector<EGLint> context_attributes;
    context_attributes.push_back(EGL_CONTEXT_CLIENT_VERSION);
    context_attributes.push_back(use_es3 ? 3 : 2);
    context_attributes.push_back(EGL_NONE);

    m_impl->m_EGLContext =
        eglCreateContext(m_impl->m_eglDisplay, m_impl->m_EGLConfig, nullptr, context_attributes.data());
    assert(m_impl->m_EGLContext != EGL_NO_CONTEXT && "eglCreateContext failed");
}

void OpenGLBackendLinuxEGLX11::makeCurrent()
{
    assert(m_impl->m_EGLSurface != EGL_NO_SURFACE);
    eglMakeCurrent(m_impl->m_eglDisplay, m_impl->m_EGLSurface, m_impl->m_EGLSurface, m_impl->m_EGLContext);
}
/*void OpenGLBackendiOS::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_defaultFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_impl->m_defaultColorRenderBuffer);
}*/

void OpenGLBackendLinuxEGLX11::swapBuffer()
{
    if (m_impl->m_EGLSurface != EGL_NO_SURFACE)
    {
        if (!eglSwapBuffers(m_impl->m_eglDisplay, m_impl->m_EGLSurface))
        {
            LOGE("eglSwapBuffers failed %d", eglGetError());
        }
    }
}
void OpenGLBackendLinuxEGLX11::createScreenSurface(void *nativeHandle)
{

    SDL_SysWMinfo sys;
    SDL_VERSION(&sys.version);
    if (SDL_FALSE != SDL_GetWindowWMInfo(m_impl->m_sdlWindow, &sys))
    {
        // g_hWnd = sys.info.win.window;
        //  HINSTANCE hInstance = sys.info.win.hinstance;
    }
    else
    {
        assert(true && "get hWnd failed");
    }

    // m_impl->m_aNativeWindow = (ANativeWindow *)nativeHandle; // todo ANativeWindow_release

    // m_impl->m_width = ANativeWindow_getWidth(m_impl->m_aNativeWindow);
    // m_impl->m_height = ANativeWindow_getHeight(m_impl->m_aNativeWindow);

   

    EGLint format;
    eglGetConfigAttrib(m_impl->m_eglDisplay, m_impl->m_EGLConfig, EGL_NATIVE_VISUAL_ID, &format);
    // EGL_CHECK_ERROR

    // ANativeWindow_setBuffersGeometry(m_impl->m_aNativeWindow, 0, 0, format);

    std::vector<EGLint> egl_window_attributes;
    egl_window_attributes.push_back(EGL_NONE);
    m_impl->m_EGLSurface = eglCreateWindowSurface(m_impl->m_eglDisplay, m_impl->m_EGLConfig,
                                                  (EGLNativeWindowType)sys.info.x11.window, &egl_window_attributes[0]);
    if (m_impl->m_EGLSurface == EGL_NO_SURFACE)
    {
        LOGE("glCreateWindowSurface failed %d", eglGetError());
    }

    if (m_impl->m_preserveBackBuffer)
    {
        //preserveBackBuffer();
    }
    
     //XWindowAttributes xgwa;
    //XGetWindowAttributes(sys.info.x11.display, sys.info.x11.window, &xgwa);
   // m_impl->m_width = xgwa.width;
    //m_impl->m_height = xgwa.height;
}
void OpenGLBackendLinuxEGLX11::destroyScreenSurface()
{
    if (m_impl->m_EGLSurface != EGL_NO_SURFACE)
    {
        eglDestroySurface(m_impl->m_eglDisplay, m_impl->m_EGLSurface);
        m_impl->m_EGLSurface = EGL_NO_SURFACE;
    }
}
void OpenGLBackendLinuxEGLX11::onScreenSurfaceResize(int width, int height)
{
    // int width = ANativeWindow_getWidth(m_impl->m_aNativeWindow);
    // int height = ANativeWindow_getHeight(m_impl->m_aNativeWindow);
    if (m_impl->m_EGLSurface != EGL_NO_SURFACE && (m_impl->m_width != width || m_impl->m_height != height))
    {
        // eglMakeCurrent(m_impl->m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
        //                EGL_NO_CONTEXT);

        // destroyScreenSurface();

        EGLint format;
        eglGetConfigAttrib(m_impl->m_eglDisplay, m_impl->m_EGLConfig, EGL_NATIVE_VISUAL_ID, &format);
        // EGL_CHECK_ERROR
        // ANativeWindow_setBuffersGeometry(m_impl->m_aNativeWindow, 0, 0, format);
        // createScreenSurface(m_impl->m_aNativeWindow);
        // makeCurrent();
        m_impl->m_width = width;
        m_impl->m_height = height;
    }
}
void OpenGLBackendLinuxEGLX11::preserveBackBuffer()
{
    eglSurfaceAttrib(m_impl->m_eglDisplay, m_impl->m_EGLSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
    EGLint error = eglGetError();
    if (error != EGL_SUCCESS)
    {
        LOGI("preserveBackBuffer error");
    }
}
void OpenGLBackendLinuxEGLX11::resetRenderStates()
{
}
} // namespace laya

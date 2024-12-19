#include "OpenGLBackendAndroidEGL.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <vector>
// #define GLES_SILENCE_DEPRECATION
#include <android/native_window_jni.h>
#include <cassert>
#include <utils/Log.h>
#if defined(USE_SWAPPY)
#include <swappy/swappyGL.h>
#endif
#include "JCSystemConfig.h"
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
struct OpenGLBackendAndroidEGLImpl
{
    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
    ANativeWindow *m_aNativeWindow;
    EGLConfig m_EGLConfig = EGL_NO_CONFIG_KHR;
    EGLContext m_EGLContext = EGL_NO_CONTEXT;
    EGLSurface m_EGLSurface = EGL_NO_SURFACE;
    int32_t m_width = {0};
    int32_t m_height = {0};
    bool m_preserveBackBuffer = true;
};
OpenGLBackendAndroidEGL::OpenGLBackendAndroidEGL() : m_impl(new OpenGLBackendAndroidEGLImpl())
{
}
OpenGLBackendAndroidEGL::~OpenGLBackendAndroidEGL()
{
#if defined(USE_SWAPPY)
    if (g_kSystemConfig.isSwappyEnabled())
    {
        SwappyGL_destroy();
    }
#endif

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

void OpenGLBackendAndroidEGL::chooseConfig(const BackendOptions &options)
{
    int rBits, gBits, bBits, aBits;
    int depthBits, stencilBits, samples;
    EGLint numConfigs = 0;
    if (options.alpha)
    {
        rBits = 8;
        gBits = 8;
        bBits = 8;
        aBits = 8;
    }
    else
    {
        rBits = 5;
        gBits = 6;
        bBits = 5;
        aBits = 0;
    }

    if (options.depth)
    {
        depthBits = 24; // 16;
    }
    else
    {
        depthBits = 0;
    }

    if (options.stencil)
    {
        stencilBits = 8;
        if (depthBits > 0)
        {
            // Many implementations only allow 24-bit depth with 8-bit stencil.
            depthBits = 24;
        }
    }
    else
    {
        stencilBits = 0;
    }

    if (options.antialiasing)
    {
        samples = 4;
    }
    else
    {
        samples = 0;
    }

    while (true)
    {
        const EGLint attribList[] = {EGL_RENDERABLE_TYPE,
                                     EGL_OPENGL_ES3_BIT,
                                     EGL_SURFACE_TYPE,
                                     EGL_WINDOW_BIT,
                                     EGL_RED_SIZE,
                                     rBits,
                                     EGL_GREEN_SIZE,
                                     gBits,
                                     EGL_BLUE_SIZE,
                                     bBits,
                                     EGL_ALPHA_SIZE,
                                     aBits,
                                     EGL_DEPTH_SIZE,
                                     depthBits,
                                     EGL_STENCIL_SIZE,
                                     stencilBits,
                                     EGL_SAMPLE_BUFFERS,
                                     samples > 0 ? 1 : 0,
                                     EGL_SAMPLES,
                                     samples > 0 ? samples : 0,
                                     EGL_NONE,
                                     EGL_NONE};
        eglChooseConfig(m_impl->m_eglDisplay, attribList, &m_impl->m_EGLConfig, 1, &numConfigs);
        if (numConfigs)
        {
            LOGI("Config found");
            printConfig(m_impl->m_eglDisplay, m_impl->m_EGLConfig);
            break;
        }
        if (samples > 0)
        {
            // Try 2x multisampling or no multisampling
            samples -= 2;
        }
        else if (depthBits > 8)
        {
            // Try 16-bit depth or 8-bit depth
            depthBits -= 8;
        }
        else if (stencilBits > 0)
        {
            // Try 16-bit depth or 8-bit depth
            stencilBits -= 8;
        }
        else
        {
            // Failure
            LOGI("eglChooseConfig() failed");
            EGLConfig configs[256];
            EGLint numTotalConfigs = 0;
            EGLBoolean result = eglGetConfigs(m_impl->m_eglDisplay, configs, 256, &numTotalConfigs);
            assert(result != EGL_FALSE);
            LOGI("Num available configs: %i", numTotalConfigs);
            m_impl->m_EGLConfig = configs[0];
            printConfig(m_impl->m_eglDisplay, m_impl->m_EGLConfig);
            break;
        }
    }
}

void OpenGLBackendAndroidEGL::create(const BackendOptions &options)
{
    m_impl->m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(m_impl->m_eglDisplay != EGL_NO_DISPLAY);
    m_impl->m_preserveBackBuffer = options.preserveBackBuffer;
    EGLint majorVersion = 0;
    EGLint minorVersion = 0;
    EGLBoolean result = eglInitialize(m_impl->m_eglDisplay, &majorVersion, &minorVersion);
    assert(result != EGL_FALSE);

    bool use_es3 = true;
    // chooseConfig(options);
    m_impl->m_EGLConfig = GetConfig(m_impl->m_eglDisplay, &use_es3);
    printConfig(m_impl->m_eglDisplay, m_impl->m_EGLConfig);
    std::vector<EGLint> context_attributes;
    context_attributes.push_back(EGL_CONTEXT_CLIENT_VERSION);
    context_attributes.push_back(use_es3 ? 3 : 2);
    context_attributes.push_back(EGL_NONE);

    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(result != EGL_FALSE && "eglBindAPI failed");
    m_impl->m_EGLContext =
        eglCreateContext(m_impl->m_eglDisplay, m_impl->m_EGLConfig, nullptr, context_attributes.data());
    assert(m_impl->m_EGLContext != EGL_NO_CONTEXT && "eglCreateContext failed");
}

void OpenGLBackendAndroidEGL::makeCurrent()
{
    assert(m_impl->m_EGLSurface != EGL_NO_SURFACE);
    eglMakeCurrent(m_impl->m_eglDisplay, m_impl->m_EGLSurface, m_impl->m_EGLSurface, m_impl->m_EGLContext);
}
/*void OpenGLBackendiOS::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_defaultFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_impl->m_defaultColorRenderBuffer);
}*/

void OpenGLBackendAndroidEGL::swapBuffer()
{

    if (m_impl->m_EGLSurface != EGL_NO_SURFACE)
    {
#if defined(USE_SWAPPY)
        if (g_kSystemConfig.isSwappyEnabled())
        {
            SwappyGL_swap(m_impl->m_eglDisplay, m_impl->m_EGLSurface);
            return;
        }
#endif
        if (!eglSwapBuffers(m_impl->m_eglDisplay, m_impl->m_EGLSurface))
        {
            LOGE("eglSwapBuffers failed %d", eglGetError());
        }
    }
}
void OpenGLBackendAndroidEGL::createScreenSurface(void *nativeHandle)
{

    m_impl->m_aNativeWindow = (ANativeWindow *)nativeHandle; // todo ANativeWindow_release

    m_impl->m_width = ANativeWindow_getWidth(m_impl->m_aNativeWindow);
    m_impl->m_height = ANativeWindow_getHeight(m_impl->m_aNativeWindow);

    EGLint format;
    eglGetConfigAttrib(m_impl->m_eglDisplay, m_impl->m_EGLConfig, EGL_NATIVE_VISUAL_ID, &format);
    // EGL_CHECK_ERROR

    ANativeWindow_setBuffersGeometry(m_impl->m_aNativeWindow, 0, 0, format);

    std::vector<EGLint> egl_window_attributes;
    egl_window_attributes.push_back(EGL_NONE);
    m_impl->m_EGLSurface =
        eglCreateWindowSurface(m_impl->m_eglDisplay, m_impl->m_EGLConfig, (EGLNativeWindowType)m_impl->m_aNativeWindow,
                               &egl_window_attributes[0]);
    if (m_impl->m_EGLSurface == EGL_NO_SURFACE)
    {
        LOGE("glCreateWindowSurface failed %d", eglGetError());
    }

    if (m_impl->m_preserveBackBuffer)
    {
        preserveBackBuffer();
    }
#if defined(USE_SWAPPY)
    if (g_kSystemConfig.isSwappyEnabled())
    {
        SwappyGL_setWindow(m_impl->m_aNativeWindow);
    }
#endif
}
void OpenGLBackendAndroidEGL::destroyScreenSurface()
{
    if (m_impl->m_EGLSurface != EGL_NO_SURFACE)
    {
        eglDestroySurface(m_impl->m_eglDisplay, m_impl->m_EGLSurface);
        m_impl->m_EGLSurface = EGL_NO_SURFACE;
    }
}
void OpenGLBackendAndroidEGL::onScreenSurfaceResize(int width, int height)
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
        ANativeWindow_setBuffersGeometry(m_impl->m_aNativeWindow, 0, 0, format);
        // createScreenSurface(m_impl->m_aNativeWindow);
        // makeCurrent();
        m_impl->m_width = width;
        m_impl->m_height = height;
    }
}
void OpenGLBackendAndroidEGL::preserveBackBuffer()
{
    eglSurfaceAttrib(m_impl->m_eglDisplay, m_impl->m_EGLSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
    EGLint error = eglGetError();
    if (error != EGL_SUCCESS)
    {
        LOGI("preserveBackBuffer error");
    }
}
void OpenGLBackendAndroidEGL::resetRenderStates()
{
}
} // namespace laya

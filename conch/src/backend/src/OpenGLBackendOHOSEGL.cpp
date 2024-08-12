#include "OpenGLBackendOHOSEGL.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <utils/Log.h>
#include <vector>

namespace laya
{

EGLConfig getConfig(int version, EGLDisplay eglDisplay)
{
    int attribList[] = {EGL_SURFACE_TYPE,
                        EGL_WINDOW_BIT,
                        EGL_RED_SIZE,
                        8,
                        EGL_GREEN_SIZE,
                        8,
                        EGL_BLUE_SIZE,
                        8,
                        EGL_ALPHA_SIZE,
                        8,
                        EGL_RENDERABLE_TYPE,
                        EGL_OPENGL_ES2_BIT,
                        EGL_STENCIL_SIZE,
                        8,
                        EGL_DEPTH_SIZE,
                        24,
                        EGL_NONE};
    EGLConfig configs = NULL;
    int configsNum;
    if (!eglChooseConfig(eglDisplay, attribList, &configs, 1, &configsNum))
    {
        // OHOS_LOGE("eglChooseConfig ERROR");
        return NULL;
    }
    return configs;
}

bool checkGlError(const char *op)
{
    // OHOS_LOGE("EGL ERROR CODE = %{public}x", eglGetError());
    GLint error;
    for (error = glGetError(); error; error = glGetError())
    {
        // OHOS_LOGE("ERROR: %{public}s, ERROR CODE = %{public}x", op, error);
        return true;
    }
    return false;
}

OpenGLBackendOHOSEGL::OpenGLBackendOHOSEGL()
{
}
OpenGLBackendOHOSEGL::~OpenGLBackendOHOSEGL()
{
}

void OpenGLBackendOHOSEGL::create(const BackendOptions &options)
{
    // OHOS_LOGD("EGLCore::GLContextInit window = %{public}p, w = %{public}d, h = %{public}d.", window, w, h);

    // 1. create sharedcontext
    mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mEGLDisplay == EGL_NO_DISPLAY)
    {
        // OHOS_LOGE("EGLCore::unable to get EGL display.");
        return;
    }

    EGLint eglMajVers, eglMinVers;
    if (!eglInitialize(mEGLDisplay, &eglMajVers, &eglMinVers))
    {
        mEGLDisplay = EGL_NO_DISPLAY;
        // OHOS_LOGE("EGLCore::unable to initialize display");
        return;
    }

    mEGLConfig = getConfig(3, mEGLDisplay);
    if (mEGLConfig == nullptr)
    {
        // OHOS_LOGE("EGLCore::GLContextInit config ERROR");
        return;
    }

    // 3. Create EGLContext from
    int attrib3_list[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

    mEGLContext = eglCreateContext(mEGLDisplay, mEGLConfig, mSharedEGLContext, attrib3_list);
}

void OpenGLBackendOHOSEGL::makeCurrent()
{
    if (!eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext))
    {
        // OHOS_LOGE("EGLCore::eglMakeCurrent error = %{public}d", eglGetError());
    }
}

void OpenGLBackendOHOSEGL::swapBuffer()
{
    eglSwapBuffers(mEGLDisplay, mEGLSurface);
}
void OpenGLBackendOHOSEGL::createScreenSurface(void *nativeHandle)
{
    mEglWindow = (EGLNativeWindowType)(nativeHandle);
    // 2. Create EGL Surface from Native Window
    if (mEglWindow)
    {
        mEGLSurface = eglCreateWindowSurface(mEGLDisplay, mEGLConfig, mEglWindow, nullptr);
        if (mEGLSurface == nullptr)
        {
            // OHOS_LOGE("EGLCore::eglCreateContext eglSurface is null");
            return;
        }
    }
}
void OpenGLBackendOHOSEGL::destroyScreenSurface()
{
}
void OpenGLBackendOHOSEGL::onScreenSurfaceResize(int width, int height)
{
    width_ = width;
    height_ = height;
}
void OpenGLBackendOHOSEGL::preserveBackBuffer()
{
}
void OpenGLBackendOHOSEGL::resetRenderStates()
{
}
} // namespace laya

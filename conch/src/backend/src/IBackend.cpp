#include <backend/IBackend.h>
#include <utils/Log.h>
#if defined(OS_IOS)
#include "OpenGLBackendiOS.h"
#elif defined(OS_LINUX)
#include "OpenGLBackendLinuxEGLX11.h"
#elif defined(OS_ANDROID)
#include "OpenGLBackendAndroidEGL.h"
#elif defined(OS_WINDOWS)
#include "OpenGLBackendWinEGL.h"
#elif defined(OS_OHOS)
#include "OpenGLBackendOHOSEGL.h"
#endif

namespace laya
{
IBackend *createBackend(const BackendOptions &options)
{
    IBackend *backend = nullptr;
#if defined(OS_IOS)
    backend = new OpenGLBackendiOS();
#elif defined(OS_LINUX)
    backend = new OpenGLBackendLinuxEGLX11();
#elif defined(OS_ANDROID)
    backend = new OpenGLBackendAndroidEGL();
#elif defined(OS_WINDOWS)
    backend = new OpenGLBackendWinEGL();
#elif defined(OS_OHOS)
    backend = new OpenGLBackendOHOSEGL();
#endif
    backend->create(options);
    return backend;
}
} // namespace laya
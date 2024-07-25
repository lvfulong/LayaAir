#include <platform/OS.h>
#include <utils/Log.h>
#if defined(OS_ANDROID)
#include "OSAndroid.h"
#elif defined(OS_IOS)
#include "OSiOS.h"
#elif defined(OS_WINDOWS)
#include "OSWin.h"
#elif defined(OS_OHOS)
#include "OSOHOS.h"
#elif defined(OS_LINUX)
#include "OSLinux.h"
#endif

namespace laya
{
std::unique_ptr<OS> createOS()
{
#if defined(OS_ANDROID)
    return std::unique_ptr<OS>(new OSAndroid());
#elif defined(OS_IOS)
    return std::unique_ptr<OS>(new OSiOS());
#elif defined(OS_WINDOWS)
    return std::unique_ptr<OS>(new OSWin());
#elif defined(OS_OHOS)
    return std::unique_ptr<OS>(new OSOHOS());
#elif defined(OS_LINUX)
    return std::unique_ptr<OS>(new OSLinux());
#endif
}
} // namespace laya

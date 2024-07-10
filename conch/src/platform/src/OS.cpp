#include <platform/OS.h>
#include <utils/Log.h>
#if OS_ANDROID
#include "OSAndroid.h"
#elif OS_IOS
#include "OSiOS.h"
#elif OS_WINDOWS
#include "OSWin.h"
#elif OS_OHOS
#include "OSOHOS.h"
#elif OS_LINUX
#include "OSLinux.h"
#endif

namespace laya
{
std::unique_ptr<OS> createOS()
{
#if OS_ANDROID
    return std::unique_ptr<OS>(new OSAndroid());
#elif OS_IOS
    return std::unique_ptr<OS>(new OSiOS());
#elif OS_WINDOWS
    return std::unique_ptr<OS>(new OSWin());
#elif OS_OHOS
    return std::unique_ptr<OS>(new OSOHOS());
#elif OS_LINUX
    return std::unique_ptr<OS>(new OSLinux());
#endif
}
} // namespace laya

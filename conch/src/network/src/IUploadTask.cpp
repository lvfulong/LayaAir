#include <network/IUploadTask.h>
#include <utils/Log.h>

#if defined(OS_ANDROID)
#include "UploadTaskAndroid.h"
#elif defined(OS_IOS)
#include "UploadTaskiOS.h"
#elif defined(OS_WINDOWS)
#include "UploadTaskCurl.h"
#elif defined(OS_OHOS)
#include "UploadTaskCurl.h"
#elif defined(OS_LINUX)
#include "UploadTaskCurl.h"
#else
#error "Unsupported platform"
#endif  


namespace laya {

IUploadTask* createUploadTask()
{   
#if defined(OS_ANDROID)
    return new UploadTaskAndroid();
#elif defined(OS_IOS)
    return new UploadTaskiOS();
#elif defined(OS_WINDOWS)
    return new UploadTaskCurl();
#elif defined(OS_OHOS)
    return new UploadTaskCurl();
#elif defined(OS_LINUX)
    return new UploadTaskCurl();
#else
#error "Unsupported platform"
#endif  
}
} // namespace laya

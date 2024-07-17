#include "OSAndroid.h"
#include "CToJavaBridge.h"
#include <utils/Log.h>
namespace laya
{

OSAndroid::~OSAndroid()
{
}
float OSAndroid::getTotalMem()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getTotalMem", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return kRet.floatRet;
    }
    return 0;
}
int OSAndroid::getAvalidMem()
{

    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getAvalidMem", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return (int)(kRet.floatRet);
    }
    return 0;
}

int OSAndroid::getMemoryUsageInByte()
{
    CToJavaBridge::JavaRet kRet;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getMemoryUsageInByte", kRet,
                                                 CToJavaBridge::JavaRet::RT_Float))
    {
        return (int)(kRet.floatRet);
    }
    return 0;
}
void OSAndroid::exit()
{
    CToJavaBridge::JavaRet ret;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "exit", ret);
}
JsValue OSAndroid::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    return JSP_TO_JS_NULL; // todo
}
std::string OSAndroid::postSyncMessage(const std::string &eventName, const std::string &data)
{
    return ""; // todo
}
} // namespace laya

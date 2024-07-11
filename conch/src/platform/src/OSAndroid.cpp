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
} // namespace laya

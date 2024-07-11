#include "OSOHOS.h"
#include <utils/Log.h>
#include <aki/jsbind.h>
namespace laya
{

OSOHOS::~OSOHOS()
{
}
float OSOHOS::getTotalMem()
{
    return 0; // todo
}
int OSOHOS::getAvalidMem()
{
    if (auto getAvalidMem = aki::JSBind::GetJSFunction("DeviceUtils.getAvalidMem"))
    {
        return getAvalidMem->Invoke<int>();
    }
    return 0;
}
int OSOHOS::getMemoryUsageInByte()
{
    return 0; // todo
}
} // namespace laya
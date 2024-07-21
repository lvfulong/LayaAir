#include "OSiOS.h"
#include "CToObjectC.h"
#include <utils/Log.h>
namespace laya
{

OSiOS::~OSiOS()
{
}
float OSiOS::getTotalMem()
{
    return CToObjectCGetTotalMem();
}
int OSiOS::getAvalidMem()
{
    return CToObjectCGetAvalidMem();
}
int OSiOS::getMemoryUsageInByte()
{
    return CToObjectCGetMemoryUsageInByte();
}
void OSiOS::exit()
{
}
JsValue OSiOS::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    return JSP_TO_JS_NULL; // todo
}
std::string OSiOS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    //handleSyncMessage is called in platform os ui thread
    std::string eventResult = CToObjectCPostSyncMessage(eventName, data);
    return eventResult;
}
} // namespace laya
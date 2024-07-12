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
std::string OSiOS::postAsyncMessage(const std::string &eventName, const std::string &data)
{
    return ""; // todo
}
std::string OSiOS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    return ""; // todo
}
} // namespace laya
#include "OSLinux.h"
#include <utils/Log.h>

namespace laya
{

OSLinux::~OSLinux()
{
}
float OSLinux::getTotalMem()
{
    return 0; // todo
}
int OSLinux::getAvalidMem()
{
    return 0; // todo
}
int OSLinux::getMemoryUsageInByte()
{
    return 0; // todo
}
void OSLinux::exit()
{
    // todo
}
std::string OSLinux::postAsyncMessage(const std::string &eventName, const std::string &data)
{
    return ""; // todo
}
std::string OSLinux::postSyncMessage(const std::string &eventName, const std::string &data)
{
    return ""; // todo
}
} // namespace laya

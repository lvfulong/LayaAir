#include "OSWin.h"
#include <Windows.h>
#include <utils/Log.h>
namespace laya
{

OSWin::~OSWin()
{
}
float OSWin::getTotalMem()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return (float)(statex.ullTotalPhys / 1024);
}
int OSWin::getAvalidMem()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return (int)(statex.ullAvailPhys / 1024);
}
int OSWin::getMemoryUsageInByte()
{
    return 0; // todo
}
void OSWin::exit()
{
    // todo
}
std::string OSWin::postAsyncMessage(const std::string &eventName, const std::string &data)
{
    return ""; // todo
}
std::string OSWin::postSyncMessage(const std::string &eventName, const std::string &data)
{
    return ""; // todo
}
} // namespace laya

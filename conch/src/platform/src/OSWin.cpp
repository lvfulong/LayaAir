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
int OSWin::getMemoryUsageInByte()
{
    return 0; // todo
}
} // namespace laya

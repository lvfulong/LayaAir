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
int OSiOS::getMemoryUsageInByte()
{
    return CToObjectCGetMemoryUsageInByte();
}
} // namespace laya
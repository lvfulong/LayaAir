#include "OSWin.h"
#include "Exports.h"
#include <JCConch.h>
#include <Windows.h>
#include <future>
#include <utils/Log.h>

extern handleSyncMessageCallback g_handleSyncMessageCb;
extern handleAsyncMessageCallback g_handleAsyncMessageCb;
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
    // handleAsyncMessage is called in platform os ui thread
    std::string syncEventResult;
    std::promise<std::string> promise;
    std::function<void(std::string)> cb = [&promise](std::string message) { promise.set_value(message); };
    if (g_handleAsyncMessageCb)
    {
        postToPlatform([eventName, data, cb]() { g_handleAsyncMessageCb(eventName, data, cb); });
    }
    syncEventResult = promise.get_future().get();
    return syncEventResult;
}
std::string OSWin::postSyncMessage(const std::string &eventName, const std::string &data)
{
    // handleSyncMessage is called in platform os ui thread
    std::string eventResult;
    std::promise<std::string> promise;
    if (g_handleSyncMessageCb)
    {
        postToPlatform([eventName, data, &promise]() {
            std::string eventResult = g_handleSyncMessageCb(eventName, data);
            promise.set_value(eventResult);
        });
    }
    eventResult = promise.get_future().get();
    return eventResult;
}
} // namespace laya

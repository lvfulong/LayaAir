#include "OSLinux.h"
#include "Exports.h"
#include <future>
#include <JCConch.h>
#include <utils/Log.h>

extern handleSyncMessageCallback g_handleSyncMessageCb;
extern handleAsyncMessageCallback g_handleAsyncMessageCb;

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
std::string OSLinux::postSyncMessage(const std::string &eventName, const std::string &data)
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

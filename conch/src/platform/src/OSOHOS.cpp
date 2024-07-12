#include "OSOHOS.h"
#include <aki/jsbind.h>
#include <utils/Log.h>
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
void OSOHOS::exit()
{
    if (auto exit = aki::JSBind::GetJSFunction("ApplicationManager.exit"))
    {
        exit->Invoke<void>();
    }
}
std::string OSOHOS::postAsyncMessage(const std::string &eventName, const std::string &data)
{
    //handleAsyncMessage is called in ohos ui thread
    std::string syncEventResult;
    std::promise<std::string> promise;
    std::function<void(std::string)> cb = [&promise](std::string message){
        promise.set_value(message); 
    };
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleAsyncMessage"))
    {
        post->Invoke<void>(eventName, data, cb);
    }
    syncEventResult = promise.get_future().get();
    return syncEventResult; 
}
std::string OSOHOS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    //handleSyncMessage is called in ohos ui thread
    std::string eventResult;
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleSyncMessage"))
    {
        eventResult = post->Invoke<std::string>(eventName, data);
    }
    return eventResult;
}
} // namespace laya
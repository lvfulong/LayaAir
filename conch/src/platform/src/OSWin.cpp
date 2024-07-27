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
int OSWin::getNetworkType()
{
    return 1;// todo
}
JsValue OSWin::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    napi_deferred deferred;
    napi_value promise;

    napi_create_promise(context, &deferred, &promise);

    std::function<void(std::string)> cb = [deferred, cbref](std::string message) {
        postToJS([deferred, message, cbref]() {
            if (!cbref.lock())
                return;
            auto isolate = v8::Isolate::GetCurrent();
            auto context = isolate->GetCurrentContext();
            napi_value v = JsValueFromV8LocalValue(Converter<const char *>::ToJs(message));
            napi_resolve_deferred(context, deferred, v);
        });
    };
    if (g_handleAsyncMessageCb)
    {
        // handleAsyncMessage is called in platform os ui thread
        postToPlatform([eventName, data, cb]() { g_handleAsyncMessageCb(eventName, data, cb); });
    }
    return V8LocalValueFromJsValue(promise);
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

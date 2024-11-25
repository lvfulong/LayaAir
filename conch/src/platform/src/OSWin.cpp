#include "OSWin.h"
#include "Exports.h"
#include <JCConch.h>
#include <Windows.h>
#include <future>
#include <utils/Log.h>
#if defined(OS_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#endif

extern handleSyncMessageCallback g_handleSyncMessageCb;
extern handleAsyncMessageCallback g_handleAsyncMessageCb;
namespace laya
{

OSWin::~OSWin()
{
}
int OSWin::getUsedMem()
{
    HANDLE handle = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
    // WorkingSetSize 使用的内存
    // PeakWorkingSetSize 峰值内存
    // PagefileUsage 虚拟内存
    // PeakPagefileUsage 峰值虚拟内存
    return pmc.WorkingSetSize / 1024;
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
    return 1; // todo
}
void OSWin::setScreenWakeLock(bool bWakeLock)
{
    // todo
}
void OSWin::setSensorAble(bool bSensorAble)
{
    // todo
}
int OSWin::getSafeInsetTop()
{
    return 0;
}
int OSWin::getSafeInsetLeft()
{
    return 0;
}
int OSWin::getSafeInsetBottom()
{
    return 0;
}
int OSWin::getSafeInsetRight()
{
    return 0;
}
jsvm_value OSWin::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
{
    //auto isolate = v8::Isolate::GetCurrent();
    //auto context = isolate->GetCurrentContext();

    //napi_deferred deferred;
    //napi_value promise;

    //napi_create_promise(context, &deferred, &promise);

    auto promise = jsbind::Promise::Make();
    std::function<void(std::string)> cb = [promise, cbref](std::string message) {
        postToJS([promise, message, cbref]() {
            if (!cbref.lock())
                return;
            //auto isolate = v8::Isolate::GetCurrent();
            //auto context = isolate->GetCurrentContext();
            //napi_value v = jsvm_valueFromV8LocalValue(jsbind::Local::Make<std::string>(message));
            //napi_resolve_deferred(context, deferred, v);
            promise.resolve(message);
        });
    };
    if (g_handleAsyncMessageCb)
    {
        // handleAsyncMessage is called in platform os ui thread
        postToPlatform([eventName, data, cb]() { g_handleAsyncMessageCb(eventName, data, cb); });
    }
    return promise.getHandle();
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

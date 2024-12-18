#include "OSWin.h"
#include "Exports.h"
#include <JCConch.h>
#include <Windows.h>
#include <future>
#include <utils/Log.h>
#include "JCSystemConfig.h"
#define WIN32_LEAN_AND_MEAN
#include <psapi.h>
#include <windows.h>


extern handleSyncMessageCallback g_handleSyncMessageCb;
extern handleAsyncMessageCallback g_handleAsyncMessageCb;
extern void conchRegisterHandleMessageHandler(const char *eventName, std::function<void(const char *)> cb);
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


    auto promise = jsbind::Promise::Make();

    conchRegisterHandleMessageHandler(eventName.c_str(), [promise, cbref](const char *message) {
        postToJS([promise, message, cbref]() {
            if (!cbref.lock())
                return;
            promise.resolve(message);
        });
    });

    if (g_handleAsyncMessageCb)
    {
        // handleAsyncMessage is called in platform os ui thread
        postToPlatform([eventName, data]() { g_handleAsyncMessageCb(eventName.c_str(), data.c_str()); });
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
        conchRegisterHandleMessageHandler(eventName.c_str(),
                                          [&promise](const char *message) { promise.set_value(message); });
        postToPlatform([eventName, data]() { g_handleSyncMessageCb(eventName.c_str(), data.c_str()); });
    }
    eventResult = promise.get_future().get();

    return eventResult;
}
void OSWin::setPreferredFramesPerSecond(uint64_t fps)
{
    if (fps > 0)
    {
        g_kSystemConfig.m_frameIntervalInMs =  (uint64_t)(1000.f / fps);
    }
}
} // namespace laya

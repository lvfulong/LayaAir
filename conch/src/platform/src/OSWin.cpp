#include <platform/OS.h>
#include "Exports.h"
#include "JCSystemConfig.h"
#include <JCConch.h>
#include <Windows.h>
#include <future>
#include <utils/Log.h>
#define WIN32_LEAN_AND_MEAN
#include <psapi.h>
#include <windows.h>
#include <utils/JCFileSystem.h>

extern handleSyncMessageCallback g_handleSyncMessageCb;
extern handleAsyncMessageCallback g_handleAsyncMessageCb;
extern void conchRegisterHandleMessageHandler(const char *eventName, std::function<void(const char *)> cb);
namespace laya
{
int OS::getUsedMem()
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
float OS::getTotalMem()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return (float)(statex.ullTotalPhys / 1024);
}
int OS::getAvalidMem()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return (int)(statex.ullAvailPhys / 1024);
}
int OS::getMemoryUsageInByte()
{
    return 0; // todo
}
void OS::exit()
{
    // todo
}
int OS::getNetworkType()
{
    return 1; // todo
}
void OS::setScreenWakeLock(bool bWakeLock)
{
    // todo
}
void OS::setSensorAble(bool bSensorAble)
{
    // todo
}
int OS::getSafeInsetTop()
{
    return 0;
}
int OS::getSafeInsetLeft()
{
    return 0;
}
int OS::getSafeInsetBottom()
{
    return 0;
}
int OS::getSafeInsetRight()
{
    return 0;
}
jsvm_value OS::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
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
        g_handleAsyncMessageCb(eventName.c_str(), data.c_str());
    }
    return promise.getHandle();
}
std::string OS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    // handleSyncMessage is called in platform os ui thread
    std::string eventResult;
    std::promise<std::string> promise;
    if (g_handleSyncMessageCb)
    {
        conchRegisterHandleMessageHandler(eventName.c_str(),
                                          [&promise](const char *message) { promise.set_value(message); });
        g_handleSyncMessageCb(eventName.c_str(), data.c_str());
    }
    eventResult = promise.get_future().get();

    return eventResult;
}
void OS::setPreferredFramesPerSecond(uint64_t fps)
{
    if (fps > 0)
    {
        g_kSystemConfig.m_frameIntervalInMs = (uint64_t)(1000.f / fps);
    }
}
std::string OS::getExecutablePath()
{
    WCHAR szPath[MAX_PATH + 1];
    ::GetModuleFileNameW(NULL, szPath, MAX_PATH + 1);
    std::string result = wideToUtf8(szPath);
    return stringReplace(result, "\\", "/");
}
std::string OS::getAssetFullPath(const std::string &assetRelativePath)
{
    return getAssetRootPath() + "/" + assetRelativePath;
}
std::string OS::getAssetRootPath()
{
    std::string exePath = laya::OS::getExecutablePath();
    std::string assetRootPath = laya::FileSystem::parent_path(exePath);
    return assetRootPath;
}
std::string OS::getPersistentDataPath()
{
    return getAssetRootPath();
}
std::string OS::getTemporaryCachePath()
{
    return getAssetRootPath();
}
} // namespace laya

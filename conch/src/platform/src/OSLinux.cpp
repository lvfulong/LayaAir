#include <platform/OS.h>
#include "Exports.h"
#include "JCSystemConfig.h"
#include <JCConch.h>
#include <future>
#include <utils/Log.h>
#include <utils/JCFileSystem.h>

extern handleSyncMessageCallback g_handleSyncMessageCb;
extern handleAsyncMessageCallback g_handleAsyncMessageCb;
extern void conchRegisterHandleMessageHandler(const char *eventName, std::function<void(const char *)> cb);

namespace laya
{
int OS::getUsedMem()
{
    return 0; // todo
}
float OS::getTotalMem()
{
    return 0; // todo
}
int OS::getAvalidMem()
{
    return 0; // todo
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
    char buf[2048];
    memset(buf, 0, 2048);
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len <= 0)
    {
        LOGE("getExecutablePath failed");
        return "";
    }
    buf[len] = 0;
    std::string ret(buf);
    return ret;
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
const std::string& OS::getFilesDir()
{
    static std::string path = getAssetRootPath();
    return path;
}
const std::string& OS::getCacheDir()
{
    static std::string path = getAssetRootPath();
    return path;
}
} // namespace laya

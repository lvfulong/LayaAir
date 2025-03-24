#include <platform/OS.h>
#include <JCConch.h>
#include <aki/jsbind.h>
#include <utils/Log.h>
#include "platform/ohos/napi/render/plugin_render.h"
#include "platform/ohos/napi/helper/Sensor.h"

namespace laya
{
int OS::getUsedMem()
{
    int usedMem = 0;
    if (auto getUsedMem = aki::JSBind::GetJSFunction("DeviceUtils.getPrivateDirty"))
    {
        usedMem = getUsedMem->Invoke<int>();
    }
    return usedMem;
}
float OS::getTotalMem()
{
    return 0; // todo
}
int OS::getAvalidMem()
{
    if (auto getAvalidMem = aki::JSBind::GetJSFunction("DeviceUtils.getAvalidMem"))
    {
        return getAvalidMem->Invoke<int>();
    }
    return 0;
}
int OS::getMemoryUsageInByte()
{
    return 0; // todo
}
void OS::exit()
{
    if (auto exit = aki::JSBind::GetJSFunction("ApplicationManager.exit"))
    {
        exit->Invoke<void>();
    }
}
int OS::getNetworkType()
{
    int networkType = 0;
    if (auto getNetworkType = aki::JSBind::GetJSFunction("NetworkUtils.getNetworkType"))
    {
        networkType = getNetworkType->Invoke<int>();
    }
    return networkType;
}
void OS::setScreenWakeLock(bool bWakeLock)
{
    if (auto setKeepScreenOn = aki::JSBind::GetJSFunction("DeviceUtils.setKeepScreenOn"))
    {
        setKeepScreenOn->Invoke<void>(bWakeLock);
    }
}
void OS::setSensorAble(bool bSensorAble)
{
    if (bSensorAble)
    {
       Sensor::enableSensor();
    }
    else
    {
       Sensor::disableSensor();
    }
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
    std::function<void(std::string)> cb = [promise, cbref](std::string message) {
        postToJS([promise, message, cbref]() {
            if (!cbref.lock())
                return;
             promise.resolve(message);
        });
    };
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleAsyncMessage"))
    {
        post->Invoke<void>(eventName, data, cb);
    }
    return promise.getHandle();
}
std::string OS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    // handleSyncMessage is called in ohos ui thread
    std::string eventResult;
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleSyncMessage"))
    {
        eventResult = post->Invoke<std::string>(eventName, data);
    }
    return eventResult;
}
void OS::setPreferredFramesPerSecond(uint64_t fps)
{
    if (fps > 0)
    {
        uint64_t animationIntervalMs = (uint64_t)(1000.f / fps);
        PluginRender::GetInstance()->changeFPS(animationIntervalMs);
    }
}
std::string OS::getExecutablePath()
{
    return "";//todo
}
} // namespace laya
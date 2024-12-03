#include "OSOHOS.h"
#include <JCConch.h>
#include <aki/jsbind.h>
#include <utils/Log.h>
#include "platform/ohos/napi/render/plugin_render.h"

namespace laya
{

OSOHOS::~OSOHOS()
{
}
int OSOHOS::getUsedMem()
{
    int usedMem = 0;
    if (auto getUsedMem = aki::JSBind::GetJSFunction("DeviceUtils.getPrivateDirty"))
    {
        usedMem = getUsedMem->Invoke<int>();
    }
    return usedMem;
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
int OSOHOS::getNetworkType()
{
    int networkType = 0;
    if (auto getNetworkType = aki::JSBind::GetJSFunction("NetworkUtils.getNetworkType"))
    {
        networkType = getNetworkType->Invoke<int>();
    }
    return networkType;
}
void OSOHOS::setScreenWakeLock(bool bWakeLock)
{
    if (auto setKeepScreenOn = aki::JSBind::GetJSFunction("DeviceUtils.setKeepScreenOn"))
    {
        setKeepScreenOn->Invoke<void>(bWakeLock);
    }
}
void OSOHOS::setSensorAble(bool bSensorAble)
{
    if (bSensorAble)
    {
        if (auto accelerometerEnable = aki::JSBind::GetJSFunction("Accelerometer.enable"))
        {
            accelerometerEnable->Invoke<void>();
        }
        if (auto orientationEnable = aki::JSBind::GetJSFunction("Orientation.enable"))
        {
            orientationEnable->Invoke<void>();
        }
    }
    else
    {
        if (auto accelerometerDisable = aki::JSBind::GetJSFunction("Accelerometer.disable"))
        {
            accelerometerDisable->Invoke<void>();
        }
        if (auto orientationDisable = aki::JSBind::GetJSFunction("Orientation.disable"))
        {
            orientationDisable->Invoke<void>();
        }
    }
}
int OSOHOS::getSafeInsetTop()
{
    return 0;
}
int OSOHOS::getSafeInsetLeft()
{
    return 0;
}
int OSOHOS::getSafeInsetBottom()
{
    return 0;
}
int OSOHOS::getSafeInsetRight()
{
    return 0;
}
jsvm_value OSOHOS::postAsyncMessage(std::weak_ptr<int> cbref, const std::string &eventName, const std::string &data)
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
std::string OSOHOS::postSyncMessage(const std::string &eventName, const std::string &data)
{
    // handleSyncMessage is called in ohos ui thread
    std::string eventResult;
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleSyncMessage"))
    {
        eventResult = post->Invoke<std::string>(eventName, data);
    }
    return eventResult;
}
void OSOHOS::setPreferredFramesPerSecond(uint64_t fps)
{
    if (fps > 0)
    {
        uint64_t animationIntervalMs = (uint64_t)(1000.f / fps);
        PluginRender::GetInstance()->changeFPS(fps);
    }
}
} // namespace laya
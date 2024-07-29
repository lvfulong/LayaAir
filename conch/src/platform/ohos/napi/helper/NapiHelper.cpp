#include "NapiHelper.h"
#include <aki/jsbind.h>

using namespace laya;

NapiHelper NapiHelper::help_;

std::string NapiHelper::getDeviceInfo()
{
    std::string deviceInfo;
    if (auto getDeviceInfo = aki::JSBind::GetJSFunction("DeviceUtils.getDeviceInfo"))
    {
        deviceInfo = getDeviceInfo->Invoke<std::string>();
    }
    return deviceInfo;
}
void NapiHelper::playBackgroundMusic(const char *p_sUrl, int p_nTimes, float nCurrentTime)
{
    if (auto playBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.playBackgroundMusic"))
    {
        playBackgroundMusic->Invoke<void>(p_sUrl, p_nTimes, (int)(nCurrentTime * 1000));
    }
}
void NapiHelper::pauseBackgroundMusic()
{
    if (auto pauseBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.pauseBackgroundMusic"))
    {
        pauseBackgroundMusic->Invoke<void>();
    }
}
void NapiHelper::stopBackgroundMusic()
{
    if (auto stopBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.stopBackgroundMusic"))
    {
        stopBackgroundMusic->Invoke<void>();
    }
}
void NapiHelper::resumeBackgroundMusic()
{
    if (auto resumeBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.resumeBackgroundMusic"))
    {
        resumeBackgroundMusic->Invoke<void>();
    }
}
void NapiHelper::setBackgroundMusicVolume(float p_nVolume)
{
    if (auto setBackgroundMusicVolume = aki::JSBind::GetJSFunction("SoundUtils.setBackgroundMusicVolume"))
    {
        setBackgroundMusicVolume->Invoke<void>(p_nVolume);
    }
}
void NapiHelper::setCurrentTime(double nCurrentTime)
{
    if (auto setCurrentTime = aki::JSBind::GetJSFunction("SoundUtils.setCurrentTime"))
    {
        setCurrentTime->Invoke<void>(nCurrentTime);
    }
}
double NapiHelper::getCurrentTime()
{
    double currentTime = 0.0f;
    if (auto getCurrentTime = aki::JSBind::GetJSFunction("SoundUtils.getCurrentTime"))
    {
        currentTime = getCurrentTime->Invoke<double>();
    }
    return currentTime;
}
double NapiHelper::getDuration()
{
    double duration = 0.0f;
    if (auto getMp3Duration = aki::JSBind::GetJSFunction("SoundUtils.getDuration"))
    {
        duration = getMp3Duration->Invoke<double>();
    }
    return duration;
}
void NapiHelper::startVibration(float duration)
{
    if (auto startVibration = aki::JSBind::GetJSFunction("DeviceUtils.startVibration"))
    {
        startVibration->Invoke<void>(duration);
    }
}
std::string NapiHelper::getAppVersion()
{
    std::string appVersion;
    if (auto getAppVersion = aki::JSBind::GetJSFunction("ApplicationManager.getAppVersion"))
    {
        appVersion = getAppVersion->Invoke<std::string>();
    }
    return appVersion;
}
std::string NapiHelper::getAppLocalVersion()
{
    std::string appLocalVersion;
    if (auto getAppLocalVersion = aki::JSBind::GetJSFunction("ApplicationManager.getAppLocalVersion"))
    {
        appLocalVersion = getAppLocalVersion->Invoke<std::string>();
    }
    return appLocalVersion;
}
void NapiHelper::createVideoPlayer(int m_tag)
{
    if (auto createVideoPlayer = aki::JSBind::GetJSFunction("VideoPlayer.createVideoPlayer"))
    {
        createVideoPlayer->Invoke<void>(m_tag);
    }
}
void NapiHelper::removeVideoPlayer(int m_tag)
{
    if (auto removeVideoPlayer = aki::JSBind::GetJSFunction("VideoPlayer.removeVideoPlayer"))
    {
        removeVideoPlayer->Invoke<void>(m_tag);
    }
}
void NapiHelper::setURL(int m_tag, std::string path, int isurl)
{
    if (auto setURL = aki::JSBind::GetJSFunction("VideoPlayer.setURL"))
    {
        setURL->Invoke<void>(m_tag, path, isurl);
    }
}
void NapiHelper::play(int m_tag)
{
    if (auto play = aki::JSBind::GetJSFunction("VideoPlayer.play"))
    {
        play->Invoke<void>(m_tag);
    }
}
void NapiHelper::pause(int m_tag)
{
    if (auto pause = aki::JSBind::GetJSFunction("VideoPlayer.pause"))
    {
        pause->Invoke<void>(m_tag);
    }
}
void NapiHelper::stop(int m_tag)
{
    if (auto stop = aki::JSBind::GetJSFunction("VideoPlayer.stop"))
    {
        stop->Invoke<void>(m_tag);
    }
}
void NapiHelper::setLooping(int m_tag, bool value)
{
    if (auto setLooping = aki::JSBind::GetJSFunction("VideoPlayer.setLooping"))
    {
        setLooping->Invoke<void>(m_tag, value);
    }
}
void NapiHelper::setAutoplay(int m_tag, bool value)
{
    if (auto setAutoplay = aki::JSBind::GetJSFunction("VideoPlayer.setAutoplay"))
    {
        setAutoplay->Invoke<void>(m_tag, value);
    }
}
void NapiHelper::setX(int m_tag, double value)
{
    if (auto setX = aki::JSBind::GetJSFunction("VideoPlayer.setX"))
    {
        setX->Invoke<void>(m_tag, value);
    }
}
void NapiHelper::setY(int m_tag, double value)
{
    if (auto setY = aki::JSBind::GetJSFunction("VideoPlayer.setY"))
    {
        setY->Invoke<void>(m_tag, value);
    }
}
void NapiHelper::setWidth(int m_tag, double value)
{
    if (auto setWidth = aki::JSBind::GetJSFunction("VideoPlayer.setWidth"))
    {
        setWidth->Invoke<void>(m_tag, value);
    }
}
void NapiHelper::setHeight(int m_tag, double value)
{
    if (auto setHeight = aki::JSBind::GetJSFunction("VideoPlayer.setHeight"))
    {
        setHeight->Invoke<void>(m_tag, value);
    }
}

void NapiHelper::setVideoCurrentTime(int m_tag, double value)
{
    if (auto setCurrentTime = aki::JSBind::GetJSFunction("VideoPlayer.setCurrentTime"))
    {
        setCurrentTime->Invoke<void>(m_tag, value);
    }
}

void NapiHelper::setVolume(int m_tag, double value)
{
    if (auto setVolume = aki::JSBind::GetJSFunction("VideoPlayer.setVolume"))
    {
        setVolume->Invoke<void>(m_tag, value);
    }
}

int NapiHelper::getVolume(int m_tag)
{
    int volume = 0;
    if (auto getVolume = aki::JSBind::GetJSFunction("VideoPlayer.getVolume"))
    {
        volume = getVolume->Invoke<int>(m_tag);
    }
    return volume;
}
void NapiHelper::showDialog(const char *p_sBuffer)
{
    std::string strBuffer = p_sBuffer;
    if (auto showDialog = aki::JSBind::GetJSFunction("Dialog.showDialog"))
    {
        showDialog->Invoke<void>(strBuffer.c_str());
    }
}
void NapiHelper::setPreferredOrientation(int orientation)
{
    if (auto setPreferredOrientation = aki::JSBind::GetJSFunction("DeviceUtils.setPreferredOrientation"))
    {
        setPreferredOrientation->Invoke<void>(orientation);
    }
}
float NapiHelper::getScreenInch()
{
    std::promise<float> promise;
    std::function<void(float)> cb = [&promise](float message) { promise.set_value(message); };
    if (auto getScreenInch = aki::JSBind::GetJSFunction("DeviceUtils.getScreenInch"))
    {
        getScreenInch->Invoke<void>(cb);
    }
    float screenInch = promise.get_future().get();
    return screenInch;
}
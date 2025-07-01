#include "ALSourcePool.h"
#include <audio/AudioPlayer.h>
#include <audio/Decoder.h>
#include <audio/StaticDecoder.h>
#include <audio/StreamDecoder.h>
#include <cstring>
#include <utils/Log.h>
#if defined(OS_ANDROID) || defined(OS_OHOS)
#include "AL/alext.h"
#endif
#include <profiler/Profiler.h>
using namespace laya;
namespace audio
{
AudioPlayer::AudioPlayer()
{
    Profiler_ZoneScoped("audio::AudioPlayer::AudioPlayer", 0xff0000);
    m_alDevice = alcOpenDevice(NULL);
    m_alContext = alcCreateContext(m_alDevice, NULL);
    alcMakeContextCurrent(m_alContext);
    m_pool = std::make_unique<ALSourcePool>();
}

AudioPlayer::~AudioPlayer()
{
    m_pool.reset();
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_alContext);
    alcCloseDevice(m_alDevice);
}
std::shared_ptr<Audio> AudioPlayer::createAudio(std::shared_ptr<Decoder> decoder)
{
    Profiler_ZoneScoped("audio::AudioPlayer::createAudio", 0xff0000);
    if (!decoder) {
        return nullptr;
    }
    
    switch (decoder->getType())
    {
    case Decoder::TYPE_STATIC:
        return createAudio(std::static_pointer_cast<StaticDecoder>(decoder));
    case Decoder::TYPE_STREAM:
        return createAudio(std::static_pointer_cast<StreamDecoder>(decoder));
    default:
        return nullptr;
    }
}
std::shared_ptr<Audio> AudioPlayer::createAudio(std::shared_ptr<StaticDecoder> decoder)
{
    Profiler_ZoneScoped("audio::AudioPlayer::createAudio", 0xff0000);
    return std::make_shared<Audio>(this, decoder);
}
std::shared_ptr<Audio> AudioPlayer::createAudio(std::shared_ptr<StreamDecoder> decoder)
{
    Profiler_ZoneScoped("audio::AudioPlayer::createAudio", 0xff0000);
    return std::make_shared<Audio>(this, decoder);
}
void AudioPlayer::play(std::shared_ptr<Audio> audio)
{
    Profiler_ZoneScoped("audio::AudioPlayer::play", 0xff0000);
    audio->play();
}
void AudioPlayer::pause(std::shared_ptr<Audio> audio)
{
    Profiler_ZoneScoped("audio::AudioPlayer::pause", 0xff0000);
    audio->pause();
}
void AudioPlayer::stop(std::shared_ptr<Audio> audio)
{
    Profiler_ZoneScoped("audio::AudioPlayer::stop", 0xff0000);
    audio->stop();
}
void AudioPlayer::setVolume(std::shared_ptr<Audio> audio, float volume)
{
    Profiler_ZoneScoped("audio::AudioPlayer::setVolume", 0xff0000);
    audio->setVolume(volume);
}
float AudioPlayer::getVolume(std::shared_ptr<Audio> audio)
{   
    Profiler_ZoneScoped("audio::AudioPlayer::getVolume", 0xff0000);
    return audio->getVolume();
}
double AudioPlayer::getDuration(std::shared_ptr<Audio> audio)
{   
    Profiler_ZoneScoped("audio::AudioPlayer::getDuration", 0xff0000);
    return audio->getDuration();
}
void AudioPlayer::seek(std::shared_ptr<Audio> audio, double seconds)
{
    Profiler_ZoneScoped("audio::AudioPlayer::seek", 0xff0000);
    audio->seek(seconds);
}
double AudioPlayer::tell(std::shared_ptr<Audio> audio)
{
    Profiler_ZoneScoped("audio::AudioPlayer::tell", 0xff0000);
    return audio->tell();
}
void AudioPlayer::onPause()
{
    Profiler_ZoneScoped("audio::AudioPlayer::onPause", 0xff0000);
    ALSourcePool::ScopedLock lock(*m_pool);
    
    #if defined(OS_ANDROID) || defined(OS_OHOS)
        alcDevicePauseSOFT(m_alDevice);
    #else
        alcMakeContextCurrent(nullptr);
    #endif
}
void AudioPlayer::onResume()
{
    Profiler_ZoneScoped("audio::AudioPlayer::onResume", 0xff0000);
    ALSourcePool::ScopedLock lock(*m_pool);
    #if defined(OS_ANDROID) || defined(OS_OHOS)
        alcDeviceResumeSOFT(m_alDevice);
    #else
    if (m_alContext && alcGetCurrentContext() != m_alContext)
    {
		alcMakeContextCurrent(m_alContext);
    }
    #endif
}
void AudioPlayer::clear()
{
    Profiler_ZoneScoped("audio::AudioPlayer::clear", 0xff0000);
    m_pool->clear();
}
} // namespace audio

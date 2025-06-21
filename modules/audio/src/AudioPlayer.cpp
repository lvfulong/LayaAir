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
namespace audio
{
AudioPlayer::AudioPlayer()
{
    m_alDevice = alcOpenDevice(NULL);
    m_alContext = alcCreateContext(m_alDevice, NULL);
    alcMakeContextCurrent(m_alContext);
    m_pool = std::make_unique<ALSourcePool>();
    m_thread = std::thread([this]() {
        while (true)
        {
            if (m_exit)
                break;
            m_pool->update();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
}

AudioPlayer::~AudioPlayer()
{
    m_exit = true;
    m_thread.join();
    m_pool.reset();
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_alContext);
    alcCloseDevice(m_alDevice);
}
std::shared_ptr<Audio> AudioPlayer::createAudio(std::shared_ptr<Decoder> decoder)
{
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
    return std::make_shared<Audio>(this, decoder);
}
std::shared_ptr<Audio> AudioPlayer::createAudio(std::shared_ptr<StreamDecoder> decoder)
{
    return std::make_shared<Audio>(this, decoder);
}
void AudioPlayer::play(std::shared_ptr<Audio> audio)
{
    audio->play();
}
void AudioPlayer::pause(std::shared_ptr<Audio> audio)
{
    audio->pause();
}
void AudioPlayer::stop(std::shared_ptr<Audio> audio)
{
    audio->stop();
}
void AudioPlayer::setVolume(std::shared_ptr<Audio> audio, float volume)
{
    audio->setVolume(volume);
}
float AudioPlayer::getVolume(std::shared_ptr<Audio> audio)
{
    return audio->getVolume();
}
double AudioPlayer::getDuration(std::shared_ptr<Audio> audio)
{
    return audio->getDuration();
}
void AudioPlayer::seek(std::shared_ptr<Audio> audio, double seconds)
{
    audio->seek(seconds);
}
double AudioPlayer::tell(std::shared_ptr<Audio> audio)
{
    return audio->tell();
}
void AudioPlayer::onPause()
{
    ALSourcePool::ScopedLock lock(*m_pool);
    
    #if defined(OS_ANDROID) || defined(OS_OHOS)
        alcDevicePauseSOFT(m_alDevice);
    #else
        alcMakeContextCurrent(nullptr);
    #endif
}
void AudioPlayer::onResume()
{
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
    m_pool->clear();
}
} // namespace audio

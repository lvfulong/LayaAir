#ifndef __AUDIO_PLAYER_H__
#define __AUDIO_PLAYER_H__

#ifdef OS_WINDOWS
#include <AL/al.h>
#include <AL/alc.h>
#include <xaudio2.h>
#elif defined(OS_IOS)
#include <OpenAL/al.h>
#include <OpenAl/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <atomic>
#include <cstddef>
#include <memory>
#include <thread>

namespace audio
{
class Audio;
class StaticDecoder;
class StreamDecoder;
class ALSourcePool;
class Decoder;
class AudioPlayer
{
  public:
    AudioPlayer();
    ~AudioPlayer();
    std::shared_ptr<Audio> createAudio(std::shared_ptr<Decoder> decoder);
    std::shared_ptr<Audio> createAudio(std::shared_ptr<StaticDecoder> decoder);
    std::shared_ptr<Audio> createAudio(std::shared_ptr<StreamDecoder> decoder);
    void play(std::shared_ptr<Audio> audio);
    void pause(std::shared_ptr<Audio> audio);
    void stop(std::shared_ptr<Audio> audio);
    void setVolume(std::shared_ptr<Audio> audio, float volume);
    float getVolume(std::shared_ptr<Audio> audio);
    double getDuration(std::shared_ptr<Audio> audio);
    void seek(std::shared_ptr<Audio> audio, double seconds);
    double tell(std::shared_ptr<Audio> audio);
    void onPause();
    void onResume();
    void clear();
  private:
    friend class Audio;
    std::unique_ptr<ALSourcePool> m_pool;
    ALCdevice *m_alDevice;
    ALCcontext *m_alContext;
    std::thread m_thread;
};
} // namespace audio
#endif
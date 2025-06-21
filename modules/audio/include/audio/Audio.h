#ifndef __AUDIO_H__
#define __AUDIO_H__

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
#include <cstddef>
#include <memory>
#include <queue>
#include <functional>

namespace audio
{
class AudioPlayer;
class StaticDecoder;
class StreamDecoder;
class Audio : public std::enable_shared_from_this<Audio>
{
  public:
    enum Type
    {
        TYPE_STATIC,
        TYPE_STREAM,
        TYPE_MAX_ENUM
    };
    Audio(AudioPlayer *player, std::shared_ptr<StaticDecoder> decoder);
    Audio(AudioPlayer *player, std::shared_ptr<StreamDecoder> decoder);
    ~Audio();
    const static int NumBuffers = 8;
    const static int DefaultBufferSize = 16384;
    bool play();
    void pause();
    void stop();
    void setVolume(float volume);
    float getVolume() const;
    double getDuration() const;
    bool getLoop() const;
    void setLoop(bool enable);
    void setOnPlayEnd(std::function<void()> onPlayEnd)
    {
      m_onPlayEnd = onPlayEnd;
    }
    void seek(double seconds);
    double tell() const;
    bool isFinished() const;
  private:
    friend class ALSourcePool;
    bool doPlay(ALuint source);
    void doStop();
    void doPrepare();
    void doResume();
    bool update();
    void reset();
    bool isPlaying();
    bool isValid() const
    {
        return m_source != AL_NONE;
    }
    ALenum getFormat(int bitPerSample, int channels);
  private:
    ALuint m_staticBuffer;
    Type m_type;
    ALuint m_source = AL_NONE;
    int m_offsetSamples = 0;
    bool m_loop = false;
    AudioPlayer *m_player;
    float m_volume = 1.0f;
    ALuint m_buffers[NumBuffers];
    std::shared_ptr<StreamDecoder> m_streamDecoder{nullptr};
    std::shared_ptr<StaticDecoder> m_staticDecoder{nullptr};
    uint8_t m_streamBuffer[DefaultBufferSize] = {0};
    std::function<void()> m_onPlayEnd;
};
} // namespace audio
#endif

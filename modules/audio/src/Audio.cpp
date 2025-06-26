#include <audio/Audio.h>
#include <audio/StaticDecoder.h>
#include <audio/StreamDecoder.h>
#include <cstring>
#include <utils/Log.h>
#include <utils/Preprocessor.h>
#include <profiler/Profiler.h>

namespace audio
{
Audio::Audio(AudioPlayer *player, std::shared_ptr<StaticDecoder> decoder)
    : m_player(player), m_type(TYPE_STATIC), m_staticDecoder(decoder)
{
    ALenum format = Audio::getFormat(decoder->getBitPerSample(), decoder->getChannelCount());
    if (format == AL_NONE)
    {
        LOGE("Audio::Audio format is not supported");
        format = AL_FORMAT_STEREO16;
    }
    alGenBuffers(1, &m_staticBuffer);
    alBufferData(m_staticBuffer, format, decoder->getData(), decoder->getSizeInBytes(), decoder->getSampleRate());
}
Audio::Audio(AudioPlayer *player, std::shared_ptr<StreamDecoder> decoder)
    : m_player(player), m_type(TYPE_STREAM), m_streamDecoder(decoder)
{
    ALenum format = Audio::getFormat(decoder->getBitPerSample(), decoder->getChannelCount());
    if (format == AL_NONE)
    {
        LOGE("Audio::Audio format is not supported");
        format = AL_FORMAT_STEREO16;
    }
    alGenBuffers(NumBuffers, m_buffers);
    DEBUG_CHECK(alGetError() == AL_NO_ERROR && "Could not create buffers");
}
Audio::~Audio()
{
    if (m_type == TYPE_STATIC)
    {
        alDeleteBuffers(1, &m_staticBuffer);
    }
}
ALenum Audio::getFormat(int bitPerSample, int channels)
{
    if (bitPerSample != 8 && bitPerSample != 16)
        return AL_NONE;

    if (channels == 1)
        return bitPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    else if (channels == 2)
        return bitPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    return AL_NONE;
}
bool Audio::isPlaying()
{
    if (!isValid())
        return false;
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool Audio::isFinished() const
{
    if (!isValid())
        return false;
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}
void Audio::setLoop(bool enable)
{
    ALSourcePool::ScopedLock lock(*m_player->m_pool);
    if (isValid())
    {
        if (m_type == TYPE_STATIC)
        {
            alSourcei(m_source, AL_LOOPING, enable ? AL_TRUE : AL_FALSE);
        }
        else if (m_type == TYPE_STREAM)
        {
            if (m_streamDecoder)
            {
                m_streamDecoder->setLoop(enable);
            }
        }
    }
    m_loop = enable;
}
bool Audio::getLoop() const
{
    return m_loop;
}
bool Audio::play()
{
    if (!m_player || !m_player->m_pool)
    {
        LOGE("Error: player or pool is null");
        return false;
    }
    Profiler_ZoneScoped("audio::Audio::play outter", 0xff0000);
    ALSourcePool::ScopedLock lock(*m_player->m_pool);
    {
        Profiler_ZoneScoped("audio::Audio::play inner", 0x00ff00);
        ALuint out;
        bool isInPool;
        if (!m_player->m_pool->allocateSource(Audio::shared_from_this(), out, isInPool))
        {
            return false;
        }

        if (isInPool)
        {
            doResume();
            return true;
        }
        else
        {
            return doPlay(out);
        }
    }
}
void Audio::pause()
{
    if (!m_player || !m_player->m_pool)
    {
        LOGE("Error: player or pool is null in pause");
        return;
    }
    
    ALSourcePool::ScopedLock lock(*m_player->m_pool);
    if (isValid() && m_player->m_pool->isInPool(shared_from_this()))
    {
        alSourcePause(m_source);
    }
}
void Audio::doResume()
{
    if (!m_player || !m_player->m_pool)
    {
        LOGE("Error: player or pool is null in doResume");
        return;
    }
    
    if (isValid() && !isPlaying())
    {
        alSourcePlay(m_source);
        if (alGetError() == AL_INVALID_VALUE)
        {
            m_player->m_pool->recycleSource(shared_from_this(), true);
        }
    }
}
void Audio::stop()
{
    ALSourcePool::ScopedLock lock(*m_player->m_pool);
    if (isValid())
    {
        m_player->m_pool->recycleSource(shared_from_this(), true);
    }
}
void Audio::setVolume(float volume)
{
    ALSourcePool::ScopedLock lock(*m_player->m_pool);
    if (isValid())
    {
        alSourcef(m_source, AL_GAIN, volume);
    }
    m_volume = volume;
}
float Audio::getVolume() const
{
    ALSourcePool::ScopedLock lock(*m_player->m_pool);
    if (isValid())
    {
        ALfloat v;
        alGetSourcef(m_source, AL_GAIN, &v);
        return v;
    }
    return m_volume;
}
double Audio::getDuration() const
{
    if (m_type == TYPE_STATIC)
    {
        return m_staticDecoder->getDuration();
    }
    else if (m_type == TYPE_STREAM)
    {
        if (m_streamDecoder)
        {
            return m_streamDecoder->getDuration();
        }
        return 0.0;
    }
    return 0.0;
}
bool Audio::update()
{
    if (!isValid())
        return false;

    switch (m_type)
    {
    case TYPE_STATIC: {
        return !isFinished();
    }
    case TYPE_STREAM: {
        if (!m_streamDecoder)
        {
            LOGE("Error: stream decoder is null in update");
            return false;
        }
        
        ALint processed;
        ALint state;

        /* Get relevant source info */
        alGetSourcei(m_source, AL_SOURCE_STATE, &state);
        alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);
        if (alGetError() != AL_NO_ERROR)
        {
            LOGE("Error checking source state");
            return false;
        }

        /* Unqueue and handle each processed buffer */
        while (processed > 0)
        {
            ALuint bufid;
            alSourceUnqueueBuffers(m_source, 1, &bufid);
            processed--;

            int decoded = std::max(m_streamDecoder->getAudio(m_streamBuffer, DefaultBufferSize), (size_t)0);
            if (decoded > 0)
            {
                int fmt = Audio::getFormat(m_streamDecoder->getBitPerSample(), m_streamDecoder->getChannelCount());
                DEBUG_CHECK(fmt != AL_NONE);
                alBufferData(bufid, fmt, m_streamBuffer, decoded, m_streamDecoder->getSampleRate());
                alSourceQueueBuffers(m_source, 1, &bufid);
                if (decoded < DefaultBufferSize)
                {
                    break;
                }
            }
            else
            {
                break;
            }
            if (alGetError() != AL_NO_ERROR)
            {
                LOGE("Error buffering data");
                return false;
            }
        }
        /* Make sure the source hasn't underrun */
        if (state != AL_PLAYING && state != AL_PAUSED)
        {
            ALint queued;

            /* If no buffers are queued, playback is finished */
            alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued);
            if (queued == 0)
                return false;

            alSourcePlay(m_source);
            if (alGetError() != AL_NO_ERROR)
            {
                LOGE("Error restarting playback");
                return false;
            }
        }
        return true;
    }
    }
    return false;
}
void Audio::doStop()
{
    if (!isValid())
        return;
    alSourceStop(m_source);
    switch (m_type)
    {
    case TYPE_STATIC:
        break;
    case TYPE_STREAM: {
        if (m_streamDecoder)
        {
            m_streamDecoder->rewind();
        }
        
        ALint queued = 0;
        ALuint buffers[NumBuffers];

        alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued);
        alSourceUnqueueBuffers(m_source, queued, buffers);
        break;
    }
    case TYPE_MAX_ENUM:
        break;
    }
    alSourcei(m_source, AL_BUFFER, AL_NONE);
    m_offsetSamples = 0;
    m_source = AL_NONE;

    if (m_onPlayEnd)
    {
        m_onPlayEnd();
    }
}
void Audio::doPrepare()
{
    reset();
    switch (m_type)
    {
    case TYPE_STATIC:
        alSourcei(m_source, AL_BUFFER, m_staticBuffer);
        break;
    case TYPE_STREAM:
        if (!m_streamDecoder)
        {
            LOGE("Error: stream decoder is null");
            m_player->m_pool->recycleSource(shared_from_this(), true);
            break;
        }
        int i;
        for (i = 0; i < NumBuffers; i++)
        {
            int decoded = std::max(m_streamDecoder->getAudio(m_streamBuffer, DefaultBufferSize), (size_t)0);
            if (decoded > 0)
            {
                int fmt = Audio::getFormat(m_streamDecoder->getBitPerSample(), m_streamDecoder->getChannelCount());
                DEBUG_CHECK(fmt != AL_NONE);
                alBufferData(m_buffers[i], fmt, m_streamBuffer, decoded, m_streamDecoder->getSampleRate());
            }
            else
            {
                break;
            }
        }
        if (alGetError() != AL_NO_ERROR)
        {
            LOGE("Error buffering for playback");
            m_player->m_pool->recycleSource(shared_from_this(), true);
        }
        alSourceQueueBuffers(m_source, i, m_buffers);
        if (alGetError() != AL_NO_ERROR)
        {
            LOGE("Error starting playback");
            m_player->m_pool->recycleSource(shared_from_this(), true);
        }
        break;
    }
    if (m_type == TYPE_STATIC)
    {
        alSourcei(m_source, AL_SAMPLE_OFFSET, m_offsetSamples);
    }
}
bool Audio::doPlay(ALuint source)
{
    m_source = source;
    doPrepare();
    alGetError();
    alSourcePlay(source);
    bool success = alGetError() == AL_NO_ERROR;
    if (m_type == TYPE_STREAM)
    {
        if (!isPlaying())
            success = false;
    }

    if (!success)
    {
        stop();
    }
    if (m_type == TYPE_STATIC)
    {
        m_offsetSamples = 0;
    }

    return success;
}
void Audio::reset()
{
    alSourceRewind(m_source);
    alSourcei(m_source, AL_BUFFER, AL_NONE);
    alSourcef(m_source, AL_GAIN, m_volume);
    alSourcei(m_source, AL_LOOPING, m_loop ? AL_TRUE : AL_FALSE);
}
void Audio::seek(double seconds)
{
    ALSourcePool::ScopedLock lock(*m_player->m_pool);
    if (m_type == TYPE_STREAM)
    {
        if (!isValid() || !m_streamDecoder)
            return;
            
        // 停止当前播放
        alSourceStop(m_source);
        
        // 清空所有已排队的缓冲区
        ALint queued = 0;
        ALuint buffers[NumBuffers];
        alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued);
        if (queued > 0)
        {
            alSourceUnqueueBuffers(m_source, queued, buffers);
        }
        
        // 调用解码器的seek函数
        bool seekSuccess = m_streamDecoder->seek((size_t)seconds);
        if (!seekSuccess)
        {
            LOGE("Failed to seek to %f seconds", seconds);
            return;
        }
        
        // 重新填充缓冲区
        int i;
        for (i = 0; i < NumBuffers; i++)
        {
            int decoded = std::max(m_streamDecoder->getAudio(m_streamBuffer, DefaultBufferSize), (size_t)0);
            if (decoded > 0)
            {
                int fmt = Audio::getFormat(m_streamDecoder->getBitPerSample(), m_streamDecoder->getChannelCount());
                DEBUG_CHECK(fmt != AL_NONE);
                alBufferData(m_buffers[i], fmt, m_streamBuffer, decoded, m_streamDecoder->getSampleRate());
            }
            else
            {
                break;
            }
        }
        
        if (alGetError() != AL_NO_ERROR)
        {
            LOGE("Error buffering data after seek");
            return;
        }
        
        // 重新排队缓冲区
        if (i > 0)
        {
            alSourceQueueBuffers(m_source, i, m_buffers);
            if (alGetError() != AL_NO_ERROR)
            {
                LOGE("Error queuing buffers after seek");
                return;
            }
        }
        
        // 如果之前在播放，则继续播放
        if (isPlaying())
        {
            alSourcePlay(m_source);
            if (alGetError() != AL_NO_ERROR)
            {
                LOGE("Error resuming playback after seek");
            }
        }
    }
    else if (m_type == TYPE_STATIC)
    {
        if (isPlaying())
        {
            alSourcef(m_source, AL_SEC_OFFSET, seconds);
        }
        m_offsetSamples = seconds * m_staticDecoder->getSampleRate();
    }
}
double Audio::tell() const
{
    ALSourcePool::ScopedLock lock(*m_player->m_pool);
    if (m_type == TYPE_STREAM)
    {
        if (!isValid() || !m_streamDecoder)
            return 0.0;
            
        // 使用解码器的tell方法获取精确位置
        return m_streamDecoder->tell();
    }
    else if (m_type == TYPE_STATIC)
    {
        int offset = 0;
        if (isValid())
        {
            alGetSourcei(m_source, AL_SAMPLE_OFFSET, &offset);
            return offset / (double)m_staticDecoder->getSampleRate();
        }
    }
    return 0.0;
}
} // namespace audio

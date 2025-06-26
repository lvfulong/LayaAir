#include "StreamDecoderMp3.h"
#include <cstring>
#include <memory>
#include <utils/Log.h>
#include <profiler/Profiler.h>
using namespace laya;
namespace audio
{
StreamDecoderMp3::~StreamDecoderMp3()
{
    if (m_decoder != nullptr)
    {
        drmp3_uninit(m_decoder);
        delete m_decoder;
        m_decoder = nullptr;
    }
}
bool StreamDecoderMp3::load(const std::shared_ptr<laya::Data>& data)
{
    {
        Profiler_ZoneScoped("audio::StreamDecoderMp3::load reference data", 0xff0000);
        m_data = data;
    }
    
    Profiler_ZoneScoped("audio::StreamDecoderMp3::load", 0x00ff00);
    m_decoder = new drmp3();
    if (!drmp3_init_memory(m_decoder, m_data->data(), m_data->size(), NULL))
    {
        delete m_decoder;
        m_decoder = nullptr;
        return false;
    }

    drmp3_uint64 samples = drmp3_get_pcm_frame_count(m_decoder);

    if (!samples)
    {
        return false;
    }
    m_size = samples * m_decoder->channels * sizeof(uint16_t);
    m_sampleRate = (float)m_decoder->sampleRate;
    m_sampleCount = (unsigned int)samples;
    m_channelCount = m_decoder->channels;

    return true;
}
bool StreamDecoderMp3::canDecode(int8_t *data, size_t size)
{
    return Decoder::isMp3Format((const uint8_t *)data, size);
}

size_t StreamDecoderMp3::getAudio(uint8_t *buffer, size_t bufferSize)
{
    if (!m_decoder)
        return 0;
        
    int maxRead = bufferSize / sizeof(int16_t) / m_decoder->channels;
    int read = (int)drmp3_read_pcm_frames_s16(m_decoder, maxRead, (drmp3_int16 *)buffer);

    if (read < maxRead)
    {
        if (m_loop)
        {
            rewind();
        }
        else
        {
            m_eof = true;
        }
    }
    return read * sizeof(int16_t) * m_decoder->channels;
}
bool StreamDecoderMp3::seek(size_t seconds)
{
    if (!m_decoder)
        return false;
        
    drmp3_uint64 samples = (drmp3_uint64)(seconds * m_decoder->sampleRate);
    drmp3_bool32 success = drmp3_seek_to_pcm_frame(m_decoder, samples);

    if (success)
    {
        m_eof = false;
    }

    return success;
}
bool StreamDecoderMp3::rewind()
{
    return seek(0);
}
bool StreamDecoderMp3::isFinished()
{
    return m_eof;
}
double StreamDecoderMp3::tell() const
{
    if (!m_decoder || m_decoder->sampleRate == 0)
        return 0.0;
        
    // 使用drmp3结构体中的currentPCMFrame字段获取当前位置
    return (double)m_decoder->currentPCMFrame / m_decoder->sampleRate;
}
} // namespace audio

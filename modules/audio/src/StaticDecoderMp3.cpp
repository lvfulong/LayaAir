#include "StaticDecoderMp3.h"
#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"
#include <cstring>
#include <utils/Log.h>
#include <profiler/Profiler.h>

#define MAX_CHANNELS 8
using namespace laya;
namespace audio
{
bool StaticDecoderMp3::load(const uint8_t *data, size_t size)
{
   
    // 首先检查是否为MP3格式
    if (!Decoder::isMp3Format(data, size))
    {
        return false;
    }
    Profiler_ZoneScoped("audio::StaticDecoderMp3::load", 0x00ff00);
    drmp3 decoder;
    if (!drmp3_init_memory(&decoder, data, size, NULL))
    {
        return false;
    }

    drmp3_uint64 samples = drmp3_get_pcm_frame_count(&decoder);

    if (!samples)
    {
        drmp3_uninit(&decoder);
        return false;
    }
    m_size = samples * decoder.channels * sizeof(uint16_t);
    m_data = new uint16_t[(unsigned int)(samples * decoder.channels)];
    m_sampleRate = (float)decoder.sampleRate;
    m_sampleCount = (unsigned int)samples;
    m_channelCount = decoder.channels;
    drmp3_seek_to_pcm_frame(&decoder, 0);
    drmp3_uint64 framesRead = drmp3_read_pcm_frames_s16(&decoder, m_sampleCount, (int16_t *)m_data);
    drmp3_uninit(&decoder);
    return true;
}

bool StaticDecoderMp3::canDecode(int8_t *data, size_t size)
{
    return Decoder::isMp3Format((const uint8_t *)data, size);
}
} // namespace audio

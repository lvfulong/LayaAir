#include "StaticDecoderWav.h"
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"
#include <cstring>
#include <utils/Log.h>
#include <profiler/Profiler.h>
using namespace laya;
namespace audio
{
bool StaticDecoderWav::load(const std::shared_ptr<laya::Data>& data)
{
    // 首先检查是否为WAV格式
    if (!Decoder::isWavFormat(data->bytes(), data->size()))
    {
        return false;
    }
    Profiler_ZoneScoped("audio::StaticDecoderWav::load", 0x00ff00);
    drwav decoder;
    if (!drwav_init_memory(&decoder, data->data(), data->size(), NULL))
    {
        return false;
    }

    drwav_uint64 samples = decoder.totalPCMFrameCount;

    if (!samples)
    {
        drwav_uninit(&decoder);
        return false;
    }
    m_size = samples * decoder.channels * sizeof(uint16_t);
    m_data = new uint16_t[(unsigned int)(samples * decoder.channels)];
    m_sampleRate = (float)decoder.sampleRate;
    m_sampleCount = (unsigned int)samples;
    m_channelCount = decoder.channels;
    drwav_seek_to_pcm_frame(&decoder, 0);

    drwav_uint64 framesRead = drwav_read_pcm_frames_s16(&decoder, m_sampleCount, (int16_t *)m_data);


    drwav_uninit(&decoder);

    return true;
}

bool StaticDecoderWav::canDecode(int8_t *data, size_t size)
{
    return Decoder::isWavFormat((const uint8_t *)data, size);
}
} // namespace audio

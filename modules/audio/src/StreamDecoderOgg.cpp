#include "StreamDecoderOgg.h"
#include <cstring>
#include <memory>
#include <utils/Log.h>
#include <profiler/Profiler.h>
using namespace laya;
namespace audio
{
StreamDecoderOgg::~StreamDecoderOgg()
{
    if (m_decoder != nullptr)
    {
        ov_clear(m_decoder);
        delete m_decoder;
        m_decoder = nullptr;
    }
}

bool StreamDecoderOgg::load(const std::shared_ptr<laya::Data>& data)
{
    // 首先检查是否为OGG格式
    if (!Decoder::isOggFormat(data->bytes(), data->size()))
    {
        return false;
    }
    {
        Profiler_ZoneScoped("audio::StreamDecoderOgg::load reference data", 0xff0000);
        m_data = data;
    }
    DEBUG_CHECK(m_decoder == nullptr);
    
    Profiler_ZoneScoped("audio::StreamDecoderOgg::load", 0x00ff00);
    m_decoder = new OggVorbis_File();
    int result = ov_open_callbacks(NULL, m_decoder, (const char *)m_data->data(), m_data->size(), OV_CALLBACKS_DEFAULT);
    if (result < 0)
    {
        delete m_decoder;
        m_decoder = nullptr;
        return false;
    }

    m_vorbisInfo = ov_info(m_decoder, -1);
    if (m_vorbisInfo == nullptr)
    {
        ov_clear(m_decoder);
        delete m_decoder;
        m_decoder = nullptr;
        return false;
    }

    // 不依赖 ov_pcm_total，而是通过 ov_read 来获取音频信息
    // 先读取一小部分数据来验证解码器工作正常
    char testBuffer[4096];
    int bitstream = 0;
    int testRead = ov_read(m_decoder, testBuffer, sizeof(testBuffer), 0, 2, 1, &bitstream);

    
    if (testRead < 0)
    {
        ov_clear(m_decoder);
        delete m_decoder;
        m_decoder = nullptr;
        return false;
    }

    // 尝试重置到开始位置，但不强制要求成功
    int seekResult = ov_pcm_seek(m_decoder, 0);

    if (seekResult < 0)
    {
        //LOGE("StreamDecoderOgg::load: ov_pcm_seek failed with code %d, but continuing", seekResult);
        // 对于流式解码器，seek失败不是致命错误，可以继续
    }

    // 尝试获取总样本数，但不强制要求成功
    ogg_int64_t totalSamples = ov_pcm_total(m_decoder, -1);
    
    if (totalSamples < 0)
    {
        // 如果 ov_pcm_total 失败，我们仍然可以继续，只是不知道总长度
        //LOGE("StreamDecoderOgg::load: ov_pcm_total failed, but continuing with unknown length");
        m_sampleCount = 0; // 未知长度
    }
    else
    {
        m_sampleCount = (unsigned int)totalSamples;
    }

    m_sampleRate = (float)m_vorbisInfo->rate;
    m_channelCount = m_vorbisInfo->channels;
    
    if (m_sampleCount > 0)
    {
        m_size = totalSamples * m_vorbisInfo->channels * sizeof(int16_t);
    }
    else
    {
        m_size = 0; // 未知大小
    }
    
    m_eof = false;

    return true;
}

bool StreamDecoderOgg::canDecode(int8_t *data, size_t size)
{
    bool result = Decoder::isOggFormat((const uint8_t *)data, size);
    return result;
}

size_t StreamDecoderOgg::getAudio(uint8_t *buffer, size_t bufferSize)
{
    if (!m_decoder || m_eof)
    {
        return 0;
    }

    int bitstream = 0;
    int totalRead = 0;
    int maxRead = bufferSize / sizeof(int16_t) / m_channelCount;
    
    while (totalRead < maxRead)
    {
        int read = ov_read(m_decoder, (char*)buffer + totalRead * sizeof(int16_t) * m_channelCount, 
                          (maxRead - totalRead) * sizeof(int16_t) * m_channelCount, 
                          0, 2, 1, &bitstream);
        
        if (read > 0)
        {
            totalRead += read / (sizeof(int16_t) * m_channelCount);
        }
        else if (read == 0)
        {
            // 到达文件末尾
            if (m_loop)
            {
                rewind();
                continue;
            }
            else
            {
                m_eof = true;
                break;
            }
        }
        else
        {
            // 解码错误
            LOGE("StreamDecoderOgg error: Decoding failed with code %d", read);
            m_eof = true;
            break;
        }
    }

    return totalRead * sizeof(int16_t) * m_channelCount;
}

bool StreamDecoderOgg::seek(size_t seconds)
{
    if (!m_decoder)
    {
        LOGW("StreamDecoderOgg::seek: decoder is null");
        return false;
    }

    ogg_int64_t targetSample = (ogg_int64_t)(seconds * m_sampleRate);
    int result = ov_pcm_seek(m_decoder, targetSample);
    
    if (result == 0)
    {
        m_eof = false;
        return true;
    }
    
    LOGW("StreamDecoderOgg::seek: failed with code %d", result);
    return false;
}

bool StreamDecoderOgg::rewind()
{
    return seek(0);
}

bool StreamDecoderOgg::isFinished()
{
    return m_eof;
}

double StreamDecoderOgg::tell() const
{
    if (!m_decoder || m_sampleRate == 0)
    {
        LOGW("StreamDecoderOgg::tell: decoder is null or sample rate is zero");
        return 0.0;
    }
    
    ogg_int64_t currentSample = ov_pcm_tell(m_decoder);
    double position = (currentSample < 0) ? 0.0 : (double)currentSample / m_sampleRate;
  
    return position;
}
} // namespace audio

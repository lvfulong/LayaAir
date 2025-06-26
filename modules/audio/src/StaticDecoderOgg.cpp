#include "StaticDecoderOgg.h"
#include <cstring>
#include <ogg/ogg.h>
#include <utils/Log.h>
#include <vorbis/vorbisfile.h>
#include <profiler/Profiler.h>
using namespace laya;
namespace audio
{
bool StaticDecoderOgg::load(const std::shared_ptr<laya::Data>& data)
{
    DEBUG_CHECK(data);
    DEBUG_CHECK(m_data == nullptr);
    
    // 首先检查是否为OGG格式
    if (!Decoder::isOggFormat(data->bytes(), data->size()))
    {
        return false;
    }
    Profiler_ZoneScoped("audio::StaticDecoderOgg::load", 0x00ff00);
    OggVorbis_File oggStream;
    int result = ov_open_callbacks(NULL, &oggStream, (const char *)data->data(), data->size(), OV_CALLBACKS_DEFAULT);
    if (result < 0)
    {
        return false;
    }

    // 使用RAII确保资源清理
    struct OggStreamGuard
    {
        OggVorbis_File *stream;
        OggStreamGuard(OggVorbis_File *s) : stream(s)
        {
        }
        ~OggStreamGuard()
        {
            ov_clear(stream);
        }
    } guard(&oggStream);

    vorbis_info *vorbisInfo = ov_info(&oggStream, -1);
    if (vorbisInfo == nullptr)
    {
        LOGE("StaticDecoderOgg error : Failed to get vorbis info");
        return false;
    }

    const size_t BUFFER_SIZE = 524288;
    size_t bufferSize = BUFFER_SIZE;

    // 使用智能指针管理内存
    std::unique_ptr<uint16_t[]> dataBuffer(new (std::nothrow) uint16_t[BUFFER_SIZE]);
    if (!dataBuffer)
    {
        LOGE("StaticDecoderOgg error : Failed to allocate memory");
        return false;
    }

    std::vector<char> tmpBuffer(BUFFER_SIZE); // 使用vector替代静态数组
    int size = 0;
    int bitstream = 0;
    int decodedSize = 0;
    ogg_int64_t totalPcmSamples = 0;

    while (true)
    {
        decodedSize = ov_read(&oggStream, tmpBuffer.data(), bufferSize, 0, 2, 1, &bitstream);
        if (decodedSize > 0)
        {
            if (bufferSize < size + decodedSize)
            {
                size_t newBufferSize = bufferSize;
                while (newBufferSize < size + decodedSize)
                {
                    newBufferSize <<= 1;
                    if (newBufferSize == 0)
                    { // 检查溢出
                        LOGE("StaticDecoderOgg error : Buffer size overflow");
                        return false;
                    }
                }

                std::unique_ptr<uint16_t[]> newData(new (std::nothrow) uint16_t[newBufferSize]);
                if (!newData)
                {
                    LOGE("StaticDecoderOgg error : Failed to allocate memory");
                    return false;
                }

                memcpy(newData.get(), dataBuffer.get(), size);
                dataBuffer = std::move(newData);
                bufferSize = newBufferSize;
            }

            memcpy((uint8_t *)dataBuffer.get() + size, tmpBuffer.data(), decodedSize);
            size += decodedSize;
            totalPcmSamples += decodedSize / (vorbisInfo->channels * sizeof(int16_t));
        }
        else if (decodedSize == 0)
        {
            break;
        }
        else
        {
            LOGE("StaticDecoderOgg error : Decoding failed with code %d", decodedSize);
            return false;
        }
    }

    if (size == 0)
    {
        LOGE("StaticDecoderOgg error : No data decoded");
        return false;
    }

    // 收缩缓冲区
    if (bufferSize > size)
    {
        std::unique_ptr<uint8_t[]> newData(new (std::nothrow) uint8_t[size]);
        if (!newData)
        {
            LOGE("StaticDecoderOgg error : Failed to allocate memory");
            return false;
        }

        memcpy(newData.get(), dataBuffer.get(), size);
        m_data = (uint16_t *)newData.release();
    }
    else
    {
        m_data = dataBuffer.release();
    }

    m_sampleCount = (unsigned int)totalPcmSamples;
    m_channelCount = vorbisInfo->channels;
    m_sampleRate = vorbisInfo->rate;
    m_size = totalPcmSamples * vorbisInfo->channels * sizeof(int16_t);
    DEBUG_CHECK(m_size == size);

    return true;
}
bool StaticDecoderOgg::canDecode(int8_t *data, size_t size)
{
    return Decoder::isOggFormat((const uint8_t *)data, size);
}
} // namespace audio
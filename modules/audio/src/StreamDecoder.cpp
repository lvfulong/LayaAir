#include <audio/StreamDecoder.h>
#include <cstring>
#include <memory>
#include <utils/Log.h>
#include "StreamDecoderMp3.h"
#include "StreamDecoderOgg.h"

namespace audio
{

std::shared_ptr<StreamDecoder> StreamDecoder::create(const std::string &url, const uint8_t *data, size_t dataSize)
{
    // 首先尝试Ogg格式
    auto oggDecoder = std::make_shared<StreamDecoderOgg>();
    if (oggDecoder->canDecode((int8_t*)data, dataSize) && oggDecoder->load(data, dataSize))
    {
        return oggDecoder;
    }
    
    // 然后尝试MP3格式
    auto mp3Decoder = std::make_shared<StreamDecoderMp3>();
    if (mp3Decoder->canDecode((int8_t*)data, dataSize) && mp3Decoder->load(data, dataSize))
    {
        return mp3Decoder;
    }
    
    return nullptr;
}
} // namespace audio

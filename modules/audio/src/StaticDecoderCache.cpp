#include "StaticDecoderMp3.h"
#include "StaticDecoderOgg.h"
#include "StaticDecoderWav.h"
#include "StreamDecoderMp3.h"
#include <audio/StaticDecoder.h>
#include <audio/StaticDecoderCache.h>
#include <audio/StreamDecoder.h>
#include <cstring>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utils/Log.h>
#include <vector>

namespace audio
{
laya::LRUCache<std::string, std::shared_ptr<Decoder>> StaticDecoderCache::m_cache(128);
void StaticDecoderCache::setMaxCacheNum(int num)
{
    StaticDecoderCache::m_cache.clearAndSetCapacity(num);
}
int StaticDecoderCache::getMaxCacheNum()
{
    return StaticDecoderCache::m_cache.getCapacity();
}
std::shared_ptr<Decoder> StaticDecoderCache::get(const std::string &url)
{
    return StaticDecoderCache::m_cache.get(url).value_or(nullptr);
}
using DecoderCreateFunctionType = std::function<std::shared_ptr<Decoder>(uint8_t *, size_t)>;
template <typename DecoderType> DecoderCreateFunctionType DecoderCreateTrait()
{
    return [](uint8_t *data, size_t size) -> std::shared_ptr<Decoder> {
        auto decoder = std::make_shared<DecoderType>();
        if (decoder->load(data, size))
        {
            return decoder;
        }
        return nullptr;
    };
}
std::shared_ptr<Decoder> StaticDecoderCache::createDecoder(const std::string &url, uint8_t *data, size_t size)
{
    static std::vector<DecoderCreateFunctionType> allStaticDecoders = {
        DecoderCreateTrait<StaticDecoderMp3>(),
        DecoderCreateTrait<StaticDecoderWav>(),
        DecoderCreateTrait<StaticDecoderOgg>(),
    };
    for (auto &testDecoder : allStaticDecoders)
    {
        auto d = testDecoder(data, size);
        if (d)
        {
            StaticDecoderCache::m_cache.put(url, d);
            return d;
        }
    }
    return nullptr;
}

} // namespace audio

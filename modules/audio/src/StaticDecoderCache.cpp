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
laya::LRUCache<std::string, std::shared_ptr<Decoder>> StaticDecoderCache::m_cache(laya::LRUCache<std::string, std::shared_ptr<Decoder>>::UnlimitedCapacity, [](const std::string& key, const std::shared_ptr<Decoder>& value) {
    StaticDecoderCache::m_currentSizeInBytes -= value->getSizeInBytes();
});
size_t StaticDecoderCache::m_maxSizeInBytes{ 128 * 1024 * 1024 }; // 128MB
size_t StaticDecoderCache::m_currentSizeInBytes{ 0 };
void StaticDecoderCache::clear()
{
    StaticDecoderCache::m_cache.clear();
    StaticDecoderCache::m_currentSizeInBytes = 0;
}
void StaticDecoderCache::setMaxSizeInBytes(size_t size)
{
    StaticDecoderCache::m_maxSizeInBytes = size;
    while (StaticDecoderCache::m_currentSizeInBytes > StaticDecoderCache::m_maxSizeInBytes)
    {
        StaticDecoderCache::m_cache.removeOldest();
    }
}
size_t StaticDecoderCache::getMaxSizeInBytes()
{
    return StaticDecoderCache::m_maxSizeInBytes;
}
size_t StaticDecoderCache::getCurrentSizeInBytes()
{
    return StaticDecoderCache::m_currentSizeInBytes;
}
std::shared_ptr<Decoder> StaticDecoderCache::get(const std::string &url)
{
    return StaticDecoderCache::m_cache.get(url).value_or(nullptr);
}
using DecoderCreateFunctionType = std::function<std::shared_ptr<Decoder>(const std::shared_ptr<laya::Data>&)>;
template <typename DecoderType> DecoderCreateFunctionType DecoderCreateTrait()
{
    return [](const std::shared_ptr<laya::Data>& data) -> std::shared_ptr<Decoder> {
        auto decoder = std::make_shared<DecoderType>();
        if (decoder->load(data))
        {
            return decoder;
        }
        return nullptr;
    };
}
std::shared_ptr<Decoder> StaticDecoderCache::createDecoder(const std::string &url, const std::shared_ptr<laya::Data>& data)
{
    static std::vector<DecoderCreateFunctionType> allStaticDecoders = {
        DecoderCreateTrait<StaticDecoderMp3>(),
        DecoderCreateTrait<StaticDecoderWav>(),
        DecoderCreateTrait<StaticDecoderOgg>(),
    };
    for (auto &testDecoder : allStaticDecoders)
    {
        auto d = testDecoder(data);
        if (d)
        {
            bool canCache = data->size() < m_maxSizeInBytes;
            while (canCache && m_currentSizeInBytes + data->size() > m_maxSizeInBytes) 
            {
                auto oldest = m_cache.peekOldest();
                if (oldest) 
                {
                    m_cache.removeOldest();
                } 
                else 
                {
                    canCache = false;
                }
            }
            if (canCache)
            {
                m_cache.put(url, d);
                m_currentSizeInBytes += d->getSizeInBytes();
                return d;
            }
        }
    }
    return nullptr;
}

} // namespace audio

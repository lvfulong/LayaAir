#include "StaticDecoderMp3.h"
#include "StaticDecoderOgg.h"
#include "StaticDecoderWav.h"
#include "StreamDecoderMp3.h"
#include "StreamDecoderOgg.h"
#include <audio/AudioDecoderCache.h>
#include <audio/StaticDecoder.h>
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
laya::LRUCache<std::string, AudioDecoderCache::DecoderInfo> AudioDecoderCache::m_cache(
    laya::LRUCache<std::string, DecoderInfo>::kUnlimitedCapacity, [](const std::string &key, const DecoderInfo &value) {
        AudioDecoderCache::m_currentSizeInBytes -= value.getSizeInBytes();
    });
size_t AudioDecoderCache::m_maxSizeInBytes{256 * 1024 * 1024}; // 256MB
size_t AudioDecoderCache::m_currentSizeInBytes{0};
void AudioDecoderCache::clear()
{
    AudioDecoderCache::m_cache.clear();
    AudioDecoderCache::m_currentSizeInBytes = 0;
}
void AudioDecoderCache::setMaxSizeInBytes(size_t size)
{
    AudioDecoderCache::m_maxSizeInBytes = size;
    while (AudioDecoderCache::m_currentSizeInBytes > AudioDecoderCache::m_maxSizeInBytes)
    {
        AudioDecoderCache::m_cache.removeOldest();
    }
}
size_t AudioDecoderCache::getMaxSizeInBytes()
{
    return AudioDecoderCache::m_maxSizeInBytes;
}
size_t AudioDecoderCache::getCurrentSizeInBytes()
{
    return AudioDecoderCache::m_currentSizeInBytes;
}
std::shared_ptr<Decoder> AudioDecoderCache::get(const std::string &url)
{
    auto decoderInfo = AudioDecoderCache::m_cache.get(url);
    if (decoderInfo)
    {
        return decoderInfo.value().getDecoder();
    }
    return nullptr;
}
using DecoderCreateFunctionType = std::function<std::shared_ptr<Decoder>(const std::shared_ptr<laya::Data> &)>;
template <typename DecoderType> DecoderCreateFunctionType DecoderCreateTrait()
{
    return [](const std::shared_ptr<laya::Data> &data) -> std::shared_ptr<Decoder> {
        auto decoder = std::make_shared<DecoderType>();
        if (decoder->load(data))
        {
            return decoder;
        }
        return nullptr;
    };
}
std::shared_ptr<Decoder> AudioDecoderCache::createStaticDecoder(const std::string &url,
                                                                const std::shared_ptr<laya::Data> &data)
{
    static std::vector<DecoderCreateFunctionType> allStaticDecoders = {
        DecoderCreateTrait<StaticDecoderMp3>(),
        DecoderCreateTrait<StaticDecoderWav>(),
        DecoderCreateTrait<StaticDecoderOgg>(),
    };
    if (!data)
    {
        return nullptr;
    }
    for (auto &testDecoder : allStaticDecoders)
    {
        auto decoder = testDecoder(data);
        if (decoder)
        {
            bool canCache = decoder->getSizeInBytes() < m_maxSizeInBytes;
            while (canCache && m_currentSizeInBytes + decoder->getSizeInBytes() > m_maxSizeInBytes)
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
                m_cache.put(url, DecoderInfo(decoder));
                m_currentSizeInBytes += decoder->getSizeInBytes();
                return decoder;
            }
        }
    }
    return nullptr;
}
std::shared_ptr<Decoder> AudioDecoderCache::createStreamDecoder(const std::string &url,
                                                                const std::shared_ptr<laya::Data> &data)
{
    static std::vector<DecoderCreateFunctionType> allStreamDecoders = {
        DecoderCreateTrait<StreamDecoderMp3>(),
        DecoderCreateTrait<StreamDecoderOgg>(),
    };
    if (!data)
    {
        return nullptr;
    }
    for (auto &testDecoder : allStreamDecoders)
    {
        auto decoder = testDecoder(data);
        if (decoder)
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
                m_cache.put(url, DecoderInfo(data));
                m_currentSizeInBytes += data->size();
                return decoder;
            }
        }
    }
    return nullptr;
}
} // namespace audio

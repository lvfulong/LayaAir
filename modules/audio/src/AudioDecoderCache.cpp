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
    m_maxSizeInBytes = size;
    while (m_currentSizeInBytes > m_maxSizeInBytes && m_cache.size() > 0)
    {
        m_cache.removeOldest();
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

// 新增：统一的缓存空间回收逻辑
bool AudioDecoderCache::tryMakeSpace(size_t requiredSize)
{
    if (requiredSize >= m_maxSizeInBytes)
    {
        return false;
    }
    while (m_currentSizeInBytes + requiredSize > m_maxSizeInBytes && m_cache.size() > 0)
    {
        auto oldest = m_cache.peekOldest();
        if (oldest.has_value())
        {
            m_cache.removeOldest();
        }
        else
        {
            break;
        }
    }
    return m_currentSizeInBytes + requiredSize <= m_maxSizeInBytes;
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
            size_t decoderSize = decoder->getSizeInBytes();
            if (tryMakeSpace(decoderSize))
            {
                m_cache.put(url, DecoderInfo(decoder));
                m_currentSizeInBytes += decoderSize;
                return decoder;
            }
            // 无法缓存也返回解码器
            return decoder;
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
            size_t dataSize = data->size();
            if (tryMakeSpace(dataSize))
            {
                m_cache.put(url, DecoderInfo(data));
                m_currentSizeInBytes += dataSize;
                return decoder;
            }
            // 无法缓存也返回解码器
            return decoder;
        }
    }
    return nullptr;
}

} // namespace audio

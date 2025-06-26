#ifndef __AUDIO_DECODER_CACHE_H__
#define __AUDIO_DECODER_CACHE_H__

#ifdef OS_WINDOWS
#include <AL/al.h>
#include <AL/alc.h>
#include <xaudio2.h>
#elif defined(OS_IOS)
#include <OpenAL/al.h>
#include <OpenAl/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <audio/Decoder.h>
#include <audio/StreamDecoder.h>
#include <cstddef>
#include <memory>
#include <queue>
#include <unordered_map>
#include <utils/Data.h>
#include <utils/LRUCache.h>
#include <utils/Log.h>

namespace audio
{
class AudioDecoderCache
{
  public:
    AudioDecoderCache() = default;
    ~AudioDecoderCache() = default;
    static std::shared_ptr<Decoder> get(const std::string &url);
    static std::shared_ptr<Decoder> createStaticDecoder(const std::string &url,
                                                        const std::shared_ptr<laya::Data> &data);
    static std::shared_ptr<Decoder> createStreamDecoder(const std::string &url,
                                                        const std::shared_ptr<laya::Data> &data);
    static void setMaxSizeInBytes(size_t size);
    static size_t getMaxSizeInBytes();
    static size_t getCurrentSizeInBytes();
    static void clear();

  private:
    static bool tryMakeSpace(size_t requiredSize);

    enum class DecoderType
    {
        Static,
        Stream,
        Invalid,
    };
    class DecoderInfo
    {
      public:
        DecoderInfo() : m_decoderType(DecoderType::Invalid), m_staticDecoder(nullptr), m_streamData(nullptr)
        {
        }
        DecoderInfo(std::shared_ptr<Decoder> staticDecoder)
            : m_decoderType(DecoderType::Static), m_staticDecoder(staticDecoder), m_streamData(nullptr)
        {
        }
        DecoderInfo(std::shared_ptr<laya::Data> streamData)
            : m_decoderType(DecoderType::Stream), m_staticDecoder(nullptr), m_streamData(streamData)
        {
        }

        size_t getSizeInBytes() const
        {
            switch (m_decoderType)
            {
            case DecoderType::Static:
                return m_staticDecoder->getSizeInBytes();
            case DecoderType::Stream:
                return m_streamData->size();
            default:
                DEBUG_CHECK(false);
                return 0;
            }
        }
        std::shared_ptr<Decoder> getDecoder() const
        {
            switch (m_decoderType)
            {
            case DecoderType::Static:
                return m_staticDecoder;
            case DecoderType::Stream:
                return StreamDecoder::create(m_streamData);
            default:
                DEBUG_CHECK(false);
                return nullptr;
            }
        }

      private:
        DecoderType m_decoderType;
        std::shared_ptr<Decoder> m_staticDecoder{nullptr};
        std::shared_ptr<laya::Data> m_streamData{nullptr}; // stream decoder不能共用，只能共享内存数据，所以存原始数据
    };
    static laya::LRUCache<std::string, DecoderInfo> m_cache;
    static size_t m_maxSizeInBytes;
    static size_t m_currentSizeInBytes;
};
} // namespace audio
#endif

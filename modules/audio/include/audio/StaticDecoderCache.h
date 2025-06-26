#ifndef __STATIC_DECODER_CACHE_H__
#define __STATIC_DECODER_CACHE_H__

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
#include <cstddef>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <utils/LRUCache.h>
#include <utils/Data.h>

namespace audio
{
class Decoder;
class StaticDecoderCache
{
  public:
    StaticDecoderCache() = default;
    ~StaticDecoderCache() = default;
    static std::shared_ptr<Decoder> get(const std::string &url);
    static std::shared_ptr<Decoder> createDecoder(const std::string &url, const std::shared_ptr<laya::Data>& data);
    static void setMaxSizeInBytes(size_t size);
    static size_t getMaxSizeInBytes();
    static size_t getCurrentSizeInBytes();
    static void clear();
  private:
    static laya::LRUCache<std::string, std::shared_ptr<Decoder>> m_cache;
    static size_t m_maxSizeInBytes;
    static size_t m_currentSizeInBytes;
};
} // namespace audio
#endif

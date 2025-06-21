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

namespace audio
{
class Decoder;
class StaticDecoderCache
{
  public:
    StaticDecoderCache() = default;
    ~StaticDecoderCache() = default;
    static std::shared_ptr<Decoder> get(const std::string &url);
    static std::shared_ptr<Decoder> createDecoder(const std::string &url, uint8_t *data, size_t size);
    static void setMaxCacheNum(int num);
    static int getMaxCacheNum();
  private:
    static laya::LRUCache<std::string, std::shared_ptr<Decoder>> m_cache;
};
} // namespace audio
#endif

#ifndef __AL_SOURCE_POOL_H__
#define __AL_SOURCE_POOL_H__

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
#include <unordered_map>

namespace audio
{

class Audio;
class ALSourcePool
{
  public:
    class ScopedLock
    {
      public:
        ScopedLock(ALSourcePool &pool) : m_pool(pool)
        {
            m_pool.lock();
        }
        ~ScopedLock()
        {
            m_pool.unlock();
        }

      private:
        ALSourcePool &m_pool;
    };
    static const int MAX_SOURCES = 64;
    ALSourcePool();
    ~ALSourcePool();
    bool allocateSource(std::shared_ptr<Audio> audio, ALuint &out, bool &isInPool);
    bool recycleSource(std::shared_ptr<Audio> audio, bool stop);
    void lock();
    void unlock();
    void update();
    bool isInPool(std::shared_ptr<Audio> audio);
    void clear();
  private:
    std::mutex m_mutex;
    std::queue<ALuint> m_sources;
    int m_sourceCount{0};
    std::unordered_map<std::shared_ptr<Audio>, ALuint> m_playing;
};
} // namespace audio
#endif

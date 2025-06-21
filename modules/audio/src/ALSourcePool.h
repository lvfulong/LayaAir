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
#include <condition_variable>
#include <atomic>

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
    bool hasActiveAudio() const;
    
    // 信号量机制相关方法
    void waitForAudio();
    void notifyAudioAdded();
    void notifyAudioRemoved();
    
    // 线程安全的活跃音频检查
    bool hasActiveAudioThreadSafe() const;
    void requestShutdown();
    bool isShutdownRequested() const;
  private:
    std::mutex m_mutex;
    std::queue<ALuint> m_sources;
    int m_sourceCount{0};
    std::unordered_map<std::shared_ptr<Audio>, ALuint> m_playing;
    
    // 信号量机制相关成员 - 使用独立的锁
    std::condition_variable m_audioCondition;
    std::atomic<int> m_activeAudioCount{0};
    std::atomic<bool> m_shutdown{false};
    std::mutex m_waitMutex;  // 专门用于等待的独立锁
};
} // namespace audio
#endif

#include "ALSourcePool.h"
#include <cstring>
#include <utils/Log.h>
#include <thread> // Added for std::thread and std::this_thread::sleep_for
#include <chrono> // Added for std::chrono::milliseconds
#include <profiler/Profiler.h>
using namespace laya;
namespace audio
{
ALSourcePool::ALSourcePool()
{
    ALuint sources[MAX_SOURCES];
    for (int i = 0; i < MAX_SOURCES; i++)
    {
        alGenSources(1, &sources[i]);

        if (alGetError() != AL_NO_ERROR)
            break;

        m_sourceCount++;
    }

    for (int i = 0; i < m_sourceCount; i++)
        m_sources.push(sources[i]);
    
    // 启动更新线程
    startUpdateThread();
}

ALSourcePool::~ALSourcePool()
{
   stopUpdateThread();
}

// 线程管理实现
void ALSourcePool::startUpdateThread()
{
    if (m_updateThread.joinable())
        return;
        
    m_updateThread = std::thread(&ALSourcePool::updateThreadWorker, this);
}

void ALSourcePool::stopUpdateThread()
{
    if (!m_updateThread.joinable())
        return;
        
    requestShutdown();
    m_updateThread.join();
}

bool ALSourcePool::isUpdateThreadRunning() const
{
    return m_updateThread.joinable();
}

void ALSourcePool::updateThreadWorker()
{
    while (!isShutdownRequested())
    {
        Profiler_ZoneScoped("audio::ALSourcePool::updateThreadWorker", 0xff0000);
        // 检查是否有活跃的音频播放
        if (hasActiveAudio())
        {
            Profiler_ZoneScoped("audio::ALSourcePool::update", 0xff0000);
            update();
            // 有音频播放时使用较短间隔
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        else
        {
            Profiler_ZoneScoped("audio::ALSourcePool::waitForAudio", 0x00ff00);
            // 无音频播放时，使用信号量等待
            waitForAudio();
            
            // 被唤醒后，检查是否应该退出
            if (isShutdownRequested())
                break;
        }
    }
}

void ALSourcePool::requestShutdown()
{
    m_shutdown.store(true);
    m_audioCondition.notify_all();
}

bool ALSourcePool::isShutdownRequested() const
{
    return m_shutdown.load();
}
void ALSourcePool::lock()
{
    m_mutex.lock();
}

void ALSourcePool::unlock()
{
    m_mutex.unlock();
}

bool ALSourcePool::allocateSource(std::shared_ptr<Audio> audio, ALuint &out, bool &isInPool)
{
    out = 0;

    auto i = m_playing.find(audio);
    if (i != m_playing.end())
    {
        return isInPool = true;
    }

    isInPool = false;

    if (m_sources.empty())
    {
        LOGE("ALSourcePool::allocateSource: No available sources");
        return false;
    }

    out = m_sources.front();
    m_sources.pop();

    m_playing.insert(std::make_pair(audio, out));
    
    // 通知有新音频添加
    notifyAudioAdded();
    
    return true;
}

bool ALSourcePool::recycleSource(std::shared_ptr<Audio> audio, bool stop)
{
    auto i = m_playing.find(audio);
    if (i != m_playing.end())
    {
        if (stop)
            audio->doStop(false);
        m_sources.push(i->second);
        m_playing.erase(audio);
        
        // 通知有音频移除
        notifyAudioRemoved();
        
        return true;
    }

    return false;
}

bool ALSourcePool::isInPool(std::shared_ptr<Audio> audio)
{
    return m_playing.find(audio) != m_playing.end();
}

void ALSourcePool::update()
{
    ScopedLock lock(*this);
    std::vector<std::shared_ptr<Audio>> recycle;

    for (const auto &i : m_playing)
    {
        if (!i.first->update())
            recycle.push_back(i.first);
    }

    for (auto s : recycle)
        recycleSource(s, true);
}

void ALSourcePool::clear()
{
    ScopedLock lock(*this);
    for (const auto &i : m_playing)
    {
        i.first->doStop(true);
    }
    m_playing.clear();
    m_activeAudioCount.store(0);
    m_audioCondition.notify_all();
}

bool ALSourcePool::hasActiveAudio() const
{
    return m_activeAudioCount.load() > 0;
}

bool ALSourcePool::hasActiveAudioThreadSafe() const
{
    // 使用原子操作，无需锁
    return m_activeAudioCount.load() > 0;
}

// 信号量机制实现
void ALSourcePool::waitForAudio()
{
    std::unique_lock<std::mutex> lock(m_waitMutex);  // 使用独立的等待锁
    
    // 等待条件：有活跃音频或关闭信号
    m_audioCondition.wait(lock, [this]() {
        return m_activeAudioCount.load() > 0 || m_shutdown.load();
    });
}

void ALSourcePool::notifyAudioAdded()
{
    int oldCount = m_activeAudioCount.fetch_add(1);
    if (oldCount == 0)
    {
        // 从0变为1，通知等待的线程
        // 不需要锁，因为condition_variable的notify是线程安全的
        m_audioCondition.notify_one();
        //LOGD("Audio added, waking up audio thread. Active count: %d", m_activeAudioCount.load());
    }
}

void ALSourcePool::notifyAudioRemoved()
{
    int newCount = m_activeAudioCount.fetch_sub(1) - 1;
    if (newCount == 0)
    {
        LOGI("All audio stopped, audio thread will wait. Active count: %d", newCount);
    }
}
} // namespace audio

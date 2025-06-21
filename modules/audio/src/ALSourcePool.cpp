#include "ALSourcePool.h"
#include <cstring>
#include <utils/Log.h>

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
}
ALSourcePool::~ALSourcePool()
{
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
    return true;
}
bool ALSourcePool::recycleSource(std::shared_ptr<Audio> audio, bool stop)
{
    auto i = m_playing.find(audio);
    if (i != m_playing.end())
    {
        if (stop)
            audio->doStop();
        m_sources.push(i->second);
        m_playing.erase(audio);
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
        i.first->doStop();
    }
    m_playing.clear();
}
} // namespace audio

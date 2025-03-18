#ifndef __BLOCKINGQUEUE_H__
#define __BLOCKINGQUEUE_H__

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <utils/Marcos.h>

namespace laya
{
template <typename T> class BlockingQueue
{
    DISALLOW_COPY_AND_ASSIGN(BlockingQueue)
  public:
    BlockingQueue() = default;
    ~BlockingQueue() = default;
    void put(const T &value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(value);
        m_condition.notify_one();
    }
    void put(T &&value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::move(value));
        m_condition.notify_one();
    }
    T take()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this]() { return !m_queue.empty(); });
        DEBUG_CHECK(!m_queue.empty());
        T front(std::move(m_queue.front()));
        m_queue.pop_front();
        return front;
    }
    size_t size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

  private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::queue<T> m_queue;
};
} // namespace laya
#endif
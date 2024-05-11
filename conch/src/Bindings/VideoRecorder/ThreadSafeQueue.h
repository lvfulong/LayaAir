#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <cassert>

template<typename T>
class ThreadSafeQueue {

public:
    ThreadSafeQueue() {}

    /*ThreadSafeQueue(ThreadSafeQueue const &other) {
        std::lock_guard<std::mutex> lk(other.m_mutex);
        m_dataQueue = other.m_dataQueue;
    }*/

	ThreadSafeQueue(const ThreadSafeQueue&) = delete;
	void operator=(const ThreadSafeQueue&) = delete;

    void Push(T new_value)
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_dataQueue.push(new_value);
        m_condVar.notify_one();
    }

    T Pop(){
        std::unique_lock<std::mutex> lk(m_mutex);
		m_condVar.wait(lk, [this] { return !m_dataQueue.empty(); });
		assert(!m_dataQueue.empty());
       // if(Empty()) return nullptr;
        T res = m_dataQueue.front();
        m_dataQueue.pop();
        return res;
    }

    bool Empty() const {
		std::unique_lock<std::mutex> lk(m_mutex);
        return m_dataQueue.empty();
    }

    int Size() {
        std::unique_lock<std::mutex> lk(m_mutex);
        return m_dataQueue.size();
    }

private:
    mutable std::mutex m_mutex;
    std::queue<T> m_dataQueue;
    std::condition_variable m_condVar;
};
#endif //THREADSAFEQUEUE_H

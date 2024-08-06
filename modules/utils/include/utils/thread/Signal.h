#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <condition_variable>
#include <mutex>
#include <utils/Marcos.h>

namespace laya
{
class ManualResetSignal final
{
  public:
    ManualResetSignal()
    {
    }
    ~ManualResetSignal()
    {
    }

    void signal()
    {
        std::scoped_lock locker(m_mutex);
        m_signaled = true;
        m_signal_id++;
        m_condition.notify_all();
    }

    void wait()
    {
        std::unique_lock<std::mutex> locker(m_mutex);

        if (m_signaled)
        {
            return;
        }

        auto last_signal_id = m_signaled;
        do
        {
            m_condition.wait(locker);
        } while (m_signaled == last_signal_id);
    }

    void reset()
    {
        std::scoped_lock locker(m_mutex);
        m_signaled = false;
    }

  private:
    bool m_signaled = false;
    std::condition_variable m_condition;
    std::mutex m_mutex;
    unsigned m_signal_id = 0u;
    DISALLOW_COPY_AND_ASSIGN(ManualResetSignal);
};

class AutoResetSignal final
{
  public:
    AutoResetSignal()
    {
    }
    ~AutoResetSignal()
    {
    }

    void signal()
    {
        std::scoped_lock locker(m_mutex);
        m_signaled = true;
        m_condition.notify_one();
    }

    void wait()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        while (!m_signaled)
        {
            m_condition.wait(locker);
        }
        m_signaled = false;
    }

    void reset()
    {
        std::scoped_lock locker(m_mutex);
        m_signaled = false;
    }

  private:
    bool m_signaled = false;
    std::condition_variable m_condition;
    std::mutex m_mutex;
    DISALLOW_COPY_AND_ASSIGN(AutoResetSignal);
};
} // namespace laya

#endif
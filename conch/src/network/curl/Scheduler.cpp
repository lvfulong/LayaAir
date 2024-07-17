#include "Scheduler.h"
#include <utils/Log.h>

namespace laya
{
Scheduler::Scheduler()
{
}

Scheduler::~Scheduler()
{
}
void Scheduler::executeTasks()
{
    // ASSERT(!isMainThread());

    std::vector<std::function<void()>> taskQueue;

    {
        std::unique_lock<std::mutex> locker(m_mutex);
        taskQueue = std::move(m_taskQueue);
    }

    for (auto &task : taskQueue)
        task();
}
void Scheduler::stopThreadIfNoMoreJobRunning()
{
    // ASSERT(!isMainThread());

    std::unique_lock<std::mutex> locker(m_mutex);
    if (/*m_activeJobs.size() || */ m_taskQueue.size())
        return;

    m_runThread = false;
}
void Scheduler::stopThread()
{
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_runThread = false;
    }

    /*if (m_thread) {
        wakeUpThreadIfPossible();
        m_thread->waitForCompletion();
        m_thread = nullptr;
    }*/
}
} // namespace laya
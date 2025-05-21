#ifndef __MESSAGELOOPGENERIC_H__
#define __MESSAGELOOPGENERIC_H__

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <utils/Marcos.h>
#include <utils/MessageLoopImpl.h>
#include <utils/Time.h>
#include <atomic>

namespace laya
{

class MessageLoopGeneric : public MessageLoopImpl
{
  private:
    struct Task
    {
        DISALLOW_COPY_AND_ASSIGN(Task)
        Task() = delete;
        ~Task() = default;
        Task(Task &&other) = default;
        Task &operator=(Task &&other) = default;
        Task(std::function<void()> &&task, nano_seconds_t time) : task(std::move(task)), time(time)
        {
        }
        std::function<void()> task;
        nano_seconds_t time;
    };

  public:
    void iterate() override
    {
        if (m_running)
        {
            //waitForWork();
            processExpiredTasks();
        }
    }
    void processExpiredTasks() 
    {
        auto now = MonotonicClock::now();
        std::vector<Task> toProcess;
        {
            std::unique_lock lock{m_mutex};
            if (m_tasks.empty())
                return;
            toProcess = std::move(m_tasks);
            auto moveBack =
                find_if(std::begin(toProcess), std::end(toProcess), [&now](Task &item) { return item.time > now; });
            if (moveBack != std::end(toProcess))
            {
                m_tasks.reserve(std::distance(moveBack, std::end(toProcess)) + 5);
                std::move(moveBack, std::end(toProcess), std::back_inserter(m_tasks));
                toProcess.erase(moveBack, std::end(toProcess));
            }
        }
        for (auto &item : toProcess)
        {
            item.task();
        }
    }
    void enqueue(nano_seconds_t time, std::function<void()> &&task) override
    {
        if (!m_running)
            return;
        enqueueTask(Task{std::move(task), time});
    }
    void stop() override
    {
        // 福龙看这里，现在有问题，刷新要=true，否则流程就不跑了。刷新应该把当前的任务全部清理
        m_running = false;
    }   
  private:
    nano_seconds_t nextWakeup()
    {
        if (m_tasks.empty())
        {
            return std::numeric_limits<nano_seconds_t>::max();
        }
        else
        {
            return std::begin(m_tasks)->time;
        }
    }

    void enqueueTask(Task &&item)
    {
        bool needsWakeup;
        {
            std::unique_lock lock{m_mutex};
            auto insertAt = std::find_if(std::begin(m_tasks), std::end(m_tasks),
                                         [time = item.time](Task &item) { return item.time > time; });
            needsWakeup = std::begin(m_tasks) == insertAt;
            m_tasks.emplace(insertAt, std::move(item));
        }
        if (needsWakeup)
        {
            wakeUp();
        }
    }
    void wakeUp()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.notify_one();
    }
    void waitForWork()
    {
        std::unique_lock lock{m_mutex};
        nano_seconds_t nextWakeup = this->nextWakeup();
        std::chrono::nanoseconds duration = std::chrono::nanoseconds::max();
        if (nextWakeup < std::numeric_limits<nano_seconds_t>::max())
        {
            nano_seconds_t timeout = nextWakeup - MonotonicClock::now();
            if (timeout < 0)
                timeout = 0;
            duration = std::chrono::nanoseconds(timeout);
        }
        m_condition.wait_for(lock, duration);
    }

  private:
    std::vector<Task> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::atomic_bool m_running = true;
};
} // namespace laya
#endif
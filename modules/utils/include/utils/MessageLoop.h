#ifndef __MESSAGELOOP_H__
#define __MESSAGELOOP_H__

#include <future>
#include <memory>
#include <functional>
#include <utils/Marcos.h>
#include <utils/Time.h>
#include <utils/MessageLoopImpl.h>

namespace laya
{
class MessageLoop
{
    DISALLOW_COPY_AND_ASSIGN(MessageLoop)
  public:
    static MessageLoop &getCurrent();
    static void initializeMessageLoopForCurrentThread();

    MessageLoop();
    ~MessageLoop();
    template <class F> void postAt(nano_seconds_t time, F &&func)
    {
        m_impl->enqueue(time, std::function<void()>(std::forward<F>(func)));
    }

    template <class F> void postDelayed(nano_seconds_t delay, F &&func)
    {
        m_impl->enqueue(MonotonicClock::now() + delay, std::function<void()>(std::forward<F>(func)));
    }

    template <class F> void post(F &&func)
    {
        postAt(0, std::forward<F>(func));
    }

    template <class F> auto async(F &&func) -> std::future<decltype(func())>
    {
        using task_t = std::packaged_task<decltype(func())()>;
        auto task = std::make_shared<task_t>(std::forward<F>(func));
        post([task]() { std::invoke(*task); });
        return task->get_future();
    }

    template <class F> auto runSync(F &&func) -> decltype(func())
    {
        std::packaged_task<decltype(func())()> task{std::forward<F>(func)};
        post([&task]() { std::invoke(task); });
        return task.get_future().get();
    }
    void iterate()
    {
        m_impl->iterate();
    }
    void start()
    {
        m_impl->start();
    }
    void stop()
    {
        m_impl->stop();
    }
  private:
    std::unique_ptr<MessageLoopImpl> m_impl;
};
} // namespace laya
#endif
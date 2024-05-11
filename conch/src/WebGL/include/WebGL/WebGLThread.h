#ifndef __WEBGL_THREAD_H__
#define __WEBGL_THREAD_H__

#include <memory>
#include <mutex>
#include <thread>
#include <queue>
#include <future>
#include <functional>

namespace laya {
class WebGLThread {
public:
    WebGLThread();
    ~WebGLThread();
    void requestExit();
    template <typename F, typename ... Args>
    auto postTaskSync(F&& f, Args&& ... args)->std::future<std::result_of_t<F(Args...)>> {
        using returnType = std::result_of_t<F(Args ...)>;
        auto task = std::make_shared<std::packaged_task<returnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<returnType> ret = task->get_future();
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_taskQueue.emplace([task]() { (*task)(); });
        }
        m_condition.notify_one();
        return std::move(std::future<std::result_of_t<F(Args...)>>(std::move(ret)));
    }
    void postTaskAsync(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_taskQueue.emplace(task);
        }
        m_condition.notify_one();
    }
    
    WebGLThread(const WebGLThread&) = delete;
    WebGLThread& operator=(const WebGLThread&) = delete;
private:
    std::thread m_thread;
    bool m_requestExit = false;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::queue<std::function<void()>> m_taskQueue;
};
}

#endif


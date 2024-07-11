#ifndef __CURL_SCHEDULER_H__
#define __CURL_SCHEDULER_H__

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace laya
{
class Scheduler
{
  public:
    Scheduler();

    ~Scheduler();

  private:
    void executeTasks();
    void stopThread();
    void stopThreadIfNoMoreJobRunning();

  private:
    std::vector<std::function<void()>> m_taskQueue;
    std::mutex m_mutex;
    bool m_runThread{false};
};
} // namespace laya
#endif

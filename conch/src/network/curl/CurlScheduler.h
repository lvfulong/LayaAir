#ifndef __CURL_SCHEDULER_H__
#define __CURL_SCHEDULER_H__

#include "CurlMultiHandle.h"
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <unordered_map>
#include <utils/thread/Signal.h>
#include <vector>

namespace laya
{
class CurlSchedulerClient;
class CurlScheduler
{
  public:
    CurlScheduler(long maxConnects, long maxTotalConnections, long maxHostConnections);
    ~CurlScheduler()
    {
        stopThread();
    }

    bool add(CurlSchedulerClient *client);

    void cancel(CurlSchedulerClient *client);

  private:
    void callOnWorkerThread(std::function<void()> &&task);
    void executeTasks();
    void stopThread();
    void workerThread();
    void startOrWakeUpThread();
    void completeTransfer(CurlSchedulerClient *client, CURLcode result);
    void wakeUpThreadIfPossible();
    void cancelTransfer(CurlSchedulerClient *client);
    void startTransfer(CurlSchedulerClient *client);
    void finalizeTransfer(CurlSchedulerClient *client, std::function<void()> completionHandler);

  private:
    long m_maxConnects;
    long m_maxTotalConnections;
    long m_maxHostConnections;
    std::vector<std::function<void()>> m_taskQueue;
    std::set<CurlSchedulerClient *> m_activeJobs;
    std::unordered_map<CURL *, CurlSchedulerClient *> m_clientMaps;
    std::mutex m_mutex;
    bool m_runThread{false};
    std::optional<CurlMultiHandle> m_curlMultiHandle;
    std::mutex m_multiHandleMutex;
    std::thread m_thread;
    AutoResetSignal m_signal;
};
} // namespace laya
#endif
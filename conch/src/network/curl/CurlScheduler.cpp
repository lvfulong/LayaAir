#include "CurlScheduler.h"
#include "CurlSchedulerClient.h"
#include <utils/Log.h>

namespace laya
{
CurlScheduler::CurlScheduler(long maxConnects, long maxTotalConnections, long maxHostConnections)
    : m_maxConnects(maxConnects), m_maxTotalConnections(maxTotalConnections), m_maxHostConnections(maxHostConnections)
{
    startOrWakeUpThread();
}

bool CurlScheduler::add(CurlSchedulerClient *client)
{
    // ASSERT(isMainThread());

    if (!client)
        return false;

    startTransfer(client);
    startOrWakeUpThread();

    return true;
}

void CurlScheduler::cancel(CurlSchedulerClient *client)
{
    // ASSERT(isMainThread());

    if (!client)
        return;

    cancelTransfer(client);
}

void CurlScheduler::callOnWorkerThread(std::function<void()> &&task)
{
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_taskQueue.emplace_back(std::move(task));
    }

    startOrWakeUpThread();
}

void CurlScheduler::executeTasks()
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
void CurlScheduler::startOrWakeUpThread()
{
    // ASSERT(isMainThread());

    {
        std::unique_lock<std::mutex> locker(m_mutex);
        if (m_runThread)
        {
            wakeUpThreadIfPossible();
            return;
        }
    }

    // if (m_thread)
    //     m_thread->waitForCompletion();

    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_runThread = true;
    }

    // todo thread name
    m_thread = std::thread([this]() { workerThread(); });
}
void CurlScheduler::wakeUpThreadIfPossible()
{
    std::unique_lock<std::mutex> locker{m_multiHandleMutex};
    if (!m_curlMultiHandle)
        return;

    m_curlMultiHandle->wakeUp();
}
void CurlScheduler::stopThread()
{
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_runThread = false;
    }

    // if (m_thread) {
    wakeUpThreadIfPossible();
    if (m_thread.joinable())
    {
        m_thread.join();
    }
    //    m_thread = nullptr;
    //}
}
void CurlScheduler::completeTransfer(CurlSchedulerClient *client, CURLcode result)
{
    finalizeTransfer(client, [client, result]() { client->didCompleteTransfer(result); });
}

void CurlScheduler::cancelTransfer(CurlSchedulerClient *client)
{
    finalizeTransfer(client, [client]() { client->didCancelTransfer(); });
}
void CurlScheduler::startTransfer(CurlSchedulerClient *client)
{
    // client->retain();

    auto task = [this, client]() {
        CURL *handle = client->setupTransfer();
        if (!handle)
        {
            completeTransfer(client, CURLE_FAILED_INIT);
            return;
        }

        m_curlMultiHandle->addHandle(handle);

        // ASSERT(!m_clientMaps.contains(handle));
        m_clientMaps.insert({handle, client});
    };

    std::unique_lock<std::mutex> locker(m_mutex);
    m_activeJobs.insert(client);
    m_taskQueue.emplace_back(std::move(task));
}
void CurlScheduler::finalizeTransfer(CurlSchedulerClient *client, std::function<void()> completionHandler)
{
    std::unique_lock<std::mutex> locker(m_mutex);

    if (m_activeJobs.count(client) == 0)
        return;

    m_activeJobs.erase(client);

    auto task = [this, client, completionHandler = std::move(completionHandler)]() {
        if (client->handle())
        {
            // ASSERT(m_clientMaps.contains(client->handle()));
            m_clientMaps.erase(client->handle());
            m_curlMultiHandle->removeHandle(client->handle());
        }

        completionHandler();

        // todo callOnMainThread([client]() { client->release(); });
    };

    m_taskQueue.emplace_back(std::move(task));
}
void CurlScheduler::workerThread()
{
    // ASSERT(!isMainThread());

    {
        std::unique_lock<std::mutex> locker{m_multiHandleMutex};
        m_curlMultiHandle.emplace();
        m_curlMultiHandle->setMaxConnects(m_maxConnects);
        m_curlMultiHandle->setMaxTotalConnections(m_maxTotalConnections);
        m_curlMultiHandle->setMaxHostConnections(m_maxHostConnections);
    }

    while (true)
    {
        {
            std::unique_lock<std::mutex> locker(m_mutex);
            if (!m_runThread)
                break;
        }

        executeTasks();

        const int selectTimeoutMS = INT_MAX;
        m_curlMultiHandle->poll({}, selectTimeoutMS);

        int activeCount = 0;
        while (m_curlMultiHandle->perform(activeCount) == CURLM_CALL_MULTI_PERFORM)
        {
        }
        while (true)
        {
            int messagesInQueue = 0;
            CURLMsg *msg = m_curlMultiHandle->readInfo(messagesInQueue);
            if (!msg)
                break;

            // ASSERT(msg->msg == CURLMSG_DONE);
            auto it = m_clientMaps.find(msg->easy_handle);
            if (it != m_clientMaps.end())
            {
                completeTransfer(it->second, msg->data.result);
            }
        }
    }
    {
        std::unique_lock<std::mutex> locker{m_multiHandleMutex};
        m_curlMultiHandle.reset();
    }
}
} // namespace laya
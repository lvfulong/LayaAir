#include <utils/Log.h>
#include <network/IHttpClient.h>
#if OS_ANDROID
#include "network/HttpClientAndroid.h"
#elif OS_IOS
#include "network/HttpClientiOS.h"
#elif OS_OHOS
#include "network/HttpClientOHOS.h"
#elif OS_WINDOWS
#endif

namespace laya
{
IHttpClient::~IHttpClient()
{
    if (auto p = m_httpClientManager.lock())
    {
        p->removeHttpClient(this);
    }
}
HttpClientManager::HttpClientManager()
{
}
HttpClientManager::~HttpClientManager()
{
    cancelAllHttpClients();
}
IHttpClient *HttpClientManager::createHttpClient(const char *url, const char *localFilePath,
                                                 const IHttpClient::onProgressFunction &functionOnProgress,
                                                 const IHttpClient::onEndFunction &functionOnEnd)
{
    IHttpClient *httpClient = nullptr;
#if OS_ANDROID
    httpClient = new HttpClientAndroid(url, localFilePath, functionOnProgress, functionOnEnd, shared_from_this());
#elif OS_IOS
    httpClient = new HttpClientiOS(url, localFilePath, functionOnProgress, functionOnEnd, shared_from_this());
#elif OS_WINDOWS
    httpClient = nullptr; // TODO
#elif OS_OHOS
    httpClient = new HttpClientOHOS(url, localFilePath, functionOnProgress, functionOnEnd, shared_from_this());
#endif
    addHttpClient(httpClient);
    return httpClient;
}

void HttpClientManager::cancelAllHttpClients()
{
    std::set<IHttpClient *> httpClientsCopy;
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        httpClientsCopy = m_httpClients; // move?
        m_httpClients.clear();
    }
    for (auto it = httpClientsCopy.begin(); it != httpClientsCopy.end(); it++)
    {
        (*it)->cancel();
        // delete *it;
    }
}
} // namespace laya

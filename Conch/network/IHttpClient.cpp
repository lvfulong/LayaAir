#include <Utils/Log.h>
#include <network/IHttpClient.h>
#if ANDROID
#include "network/HttpClientAndroid.h"
#elif __APPLE__
#include "network/HttpClientiOS.h"
#elif _WIN32
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
#if ANDROID
    httpClient = new HttpClientAndroid(url, localFilePath, functionOnProgress, functionOnEnd, shared_from_this());
#elif __APPLE__
    httpClient = new HttpClientiOS(url, localFilePath, functionOnProgress, functionOnEnd, shared_from_this());
#elif _WIN32
    httpClient = nullptr; // TODO
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

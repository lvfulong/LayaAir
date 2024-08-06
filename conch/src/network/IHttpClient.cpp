#include <network/IHttpClient.h>
#include <utils/Log.h>
#if defined(OS_ANDROID)
#include "network/HttpClientAndroid.h"
#elif defined(OS_IOS)
#include "network/HttpClientiOS.h"
#elif defined(OS_OHOS)
#include "network/HttpClientOHOS.h"
#elif defined(OS_WINDOWS)
#include "network/HttpClientCurl.h"
#include "network/curl/CurlContext.h"
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
#if defined(OS_WINDOWS)
    CurlContext::GetInstance();
#endif
}
HttpClientManager::~HttpClientManager()
{
    cancelAllHttpClients();
}
IHttpClient *HttpClientManager::createHttpClient(const std::string &url, const std::string &localFilePath,
                                                 const IHttpClient::onProgressFunction &functionOnProgress,
                                                 const IHttpClient::onEndFunction &functionOnEnd)
{
    IHttpClient *httpClient = nullptr;
#if defined(OS_ANDROID)
    httpClient = new HttpClientAndroid(url, localFilePath, functionOnProgress, functionOnEnd, shared_from_this());
#elif defined(OS_IOS)
    httpClient = new HttpClientiOS(url, localFilePath, functionOnProgress, functionOnEnd, shared_from_this());
#elif defined(OS_WINDOWS)
    httpClient = new HttpClientCurl(url, localFilePath, functionOnProgress, functionOnEnd, shared_from_this());
#elif defined(OS_OHOS)
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

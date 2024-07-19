#include "HttpClientCurl.h"
#include <utils/JCCommonMethod.h>
#include <utils/Log.h>

namespace laya
{
HttpClientCurl::HttpClientCurl(const char *url, const char *localFilePath, const onProgressFunction &functionOnProgress,
                               const onEndFunction &functionOnEnd, std::weak_ptr<HttpClientManager> httpClientManager)
    : IHttpClient(httpClientManager)
{
    m_url = encodeURI(url);
    m_localFilePath = localFilePath;
    m_functionOnEnd = functionOnEnd;
    m_functionOnProgress = functionOnProgress;
}

HttpClientCurl::~HttpClientCurl()
{
}

void HttpClientCurl::doRequest()
{
}

void HttpClientCurl::setMethod(const char *method)
{
}

void HttpClientCurl::addHeader(const char *key, const char *value)
{
}
void HttpClientCurl::postData(const char *pData, int nLen)
{
}

void HttpClientCurl::setReadTimeout(int miliseconds)
{
}

void HttpClientCurl::setConnectTimeout(int miliseconds)
{
}
void HttpClientCurl::cancel()
{
}
CURL *HttpClientCurl::handle()
{
    return nullptr;
}
CURL *HttpClientCurl::setupTransfer()
{
    return nullptr;
}
void HttpClientCurl::didCompleteTransfer(CURLcode)
{
}
void HttpClientCurl::didCancelTransfer()
{
}
} // namespace laya
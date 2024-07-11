#include "HttpClientOHOS.h"
#include <utils/JCCommonMethod.h>
#include <utils/Log.h>

namespace laya
{

HttpClientOHOS::HttpClientOHOS(const char *url, const char *localFilePath, const onProgressFunction &functionOnProgress,
                               const onEndFunction &functionOnEnd, std::weak_ptr<HttpClientManager> httpClientManager)
    : IHttpClient(httpClientManager)
{
    m_url = encodeURI(url);
    m_localFilePath = localFilePath;
    m_functionOnEnd = functionOnEnd;
    m_functionOnProgress = functionOnProgress;
}

HttpClientOHOS::~HttpClientOHOS()
{
}

void HttpClientOHOS::doRequest()
{
}

void HttpClientOHOS::setMethod(const char *method)
{
    // static const char* s_methodSign =
    // "(Llayaair/game/browser/LayaHttpClient;Ljava/lang/String;)V";
}

void HttpClientOHOS::addHeader(const char *key, const char *value)
{
}
void HttpClientOHOS::postData(const char *pData, int nLen)
{
}

void HttpClientOHOS::setReadTimeout(int miliseconds)
{
}

void HttpClientOHOS::setConnectTimeout(int miliseconds)
{
}
void HttpClientOHOS::cancel()
{
}
} // namespace laya
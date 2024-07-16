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

    if (auto create = aki::JSBind::GetJSFunction("LayaHttpClient.create"))
    {
        m_object = create->Invoke<aki::Value>(*reinterpret_cast<double *>(this), m_url, m_localFilePath);
    }
}

HttpClientOHOS::~HttpClientOHOS()
{
}

void HttpClientOHOS::doRequest()
{
    LOGI("ccc doRequest (0)");
    if (auto doRequest = aki::JSBind::GetJSFunction("LayaHttpClient.doRequest"))
    {
        doRequest->Invoke<void>(m_object);
        LOGI("ccc doRequest (1)");
    }
}

void HttpClientOHOS::setMethod(const char *method)
{
}

void HttpClientOHOS::addHeader(const char *key, const char *value)
{
    LOGI("ccc addHeader (0)");
    if (auto addHeader = aki::JSBind::GetJSFunction("LayaHttpClient.addHeader"))
    {
        addHeader->Invoke<void>(m_object, key, value);
        LOGI("ccc addHeader (1)");
    }
}
void HttpClientOHOS::postData(const char *pData, int nLen)
{
    if (auto postData = aki::JSBind::GetJSFunction("LayaHttpClient.postData"))
    {
        postData->Invoke<void>(m_object);
        LOGI("ccc postData (1)");
    }
}

void HttpClientOHOS::setReadTimeout(int miliseconds)
{
    LOGI("ccc setReadTimeout (0)");
    if (auto setReadTimeout = aki::JSBind::GetJSFunction("LayaHttpClient.setReadTimeout"))
    {

        setReadTimeout->Invoke<void>(m_object, (double)miliseconds);
    }
    LOGI("ccc setReadTimeout (1)");
}

void HttpClientOHOS::setConnectTimeout(int miliseconds)
{
    LOGI("ccc setConnectTimeout (0)");
    if (auto setConnectTimeout = aki::JSBind::GetJSFunction("LayaHttpClient.setConnectTimeout"))
    {
        setConnectTimeout->Invoke<void>(m_object, (double)miliseconds);
    }
    LOGI("ccc setConnectTimeout (3)");
}
void HttpClientOHOS::cancel()
{
    if (auto cancel = aki::JSBind::GetJSFunction("LayaHttpClient.cancel"))
    {
        cancel->Invoke<void>(m_object);
    }
}

} // namespace laya

napi_value HttpClientOHOS_onFailure(napi_env env, napi_callback_info info)
{
    LOGI("ccc HttpClientOHOS_onFailure (1)");
    return nullptr;
}

napi_value HttpClientOHOS_onResponse(napi_env env, napi_callback_info info)
{
    LOGI("ccc HttpClientOHOS_onResponse (1)");
    return nullptr;
}

napi_value HttpClientOHOS_onProgress(napi_env env, napi_callback_info info)
{
    LOGI("ccc HttpClientOHOS_onProgress (1)");
    return nullptr;
}

#include "HttpClientCurl.h"
#include "curl/CurlContext.h"
#include <utils/JCCommonMethod.h>
#include <utils/Log.h>

namespace laya
{
HttpClientCurl::HttpClientCurl(const std::string& url, const std::string&  localFilePath, const onProgressFunction &functionOnProgress,
                               const onEndFunction &functionOnEnd, std::weak_ptr<HttpClientManager> httpClientManager)
    : IHttpClient(httpClientManager)
{
    m_url = encodeURI(url.c_str());
    m_localFilePath = localFilePath;
    m_functionOnEnd = functionOnEnd;
    m_functionOnProgress = functionOnProgress;

    m_recieveData.reserve(1024);
}

HttpClientCurl::~HttpClientCurl()
{
}

void HttpClientCurl::doRequest()
{
    // ASSERT(isMainThread());
    CurlContext::GetInstance().getScheduler().add(this);
}

void HttpClientCurl::setMethod(const std::string& method)
{
    m_method = method;
}

void HttpClientCurl::addHeader(const std::string&  key, const std::string& value)
{
    LOGI("HttpClientCurl addHeader");
}
void HttpClientCurl::postData(const char *pData, int nLen)
{
    m_data.resize(nLen);
    memcpy(&m_data[0], pData, nLen);
}

void HttpClientCurl::setReadTimeout(int miliseconds)
{
    m_readTimeout = miliseconds;
}

void HttpClientCurl::setConnectTimeout(int miliseconds)
{
    m_connectTimeout = miliseconds;
}
void HttpClientCurl::cancel()
{
    LOGI("HttpClientCurl cancel");
}
CURL *HttpClientCurl::handle()
{
    LOGI("HttpClientCurl handle");
    return nullptr;
}
size_t HttpClientCurl::willSendData(char *buffer, size_t blockSize, size_t numberOfBlocks)
{
    LOGI("HttpClientCurl willSendData");
    // todo
    return 0;
}
size_t HttpClientCurl::willSendDataCallback(char *ptr, size_t blockSize, size_t numberOfBlocks, void *userData)
{
    return static_cast<HttpClientCurl *>(userData)->willSendData(ptr, blockSize, numberOfBlocks);
}
size_t HttpClientCurl::didReceiveHeader(std::string &&header)
{
    size_t size = header.size();
    m_responseHead.append(header.c_str(), size);
    return size;
}
size_t HttpClientCurl::didReceiveHeaderCallback(char *ptr, size_t blockSize, size_t numberOfBlocks, void *userData)
{
    return static_cast<HttpClientCurl *>(userData)->didReceiveHeader(std::string({ptr, blockSize * numberOfBlocks}));
}
size_t HttpClientCurl::didReceiveData(uint8_t *receivedData, size_t bytes)
{
    if (bytes > 0)
    {
        size_t capacity = m_recieveData.capacity();
        size_t size = m_recieveData.size();
        if (size + bytes < capacity)
        {
            m_recieveData.reserve(capacity << 1);
        }
        m_recieveData.resize(size + bytes);
        memcpy(&m_recieveData[size], receivedData, bytes);
    }
    return bytes;
}
size_t HttpClientCurl::didReceiveDataCallback(char *ptr, size_t blockSize, size_t numberOfBlocks, void *userData)
{
    return static_cast<HttpClientCurl *>(userData)->didReceiveData((uint8_t *)(ptr), blockSize * numberOfBlocks);
}
void HttpClientCurl::setupPOST()
{
    curl_off_t totalSize = m_data.size();

    // m_curlHandle->enableHttpPostRequest(totalSize)
    curl_easy_setopt(m_curlHandle->handle(), CURLOPT_POSTFIELDS, m_data.data());
    curl_easy_setopt(m_curlHandle->handle(), CURLOPT_POSTFIELDSIZE, m_data.size());
    curl_easy_setopt(m_curlHandle->handle(), CURLOPT_POST, 1L);

    // m_curlHandle->setReadCallbackFunction(willSendDataCallback, this);
}
void HttpClientCurl::setupPUT()
{
    // todo
    LOGI("HttpClientCurl setupPUT");
}
CURL *HttpClientCurl::setupTransfer()
{
    m_curlHandle = std::make_unique<CurlHandle>();

    m_curlHandle->setURL(m_url.c_str());

    // m_curlHandle->appendRequestHeaders(httpHeaderFields);

    if (m_method == "GET")
        m_curlHandle->enableHttpGetRequest();
    else if (m_method == "POST")
        setupPOST();
    else if (m_method == "PUT")
        setupPUT();
    else if (m_method == "HEAD")
        m_curlHandle->enableHttpHeadRequest();
    else
    {
        m_curlHandle->setHttpCustomRequest(m_method);
        setupPUT();
    }
    //curl_easy_setopt(m_curlHandle->handle(), CURLOPT_HTTPHEADER, nullptr);
    m_curlHandle->disableServerTrustEvaluation();
    m_curlHandle->setHeaderCallbackFunction(didReceiveHeaderCallback, this);
    m_curlHandle->setWriteCallbackFunction(didReceiveDataCallback, this);
    /*m_curlHandle->setDebugCallbackFunction(didReceiveDebugInfoCallback, this);

    m_curlHandle->setTimeout(timeoutInterval());*/

    // m_performStartTime = MonotonicTime::now();

    return m_curlHandle->handle();
}
void HttpClientCurl::didCompleteTransfer(CURLcode)
{
    LOGI("HttpClientCurl didCompleteTransfer");
}
void HttpClientCurl::didCancelTransfer()
{
    LOGI("HttpClientCurl didCancelTransfer");
}
} // namespace laya
#include "HttpClientCurl.h"
#include "network/curl/CurlContext.h"
#include <utils/JCCommonMethod.h>
#include <utils/Log.h>

namespace laya
{
HttpClientCurl::HttpClientCurl(const std::string &url, const std::string &localFilePath,
                               const onProgressFunction &functionOnProgress, const onEndFunction &functionOnEnd,
                               std::weak_ptr<HttpClientManager> httpClientManager)
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

void HttpClientCurl::setMethod(const std::string &method)
{
    m_method = method;
}

void HttpClientCurl::addHeader(const std::string &key, const std::string &value)
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
CURL *HttpClientCurl::getHandle()
{
    return m_curlHandle ? m_curlHandle->handle() : nullptr;
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
    static constexpr auto emptyLineCRLF = "\r\n";
    static constexpr auto emptyLineLF = "\n";

    auto receiveBytes = static_cast<size_t>(header.length());
    if ((header != emptyLineCRLF) && (header != emptyLineLF))
    {
        size_t size = header.size();
        m_responseHead.append(header.c_str(), size);
        return receiveBytes;
    }

    long statusCode = 0;
    if (auto code = m_curlHandle->getResponseCode())
        statusCode = *code;

    long httpConnectCode = 0;
    if (auto code = m_curlHandle->getHttpConnectCode())
        httpConnectCode = *code;

    m_statusCode = statusCode;
    m_httpConnectCode = httpConnectCode;
    return receiveBytes;
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
        size_t needSize = size + bytes;
        if (needSize < capacity)
        {
            m_recieveData.reserve(needSize << 1);
        }
        m_recieveData.resize(needSize);
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
    // curl_easy_setopt(m_curlHandle->handle(), CURLOPT_HTTPHEADER, nullptr);
    m_curlHandle->disableServerTrustEvaluation();
    m_curlHandle->setHeaderCallbackFunction(didReceiveHeaderCallback, this);
    m_curlHandle->setWriteCallbackFunction(didReceiveDataCallback, this);
    /*m_curlHandle->setDebugCallbackFunction(didReceiveDebugInfoCallback, this);

    m_curlHandle->setTimeout(timeoutInterval());*/

    // m_performStartTime = MonotonicTime::now();

    return m_curlHandle->handle();
}
void HttpClientCurl::didCompleteTransfer(CURLcode result)
{
    // todo CURLcode ?
    if (result == CURLE_OK)
    {
        LOGI("Download end:%d", m_statusCode); //??
        bool bBigFile = this->isBigFile();
        auto bufferBytes = m_recieveData.size();
        if (bBigFile)
        {
            // ���ļ�û��buffer
            std::shared_ptr<Data> data = Data::makeEmpty();
            this->m_functionOnEnd(data, "", "", 0 /*CURLE_OK*/, m_statusCode, m_responseHead);
        }
        else
        {
            if (bufferBytes <= 0)
            {
                std::shared_ptr<Data> data = Data::makeEmpty();
                this->m_functionOnEnd(data, "", "", 0 /*CURLE_OK*/, m_statusCode, m_responseHead);
            }
            else
            {
				// todo not copy
                std::shared_ptr<Data> data = Data::makeWithCopy((void *)m_recieveData.data(), bufferBytes);
                // request->m_responseCallback(buf, pCurl->m_strLocalAddr,
                // pCurl->m_strSvAddr, 0/*CURLE_OK*/, pCurl->m_nResponseCode,
                // pCurl->m_strResponseHead);
                this->m_functionOnEnd(data, "", "", 0 /*CURLE_OK*/, m_statusCode, m_responseHead);
            }
        }
    }
    else
    {
    }

    delete this;
}
void HttpClientCurl::didCancelTransfer()
{
    LOGI("HttpClientCurl didCancelTransfer");
}
} // namespace laya
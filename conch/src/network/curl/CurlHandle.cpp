#include "CurlHandle.h"
#include "CurlContext.h"
#include <utils/Log.h>

namespace laya
{
CurlHandle::CurlHandle()
{
    m_handle = curl_easy_init();
    curl_easy_setopt(m_handle, CURLOPT_ERRORBUFFER, m_errorBuffer);
    curl_easy_setopt(m_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(m_handle, CURLOPT_COOKIEFILE, nullptr);

    enableShareHandle();
    enableAcceptEncoding();
    enableAllowedProtocols();
    // enableAltSvc();

    // setDnsCacheTimeout(CurlContext::singleton().dnsCacheTimeout());
    // setConnectTimeout(CurlContext::singleton().connectTimeout());

    // enableProxyIfExists();
}

CurlHandle::~CurlHandle()
{
    if (m_handle)
    {
        curl_easy_cleanup(m_handle);
        m_handle = nullptr;
    }
}
void CurlHandle::setURL(const std::string &url)
{
    curl_easy_setopt(m_handle, CURLOPT_URL, url.c_str());
}
void CurlHandle::enableHttpGetRequest()
{
    enableHttp();
    curl_easy_setopt(m_handle, CURLOPT_HTTPGET, 1L);
}
void CurlHandle::enableHttp()
{
}
void CurlHandle::enableHttpPostRequest(curl_off_t size)
{
    enableHttp();
    curl_easy_setopt(m_handle, CURLOPT_POST, 1L);
    curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE_LARGE, size);
}
void CurlHandle::disableServerTrustEvaluation()
{
    curl_easy_setopt(m_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(m_handle, CURLOPT_SSL_VERIFYHOST, 0L);
}
void CurlHandle::enableHttpHeadRequest()
{
    enableHttp();
    curl_easy_setopt(m_handle, CURLOPT_NOBODY, 1L);
}
void CurlHandle::setHttpCustomRequest(const std::string &method)
{
    enableHttp();
    curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST, method.c_str());
}
void CurlHandle::setReadCallbackFunction(curl_read_callback callbackFunc, void *userData)
{
    curl_easy_setopt(m_handle, CURLOPT_READFUNCTION, callbackFunc);
    curl_easy_setopt(m_handle, CURLOPT_READDATA, userData);
}
void CurlHandle::setHeaderCallbackFunction(curl_write_callback callbackFunc, void *userData)
{
    curl_easy_setopt(m_handle, CURLOPT_HEADERFUNCTION, callbackFunc);
    curl_easy_setopt(m_handle, CURLOPT_HEADERDATA, userData);
}
void CurlHandle::setWriteCallbackFunction(curl_write_callback callbackFunc, void *userData)
{
    curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, callbackFunc);
    curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, userData);
}
void CurlHandle::enableShareHandle()
{
    curl_easy_setopt(m_handle, CURLOPT_SHARE, CurlContext::GetInstance().getShareHandle().handle());
}
void CurlHandle::enableAcceptEncoding()
{
    curl_easy_setopt(m_handle, CURLOPT_ACCEPT_ENCODING, "");
}
void CurlHandle::enableAllowedProtocols()
{
    // auto allowedProtocols = "file,ftp,ftps,http,https";
    auto allowedProtocols = "file,http,https";
    curl_easy_setopt(m_handle, CURLOPT_PROTOCOLS_STR, allowedProtocols);
}
} // namespace laya
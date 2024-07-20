#include "CurlHandle.h"
#include <utils/Log.h>

namespace laya
{
CurlHandle::CurlHandle()
{
}

CurlHandle::~CurlHandle()
{
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
} // namespace laya
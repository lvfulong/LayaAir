#ifndef __CURL_HANDLE_H__
#define __CURL_HANDLE_H__

// 这个宏指示是在使用 CURL 的静态库，必须要有
#define CURL_STATICLIB
#ifdef __cplusplus
extern "C"
{
#endif
#include <curl/curl.h>
#ifdef __cplusplus
}
#endif
#include <string>
#include <vector>
namespace laya
{
class CurlHandle
{
  public:
    CurlHandle();
    ~CurlHandle();
    CURL *handle() const
    {
        return m_handle;
    }
    void setURL(const std::string &url);
    void enableHttpGetRequest();
    void enableHttpPostRequest(curl_off_t size);
    void disableServerTrustEvaluation();
    void enableHttpHeadRequest();
    void setHttpCustomRequest(const std::string &method);
    void setReadCallbackFunction(curl_read_callback callbackFunc, void *userData);
    void setHeaderCallbackFunction(curl_write_callback callbackFunc, void *userData);
    void CurlHandle::setWriteCallbackFunction(curl_write_callback callbackFunc, void *userData);

  private:
    void enableAcceptEncoding();
    void enableShareHandle();
    void enableAllowedProtocols();
    void enableHttp();

  private:
    CURL *m_handle{nullptr};
    char m_errorBuffer[CURL_ERROR_SIZE]{};
};
} // namespace laya
#endif

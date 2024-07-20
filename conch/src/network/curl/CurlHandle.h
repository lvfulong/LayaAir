#ifndef __CURL_HANDLE_H__
#define __CURL_HANDLE_H__
#include <curl/curl.h>
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
    void enableHttp();

  private:
    CURL *m_handle{nullptr};
};
} // namespace laya
#endif

#ifndef __CURL_MULTI_HANDLE_H__
#define __CURL_MULTI_HANDLE_H__
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
#include <vector>
namespace laya
{
class CurlMultiHandle
{
  public:
    CurlMultiHandle();
    ~CurlMultiHandle();

    void setMaxConnects(long);
    void setMaxTotalConnections(long);
    void setMaxHostConnections(long);

    CURLMcode addHandle(CURL *);
    CURLMcode removeHandle(CURL *);

    CURLMcode poll(const std::vector<curl_waitfd> &, int);
    CURLMcode wakeUp();
    CURLMcode perform(int &);
    CURLMsg *readInfo(int &);

  private:
    CURLM *m_multiHandle{nullptr};
};
} // namespace laya
#endif

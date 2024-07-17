#ifndef __CURL_MULTI_HANDLE_H__
#define __CURL_MULTI_HANDLE_H__
#include <curl/curl.h>
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

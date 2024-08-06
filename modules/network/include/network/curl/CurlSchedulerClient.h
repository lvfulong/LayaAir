#ifndef __CURL_SCHEDULER_CLIENT_H__
#define __CURL_SCHEDULER_CLIENT_H__
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
namespace laya
{

class CurlSchedulerClient
{
  public:
    // virtual void retain() = 0;
    // virtual void release() = 0;

    virtual CURL *getHandle() = 0;
    virtual CURL *setupTransfer() = 0;
    virtual void didCompleteTransfer(CURLcode) = 0;
    virtual void didCancelTransfer() = 0;

  protected:
    ~CurlSchedulerClient()
    {
    }
};
} // namespace laya
#endif

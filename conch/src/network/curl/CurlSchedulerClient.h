#ifndef __CURL_SCHEDULER_CLIENT_H__
#define __CURL_SCHEDULER_CLIENT_H__
#include <curl/curl.h>
namespace laya
{

class CurlSchedulerClient
{
  public:
    // virtual void retain() = 0;
    // virtual void release() = 0;

    virtual CURL *handle() = 0;
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

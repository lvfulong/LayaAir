#ifndef __CURL_CONTEXT_H__
#define __CURL_CONTEXT_H__
#include "CurlScheduler.h"
#include "CurlShareHandle.h"

#include <curl/curl.h>

#ifdef OS_WINDOWS
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "wldap32.lib")
#endif
namespace laya
{
class CurlContext
{
  public:
    CurlContext();
    ~CurlContext();

  private:
    void initShareHandle();
    CurlShareHandle m_shareHandle;
    std::unique_ptr<CurlScheduler> m_scheduler;
};
} // namespace laya
#endif

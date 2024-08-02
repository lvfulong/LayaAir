#ifndef __CURL_CONTEXT_H__
#define __CURL_CONTEXT_H__
#include "CurlScheduler.h"
#include "CurlShareHandle.h"

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

#ifdef OS_WINDOWS
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "wldap32.lib")
#endif
namespace laya
{

const long CurlDefaultMaxConnects{-1};
const long CurlDefaultMaxTotalConnections{17};
const long CurlDefaultMaxHostConnections{6};
class CurlContext
{
  public:
    CurlContext();
    ~CurlContext();
    static CurlContext &GetInstance();
    CurlScheduler& getScheduler() { return *m_scheduler; }
  private:
    void initShareHandle();
    CurlShareHandle m_shareHandle;
    std::unique_ptr<CurlScheduler> m_scheduler;
};
} // namespace laya
#endif

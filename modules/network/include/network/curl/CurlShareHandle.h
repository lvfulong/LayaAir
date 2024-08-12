#ifndef __CURL_SHARE_HANDLE_H__
#define __CURL_SHARE_HANDLE_H__
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
#include <mutex>
#include <vector>
namespace laya
{
class CurlShareHandle
{
  public:
    CurlShareHandle();
    ~CurlShareHandle();

    CURLSH *handle() const
    {
        return m_shareHandle;
    }

  private:
    static void lockCallback(CURL *, curl_lock_data, curl_lock_access, void *);
    static void unlockCallback(CURL *, curl_lock_data, void *);
    static std::mutex *mutexFor(curl_lock_data);

    CURLSH *m_shareHandle{nullptr};
};
} // namespace laya
#endif

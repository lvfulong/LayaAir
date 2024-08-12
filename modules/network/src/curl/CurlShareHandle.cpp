#include <network/curl/CurlShareHandle.h>
#include <utils/Log.h>

namespace laya
{
CurlShareHandle::CurlShareHandle()
{
    m_shareHandle = curl_share_init();
    curl_share_setopt(m_shareHandle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    curl_share_setopt(m_shareHandle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    curl_share_setopt(m_shareHandle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    curl_share_setopt(m_shareHandle, CURLSHOPT_LOCKFUNC, lockCallback);
    curl_share_setopt(m_shareHandle, CURLSHOPT_UNLOCKFUNC, unlockCallback);
}

CurlShareHandle::~CurlShareHandle()
{
    if (m_shareHandle)
        curl_share_cleanup(m_shareHandle);
}

void CurlShareHandle::lockCallback(CURL *, curl_lock_data data, curl_lock_access, void *)
{
    if (auto *mutex = mutexFor(data))
        mutex->lock();
}

void CurlShareHandle::unlockCallback(CURL *, curl_lock_data data, void *)
{
    if (auto *mutex = mutexFor(data))
        mutex->unlock();
}

std::mutex *CurlShareHandle::mutexFor(curl_lock_data data)
{
    static std::mutex cookieMutex;
    static std::mutex dnsMutex;
    static std::mutex shareMutex;
    static std::mutex sslSessionMutex;

    switch (data)
    {
    case CURL_LOCK_DATA_COOKIE:
        return &cookieMutex;
    case CURL_LOCK_DATA_DNS:
        return &dnsMutex;
    case CURL_LOCK_DATA_SHARE:
        return &shareMutex;
    case CURL_LOCK_DATA_SSL_SESSION:
        return &sslSessionMutex;
    default:
        // ASSERT_NOT_REACHED();
        return nullptr;
    }
}

} // namespace laya
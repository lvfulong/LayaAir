#include "CurlMultiHandle.h"
#include <utils/Log.h>

namespace laya
{
CurlMultiHandle::CurlMultiHandle()
{
    m_multiHandle = curl_multi_init();
}

CurlMultiHandle::~CurlMultiHandle()
{
    if (m_multiHandle)
        curl_multi_cleanup(m_multiHandle);
}

void CurlMultiHandle::setMaxConnects(long maxConnects)
{
    if (maxConnects < 0)
        return;

    curl_multi_setopt(m_multiHandle, CURLMOPT_MAXCONNECTS, maxConnects);
}

void CurlMultiHandle::setMaxTotalConnections(long maxTotalConnections)
{
    curl_multi_setopt(m_multiHandle, CURLMOPT_MAX_TOTAL_CONNECTIONS, maxTotalConnections);
}

void CurlMultiHandle::setMaxHostConnections(long maxHostConnections)
{
    curl_multi_setopt(m_multiHandle, CURLMOPT_MAX_HOST_CONNECTIONS, maxHostConnections);
}

CURLMcode CurlMultiHandle::addHandle(CURL *handle)
{
    return curl_multi_add_handle(m_multiHandle, handle);
}

CURLMcode CurlMultiHandle::removeHandle(CURL *handle)
{
    return curl_multi_remove_handle(m_multiHandle, handle);
}

CURLMcode CurlMultiHandle::poll(const std::vector<curl_waitfd> &extraFds, int timeoutMS)
{
    int numFds = 0;
    return curl_multi_poll(m_multiHandle, const_cast<curl_waitfd *>(extraFds.data()), extraFds.size(), timeoutMS,
                           &numFds);
}

CURLMcode CurlMultiHandle::wakeUp()
{
    return curl_multi_wakeup(m_multiHandle);
}

CURLMcode CurlMultiHandle::perform(int &runningHandles)
{
    return curl_multi_perform(m_multiHandle, &runningHandles);
}

CURLMsg *CurlMultiHandle::readInfo(int &messagesInQueue)
{
    return curl_multi_info_read(m_multiHandle, &messagesInQueue);
}
} // namespace laya
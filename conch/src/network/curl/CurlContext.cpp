#include "CurlContext.h"
#include <utils/Log.h>

namespace laya
{
CurlContext::CurlContext()
{
    curl_global_init(CURL_GLOBAL_ALL);
    initShareHandle();
    long maxConnects{CurlDefaultMaxConnects};
    long maxTotalConnections{CurlDefaultMaxTotalConnections};
    long maxHostConnections{CurlDefaultMaxHostConnections};
    m_scheduler = std::make_unique<CurlScheduler>(maxConnects, maxTotalConnections, maxHostConnections);
}
CurlContext &CurlContext::GetInstance()
{
    static CurlContext sharedInstance;
    return sharedInstance;
}
CurlContext::~CurlContext()
{
    curl_global_cleanup();
}
void CurlContext::initShareHandle()
{
    CURL *curl = curl_easy_init();

    if (!curl)
        return;

    curl_easy_setopt(curl, CURLOPT_SHARE, m_shareHandle.handle());

    curl_easy_cleanup(curl);
}
} // namespace laya
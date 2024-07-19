#ifndef __HTTP_CLIENT_CURL_H__
#define __HTTP_CLIENT_CURL_H__

#include "curl/CurlSchedulerClient.h"
#include "network/IHttpClient.h"
#include <string>

namespace laya
{
class HttpClientCurl : public IHttpClient, public CurlSchedulerClient
{
  public:
    HttpClientCurl(const char *url, const char *localFilePath, const onProgressFunction &functionOnProgress,
                   const onEndFunction &functionOnEnd, std::weak_ptr<HttpClientManager> httpClientManager);

    ~HttpClientCurl();
    void doRequest() override;
    void addHeader(const char *key, const char *value) override;
    void postData(const char *pData, int nLen) override;
    void setMethod(const char *method) override;
    void setReadTimeout(int miliseconds) override;
    void setConnectTimeout(int miliseconds) override;
    void cancel() override;

  public:
    CURL *handle() override;
    CURL *setupTransfer() override;
    void didCompleteTransfer(CURLcode) override;
    void didCancelTransfer() override;

  private:
};
} // namespace laya
#endif

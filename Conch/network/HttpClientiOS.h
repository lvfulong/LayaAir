#ifndef __HTTP_CLIENT_IOS_H__
#define __HTTP_CLIENT_IOS_H__

#include <network/IHttpClient.h>
#include <string>

namespace laya
{
class HttpClientiOS : public IHttpClient
{
  public:
    HttpClientiOS(const char *url, const char *localFilePath, const onProgressFunction &functionOnProgress,
                  const onEndFunction &functionOnEnd, std::weak_ptr<HttpClientManager> httpClientManager);

    ~HttpClientiOS();
    void doRequest() override;
    void addHeader(const char *key, const char *value) override;
    void postData(const char *pData, int nLen) override;
    void setMethod(const char *method) override;
    void setReadTimeout(int miliseconds) override;
    void setConnectTimeout(int miliseconds) override;
    void cancel() override;
    int m_readTimeout = 10.0;
    int m_connectTimeout = 10.0;

  private:
    void onFailure(int code);

    class HttpURLSessionDownloaderImpl *m_impl;
};
} // namespace laya
#endif

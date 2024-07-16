#ifndef __HTTP_CLIENT_OHOS_H__
#define __HTTP_CLIENT_OHOS_H__

#include "network/IHttpClient.h"
#include <aki/jsbind.h>
#include <string>

namespace laya
{
class HttpClientOHOS : public IHttpClient
{
  public:
    HttpClientOHOS(const char *url, const char *localFilePath, const onProgressFunction &functionOnProgress,
                   const onEndFunction &functionOnEnd, std::weak_ptr<HttpClientManager> httpClientManager);

    ~HttpClientOHOS();
    void doRequest() override;
    void addHeader(const char *key, const char *value) override;
    void postData(const char *pData, int nLen) override;
    void setMethod(const char *method) override;
    void setReadTimeout(int miliseconds) override;
    void setConnectTimeout(int miliseconds) override;
    void cancel() override;

  private:
    aki::Value m_object;
};

} // namespace laya

napi_value HttpClientOHOS_onFailure(napi_env env, napi_callback_info info);
napi_value HttpClientOHOS_onResponse(napi_env env, napi_callback_info info);
napi_value HttpClientOHOS_onProgress(napi_env env, napi_callback_info info);

#endif

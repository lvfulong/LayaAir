#ifndef __HTTP_CLIENT_ADNROID_H__
#define __HTTP_CLIENT_ADNROID_H__

#include "CToJavaBridge.h"
#include "network/IHttpClient.h"
#include <string>

namespace laya
{
class HttpClientAndroid : public IHttpClient
{
  public:
    HttpClientAndroid(const char *url, const char *localFilePath, const onProgressFunction &functionOnProgress,
                      const onEndFunction &functionOnEnd, std::weak_ptr<HttpClientManager> httpClientManager);

    ~HttpClientAndroid();
    static void addStaticMethod(JNIEnv *env, const char *className);
    void doRequest() override;
    void addHeader(const char *key, const char *value) override;
    void postData(const char *pData, int nLen) override;
    void setMethod(const char *method) override;
    void setReadTimeout(int miliseconds) override;
    void setConnectTimeout(int miliseconds) override;
    void cancel() override;

  private:
    jobject m_downloader = nullptr;

    static jclass s_cls;
    static jmethodID s_createMethodID;
    static jmethodID s_addHeader;
    static jmethodID s_postData;
    static jmethodID s_setReadTimeout;
    static jmethodID s_setConnectTimeout;
    static jmethodID s_doRequest;
    static jmethodID s_cancel;
};
} // namespace laya
#endif

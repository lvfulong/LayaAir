#ifndef __HTTP_CLIENT_CURL_H__
#define __HTTP_CLIENT_CURL_H__

#include "curl/CurlHandle.h"
#include "curl/CurlSchedulerClient.h"
#include "network/IHttpClient.h"
#include <string>
#include <vector>
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

  private:
    CURL *handle() override;
    CURL *setupTransfer() override;
    void didCompleteTransfer(CURLcode) override;
    void didCancelTransfer() override;
    void setupPOST();
    void setupPUT();

    static size_t willSendDataCallback(char *ptr, size_t blockSize, size_t numberOfBlocks, void *userData);
    static size_t didReceiveHeaderCallback(char *ptr, size_t blockSize, size_t numberOfBlocks, void *userData);
    static size_t didReceiveDataCallback(char *ptr, size_t blockSize, size_t numberOfBlocks, void *userData);
    size_t willSendData(char *buffer, size_t blockSize, size_t numberOfBlocks);
    size_t didReceiveHeader(std::string &&header);

    size_t didReceiveData(uint8_t *receivedData, size_t bytes);

  private:
    std::unique_ptr<CurlHandle> m_curlHandle;
    std::string m_method;
    std::vector<uint8_t> m_data;
    int m_readTimeout = 0;    // todo
    int m_connectTimeout = 0; // todo
    std::string m_responseHead;
    std::vector<uint8_t> m_recieveData;
};
} // namespace laya
#endif

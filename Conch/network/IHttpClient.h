#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <utils/JCBuffer.h>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <string>

namespace laya
{

class HttpClientManager;
class IHttpClient
{
  public:
    IHttpClient(std::weak_ptr<HttpClientManager> httpClientManager) : m_httpClientManager(httpClientManager)
    {
    }
    virtual ~IHttpClient();
    virtual void doRequest() = 0;
    virtual void addHeader(const char *key, const char *value) = 0;
    virtual void postData(const char *pData, int nLen) = 0;
    virtual void setMethod(const char *method) = 0;
    virtual void setReadTimeout(int miliseconds) = 0;
    virtual void setConnectTimeout(int miliseconds) = 0;
    virtual void cancel() = 0;
    std::weak_ptr<HttpClientManager> m_httpClientManager;
    std::string m_url;
    std::string m_localFilePath;
    typedef std::function<int(unsigned int, unsigned int, float)> onProgressFunction;
    typedef std::function<void(JCBuffer &buff, const std::string &localip, const std::string &svip, int curlret,
                               int httpret, const std::string &httpresheader)>
        onEndFunction;
    onProgressFunction m_functionOnProgress;
    onEndFunction m_functionOnEnd;

    bool isBigFile()
    {
        return !m_localFilePath.empty();
    }
};
class HttpClientManager : public std::enable_shared_from_this<HttpClientManager>
{
  public:
    HttpClientManager();
    ~HttpClientManager();
    IHttpClient *createHttpClient(const char *url, const char *localFilePath,
                                  const IHttpClient::onProgressFunction &functionOnProgress,
                                  const IHttpClient::onEndFunction &functionOnEnd);
    void cancelAllHttpClients();
    void addHttpClient(IHttpClient *httpClinet)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_httpClients.insert(httpClinet);
    }
    void removeHttpClient(IHttpClient *httpClinet)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        auto it = m_httpClients.find(httpClinet);
        if (it != m_httpClients.end())
        {
            m_httpClients.erase(it);
        }
    }

  private:
    std::set<IHttpClient *> m_httpClients;
    std::mutex m_mutex;
};
} // namespace laya
#endif

#ifndef __I_UPLOAD_TASK_H__ 
#define __I_UPLOAD_TASK_H__

#include <string>
#include <functional>
#include <unordered_map>

namespace laya
{

class IUploadTask
{
  public:
    using onSuccessCallback = std::function<void(int statusCode, const std::string &data)>;
    using onFailCallback = std::function<void(int statusCode, const std::string &errorMsg)>;
    using onCompleteCallback = std::function<void()>;  
    using onHeadersReceivedCallback = std::function<void(const std::unordered_map<std::string, std::string> &headers)>;
    using onProgressUpdateCallback = std::function<void(double progress, double totalBytesSent, double totalBytesExpectedToSend)>;
    class Description
    {
      public:
        std::string url;
        std::string filePath;
        std::string name;
        std::unordered_map<std::string, std::string> headers;
        std::unordered_map<std::string, std::string> formData;
        int timeout = 5000;//todo 默认值是否合适
    };    
    IUploadTask() : m_isPaused(false)
    {
    }
    virtual ~IUploadTask() = default; 
    virtual void upload(const Description &description, onSuccessCallback successCallback, onFailCallback failCallback, onCompleteCallback completeCallback, onHeadersReceivedCallback headersReceivedCallback, onProgressUpdateCallback progressUpdateCallback) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void abort() = 0;

  protected:
    bool m_isPaused;
};
IUploadTask* createUploadTask();
} // namespace laya
#endif

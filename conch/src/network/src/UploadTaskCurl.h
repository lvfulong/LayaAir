#ifndef __UPLOAD_TASK_CURL_H__
#define __UPLOAD_TASK_CURL_H__

#include <network/IUploadTask.h>

namespace laya
{
class UploadTaskCurl : public IUploadTask
{
  public:
    void upload(const Description &description, onSuccessCallback successCallback, onFailCallback failCallback, onCompleteCallback completeCallback, onHeadersReceivedCallback headersReceivedCallback, onProgressUpdateCallback progressUpdateCallback) override;
    void pause() override;
    void resume() override;
    void abort() override;
};
} // namespace laya
#endif

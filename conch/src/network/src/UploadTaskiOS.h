#ifndef __UPLOAD_TASK_IOS_H__ 
#define __UPLOAD_TASK_IOS_H__

#include <network/IUploadTask.h>

namespace laya
{
class UploadTaskiOS : public IUploadTask
{
  public:
    ~UploadTaskiOS();
    void upload(const Description &description, onSuccessCallback successCallback, onFailCallback failCallback, onCompleteCallback completeCallback, onHeadersReceivedCallback headersReceivedCallback, onProgressUpdateCallback progressUpdateCallback) override;
    void pause() override;
    void resume() override;
    void abort() override;
  public:
    void* m_iosManager = nullptr;
};
} // namespace laya
#endif

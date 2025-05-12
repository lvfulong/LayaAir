#ifndef __UPLOAD_TASK_ANDROID_H__ 
#define __UPLOAD_TASK_ANDROID_H__

#include <network/IUploadTask.h>
#include "CToJavaBridge.h"

namespace laya
{
class UploadTaskAndroid : public IUploadTask
{
  public:
    ~UploadTaskAndroid();
    void upload(const Description &description, onSuccessCallback successCallback, onFailCallback failCallback, onCompleteCallback completeCallback, onHeadersReceivedCallback headersReceivedCallback, onProgressUpdateCallback progressUpdateCallback) override;
    void pause() override;
    void resume() override;
    void abort() override;
    static void addStaticMethod(JNIEnv *env, const char *className);
  public:
    onSuccessCallback m_successCallback;
    onFailCallback m_failCallback;
    onCompleteCallback m_completeCallback;
    onHeadersReceivedCallback m_headersReceivedCallback;
    onProgressUpdateCallback m_progressUpdateCallback;
    jobject m_task = nullptr;
    
};
} // namespace laya
#endif

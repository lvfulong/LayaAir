#ifndef __JSNETWORK_H__
#define __JSNETWORK_H__

#include "JSRuntime.h"
#include <network/IUploadTask.h>
#include <jsbind/JSBind.h>
#include <stdio.h>
#include <string>

namespace laya
{
class JSUploadTask
{
  public:
    JSUploadTask(IUploadTask *uploadTask = nullptr);
    ~JSUploadTask();
    void abort();
    void onProgressUpdate(jsbind::Local callback);
    void offProgressUpdate(jsbind::Local callback);
    void onHeadersReceived(jsbind::Local callback);
    void offHeadersReceived(jsbind::Local callback);

  public:
    IUploadTask *m_uploadTask = nullptr;
    std::vector<jsbind::Persistent> m_progressUpdateCallbacks;
    std::vector<jsbind::Persistent> m_headersReceivedCallbacks;
    jsbind::Persistent m_onSuccessCallback;
    jsbind::Persistent m_onFailCallback;
    jsbind::Persistent m_onCompleteCallback;
};
class JSNetwork
{
  public:
    static void exportJS(jsbind::Object &context, jsbind::global_class_<JSRuntime> &class_binding);
    static jsvm_value uploadFile(jsbind::Local object);
};
} // namespace laya

#endif

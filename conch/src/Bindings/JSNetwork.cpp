#include "JSNetwork.h"
#include "JCConch.h"
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>
#include <jsbind/Local.h>

namespace laya
{

JSUploadTask::JSUploadTask(IUploadTask *uploadTask)
{
    m_uploadTask = uploadTask;
}
JSUploadTask::~JSUploadTask()
{
    if (m_uploadTask != nullptr)   
    {
        delete m_uploadTask;
        m_uploadTask = nullptr;
    }
}
void JSUploadTask::abort()
{   
    DEBUG_CHECK(m_uploadTask != nullptr);
    m_uploadTask->abort();
}
void JSUploadTask::onProgressUpdate(jsbind::Local callback)
{
    // todo
}
void JSUploadTask::offProgressUpdate(jsbind::Local callback)
{
    // todo
}
void JSUploadTask::onHeadersReceived(jsbind::Local callback)
{
    // todo
}
void JSUploadTask::offHeadersReceived(jsbind::Local callback)
{
    // todo
}
jsvm_value JSNetwork::uploadFile(jsbind::Local object)
{
    std::string url = "";
    if (object["url"].isString())
    {
        url = object["url"].as<std::string>();
    }

    std::string filePath = "";
    if (object["filePath"].isString())
    {
        filePath = object["filePath"].as<std::string>();
    }

    std::string name = "";
    if (object["name"].isString())
    {
        name = object["name"].as<std::string>();
    }

    std::unordered_map<std::string, std::string> headers;
    if (object["headers"].isObject())
    {
        headers = object["headers"].as<std::unordered_map<std::string, std::string>>();
    }

    std::unordered_map<std::string, std::string> formData;
    if (object["formData"].isObject())
    {
        formData = object["formData"].as<std::unordered_map<std::string, std::string>>();
    }

    int timeout = 0;
    if (object["timeout"].isNumber())
    {
        timeout = object["timeout"].as<int>();
    }

    auto uploadTask = new JSUploadTask(createUploadTask());
    jsvm_value jstask = jsbind::Make(uploadTask);
    
    if (object["success"].isFunction())
    {
        uploadTask->m_onSuccessCallback.reset(object["success"].as<jsvm_value>());
    }

    if (object["fail"].isFunction())
    {
        uploadTask->m_onFailCallback.reset(object["fail"].as<jsvm_value>());
    }

    if (object["complete"].isFunction())
    {
        uploadTask->m_onCompleteCallback.reset(object["complete"].as<jsvm_value>()); 
    }
    // todo ������������ �����Ϊshared_ptr
    IUploadTask::Description desc;
    desc.url = url;
    desc.filePath = filePath;
    desc.name = name;
    desc.headers = headers;
    desc.formData = formData;
    desc.timeout = timeout;

    jsbind::makeStrong(uploadTask);	//防止被釋放
    uploadTask->m_uploadTask->upload(
        desc,
        [uploadTask](int statusCode, const std::string &data) {
            LOGD("upload success %s, %d", data.c_str(), statusCode);
            postToJS([data, statusCode, uploadTask]() {
            if (uploadTask->m_onFailCallback.isValid() && uploadTask->m_onSuccessCallback.getLocal().isFunction())
            {

                auto res = jsbind::makeObject();
                jsbind::set_option(res, "data", data);
                jsbind::set_option(res, "statusCode", statusCode);

                uploadTask->m_onSuccessCallback.call<void>(jsvm::global(), res);
            }
            });
        },
        [uploadTask](int statusCode, const std::string &errorMessage) {
            LOGD("upload fail %d %s", statusCode, errorMessage.c_str());
            postToJS([statusCode, uploadTask, errorMessage]() {
            if (uploadTask->m_onFailCallback.isValid() && uploadTask->m_onFailCallback.getLocal().isFunction())
            {
                uploadTask->m_onFailCallback.call<void>(jsvm::global(), statusCode, errorMessage);
            }
            });
        },
        [uploadTask]() {
            LOGD("upload complete");
            postToJS([uploadTask]() {
            if (uploadTask->m_onFailCallback.isValid() && uploadTask->m_onCompleteCallback.getLocal().isFunction())
            {
                uploadTask->m_onCompleteCallback.call<void>(jsvm::global());
            }
            jsbind::makeWeak(uploadTask);
            });
        },
        [uploadTask](const std::unordered_map<std::string, std::string> &headers) {
            LOGD("upload headers received");
            #if 0
            for (const auto &header : headers)
            {
                LOGD("upload header %s: %s", header.first.c_str(), header.second.c_str());
            }   
            #endif
            postToJS([headers, uploadTask]() {
            // todo
            });
        },
        [uploadTask](double progress, double totalBytesSent, double totalBytesExpectedToSend) {     
            LOGD("upload progress %f, %f, %f", progress, totalBytesSent, totalBytesExpectedToSend);
            postToJS([progress, totalBytesSent, totalBytesExpectedToSend, uploadTask]() {
            // todo
            });
        });
    return jstask;
}
void JSNetwork::exportJS(jsbind::Object &context, jsbind::global_class_<JSRuntime> &class_binding)
{

    class_binding.class_function("uploadFile", &JSNetwork::uploadFile);

    {
        jsbind::class_<JSUploadTask> class_binding;
        class_binding.function("abort", &JSUploadTask::abort);
        class_binding.constructor<>();
        context.class_("conchUploadTask", class_binding);
    }

}
} // namespace laya

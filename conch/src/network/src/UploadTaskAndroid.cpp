#include "UploadTaskAndroid.h"
#include <utils/Log.h>
#include "CToJavaBridge.h"


static const char *s_className = "layaair/game/browser/LayaUploadTask";
namespace laya {

static jclass s_cls = NULL;
static jmethodID s_upload = NULL;
static jmethodID s_abort = NULL;

static jclass s_description_cls = NULL;
static jmethodID s_description_constructorID = NULL;
static jmethodID s_description_setUrlID = NULL;
static jmethodID s_description_setFilePathID = NULL;  
static jmethodID s_description_setHeadersID = NULL;
static jmethodID s_description_setFormDataID = NULL;
static jmethodID s_description_setTimeoutID = NULL;

static jclass s_map_cls = NULL;
static jmethodID s_map_constructor = NULL;
static jmethodID s_map_put = NULL;

void UploadTaskAndroid::addStaticMethod(JNIEnv *env, const char *className)
{
    {
        jclass cls = env->FindClass(className);
        s_cls = (jclass)env->NewGlobalRef(cls);
        s_upload = env->GetStaticMethodID(s_cls, "upload",
                                              "(JLlayaair/game/browser/LayaUploadTask$Description;)Llayaair/"
                                              "game/browser/LayaUploadTask;");

        s_abort = env->GetStaticMethodID(s_cls, "abort", "(Llayaair/game/browser/LayaUploadTask;)V");
    }
    {
        jclass cls = env->FindClass("layaair/game/browser/LayaUploadTask$Description");
        s_description_cls = (jclass)env->NewGlobalRef(cls);
        s_description_constructorID  = env->GetMethodID(s_description_cls, "<init>", "()V");
        s_description_setUrlID = env->GetMethodID(s_description_cls, "setUrl", "(Ljava/lang/String;)V");
        s_description_setFilePathID = env->GetMethodID(s_description_cls, "setFilePath", "(Ljava/lang/String;)V");
        s_description_setHeadersID = env->GetMethodID(s_description_cls, "setHeaders", "(Ljava/util/Map;)V");
        s_description_setFormDataID = env->GetMethodID(s_description_cls, "setFormData", "(Ljava/util/Map;)V");
        s_description_setTimeoutID = env->GetMethodID(s_description_cls, "setTimeout", "(I)V");
    }
    {
        jclass mapClass = env->FindClass("java/util/HashMap");
        s_map_cls = (jclass)env->NewGlobalRef(mapClass);
        s_map_constructor = env->GetMethodID(s_map_cls, "<init>", "()V");
        s_map_put = env->GetMethodID(s_map_cls, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    }
}
void UploadTaskAndroid::upload(const Description &description, 
                       onSuccessCallback successCallback,
                       onFailCallback failCallback,
                       onCompleteCallback completeCallback,
                       onHeadersReceivedCallback headersReceivedCallback,
                       onProgressUpdateCallback progressUpdateCallback) {  

    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    m_successCallback = successCallback;
    m_failCallback = failCallback;
    m_completeCallback = completeCallback;
    m_headersReceivedCallback = headersReceivedCallback;
    m_progressUpdateCallback = progressUpdateCallback;


    jstring jurl = env->NewStringUTF(description.url.c_str());
    jstring jfilePath = env->NewStringUTF(description.filePath.c_str());
    jobject jheaders = env->NewObject(s_map_cls, s_map_constructor);
    for (auto &header : description.headers) {      
        jstring jkey = env->NewStringUTF(header.first.c_str());
        jstring jvalue = env->NewStringUTF(header.second.c_str());
        env->CallObjectMethod(jheaders, s_map_put, jkey, jvalue);
    }
    jobject jformData = env->NewObject(s_map_cls, s_map_constructor);
    for (auto &formData : description.formData) {
        jstring jkey = env->NewStringUTF(formData.first.c_str());
        jstring jvalue = env->NewStringUTF(formData.second.c_str());
        env->CallObjectMethod(jformData, s_map_put, jkey, jvalue);
    }
    jint jtimeout = description.timeout;

    jobject jdescription = env->NewObject(s_description_cls, s_description_constructorID);
    env->CallVoidMethod(jdescription, s_description_setUrlID, jurl);
    env->CallVoidMethod(jdescription, s_description_setFilePathID, jfilePath); 
    env->CallVoidMethod(jdescription, s_description_setHeadersID, jheaders);
    env->CallVoidMethod(jdescription, s_description_setFormDataID, jformData);
    env->CallVoidMethod(jdescription, s_description_setTimeoutID, jtimeout);


    jobject jtask = env->CallStaticObjectMethod(s_cls, s_upload, (jlong)this, jdescription);
    m_task = env->NewGlobalRef(jtask);  


    env->DeleteLocalRef(jurl);
    env->DeleteLocalRef(jfilePath);
    env->DeleteLocalRef(jheaders);
    env->DeleteLocalRef(jformData);
    env->DeleteLocalRef(jdescription);
    env->DeleteLocalRef(jtask);


}

void UploadTaskAndroid::pause() {
}

void UploadTaskAndroid::resume() {
}

void UploadTaskAndroid::abort() {
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_abort, m_task);
}

UploadTaskAndroid::~UploadTaskAndroid() {
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->DeleteGlobalRef(m_task);
}   
extern "C"
{
    JNIEXPORT void JNICALL Java_layaair_game_browser_LayaUploadTask_onFailure(JNIEnv *env, jobject obj, jlong ptr,
                                                                               jint statusCode, jstring errorMsg)
    {
        UploadTaskAndroid *uploadTask = (UploadTaskAndroid *)ptr;

        const char *strErrorMsg = env->GetStringUTFChars(errorMsg, NULL);
        std::string errorMsgStr = strErrorMsg;
        env->ReleaseStringUTFChars(errorMsg, strErrorMsg);

        if (uploadTask->m_failCallback)
        {
            uploadTask->m_failCallback(statusCode, errorMsgStr);
        }
        if (uploadTask->m_completeCallback)
        {
            uploadTask->m_completeCallback();
        }   
    }
    JNIEXPORT void JNICALL Java_layaair_game_browser_LayaUploadTask_onResponse(JNIEnv *env, jobject obj, jlong ptr,
                                                                               jint statusCode, jstring data)
    {
        UploadTaskAndroid *uploadTask = (UploadTaskAndroid *)ptr;

        const char *strData = env->GetStringUTFChars(data, NULL);   
        std::string dataStr = strData;
        env->ReleaseStringUTFChars(data, strData);

        if (uploadTask->m_successCallback)
        {
            uploadTask->m_successCallback(statusCode, dataStr); 
        }   
        if (uploadTask->m_completeCallback)
        {
            uploadTask->m_completeCallback();
        }   
    }

}
} // namespace laya

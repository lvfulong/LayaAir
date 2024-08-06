#include "HttpClientAndroid.h"
#include <utils/JCCommonMethod.h>
#include <utils/Log.h>

static const char *s_className = "layaair/game/browser/LayaHttpClient";

namespace laya
{
jclass HttpClientAndroid::s_cls = NULL;
jmethodID HttpClientAndroid::s_createMethodID = NULL;
jmethodID HttpClientAndroid::s_addHeader = NULL;
jmethodID HttpClientAndroid::s_postData = NULL;
jmethodID HttpClientAndroid::s_setReadTimeout = NULL;
jmethodID HttpClientAndroid::s_setConnectTimeout = NULL;
jmethodID HttpClientAndroid::s_doRequest = NULL;
jmethodID HttpClientAndroid::s_cancel = NULL;

void HttpClientAndroid::addStaticMethod(JNIEnv *env, const char *className)
{
    jclass cls = env->FindClass(className);
    s_cls = (jclass)env->NewGlobalRef(cls);
    s_createMethodID = env->GetStaticMethodID(s_cls, "create",
                                              "(JLjava/lang/String;Ljava/lang/String;)Llayaair/"
                                              "game/browser/LayaHttpClient;");
    s_addHeader = env->GetStaticMethodID(s_cls, "addHeader",
                                         "(Llayaair/game/browser/LayaHttpClient;Ljava/lang/"
                                         "String;Ljava/lang/String;)V");
    s_postData = env->GetStaticMethodID(s_cls, "postData", "(Llayaair/game/browser/LayaHttpClient;[B)V");
    s_setReadTimeout = env->GetStaticMethodID(s_cls, "setReadTimeout", "(Llayaair/game/browser/LayaHttpClient;I)V");
    s_setConnectTimeout =
        env->GetStaticMethodID(s_cls, "setConnectTimeout", "(Llayaair/game/browser/LayaHttpClient;I)V");
    s_doRequest = env->GetStaticMethodID(s_cls, "doRequest", "(Llayaair/game/browser/LayaHttpClient;)V");
    s_cancel = env->GetStaticMethodID(s_cls, "cancel", "(Llayaair/game/browser/LayaHttpClient;)V");
}

HttpClientAndroid::HttpClientAndroid(const std::string &url, const std::string &localFilePath,
                                     const onProgressFunction &functionOnProgress, const onEndFunction &functionOnEnd,
                                     std::weak_ptr<HttpClientManager> httpClientManager)
    : IHttpClient(httpClientManager)
{
    m_url = encodeURI(url.c_str());
    m_localFilePath = localFilePath;
    m_functionOnEnd = functionOnEnd;
    m_functionOnProgress = functionOnProgress;

    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    jstring jurl = env->NewStringUTF(m_url.c_str());
    jstring jlocalFilePath = env->NewStringUTF(m_localFilePath.c_str());
    jobject jdownloader = env->CallStaticObjectMethod(s_cls, s_createMethodID, (jlong)this, jurl, jlocalFilePath);
    m_downloader = env->NewGlobalRef(jdownloader);
    env->DeleteLocalRef(jurl);
    env->DeleteLocalRef(jlocalFilePath);
    env->DeleteLocalRef(jdownloader);
}

HttpClientAndroid::~HttpClientAndroid()
{
    CToJavaBridge::ThreadJNIData *pThreadData = CToJavaBridge::GetInstance()->checkThreadJNI();
    if (!pThreadData)
        return;
    JNIEnv *pJNI = pThreadData->pThreadJNI;
    if (m_downloader != nullptr)
    {
        pJNI->DeleteGlobalRef(m_downloader);
    }
}

void HttpClientAndroid::doRequest()
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_doRequest, m_downloader);
}

void HttpClientAndroid::setMethod(const std::string &method)
{
    // static const char* s_methodSign =
    // "(Llayaair/game/browser/LayaHttpClient;Ljava/lang/String;)V";
}

void HttpClientAndroid::addHeader(const std::string &key, const std::string &value)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    jstring jKey = env->NewStringUTF(key.c_str());
    jstring jVal = env->NewStringUTF(value.c_str());
    env->CallStaticVoidMethod(s_cls, s_addHeader, m_downloader, jKey, jVal);
    env->DeleteLocalRef(jKey);
    env->DeleteLocalRef(jVal);
}
void HttpClientAndroid::postData(const char *pData, int nLen)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    jbyteArray bytearray = env->NewByteArray(nLen);
    env->SetByteArrayRegion(bytearray, 0, nLen, reinterpret_cast<const jbyte *>(pData));
    env->CallStaticVoidMethod(s_cls, s_postData, m_downloader, bytearray);
    env->DeleteLocalRef(bytearray);
}

void HttpClientAndroid::setReadTimeout(int miliseconds)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_setReadTimeout, m_downloader, miliseconds);
}

void HttpClientAndroid::setConnectTimeout(int miliseconds)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_setConnectTimeout, m_downloader, miliseconds);
}
void HttpClientAndroid::cancel()
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_cancel, m_downloader);
}
extern "C"
{
    JNIEXPORT void JNICALL Java_layaair_game_browser_LayaHttpClient_onProgress(JNIEnv *env, jobject obj, jlong ptr,
                                                                               jint total, jint now, jfloat speed)
    {
        HttpClientAndroid *downloader = (HttpClientAndroid *)ptr;
        if (downloader->m_functionOnProgress)
        {
            downloader->m_functionOnProgress((unsigned int)total, (unsigned int)now, (float)speed);
        }
    }

    JNIEXPORT void JNICALL Java_layaair_game_browser_LayaHttpClient_onResponse(JNIEnv *env, jobject obj, jlong ptr,
                                                                               jbyteArray byteArray, jint responseCode,
                                                                               jstring jheader)
    {
        HttpClientAndroid *downloader = (HttpClientAndroid *)ptr;
        LOGI("Download end:%d", responseCode);

        const char *strHeader = env->GetStringUTFChars(jheader, NULL);
        std::string header = strHeader;
        env->ReleaseStringUTFChars(jheader, strHeader);

        bool bBigFile = downloader->isBigFile();
        if (bBigFile)
        {
            // ���ļ�û��buffer
            JCBuffer jb;
            downloader->m_functionOnEnd(jb, "", "", 0 /*CURLE_OK*/, responseCode, header);
        }
        else
        {

            jint len = env->GetArrayLength(byteArray);
            if (len <= 0)
            {
                JCBuffer jb;
                downloader->m_functionOnEnd(jb, "", "", 0 /*CURLE_OK*/, responseCode, header);
            }
            else
            {
                jbyte *ba = env->GetByteArrayElements(byteArray, JNI_FALSE);
                char *result = new char[len];
                memcpy(result, ba, len);

                JCBuffer buf((void *)result, len, false, true);
                // request->m_responseCallback(buf, pCurl->m_strLocalAddr,
                // pCurl->m_strSvAddr, 0/*CURLE_OK*/, pCurl->m_nResponseCode,
                // pCurl->m_strResponseHead);
                downloader->m_functionOnEnd(buf, "", "", 0 /*CURLE_OK*/, responseCode, header);

                env->ReleaseByteArrayElements(byteArray, ba, 0);
            }
        }

        delete downloader;
    }
    JNIEXPORT void JNICALL Java_layaair_game_browser_LayaHttpClient_onFailure(JNIEnv *env, jobject obj, jlong ptr,
                                                                              jint code)
    {
        HttpClientAndroid *downloader = (HttpClientAndroid *)ptr;
        if (code == -999)
        {
            // 主动取消
        }
        else if (downloader->m_functionOnEnd)
        {
            // curl ִ��ʧ��
            static std::string nullstr;
            JCBuffer jb;
            downloader->m_functionOnEnd(jb, "", "", 7 /*CURLE_COULDNT_CONNECT*/, code, nullstr);
        }

        delete downloader;
    }
}
} // namespace laya
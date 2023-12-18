#include "JSFileReaderWX.h"
#include <resource/JCFileResManager.h>
#include <Utils/Log.h>
#include <Utils/JCCrypto.h>
#include <Utils/JCMemorySurvey.h>
#include "../../JCScriptRuntime.h"
#include <Utils/JCFileSystem.h>
#include <JCConch.h>

//extern std::string LAYA_NATIVE_FILE_CACHE_USR_PATH;
extern std::string LAYA_NATIVE_FILE_CACHE_ROOT_PATH;
extern std::string LAYA_NATIVE_FILE_CACHE_TMP_PATH;

namespace laya 
{
    JsFileReaderWX::JsFileReaderWX()
    {
        AdjustAmountOfExternalAllocatedMemory(86);
        JCMemorySurvey::GetInstance()->newClass("JSFileReaderWX", 86, this);
        m_CallbackRef.reset(new int(1));
    }
    JsFileReaderWX::~JsFileReaderWX()
    {
        m_pOnError.reset();
        m_pOnSuccess.reset();
        m_pOnComplete.reset();
        JCMemorySurvey::GetInstance()->releaseClass("JSFileReaderWX", this);
    }
    JsValue JsFileReaderWX::downloadFile(JSValueAsParam param)
    {
#ifdef JS_V8

		makeStrong(this);	//��ֹ��ጷ�

        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();

        v8::Local<v8::Value> v8Url = v8Param->Get(context, Js_Str(isolate, "url")).ToLocalChecked();

        if (v8Url->IsNullOrUndefined() || !v8Url->IsString())
        {
            return JSP_TO_JS_UNDEFINE;
        }
        m_url = JS_TO_CPP(const char*, v8Url);
        if (m_url.empty())
        {
            return JSP_TO_JS_UNDEFINE;
        }

        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();
        
        if (v8FailCallback->IsFunction())
        {
            m_pOnError.reset(v8FailCallback);
        }

        if (v8SuccessCallback->IsFunction())
        {
            m_pOnSuccess.reset(v8SuccessCallback);
        }

        if (v8CompleteCallback->IsFunction())
        {
            m_pOnComplete.reset(v8CompleteCallback);
        }


        JCFileResManager* pfsMgr = JCConch::s_pScriptRuntime->m_pFileResMgr;
        JCFileRes* res = pfsMgr->getRes(m_url.c_str());
        std::weak_ptr<int> cbref(m_CallbackRef);
        res->setOnReadyCB(std::bind(&JsFileReaderWX::onDownloadOK, this, std::placeholders::_1, cbref));
        res->setOnErrorCB(std::bind(&JsFileReaderWX::onDownloadError, this, std::placeholders::_1, std::placeholders::_2, cbref));
        return JSP_TO_JS(JsDownloadTask*, new JsDownloadTask());
#endif
    }
    void JsFileReaderWX::onDownloadError(JCResStateDispatcher* p_pRes, int p_nErrCode, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock())
            return;
        JCFileRes* pFRes = (JCFileRes*)p_pRes;
        if (p_nErrCode == 1)
        {
            onFinish(nullptr, 404);
        }
        else
        {
            onFinish(nullptr, -1);
        }
    }
    void JsFileReaderWX::onDownloadOK(JCResStateDispatcher* p_pRes, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock())
            return;
        JCResStateDispatcher* pRes = (JCResStateDispatcher*)p_pRes;
        JCFileRes* pFileRes = (JCFileRes*)pRes;
        /*if (pFileRes->m_pBuffer.get() == NULL || pFileRes->m_nLength == 0)
        {
            onFinish(nullptr , -1);
        }
        else*/
        {
            char* pBuff = (char*)(pFileRes->m_pBuffer.get());
            int nLen = pFileRes->m_nLength;
            JCUrl url;
            url.parse(m_url.c_str());
            JCMD5 md5;
            md5.GenerateMD5((unsigned char *)m_url.c_str(), m_url.length());
            std::string lnFilePath = "tmp/" + md5.ToString() + getLowercaseExtOfUrl(m_url.c_str());
            std::string strLocalTempCachePath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + lnFilePath;
            if (writeFileSync1(strLocalTempCachePath.c_str(), pBuff, nLen, 0))
            {
                onFinish((std::string("wxfile://") + lnFilePath).c_str(), 200);
            }
            else
            {
                onFinish(nullptr, -1);
            }
        }
    }
    void JsFileReaderWX::onFinish(const char *tempFilePath, int statusCode)
    {
#ifdef JS_V8

        v8::Isolate* pIso = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = pIso->GetCurrentContext();
        v8::Local<v8::Object> res = v8::Object::New(pIso);
        res->Set(context, Js_Str(pIso, "statusCode"), JSP_TO_JS(int, statusCode));
        if (tempFilePath)
        {
            res->Set(context, Js_Str(pIso, "tempFilePath"), Js_Str(pIso, tempFilePath));
        }
		//TODO more statusCode ?
        if (statusCode == 200)
        {
            m_pOnSuccess.call<void>(toLocal(this), res);
            m_pOnComplete.call<void>(toLocal(this),res);
        }
        else
        {
            m_pOnError.call<void>(toLocal(this), res);
            m_pOnComplete.call<void>(toLocal(this), res);
        }
        makeWeak(this);
#endif
    }
    void JsFileReaderWX::exportJS(Context& context)
    {
        class_<JsFileReaderWX> class_binding;
        class_binding.constructor<>();
        class_binding.function("downloadFile", &JsFileReaderWX::downloadFile);
        context.class_("FileReaderWX", class_binding);
    }
    void JsDownloadTask::exportJS(Context& context)
    {
        class_<JsDownloadTask> class_binding;
        class_binding.constructor<>();
        class_binding.function("onProgressUpdate", &JsDownloadTask::onProgressUpdate);
        context.class_("_DownloadTask", class_binding);
    }

    JsDownloadTask::JsDownloadTask()
    {

    }

    JsDownloadTask::~JsDownloadTask()
    {

    }

    void JsDownloadTask::onProgressUpdate(JSValueAsParam param)
    {

    }
}
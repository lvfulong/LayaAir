#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include "JSLayaNative.h"
#include "../../JCScriptRuntime.h"
#include <Utils/Log.h>
#include <stdio.h>
#include <Utils/JCFileSystem.h>
#ifdef ANDROID
#include "CToJavaBridge.h"
#elif __APPLE__
    #include "CToObjectC.h"
#endif
#include "VideoRecorder/MediaRecorder.h"
#include <Utils/JCCrypto.h>
#include <resource/JCFileResManager.h>
#include "JCConch.h"

namespace laya
{
	class LayaNative {
	public:
		static void postMessage(JSValueAsParam param)
		{
			JSLayaNative::getInstance()->postMessage(param);
		}
		static std::string getSystemInfoSync()
		{
            return JSLayaNative::getInstance()->getSystemInfoSync();
		}
		static void finish()
		{
            JSLayaNative::getInstance()->finish();
		}
		static void exitMiniProgram()
		{
            JSLayaNative::getInstance()->exitMiniProgram();
		}
		static	void vibrateShort(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->vibrateShort(param);
		}
		static	void vibrateLong(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->vibrateLong(param);
		}
		static void chooseImage(JSValueAsParam param) {
            JSLayaNative::getInstance()->chooseImage(param);
		}
		static void saveImageToPhotosAlbum(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->saveImageToPhotosAlbum(param);
		}
        static void showKeyboard(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->showKeyboard(param);
		}
        static void updateKeyboard(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->updateKeyboard(param);
		}
        static void onKeyboardInput(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->onKeyboardInput(param);
		}
        static void offKeyboardInput()
		{
            JSLayaNative::getInstance()->offKeyboardInput();
		}
        static void onKeyboardConfirm(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->onKeyboardConfirm(param);
		}
        static void offKeyboardConfirm()
		{
            JSLayaNative::getInstance()->offKeyboardConfirm();
		}
        static void onKeyboardComplete(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->onKeyboardComplete(param);
		}
        static void offKeyboardComplete()
		{
            JSLayaNative::getInstance()->offKeyboardComplete();
		}
        static void hideKeyboard(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->hideKeyboard(param);
		}
        static void createBufferURL(JSValueAsParam param)
		{
            JSLayaNative::getInstance()->createBufferURL(param);
		}
        static void revokeBufferURL(const char* url)
		{
            JSLayaNative::getInstance()->revokeBufferURL(url);
		}
	};
	static const char* s_className = "layaair/game/browser/LayaEditBoxNew";
    JSLayaNative*  JSLayaNative::ms_pLayaNative = NULL;
    JSLayaNative* JSLayaNative::getInstance()
    {
        if (!ms_pLayaNative)
        {
            ms_pLayaNative = new JSLayaNative();
        }
        return ms_pLayaNative;
    }
    JSLayaNative::JSLayaNative()
    {
		m_CallbackRef.reset(new int(1));
        AdjustAmountOfExternalAllocatedMemory(128);
    }
    JSLayaNative::~JSLayaNative()
    {
        ms_pLayaNative = NULL;
    }

	void JSLayaNative::postMessage(JSValueAsParam param)
	{
		return;
	}
	std::string JSLayaNative::getSystemInfoSync()
	{
#ifdef __APPLE__
		return CToObjectCGetSystemInfo();
#elif ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getSystemInfo", kRet))
		{
			return CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet).c_str();;
		}
		return "{}";
#elif WIN32
		return "{}";
#endif
	}
	void JSLayaNative::exitMiniProgram()
	{
#ifdef WIN32
		exit(0);
#elif ANDROID
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "exit", kRet);
#elif __APPLE__
        abort();
#endif
	}
    void JSLayaNative::finish()
    {
#ifdef WIN32
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "finish", kRet);
#elif __APPLE__
#endif
	}
	void JSLayaNative::vibrateShort(JSValueAsParam param)
	{
#ifdef ANDROID
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "vibrateShort", kRet);
#elif __APPLE__
        CToObjectCVibrateShort();
#endif
		LOGI("vibrateShort");
	}

	void JSLayaNative::vibrateLong(JSValueAsParam param)
	{
#ifdef ANDROID
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "vibrateLong", kRet);
#elif __APPLE__
        CToObjectCVibrateLong();
#endif	
		LOGI("vibrateLong");
	}

	void JSLayaNative::chooseImage(JSValueAsParam param)
	{
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();

        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
		if (v8Param->IsNullOrUndefined())
		{
			return;
		}

		int s1 = 9;
		v8::Local<v8::Value> v81 = v8Param->Get(context, Js_Str(isolate, "count")).ToLocalChecked();
		if (!v81->IsNullOrUndefined())
		{
			s1 = JS_TO_CPP(int, v81);
		}

		std::string s2 = "original";
		v8::Local<v8::Value> v82 = v8Param->Get(context, Js_Str(isolate, "sizeType")).ToLocalChecked();
		if (v82->IsArray())
		{
			v8::Local<v8::Object> v82Array = v82->ToObject(context).ToLocalChecked();
			v8::Local<v8::Value> v82str = v82Array->Get(context, 0).ToLocalChecked();
			if (!v82str->IsNullOrUndefined())
			{
				s2 = JS_TO_CPP(const char*, v82str);
			}
		}

		std::string s3 = "album";
		v8::Local<v8::Value> v83 = v8Param->Get(context, Js_Str(isolate, "sourceType")).ToLocalChecked();
		if (v83->IsArray())
		{
			v8::Local<v8::Object> v83Array = v83->ToObject(context).ToLocalChecked();
			v8::Local<v8::Value> v83str = v83Array->Get(context, 0).ToLocalChecked();
			if (!v83str->IsNullOrUndefined())
			{
				s3 = JS_TO_CPP(const char*, v83str);
			}
		}

		v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();

		if (v8SuccessCallback->IsFunction())
		{
			m_pOnSuccess.reset(v8SuccessCallback);
		}

		v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();

		if (v8FailCallback->IsFunction())
		{
			m_pOnFail.reset(v8FailCallback);
		}

		v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

		if (v8CompleteCallback->IsFunction())
		{
			m_pOnComplete.reset(v8CompleteCallback);
		}

#ifdef ANDROID
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "chooseImage", s1, s2.c_str(), s3.c_str(), kRet);
#elif __APPLE__
        CToObjectCChooseImage(s1, s2.c_str(), s3.c_str());
#endif
		LOGI("chooseImage");
	}

	void JSLayaNative::onComplete(std::weak_ptr<int> callbackRef, int resultCode, std::string jsonParam)
	{
		if (!callbackRef.lock())
			return;
		if (resultCode == 1)
		{
			//success
			LOGI("onComplete success");
			v8::Isolate* pIso = v8::Isolate::GetCurrent();
			v8::Local<v8::Context> context = pIso->GetCurrentContext();
			v8::Local<v8::Object> res;

			v8::Local<v8::String> v8Param = Js_Str(pIso, jsonParam.c_str());
			v8::Local<v8::Value> val = v8::JSON::Parse(context, v8Param).ToLocalChecked();
			res = val->ToObject(context).ToLocalChecked();

			m_pOnSuccess.call<void>(getCurrentContext().global(), res);
			m_pOnComplete.call<void>(getCurrentContext().global(), res);
		}
		else
		{
			//fail
			LOGI("onComplete fail");
			m_pOnFail.call<void>(getCurrentContext().global());
			m_pOnComplete.call<void>(getCurrentContext().global());
		}
		
	}

	void JSLayaNative::onCompleteCallJSFunction(int resultCode, std::string jsonParam)
	{
		LOGI("onCompleteCallJSFunction");
		std::weak_ptr<int> cbref(m_CallbackRef);
		auto pFunction = std::bind(&JSLayaNative::onComplete, this, cbref, resultCode, jsonParam);
        postToJS(pFunction);
	}

	void JSLayaNative::saveImageToPhotosAlbum(JSValueAsParam param)
	{
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();

		v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
		if (v8Param->IsNullOrUndefined())
		{
			return;
		}

		std::string s = "";
		v8::Local<v8::Value> v8Value = v8Param->Get(context, Js_Str(isolate, "filePath")).ToLocalChecked();

		if (v8Value->IsString() || v8Value->IsStringObject())
		{
			s = JS_TO_CPP(const char*, v8Value);
		}

		v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();

		if (v8SuccessCallback->IsFunction())
		{
			m_pOnSaveAlbumSuccess.reset(v8SuccessCallback);
		}

		v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();

		if (v8FailCallback->IsFunction())
		{
			m_pOnSaveAlbumFail.reset(v8FailCallback);
		}

		v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

		if (v8CompleteCallback->IsFunction())
		{
			m_pOnSaveAlbumComplete.reset(v8CompleteCallback);
		}
#ifdef ANDROID
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "saveImageToPhotosAlbum", s, kRet);
#endif
		LOGI("saveImageToPhotosAlbum");
	}

	void JSLayaNative::onSaveImageComplete(int resultCode)
	{
		LOGI("onSaveImageComplete");
		std::weak_ptr<int> cbref(m_CallbackRef);
		auto pFunction = std::bind(&JSLayaNative::saveImageComplete, this, cbref, resultCode);
        postToJS(pFunction);
	}

	void JSLayaNative::saveImageComplete(std::weak_ptr<int> callbackRef, int resultCode)
	{
		if (!callbackRef.lock())
			return;
		if (resultCode == 1)
		{
			LOGI("onComplete success");
			m_pOnSaveAlbumSuccess.call<void>(getCurrentContext().global());
			m_pOnSaveAlbumComplete.call<void>(getCurrentContext().global());
		}
		else
		{
			LOGI("onComplete fail");
			m_pOnSaveAlbumFail.call<void>(getCurrentContext().global());
			m_pOnSaveAlbumComplete.call<void>(getCurrentContext().global());
		}
	}

	void JSLayaNative::startRecord(int recorderType, const char *outUrl, int frameWidth, int frameHeight, long videoBitRate, int fps)
	{
#ifdef USE_FFMPEG
		LOGI("startRecord recorderType=%d, outUrl=%s, [w,h]=[%d,%d], videoBitRate=%ld, fps=%d", recorderType, outUrl, frameWidth, frameHeight, videoBitRate, fps);
		//std::unique_lock<std::mutex> lock(m_mutex);
		if (m_pAVRecorder == nullptr)
		{
			RecorderParam param = { 0 };
			param.frameWidth = frameHeight;
			param.frameHeight = frameWidth;
			param.videoBitRate = videoBitRate;
			param.fps = fps;
			param.audioSampleRate = DEFAULT_SAMPLE_RATE;
			param.channelLayout = AV_CH_LAYOUT_STEREO;
			param.sampleFormat = AV_SAMPLE_FMT_S16;
			m_pAVRecorder = new MediaRecorder(outUrl, &param);
			m_pAVRecorder->StartRecord();
		}	
#endif // USE_FFMPEG
	}
	void JSLayaNative::stopRecord()
	{
#ifdef USE_FFMPEG
		//std::unique_lock<std::mutex> lock(m_mutex);
		if (m_pAVRecorder != nullptr) 
		{
			m_pAVRecorder->StopRecord();
			delete m_pAVRecorder;
			m_pAVRecorder = nullptr;
		}
		return 0;
#endif // USE_FFMPEG
	}

	void JSLayaNative::onAudioData(uint8_t *pData, int size)
	{
#ifdef USE_FFMPEG
		LOGI("MediaRecorderContext::OnAudioData pData=%p, dataSize=%d", pData, size);
		AudioFrame audioFrame(pData, size, false);

		if (m_pAVRecorder != nullptr)
		{
			m_pAVRecorder->OnFrame2Encode(&audioFrame);
		}
#endif // USE_FFMPEG
	}
	void JSLayaNative::onRenderData(JSValueAsParam param) 
	{
#ifdef USE_FFMPEG
		//LOGI("MediaRecorderContext::OnGLRenderFrame ctx=%p, pImage=%p", ctx, pImage);
		//MediaRecorderContext *context = static_cast<MediaRecorderContext *>(ctx);
		//std::unique_lock<std::mutex> lock(context->m_mutex);
		if (m_pAVRecorder != nullptr)
		{
			//m_pAVRecorder->OnFrame2Encode(pImage);
		}
#endif // USE_FFMPEG
	}
	void JSLayaNative::updateKeyboard(JSValueAsParam param)
	{
#ifdef JS_V8
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();

		v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();

		v8::Local<v8::Value> v8Value = v8Param->Get(context, Js_Str(isolate, "value")).ToLocalChecked();



		v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
		v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
		v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

		if (v8FailCallback->IsFunction())
		{
			//m_pOnError.set(onerrorid, this, v8FailCallback);
		}

		if (v8SuccessCallback->IsFunction())
		{
			//m_pOnSuccess.set(onsuccessid, this, v8SuccessCallback);
		}

		if (v8CompleteCallback->IsFunction())
		{
			//m_pOnComplete.set(oncompleteid, this, v8CompleteCallback);
		}
#endif
	}

	void JSLayaNative::showKeyboard(JSValueAsParam param)
	{
#ifdef JS_V8
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();

		v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();

		v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
		v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
		v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();



		v8::Local<v8::Value> v8DefaultValue = v8Param->Get(context, Js_Str(isolate, "defaultValue")).ToLocalChecked();
		const char* defaultValue = JS_TO_CPP(const char*, v8DefaultValue);

		v8::Local<v8::Value> v8MaxLength = v8Param->Get(context, Js_Str(isolate, "maxLength")).ToLocalChecked();
		int32_t maxLength = JS_TO_CPP(int32_t, v8MaxLength);

		v8::Local<v8::Value> v8Multiple = v8Param->Get(context, Js_Str(isolate, "multiple")).ToLocalChecked();
		bool multiple = JS_TO_CPP(bool, v8Multiple);

		v8::Local<v8::Value> v8ConfirmHold = v8Param->Get(context, Js_Str(isolate, "confirmHold")).ToLocalChecked();
		bool confirmHold = JS_TO_CPP(bool, v8ConfirmHold);

		v8::Local<v8::Value> v8ConfirmType = v8Param->Get(context, Js_Str(isolate, "confirmType")).ToLocalChecked();
		const char* confirmType = JS_TO_CPP(const char*, v8ConfirmType);

		v8::Local<v8::Value> v8Prompt = v8Param->Get(context, Js_Str(isolate, "prompt")).ToLocalChecked();
		const char* prompt = JS_TO_CPP(const char*, v8Prompt);

		v8::Local<v8::Value> v8PromptColor = v8Param->Get(context, Js_Str(isolate, "promptColor")).ToLocalChecked();
		const char* promptColor = JS_TO_CPP(const char*, v8PromptColor);
        
        v8::Local<v8::Value> v8InputType = v8Param->Get(context, Js_Str(isolate, "inputType")).ToLocalChecked();
        const char* inputType = JS_TO_CPP(const char*, v8InputType);

		static const char* s_methodSign = "(Ljava/lang/String;IZZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z";
#endif
        bool ret = true;
#ifdef ANDROID
		JNIEnv* pJNI = nullptr;
		jclass thisClass = NULL;
		jmethodID methodID = NULL;

		bool ok = CToJavaBridge::GetInstance()->getClassAndStaticMethod(s_className, "show", s_methodSign, &pJNI, &thisClass, &methodID);
		if (!ok)
		{
			LOGE("getClassAndStaticMethod error");
			return;
		}

		jstring jDefaultValue = pJNI->NewStringUTF(defaultValue != nullptr ? defaultValue : "");
		jstring jConfirmType = pJNI->NewStringUTF(confirmType != nullptr ? confirmType : "");
		jstring jPrompt = pJNI->NewStringUTF( prompt != nullptr ? prompt : "");
		jstring jPromptColor = pJNI->NewStringUTF(promptColor != nullptr ? promptColor : "");
		jstring jInputType = pJNI->NewStringUTF(inputType != nullptr ? inputType : "text");
		ret = (bool)pJNI->CallStaticBooleanMethod(thisClass,
												  methodID,
												  jDefaultValue,
												  maxLength,
												  multiple ? JNI_TRUE : JNI_FALSE,
												  confirmHold ? JNI_TRUE : JNI_FALSE,
												  jConfirmType,
												  jPrompt,
												  jPromptColor,
												  jInputType);
		pJNI->DeleteLocalRef(jDefaultValue);
		pJNI->DeleteLocalRef(jConfirmType);
		pJNI->DeleteLocalRef(jPrompt);
		pJNI->DeleteLocalRef(jPromptColor);
		pJNI->DeleteLocalRef(jInputType);
#elif __APPLE__
        CToObjectCShowEditBoxWX(defaultValue, maxLength, multiple, confirmHold, confirmType != nullptr ? confirmType : "", prompt != nullptr ? prompt : "", promptColor != nullptr ? promptColor : "", inputType != nullptr ? inputType : "text");
#endif
#ifdef JS_V8
		if (!ret)
		{
			if (v8FailCallback->IsFunction())
			{
				Persistent onError;
				onError.reset(v8FailCallback);
				onError.call<void>(getCurrentContext().global());
				onError.reset();
			}
		}
		else
		{
			if (v8SuccessCallback->IsFunction())
			{
				Persistent onSuccess;
				onSuccess.reset(v8SuccessCallback);
				onSuccess.call<void>(getCurrentContext().global());
				onSuccess.reset();
			}
		}
		if (v8CompleteCallback->IsFunction())
		{
			Persistent onComplete;
			onComplete.reset(v8CompleteCallback);
			onComplete.call<void>(getCurrentContext().global());
			onComplete.reset();
		}
#endif
	}

	void JSLayaNative::onKeyboardInput(JSValueAsParam param)
	{
		if (!param.IsEmpty())
		{
			m_pOnKeyboardInput.reset(param);
		}
	}
	void JSLayaNative::offKeyboardInput()
	{
		m_pOnKeyboardInput.reset();
	}

	void JSLayaNative::handleKeyboardInput(const char* strValue)
	{
		{
			std::string value(strValue);
            postToJS([this, value]() {
				v8::Isolate* pIso = v8::Isolate::GetCurrent();
                v8::HandleScope handle_scope(pIso);
				v8::Local<v8::Context> context = pIso->GetCurrentContext();
				v8::Local<v8::Object> v8Value = v8::Object::New(pIso);
				v8Value->Set(context, Js_Str(pIso, "value"), Js_Str(pIso, value.c_str()));
				this->m_pOnKeyboardInput.call<void>(getCurrentContext().global(), v8Value);
			});
		}
	}
	void JSLayaNative::onKeyboardConfirm(JSValueAsParam param)
	{
		if (!param.IsEmpty())
		{
			m_pOnKeyboardConfirm.reset(param);
		}
	}

	void JSLayaNative::offKeyboardConfirm()
	{
		m_pOnKeyboardConfirm.reset();
	}

	void JSLayaNative::handleKeyboardConfirm(const char* strValue)
	{
		{
			std::string value(strValue);
            postToJS([this, value]() {
				v8::Isolate* pIso = v8::Isolate::GetCurrent();
				v8::Local<v8::Context> context = pIso->GetCurrentContext();
				v8::Local<v8::Object> v8Value = v8::Object::New(pIso);
				v8Value->Set(context, Js_Str(pIso, "value"), Js_Str(pIso, value.c_str()));
				this->m_pOnKeyboardConfirm.call<void>(getCurrentContext().global(),v8Value);
			});
		}
	}

	void JSLayaNative::onKeyboardComplete(JSValueAsParam param)
	{
		if (!param.IsEmpty())
		{
			m_pOnKeyboardComplete.reset(param);
		}
	}

	void JSLayaNative::offKeyboardComplete()
	{
		m_pOnKeyboardComplete.reset();
	}

	void JSLayaNative::handleKeyboardComplete(const char* strValue)
	{
		{
			std::string value(strValue);
            postToJS([this, value]() {
				v8::Isolate* pIso = v8::Isolate::GetCurrent();
				v8::Local<v8::Context> context = pIso->GetCurrentContext();
				v8::Local<v8::Object> v8Value = v8::Object::New(pIso);
				v8Value->Set(context, Js_Str(pIso, "value"), Js_Str(pIso, value.c_str()));
				this->m_pOnKeyboardComplete.call<void>(getCurrentContext().global(), v8Value);
			});
		}
	}

	void JSLayaNative::hideKeyboard(JSValueAsParam param)
	{
#ifdef JS_V8
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();

		v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();

		v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
		v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
		v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();
#endif
        bool ret = true;
#ifdef ANDROID
		static const char* s_methodSign = "()Z";

		JNIEnv* pJNI = nullptr;
		jclass thisClass = NULL;
		jmethodID methodID = NULL;

		bool ok = CToJavaBridge::GetInstance()->getClassAndStaticMethod(s_className, "hide", s_methodSign, &pJNI, &thisClass, &methodID);
		if (!ok)
		{
			LOGE("getClassAndStaticMethod error");
			return;
		}

		ret = (bool)pJNI->CallStaticBooleanMethod(thisClass, methodID);
#elif __APPLE__
        CToObjectCHideEditBoxWX();
#endif
#ifdef JS_V8
		if (!ret)
		{
			if (v8FailCallback->IsFunction())
			{
				Persistent onError;
				onError.reset(v8FailCallback);
				onError.call<void>(getCurrentContext().global());
				onError.reset();
			}
		}
		else
		{
			if (v8SuccessCallback->IsFunction())
			{
				Persistent onSuccess;
				onSuccess.reset(v8SuccessCallback);
				onSuccess.call<void>(getCurrentContext().global());
				onSuccess.reset();
			}
		}
		if (v8CompleteCallback->IsFunction())
		{
			Persistent onComplete;
			onComplete.reset(v8CompleteCallback);
			onComplete.call<void>(getCurrentContext().global());
			onComplete.reset();
		}
#endif
	}
	std::string JSLayaNative::createBufferURL(JSValueAsParam param)
	{
		char* pArrayBuffer = NULL;
		int nArrayBufferSize = 0;
		bool bIsArrayBuffer = extractJSAB(param, pArrayBuffer, nArrayBufferSize);
		if (bIsArrayBuffer)
		{
			JCMD5 md5;
			md5.GenerateMD5((unsigned char *)pArrayBuffer, nArrayBufferSize);
			std::string url = "wxblob://" + md5.ToString();
			JCFileResManager* pfsMgr = JCConch::s_pScriptRuntime->m_pFileResMgr;
			pfsMgr->createBufferURL(url, pArrayBuffer, nArrayBufferSize);
			return url;
		}
		return "";
	}
	void JSLayaNative::revokeBufferURL(const char* url)
	{
		JCFileResManager* pfsMgr = JCConch::s_pScriptRuntime->m_pFileResMgr;
		pfsMgr->revokeBufferURL(url);
	}

	void JSLayaNative::exportJS(Module& module)
    {
#ifdef JS_V8
		class_<LayaNative> class_binding;
		class_binding.class_function("postMessage", &LayaNative::postMessage);
		class_binding.class_function("getSystemInfoSyncString", &LayaNative::getSystemInfoSync);
        class_binding.class_function("finish", &LayaNative::finish);
		class_binding.class_function("exitMiniProgram", &LayaNative::exitMiniProgram);
		class_binding.class_function("vibrateShort", &LayaNative::vibrateShort);
		class_binding.class_function("vibrateLong", &LayaNative::vibrateLong);
		class_binding.class_function("chooseImage", &LayaNative::chooseImage);
		class_binding.class_function("saveImageToPhotosAlbum", &LayaNative::saveImageToPhotosAlbum);
		class_binding.class_function("showKeyboard", &LayaNative::showKeyboard);
		class_binding.class_function("updateKeyboard", &LayaNative::updateKeyboard);
		class_binding.class_function("onKeyboardInput", &LayaNative::onKeyboardInput);
		class_binding.class_function("offKeyboardInput", &LayaNative::offKeyboardInput);
		class_binding.class_function("onKeyboardConfirm", &LayaNative::onKeyboardConfirm);
		class_binding.class_function("offKeyboardConfirm", &LayaNative::offKeyboardConfirm);
		class_binding.class_function("onKeyboardComplete", &LayaNative::onKeyboardComplete);
		class_binding.class_function("offKeyboardComplete", &LayaNative::offKeyboardComplete);
		class_binding.class_function("hideKeyboard", &LayaNative::hideKeyboard);
		class_binding.class_function("createBufferURL", &LayaNative::createBufferURL);
		class_binding.class_function("revokeBufferURL", &LayaNative::revokeBufferURL);
		module.add_class_("LayaNative", class_binding);
#endif
	}
}

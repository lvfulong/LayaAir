/**
@file			JSFileSystemManager.cpp
@brief			
@author			James
@version		1.0
@date			2016.3.22
*/
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include "JSFileSystemManager.h"
#include "../../JCScriptRuntime.h"
#include <utils/Log.h>
#include <stdio.h>
#include <utils/JCFileSystem.h>
#include "JSZip.h"
#include <utils/JCCrypto.h>
#include "JSStat.h"
#include <zip/JCZip.h>
#include "../../JCConch.h"

extern std::string LAYA_NATIVE_FILE_CACHE_USR_PATH;
extern std::string LAYA_NATIVE_FILE_CACHE_ROOT_PATH;
//extern std::string LAYA_NATIVE_FILE_CACHE_LOCAL_PATH;
namespace laya
{
	extern JCWorkerThread* g_FileIOThread;
    JSFileSystemManager*  JSFileSystemManager::ms_pFileSystemManager = NULL;
    JSFileSystemManager* JSFileSystemManager::getInstance()
    {
        if (!ms_pFileSystemManager)
        {
            ms_pFileSystemManager = new JSFileSystemManager();
        }
        return ms_pFileSystemManager;
    }
    JSFileSystemManager::JSFileSystemManager()
    {
		m_CallbackRef.reset(new int(1));
        AdjustAmountOfExternalAllocatedMemory(128);
    }
    JSFileSystemManager::~JSFileSystemManager()
    {
        ms_pFileSystemManager = NULL;
    }

    void JSFileSystemManager::readDirImpl(std::weak_ptr<int> callbackref, JSValueAsParam param)
    {
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();

        v8::Local<v8::Value> v8DirPath = v8Param->Get(context, Js_Str(isolate, "dirPath")).ToLocalChecked();
        if (v8DirPath->IsNullOrUndefined() || !v8DirPath->IsString())
        {
            return;
        }
	
        const char* dirPath = JS_TO_CPP(const char*, v8DirPath);
        if (!dirPath)
        {	
			//resetJsStrBuf();
            return;
        }
		
        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

        JCUrl url;
        url.parse(dirPath);
        if (url.m_nProto != JCUrl::wxfile)
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "unknow error");
			//resetJsStrBuf();
            return;
        }

        std::string realPath = dirPath + strlen("wxfile://");
        realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

        std::error_code error;
        if (!fs::exists(realPath, error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail no such file or directory");
			//resetJsStrBuf();
            return;
        }

        if (!fs::is_directory(realPath, error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail not a directory");
			//resetJsStrBuf();
            return;
        }
		Persistent* paramHandle = new Persistent();
		paramHandle->reset(param);
		g_FileIOThread->post([realPath, this, paramHandle, callbackref]() {
			if (!callbackref.lock())
				return;
            std::error_code error;
			std::vector<std::string> files;
			fs::path fullPath(realPath);
			std::string tempStrPath;
			if (fs::exists(fullPath, error))
			{
				fs::directory_iterator item_begin(fullPath, error);
				fs::directory_iterator item_end;
				for (; item_begin != item_end; item_begin.increment(error))
				{
					if (fs::is_directory(*item_begin, error))
					{
						fs::path tempPath(*item_begin);
						tempStrPath = tempPath.string();
						tempStrPath.replace(tempStrPath.find_first_of(realPath), tempStrPath.find_first_not_of(realPath), "wxfile://");//todo TEST
						files.push_back(std::move(tempStrPath));
					}
				}
			}
            postToJS([files, this, paramHandle, callbackref] {
				if (!callbackref.lock())
					return;
				v8::Isolate* pIso = v8::Isolate::GetCurrent();
				v8::Local<v8::Context> context = pIso->GetCurrentContext();
				v8::Local<v8::Object> vData = v8::Object::New(pIso);
				vData->Set(context, Js_Str(pIso, "files"), Converter<std::vector<std::string> >::ToJs(files));
				handleOnSuccess(paramHandle, vData);
				//resetJsStrBuf();
			});
			return;
		});
#endif
    }
	static void trim(const char*& p_pszData)
	{
		char cv = *p_pszData;
		while(cv==' '||cv=='\t' ||cv=='/')
		{
			cv=*(++p_pszData);
		}
	}
    void JSFileSystemManager::readFileImpl(std::weak_ptr<int> callbackref, JSValueAsParam param)
    {
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();

        v8::Local<v8::Value> v8FilePath = v8Param->Get(context, Js_Str(isolate, "filePath")).ToLocalChecked();
        if (v8FilePath->IsNullOrUndefined() || !v8FilePath->IsString())
        {
            return;
        }
		
        const char* filePath = JS_TO_CPP(const char*, v8FilePath);
        if (!filePath)
        {
			//resetJsStrBuf();
            return;
        }
		
        v8::Local<v8::Value> v8Encoding = v8Param->Get(context, Js_Str(isolate, "encoding")).ToLocalChecked();

        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

        trim(filePath);
        JCUrl url;
        url.parse(filePath);
        if (url.m_nProto != JCUrl::wxfile && url.m_nProto != JCUrl::unk  && url.m_nProto != JCUrl::file)
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "unknow error");
			//resetJsStrBuf();
            return;
        }


        ENCODING_TYPE encodingType = ENCODING_TYPE::BINARY;
		if (v8Encoding->IsUndefined())
		{
			encodingType = ENCODING_TYPE::NONE;
		}
		else
        {
            const char* strEncoding = JS_TO_CPP(const char*, v8Encoding);
            if (strEncoding)
            {
                if (strcmp(strEncoding, "binary") == 0)
                {
                    encodingType = ENCODING_TYPE::BINARY;
                }
                else if (strcmp(strEncoding, "utf-8") == 0 || strcmp(strEncoding, "utf8") == 0)
                {
                    encodingType = ENCODING_TYPE::UTF8;
                }
				else if (strcmp(strEncoding, "") == 0)
				{
					encodingType = ENCODING_TYPE::NONE;
				}
				else
				{
					LOGE("readFile encoding type %s not support", strEncoding);
					return;
				}
            }
        }
       
        if (url.m_nProto == JCUrl::wxfile)
        { 
            std::string realPath;
            realPath = filePath + strlen("wxfile://");
            realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

            std::error_code error;
            if (!fs::exists(realPath, error))
            {
				//resetJsStrBuf();
                handleOnFailed(v8FailCallback, v8CompleteCallback, "fail no such file or directory");
                return;
            }
			Persistent* paramHandle = new Persistent();
			paramHandle->reset(param);

			g_FileIOThread->post([encodingType, realPath, this, paramHandle, callbackref]() {
				if (!callbackref.lock())
					return;
				if (encodingType == ENCODING_TYPE::NONE)
				{
					JCBuffer buf;
					bool bSucess = laya::readFileSync(realPath.c_str(), buf, JCBuffer::raw);
					buf.m_bNeedDel = false;
					char* jsBufferPtr = buf.m_pPtr;
					int jsBufferLen = buf.m_nLen;
                    postToJS([jsBufferPtr, jsBufferLen, bSucess, this, paramHandle, callbackref] {
						if (!callbackref.lock())
							return;
						if (bSucess)
						{
							v8::Isolate* pIso = v8::Isolate::GetCurrent();
							v8::Local<v8::Context> context = pIso->GetCurrentContext();
							v8::Local<v8::Object> vData = v8::Object::New(pIso);
							vData->Set(context, Js_Str(pIso, "data"), createJSAB((char*)jsBufferPtr, jsBufferLen));
							handleOnSuccess(paramHandle, vData);
							if (jsBufferPtr)
							{
								delete[] jsBufferPtr;
							}
							//resetJsStrBuf();
							return;
						}
						else
						{
							handleOnFailed(paramHandle, "unknow error");
							//resetJsStrBuf();
							return;
						}
					});
				}
				else
				{
					JCBuffer buf;
					bool bSucess = laya::readFileSync(realPath.c_str(), buf, JCBuffer::raw);
					buf.m_bNeedDel = false;
					char* jsBufferPtr = buf.m_pPtr;
					int jsBufferLen = buf.m_nLen;
                    postToJS([encodingType, jsBufferPtr, jsBufferLen, bSucess, this, paramHandle, callbackref]{
						if (!callbackref.lock())
						return;
						if (bSucess)
						{
							v8::Isolate* pIso = v8::Isolate::GetCurrent();
							v8::Local<v8::Context> context = pIso->GetCurrentContext();
							v8::Local<v8::Object> vData = v8::Object::New(pIso);
							if (encodingType == ENCODING_TYPE::BINARY)
							{
								vData->Set(context, Js_Str(pIso, "data"), v8::String::NewFromOneByte(v8::Isolate::GetCurrent(), (const uint8_t*)jsBufferPtr, v8::NewStringType::kNormal, jsBufferLen).ToLocalChecked());
							}
							else if (encodingType == ENCODING_TYPE::UTF8)
							{
								vData->Set(context, Js_Str(pIso, "data"), v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), (const char*)jsBufferPtr, v8::NewStringType::kNormal, jsBufferLen).ToLocalChecked());
							}

							handleOnSuccess(paramHandle, vData);
							//resetJsStrBuf();
							if (jsBufferPtr)
							{
								delete[] jsBufferPtr;
							}
							return;
						}
						else
						{
							handleOnFailed(paramHandle, "unknow error");
							//resetJsStrBuf();
							return;
						}
					});
				}
			});
        }
        else//JCUrl::unk
        {
            if (!JCConch::s_pConch || JCConch::s_pScriptRuntime->m_pJSZipPackage.isEmpty())
            {
                handleOnFailed(v8FailCallback, v8CompleteCallback, "unknow error");
				//resetJsStrBuf();
                return;
            }

            JSZip* zip = (JSZip*)Converter<JSZip*>::ToCpp(JCConch::s_pScriptRuntime->m_pJSZipPackage.toLocal().handle_);

            std::error_code error;
            if (!zip->exists(filePath))
            {
                handleOnFailed(v8FailCallback, v8CompleteCallback, "fail no such file or directory");
				//resetJsStrBuf();
                return;
            }
			std::string realPath = filePath;
			Persistent* paramHandle = new Persistent();
			paramHandle->reset(param);
			//g_FileIOThread->post([zip, encodingType, realPath, this, paramHandle, callbackref]() {
			//	if (!callbackref.lock())
			//		return;
				if (encodingType == ENCODING_TYPE::NONE)
				{
					JCBuffer buf;
					bool bSucess = zip->m_zip.readFileByName(realPath.c_str(), buf);
                    if (bSucess)
                    {
                        LOGI("found file local package %s", realPath.c_str());
                    }
                    else
                    {
                        LOGI("not found file local package %s", realPath.c_str());
                    }
					buf.m_bNeedDel = false;
					char* jsBufferPtr = buf.m_pPtr;
					int jsBufferLen = buf.m_nLen;
                    postToJS([jsBufferPtr, jsBufferLen, bSucess, this, paramHandle, callbackref] {
						if (!callbackref.lock())
							return;
						if (bSucess)
						{
							v8::Isolate* pIso = v8::Isolate::GetCurrent();
							v8::Local<v8::Context> context = pIso->GetCurrentContext();
							v8::Local<v8::Object> vData = v8::Object::New(pIso);
							JsValue ab = createJSAB(jsBufferPtr, jsBufferLen);
							vData->Set(context, Js_Str(pIso, "data"), ab);
							handleOnSuccess(paramHandle, vData);
							//resetJsStrBuf();
							if (jsBufferPtr)
							{
								delete[] jsBufferPtr;
							}
							return;
						}
						else
						{
							handleOnFailed(paramHandle, "unknow error");
							//resetJsStrBuf();
							return;
						}
					});
				}
				else
				{
					if (encodingType == ENCODING_TYPE::UTF8)
					{
						JCBuffer buf;
						bool bSucess = zip->m_zip.readFileAsTextByName(realPath.c_str(), buf);
                        if (bSucess)
                        {
                            LOGI("found file local package %s", realPath.c_str());
                        }
                        else
                        {
                            LOGI("not found file local package %s", realPath.c_str());
                        }
						buf.m_bNeedDel = false;
						char* jsBufferPtr = buf.m_pPtr;
						int jsBufferLen = buf.m_nLen;
						//LOGI("read   %s  %d", realPath.c_str(), jsBufferLen);
						//LOGI("read   %s  '%s' ", realPath.c_str(), jsBufferPtr);
                        postToJS([jsBufferPtr, jsBufferLen, bSucess, this, paramHandle, callbackref] {
							if (!callbackref.lock())
								return;
							if (bSucess)
							{
								v8::Isolate* pIso = v8::Isolate::GetCurrent();
								v8::Local<v8::Context> context = pIso->GetCurrentContext();
								v8::Local<v8::Object> vData = v8::Object::New(pIso);
								JsValue ab(JSP_TO_JS_STR(jsBufferPtr));
								vData->Set(context, Js_Str(pIso, "data"), ab);
								handleOnSuccess(paramHandle, vData);
								//resetJsStrBuf();
								if (jsBufferPtr)
								{
									delete[] jsBufferPtr;
								}
								return;
							}
							else
							{
								handleOnFailed(paramHandle, "unknow error");
								//resetJsStrBuf();
								return;
							}
						});
					}
					else if (encodingType == ENCODING_TYPE::BINARY)
					{
                        postToJS([this, paramHandle, callbackref] {
							if (!callbackref.lock())
								return;
							handleOnFailed(paramHandle, "readFile encoding type binary not support");
							//TODO binary
							LOGE("readFile encoding type binary not support");
						});
					}
					return;
				}
			//});
        }
#endif
    }
    JsValue JSFileSystemManager::readFileSync(const char *filePath, const char *encoding/*, const char *position, const char *length*/)
    {
#ifdef JS_V8

        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();


        ENCODING_TYPE encodingType = ENCODING_TYPE::BINARY;
        if (strcmp(encoding, "binary") == 0)
        {
            encodingType = ENCODING_TYPE::BINARY;
        }
        else if (strcmp(encoding, "utf-8") == 0 || strcmp(encoding, "utf8") == 0)
        {
            encodingType = ENCODING_TYPE::UTF8;
        }
		else if (strcmp(encoding, "Undefined") || strcmp(encoding, ""))
		{
			encodingType = ENCODING_TYPE::NONE;
		}
		else
		{
			LOGE("readFileSync encoding type %s not support", encoding);
			return JSP_TO_JS_UNDEFINE;
		}

        JCUrl url;
        url.parse(filePath);
        if (url.m_nProto != JCUrl::wxfile)
        {
            return JSP_TO_JS_UNDEFINE;
        }

        std::string realPath = filePath + strlen("wxfile://");
        realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

        std::error_code error;
        if (!fs::exists(realPath, error))
        {
            return JSP_TO_JS_UNDEFINE;
        }

        if (encodingType == ENCODING_TYPE::NONE)
        {
            JCBuffer buf;
            if (laya::readFileSync(realPath.c_str(), buf, JCBuffer::raw))
            {
                return createJSAB((char*)buf.m_pPtr, buf.m_nLen);
            }
            else
            {
                return JSP_TO_JS_UNDEFINE;
            }
        }
        else
        {
            JCBuffer buf;
            if (laya::readFileSync(realPath.c_str(), buf, JCBuffer::raw))
            {
				if (encodingType == ENCODING_TYPE::BINARY)
				{
					return v8::String::NewFromOneByte(v8::Isolate::GetCurrent(), (const uint8_t*)buf.m_pPtr, v8::NewStringType::kNormal, buf.m_nLen).ToLocalChecked();
				}
				else if (encodingType == ENCODING_TYPE::UTF8)
				{
					return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), (const char*)buf.m_pPtr, v8::NewStringType::kNormal, buf.m_nLen).ToLocalChecked();
				}
                return JSP_TO_JS_UNDEFINE;
            }
            else
            {
                return JSP_TO_JS_UNDEFINE;
            }
        }
#endif
    }
    JsValue JSFileSystemManager::writeFileSync(const char *filePath, JSValueAsParam data, const char *encoding)//string | ArrayBuffer
    {
#ifdef JS_V8

        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        JCUrl url;
        url.parse(filePath);
        if (url.m_nProto != JCUrl::wxfile)
        {
            return JSP_TO_JS_UNDEFINE;
        }

        std::string realPath = filePath + strlen("wxfile://");
        realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

        //TODO fail the maximum size of the file storage limit is exceeded
        std::error_code error;

        if (!fs::exists(fs::path(realPath).parent_path(), error))
        {
            return JSP_TO_JS_UNDEFINE;
        }

        if (data->IsArrayBuffer())
        {
			char* pABPtr = NULL;
            int nABLen = 0;
            bool bisab = extractJSAB(data, pABPtr, nABLen);
            bool bret = false;
			if (bisab)
			{
				if (pABPtr && nABLen > 0)
				{
					bret = writeFileSync1(realPath.c_str(), pABPtr, nABLen, 0);
					if (bret)
					{
						return JSP_TO_JS_UNDEFINE;
					}
					else
					{
						return JSP_TO_JS_UNDEFINE;
					}
				}
				else
				{
					return JSP_TO_JS_UNDEFINE;
				}
			}
			else
			{
				return JSP_TO_JS_UNDEFINE;
			}
        }
        else if (data->IsString())
        {
            if (strcmp(encoding, "utf-8") == 0 || strcmp(encoding, "utf8") == 0)
            {
                const char* pData = JS_TO_CPP(const char*, data);
                if (pData)
                {
					
                    int len = strlen(pData);
                    JCBuffer buf((char*)pData, len, false, false);
                    if (laya::writeFileSync(realPath.c_str(), buf, JCBuffer::utf8))
                    {
						//resetJsStrBuf();
                        return JSP_TO_JS_UNDEFINE;
                    }
                    else
                    {
						//resetJsStrBuf();
                        return JSP_TO_JS_UNDEFINE;
                    }
                }
                else
                {
					//resetJsStrBuf();
                    return JSP_TO_JS_UNDEFINE;
                }
            }
            else
            {
                LOGE("writeFileSync string not support encoding type %s", encoding);
                return JSP_TO_JS_UNDEFINE;
            }
        }
        return JSP_TO_JS_UNDEFINE;
#endif
    }
    
    void JSFileSystemManager::writeFileImpl(std::weak_ptr<int> callbackref, JSValueAsParam param)
    {
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        if (!param->IsObject())
        {
            return;
        }
        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();

        v8::Local<v8::Value> v8FilePath = v8Param->Get(context, Js_Str(isolate, "filePath")).ToLocalChecked();
        if (v8FilePath->IsNullOrUndefined() || !v8FilePath->IsString())
        {
            return;
        }

        const char* filePath = JS_TO_CPP(const char*, v8FilePath);
        if (!filePath)
        {
			//resetJsStrBuf();
            return;
        }
		
        v8::Local<v8::Value> v8Data = v8Param->Get(context, Js_Str(isolate, "data")).ToLocalChecked();
        if (v8Data->IsNullOrUndefined())
        {
			//resetJsStrBuf();
            return;
        }

        v8::Local<v8::Value> v8Encoding  = v8Param->Get(context, Js_Str(isolate, "encoding")).ToLocalChecked();

        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();
		
		ENCODING_TYPE encoding = ENCODING_TYPE::UTF8;
		if (v8Encoding->IsUndefined())
		{
			encoding = ENCODING_TYPE::NONE;
		}
		else
        {
            const char* strEncoding = JS_TO_CPP(const char*, v8Encoding);
            if (strEncoding)
            {
                if (strcmp(strEncoding, "binary") == 0)
                {
                    encoding = ENCODING_TYPE::BINARY;
                }
                else if (strcmp(strEncoding, "utf-8") == 0 || strcmp(strEncoding, "utf8") == 0)
                {
                    encoding = ENCODING_TYPE::UTF8;
                }
            }
        }

        JCUrl url;
        url.parse(filePath);
        if (url.m_nProto != JCUrl::wxfile)
        {
			//resetJsStrBuf();
            return;
        }

        std::string realPath = filePath + strlen("wxfile://");
        realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

        //TODO fail the maximum size of the file storage limit is exceeded
        std::error_code error;
        if (!fs::exists(fs::path(realPath).parent_path(), error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail no such file or directory");
			//resetJsStrBuf();
            return;
        }
		Persistent* paramHandle = new Persistent();
		paramHandle->reset(param);
		std::weak_ptr<int> wptr0(callbackref);
        if (v8Data->IsArrayBuffer())
        {
			char* pABPtr = NULL;
            int nABLen = 0;
			bool bisab = extractJSAB(v8Data, pABPtr, nABLen);
			g_FileIOThread->post([realPath, this, paramHandle, wptr0, bisab, pABPtr, nABLen]() {
				if (!wptr0.lock())
					return;
				bool bret = false;		
				std::weak_ptr<int> wptr1(wptr0);
				if (bisab)
				{

					if (pABPtr && nABLen > 0)
					{
						bret = writeFileSync1(realPath.c_str(), pABPtr, nABLen, 0);
						if (bret)
						{
							
                            postToJS([this, paramHandle, wptr1] {
								if (!wptr1.lock())
									return;
								handleOnSuccess(paramHandle);
								//resetJsStrBuf();
							});
							return;
						}
						else
						{
                            postToJS([this, paramHandle, wptr1] {
								if (!wptr1.lock())
									return;
								handleOnFailed(paramHandle, "unknow error");
								//resetJsStrBuf();
							});
							return;
						}
					}
					else
					{
                        postToJS([this, paramHandle, wptr1] {
							if (!wptr1.lock())
								return;
							handleOnFailed(paramHandle, "no data");
							//resetJsStrBuf();
						});
						return;
					}
				}
				else
				{
                    postToJS([this, paramHandle, wptr1] {
						if (!wptr1.lock())
							return;
						handleOnFailed(paramHandle, "not arraybuffer");
						//resetJsStrBuf();
					});
					return;
				}
			});
        }
        else if (v8Data->IsString())
        {
            if (encoding == ENCODING_TYPE::UTF8)
            {
                const char* pData = JS_TO_CPP(const char*, v8Data);	
				if (pData)
				{
					int length = strlen(pData);
					char* dataCopy = new char[length];
					memcpy(dataCopy, pData, length);
					g_FileIOThread->post([realPath, this, paramHandle, wptr0, dataCopy, length]() {
						if (!wptr0.lock())
							return;
						JCBuffer buf((char*)dataCopy, length, false, true);
						std::weak_ptr<int> wptr1(wptr0);
						if (laya::writeFileSync(realPath.c_str(), buf, JCBuffer::utf8))
						{
                            postToJS([this, paramHandle, wptr1] {
								if (!wptr1.lock())
									return;
								handleOnSuccess(paramHandle);
								//resetJsStrBuf();
							});
							return;
						}
						else
						{
                            postToJS([this, paramHandle, wptr1] {
								if (!wptr1.lock())
									return;
								handleOnFailed(paramHandle, "unknow error");
								//resetJsStrBuf();
							});
							return;
						}
					});
				}
				else
				{
					handleOnFailed(paramHandle, "no data");
					//resetJsStrBuf();
					return;
				}
            }
            else
            {
                postToJS([this, paramHandle, wptr0] {
					if (!wptr0.lock())
						return;
					handleOnFailed(paramHandle, "invalid encoding type");
					//resetJsStrBuf();
				});
                LOGE("writeFileSync not string support encoding type %d", encoding);
                return ;
            }
        }
#endif
    }
    
    void JSFileSystemManager::unlinkImpl(std::weak_ptr<int> callbackref, JSValueAsParam param)
    {
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        if (!param->IsObject())
        {
            return;
        }
        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();

        v8::Local<v8::Value> v8FilePath = v8Param->Get(context, Js_Str(isolate, "filePath")).ToLocalChecked();
        if (v8FilePath->IsNullOrUndefined() || !v8FilePath->IsString())
        {
            return;
        }

        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

        const char* filePath = JS_TO_CPP(const char*, v8FilePath);
        if (!filePath)
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "unknow error");		
			//resetJsStrBuf();
            return;
        }
		
        JCUrl url;
        url.parse(filePath);
        if (url.m_nProto != JCUrl::wxfile)
        {
			//resetJsStrBuf();
            return;
        }

        std::string realPath = filePath + strlen("wxfile://");
        realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

        std::error_code error;
        if (!fs::exists(realPath, error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail no such file or directory");
			//resetJsStrBuf();
            return;
        }

        if (fs::is_directory(realPath, error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail operation not permitted, unlink");
			//resetJsStrBuf();
            return;
        }
		Persistent* paramHandle = new Persistent();
		paramHandle->reset(param);
		g_FileIOThread->post([realPath, this, paramHandle, callbackref]() {
			if (!callbackref.lock())
				return;
			
			std::error_code error;
			bool bSucess = fs::remove(realPath, error);
            postToJS([this, paramHandle, callbackref, bSucess, error] {
				if (!callbackref.lock())
					return;
				if (bSucess)
				{
					handleOnSuccess(paramHandle);
					//resetJsStrBuf();
					return;
				}
				else
				{
					if (error == std::errc::permission_denied)
					{
						handleOnFailed(paramHandle, "fail permission denied");
						//resetJsStrBuf();
						return;
					}
					else
					{
						handleOnFailed(paramHandle, "unknow error");
						//resetJsStrBuf();
						return;
					}
				}
			});
		});
#endif
    }
    JsValue JSFileSystemManager::unlinkSync(const char *filePath)
    {
#ifdef JS_V8

        JCUrl url;
        url.parse(filePath);
        if (url.m_nProto != JCUrl::wxfile)
        {
            return JSP_TO_JS_UNDEFINE;
        }

        std::string realPath = filePath + strlen("wxfile://") ;
        realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

        std::error_code error;
        if (!fs::exists(realPath, error))
        {
            return JSP_TO_JS_UNDEFINE;
        }

        if (fs::is_directory(realPath, error))
        {
            return JSP_TO_JS_UNDEFINE;
        }

        if (fs::remove(realPath, error))
        {
            return JSP_TO_JS_UNDEFINE;
        }
        else
        {
            if (error == std::errc::permission_denied)
            {
                return JSP_TO_JS_UNDEFINE;
            }
            else
            {
                 return JSP_TO_JS_UNDEFINE;
            }
           
        }
#endif
    }
    void JSFileSystemManager::getFileInfoImpl(std::weak_ptr<int> callbackref, JSValueAsParam param)
    {
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        if (!param->IsObject())
        {
            return;
        }

        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
        v8::Local<v8::Value> v8FilePath = v8Param->Get(context, Js_Str(isolate, "filePath")).ToLocalChecked();
        if (v8FilePath->IsNullOrUndefined() || !v8FilePath->IsString())
        {
            return;
        }

        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

        const char* filePath = JS_TO_CPP(const char*, v8FilePath);
        if (!filePath)
        {
			//resetJsStrBuf();
            return;
        }
		
        JCUrl url;
        url.parse(filePath);
        if (url.m_nProto != JCUrl::wxfile)
        {
			//resetJsStrBuf();
            return;
        }

        std::string realPath = filePath + strlen("wxfile://");
        realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;
        std::error_code error;
        if (!fs::exists(realPath, error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail file not exist");
			//resetJsStrBuf();
            return;
        }
		Persistent* paramHandle = new Persistent();
		paramHandle->reset(param);
		g_FileIOThread->post([realPath, this, paramHandle, callbackref]() {
			if (!callbackref.lock())
				return;
			std::error_code error;
			std::uintmax_t size = fs::file_size(realPath, error);
            postToJS([size, this, paramHandle, callbackref] {
				if (!callbackref.lock())
					return;
				if (size == static_cast<std::uintmax_t>(-1))
				{
					handleOnFailed(paramHandle, "unknow error");
					//resetJsStrBuf();
					return;
				}
				else
				{
					v8::Isolate* pIso = v8::Isolate::GetCurrent();
					v8::Local<v8::Object> v8Size = v8::Object::New(pIso);
					v8Size->Set(pIso->GetCurrentContext(), Js_Str(pIso, "size"), JSP_TO_JS(double, size));
					handleOnSuccess(paramHandle, v8Size);
					//resetJsStrBuf();
					return;
				}
			});
		});
#endif
    }
    void JSFileSystemManager::handleOnFailed(JSValueAsParam failCallback, JSValueAsParam completeCallback, const char *errMsg)
    {
#ifdef JS_V8

        v8::Isolate* pIso = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = pIso->GetCurrentContext();
        v8::Local<v8::Object> msg = v8::Object::New(pIso);
        msg->Set(context, Js_Str(pIso, "errMsg"), Js_Str(pIso, errMsg));

        if (failCallback->IsFunction())
        {
            v8::Local<v8::Function> jsfun = v8::Local<v8::Function>::Cast(failCallback);

            Persistent func;
            func.reset(failCallback);
			func.call<void>(getCurrentContext().global(), msg);
			func.reset();
        }
        if (completeCallback->IsFunction())
        {
            Persistent func;
            func.reset(completeCallback);
			func.call<void>(getCurrentContext().global());
			func.reset();
        }
#endif
    }
    void JSFileSystemManager::handleOnSuccess(JSValueAsParam sucessCallback, JSValueAsParam completeCallback)
    {
#ifdef JS_V8
		
        if (sucessCallback->IsFunction())
        {
			Persistent func;
            func.reset(sucessCallback);
			func.call<void>(getCurrentContext().global());
			func.reset();
        }
        if (completeCallback->IsFunction())
        {
			Persistent func;
            func.reset(completeCallback);
			func.call<void>(getCurrentContext().global());
			func.reset();
        }
#endif
    }
    void JSFileSystemManager::handleOnSuccess(JSValueAsParam sucessCallback, JSValueAsParam completeCallback, JSValueAsParam param)
    {
#ifdef JS_V8

        if (sucessCallback->IsFunction())
        {
			Persistent func;
            func.reset(sucessCallback);
			func.call<void>(getCurrentContext().global(), param);
			func.reset();
        }
        if (completeCallback->IsFunction())
        {
			Persistent func;
            func.reset(completeCallback);
			func.call<void>(getCurrentContext().global());
			func.reset();
        }
#endif
    }
    void JSFileSystemManager::mkdirImpl(std::weak_ptr<int> callbackref, JSValueAsParam param)
    {
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        if (!param->IsObject())
        {
            return;
        }

        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
        v8::Local<v8::Value> v8DirPath = v8Param->Get(context, Js_Str(isolate, "dirPath")).ToLocalChecked();
        if (v8DirPath->IsNullOrUndefined() || !v8DirPath->IsString())
        {
            return;
        }

        bool recursive = false;
        v8::Local<v8::Value> v8Recursive = v8Param->Get(context, Js_Str(isolate, "recursive")).ToLocalChecked();
        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

        if (!v8Recursive.IsEmpty())
        {
            recursive = JS_TO_CPP(bool, v8Recursive);
        }

        const char* dirPath = JS_TO_CPP(const char*, v8DirPath);
        if (!dirPath)
        {
			//resetJsStrBuf();
            return;
        }
	
        JCUrl url;
        url.parse(dirPath);
        if (url.m_nProto != JCUrl::wxfile)
        {
			//resetJsStrBuf();
            return;
        }

        std::string realPath = dirPath + strlen("wxfile://");
        realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

        std::error_code error;
        if (fs::exists(realPath, error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "file already exists");
			//resetJsStrBuf();
            return;
        }
		Persistent* paramHandle = new Persistent();
		paramHandle->reset(param);
		g_FileIOThread->post([realPath, this, paramHandle, callbackref, recursive]() {
			if (!callbackref.lock())
				return;
			std::error_code error;
			bool result = recursive ? fs::create_directories(realPath, error) : fs::create_directory(realPath, error);
            postToJS([this, paramHandle, callbackref, result, error] {
				if (!callbackref.lock())
					return;
				if (result)
				{
					handleOnSuccess(paramHandle);
					//resetJsStrBuf();
					return;
				}
				else
				{
					if (error == std::errc::permission_denied)
					{
						handleOnFailed(paramHandle, "fail permission denied");
						//resetJsStrBuf();
						return;
					}
					else
					{
						handleOnFailed(paramHandle, "unknow error");
						//resetJsStrBuf();
						return;
					}
				}
			});
		});
#endif
    }

    void JSFileSystemManager::rmdirImpl(std::weak_ptr<int> callbackref, JSValueAsParam param)
    {
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        if (!param->IsObject())
        {
            return;
        }

        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
        v8::Local<v8::Value> v8DirPath = v8Param->Get(context, Js_Str(isolate, "dirPath")).ToLocalChecked();
        if (v8DirPath->IsNullOrUndefined() || !v8DirPath->IsString())
        {
            return;
        }

        bool recursive = false;
        v8::Local<v8::Value> v8Recursive = v8Param->Get(context, Js_Str(isolate, "recursive")).ToLocalChecked();
        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

        if (!v8Recursive.IsEmpty())
        {
            recursive = JS_TO_CPP(bool, v8Recursive);
        }

        const char* dirPath = JS_TO_CPP(const char*, v8DirPath);
        if (!dirPath)
        {
			//resetJsStrBuf();
            return;
        }
	
        JCUrl url;
        url.parse(dirPath);
        if (url.m_nProto != JCUrl::wxfile)
        {
			//resetJsStrBuf();
            return;
        }

        std::string realPath = dirPath + strlen("wxfile://");
        realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

        std::error_code error;
        if (!fs::exists(realPath, error) || !fs::is_directory(realPath, error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail no such file or directory");
			//resetJsStrBuf();
            return;
        }

        if (!recursive && !fs::is_empty(realPath, error))       
        {      
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail directory not empty");
			//resetJsStrBuf();
            return;
        }
		Persistent* paramHandle = new Persistent();
		paramHandle->reset(param);
        if (recursive)
        {
			g_FileIOThread->post([realPath, this, paramHandle, callbackref]() {
				if (!callbackref.lock())
					return;
				std::error_code error;
				if (static_cast<std::uintmax_t>(-1) != fs::remove_all(realPath, error))
				{
                    postToJS([this, paramHandle, callbackref] {
						if (!callbackref.lock())
							return;
						handleOnSuccess(paramHandle);
						//resetJsStrBuf();
					});
					return;
				}
				else
				{
					if (error == std::errc::permission_denied)
					{
                        postToJS([this, paramHandle, callbackref] {
							if (!callbackref.lock())
								return;
							handleOnFailed(paramHandle, "fail permission denied, open");
							//resetJsStrBuf();
						});
						return;
					}
					else
					{
                        postToJS([this, paramHandle, callbackref] {
							if (!callbackref.lock())
								return;
							handleOnFailed(paramHandle, "unknow error");
							//resetJsStrBuf();
						});
						return;
					}
				}
			});
        }
        else
        {
            if (fs::remove(realPath, error))
            {
                postToJS([this, paramHandle, callbackref] {
					if (!callbackref.lock())
						return;
					handleOnSuccess(paramHandle);
					//resetJsStrBuf();
				});
				return;
            }
            else
            {
                if (error == std::errc::permission_denied)
                {
                    postToJS([this, paramHandle, callbackref] {
						if (!callbackref.lock())
							return;
						handleOnFailed(paramHandle, "fail permission denied, open");
						//resetJsStrBuf();
					});
                    return;
                }
                else
                {
                    postToJS([this, paramHandle, callbackref] {
						if (!callbackref.lock())
							return;
						handleOnFailed(paramHandle, "unknow error");
						//resetJsStrBuf();
					});
                    return;
                }
            }
        }
#endif
    }
    void JSFileSystemManager::copyFileImpl(std::weak_ptr<int> callbackref, JSValueAsParam param)
    {
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        if (!param->IsObject())
        {
            return;
        }

        v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
        v8::Local<v8::Value> v8SrcPath = v8Param->Get(context, Js_Str(isolate, "srcPath")).ToLocalChecked();
        if (v8SrcPath->IsNullOrUndefined() || !v8SrcPath->IsString())
        {
            return;
        }

        v8::Local<v8::Value> v8DesPath = v8Param->Get(context, Js_Str(isolate, "destPath")).ToLocalChecked();
        if (v8DesPath->IsNullOrUndefined() || !v8DesPath->IsString())
        {
            return;
        }

        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

        const char* srcPath = JS_TO_CPP(const char*, v8SrcPath);
        if (!srcPath)
        {
			//resetJsStrBuf();
            return;
        }
		
        JCUrl srcUrl;
        srcUrl.parse(srcPath);
        if (srcUrl.m_nProto != JCUrl::wxfile)
        {
			//resetJsStrBuf();
            return;
        }

        std::string realSrcPath = srcPath + strlen("wxfile://");
        realSrcPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realSrcPath;

        const char* desPath = JS_TO_CPP(const char*, v8DesPath);
        if (!desPath)
        {
			//resetJsStrBuf();
            return;
        }

        JCUrl dstUrl;
        dstUrl.parse(desPath);
        if (dstUrl.m_nProto != JCUrl::wxfile)
        {
			//resetJsStrBuf();
            return;
        }

        std::string realDesPath = desPath + strlen("wxfile://");
        realDesPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realDesPath;

        std::error_code error;
        if (!fs::exists(realSrcPath, error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail no such file or directory, copyFile");
			//resetJsStrBuf();
            return;
        }

        if (!fs::exists(fs::path(realDesPath).parent_path(), error))
        {
            handleOnFailed(v8FailCallback, v8CompleteCallback, "fail no such file or directory, copyFile");
			//resetJsStrBuf();
            return;
        }
		Persistent* paramHandle = new Persistent();
		paramHandle->reset(param);
		g_FileIOThread->post([realSrcPath, realDesPath, this, paramHandle, callbackref]() {
			if (!callbackref.lock())
				return;
			std::error_code error;
			// todo 
			fs::copy(realSrcPath, realDesPath, error);
            postToJS([error, this, paramHandle, callbackref] {
				if (!callbackref.lock())
					return;
				if (!error)
				{
					handleOnSuccess(paramHandle);
					//resetJsStrBuf();
					return;
				}
				else if (error == std::errc::permission_denied)
				{
					handleOnFailed(paramHandle, "fail permission denied, open");
					//resetJsStrBuf();
					return;
				}
				else
				{
					handleOnFailed(paramHandle, "unknow error");
					//resetJsStrBuf();
					return;
				}
			});
		});
#endif
    }
	void JSFileSystemManager::writeFile(JSValueAsParam param)
	{
		std::weak_ptr<int> cbref(m_CallbackRef);
		writeFileImpl(cbref, param);
	}

	void JSFileSystemManager::unlink(JSValueAsParam param)
	{
		std::weak_ptr<int> cbref(m_CallbackRef);
		unlinkImpl(cbref, param);
	}

	void JSFileSystemManager::getFileInfo(JSValueAsParam param)
	{
		std::weak_ptr<int> cbref(m_CallbackRef);
		getFileInfoImpl(cbref, param);
	}

	void JSFileSystemManager::mkdir(JSValueAsParam param)
	{
		std::weak_ptr<int> cbref(m_CallbackRef);
		mkdirImpl(cbref, param);
	}
	void JSFileSystemManager::rmdir(JSValueAsParam param)
	{
		std::weak_ptr<int> cbref(m_CallbackRef);
		rmdirImpl(cbref, param);
	}

	void JSFileSystemManager::mkdirSync(const char* dirPath, bool recursive)
	{

#ifdef JS_V8

		JCUrl url;
		url.parse(dirPath);
		if (url.m_nProto != JCUrl::wxfile)
		{
			return;
		}

		std::string realPath = dirPath + strlen("wxfile://");
		realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

		std::error_code error;
		if (fs::exists(realPath, error))
		{
			LOGE("file already exists");
			return;
		}

		bool result = recursive ? fs::create_directories(realPath, error) : fs::create_directory(realPath, error);
		if (result)
		{
			return;
		}
		else
		{
			if (error == std::errc::permission_denied)
			{
				LOGE("fail permission denied");
				return;
			}
			else
			{
				LOGE("unknow error");
				return;
			}
		}
#endif
	}
	void JSFileSystemManager::copyFile(JSValueAsParam param)
	{
		std::weak_ptr<int> cbref(m_CallbackRef);
		copyFileImpl(cbref, param);
	}

	void JSFileSystemManager::readFile(JSValueAsParam param)
	{
		std::weak_ptr<int> cbref(m_CallbackRef);
		readFileImpl(cbref, param);
	}

	void JSFileSystemManager::readDir(JSValueAsParam param)
	{
		std::weak_ptr<int> cbref(m_CallbackRef);
		readDirImpl(cbref, param);
	}
	JsValue JSFileSystemManager::readdirSync(const char* dirPath)
	{

#ifdef JS_V8

		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();

	
		JCUrl url;
		url.parse(dirPath);
		if (url.m_nProto != JCUrl::wxfile)
		{
			LOGE("unknow error");
			return JSP_TO_JS_UNDEFINE;
		}

		std::string realPath = dirPath + strlen("wxfile://");
		realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

		std::error_code error;
		if (!fs::exists(realPath, error))
		{
			LOGE("fail no such file or directory");
			return JSP_TO_JS_UNDEFINE;
		}

		if (!fs::is_directory(realPath, error))
		{
			LOGE("fail not a directory");
			return JSP_TO_JS_UNDEFINE;
		}

		std::vector<std::string> files;
		fs::path fullPath(realPath);
		std::string tempStrPath;
		if (fs::exists(fullPath, error))
		{
			fs::directory_iterator item_begin(fullPath, error);
			fs::directory_iterator item_end;
			for (; item_begin != item_end; item_begin.increment(error))
			{
				if (fs::is_directory(*item_begin, error))
				{
					fs::path tempPath(*item_begin);
					tempStrPath = tempPath.string();
					tempStrPath.replace(tempStrPath.find_first_of(realPath), tempStrPath.find_first_not_of(realPath), "wxfile://");//todo TEST
					files.push_back(std::move(tempStrPath));
				}
			}
		}
		v8::Isolate* pIso = v8::Isolate::GetCurrent();
		v8::Local<v8::Object> vData = v8::Object::New(pIso);
		vData->Set(context, Js_Str(pIso, "files"), Converter<std::vector<std::string> >::ToJs(files));
		return vData;
#endif
	}
	static int getDirectorySize(const std::string& path)
	{
		int size = 0;
		fs::path fullPath(path);
		std::string tempStrPath;
        std::error_code error;
		if (fs::exists(fullPath, error))
		{
			fs::directory_iterator item_begin(fullPath, error);
			fs::directory_iterator item_end;
			for (; item_begin != item_end; item_begin.increment(error))
			{
				fs::path tempPath(*item_begin);
				tempStrPath = tempPath.string();
				if (fs::is_directory(*item_begin, error))
				{	
					size += getDirectorySize(tempStrPath);
				}
				else
				{
					std::error_code error;
					size += fs::file_size(tempStrPath, error);
				}
			}
		}
		return size;
	}
	JsValue JSFileSystemManager::statSync(const char* path/*, bool recursive*/)
	{
#ifdef JS_V8

		JCUrl url;
		url.parse(path);
		if (url.m_nProto != JCUrl::wxfile)
		{
			return JSP_TO_JS_UNDEFINE;
		}

		std::string realPath = path + strlen("wxfile://");
		realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

		std::error_code error;

		fs::file_status s = fs::status(realPath, error);
		if (fs::exists(s))
		{
			JSStat* pStat = new JSStat();
			pStat->m_isDirectory = fs::is_directory(s);
			pStat->m_isFile = fs::is_regular_file(s);
			if (fs::is_directory(s))
			{
				pStat->m_nSize = getDirectorySize(realPath);
			}
			else
			{
				std::error_code error;
				pStat->m_nSize = fs::file_size(realPath, error);
			}
			/*if (pStat->m_nSize == 0)
			{
				//LOGI("statSync size === 0");
				return JSP_TO_JS_UNDEFINE;
			}
			else*/
			{
				return JSP_TO_JS(JSStat*, pStat);
			}
		}
		else
		{
			return JSP_TO_JS_UNDEFINE;
		}
#endif
	}
	void JSFileSystemManager::stat(JSValueAsParam param)
	{
#ifdef JS_V8
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		if (!param->IsObject())
		{
			return;
		}

		v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
		v8::Local<v8::Value> v8Path = v8Param->Get(context, Js_Str(isolate, "path")).ToLocalChecked();
		if (v8Path->IsNullOrUndefined() || !v8Path->IsString())
		{
			return;
		}

		bool recursive = false;
		v8::Local<v8::Value> v8Recursive = v8Param->Get(context, Js_Str(isolate, "recursive")).ToLocalChecked();
		v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
		v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
		v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();

		if (!v8Recursive.IsEmpty())
		{
			recursive = JS_TO_CPP(bool, v8Recursive);
		}
		//TODO recursive == true
		const char* strPath = JS_TO_CPP(const char*, v8Path);
		if (!strPath)
		{
			//resetJsStrBuf();
			return;
		}
		JCUrl url;
		url.parse(strPath);
		if (url.m_nProto != JCUrl::wxfile)
		{
			return;
		}

		Persistent* paramHandle = new Persistent();
		paramHandle->reset(param);
		std::weak_ptr<int> callbackref(m_CallbackRef);

		std::string realPath = strPath + strlen("wxfile://");
		realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + realPath;

		g_FileIOThread->post([realPath, this, callbackref, paramHandle]() {
			if (!callbackref.lock())
				return;
			std::error_code error;

			fs::file_status s = fs::status(realPath, error);
			if (fs::exists(s))
			{
				bool isDirectory= fs::is_directory(s);
				bool isFile = fs::is_regular_file(s);
				int nSize = 0;
				if (fs::is_directory(s))
				{
					nSize = getDirectorySize(realPath);
				}
				else
				{
					std::error_code error;
					nSize = fs::file_size(realPath, error);
				}
                postToJS([this, callbackref, isDirectory, isFile, nSize, paramHandle] {
					if (!callbackref.lock())
						return;
					JSStat* pStat = new JSStat();
					pStat->m_isDirectory = isDirectory;
					pStat->m_isFile = isFile;
					pStat->m_nSize = nSize;

					v8::Isolate* pIso = v8::Isolate::GetCurrent();
					v8::Local<v8::Object> vData = v8::Object::New(pIso);
					v8::Local<v8::Context> context = pIso->GetCurrentContext();
					vData->Set(context, Js_Str(pIso, "stats"), JSP_TO_JS(JSStat*, pStat));
					handleOnSuccess(paramHandle, vData);
				});
			}
			else
			{
                postToJS([this, callbackref, paramHandle] {
					if (!callbackref.lock())
						return;
					handleOnFailed(paramHandle, "fail no such file or directory");
				});
			}
		});
#endif
	}
	void JSFileSystemManager::handleOnFailed(Persistent* paramHandle, const char *errMsg)
	{
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		JsValue param = paramHandle->toLocal().handle_;
		v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
		//paramHandle->set(paramHandle->m_nID, this, JSP_TO_JS_NULL);
		delete  paramHandle;

        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();


		handleOnFailed(v8FailCallback, v8CompleteCallback, errMsg);
	}
	void JSFileSystemManager::handleOnSuccess(Persistent* paramHandle, JSValueAsParam param1)
	{
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		JsValue param = paramHandle->toLocal().handle_;
		v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
		//paramHandle->set(paramHandle->m_nID, this, JSP_TO_JS_NULL);
		delete  paramHandle;

        v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
        v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
        v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();


		handleOnSuccess(v8SuccessCallback, v8CompleteCallback, param1);
	}
	void JSFileSystemManager::handleOnSuccess(Persistent* paramHandle)
	{
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		JsValue param = paramHandle->toLocal().handle_;
		v8::Local<v8::Object> v8Param = param->ToObject(context).ToLocalChecked();
		//paramHandle->set(paramHandle->m_nID, this, JSP_TO_JS_NULL);
		delete  paramHandle;
		
		v8::Local<v8::Value> v8SuccessCallback = v8Param->Get(context, Js_Str(isolate, "success")).ToLocalChecked();
		v8::Local<v8::Value> v8FailCallback = v8Param->Get(context, Js_Str(isolate, "fail")).ToLocalChecked();
		v8::Local<v8::Value> v8CompleteCallback = v8Param->Get(context, Js_Str(isolate, "complete")).ToLocalChecked();


		handleOnSuccess(v8SuccessCallback, v8CompleteCallback);
	}
	class FileSystemManager
	{
		public:
		static void unlink(JSValueAsParam param)
		{
			JSFileSystemManager::getInstance()->unlink(param);
		}
		static JsValue unlinkSync(const char *filePath)
		{
			return JSFileSystemManager::getInstance()->unlinkSync(filePath);
		}
		static void mkdir(JSValueAsParam param)
		{
			JSFileSystemManager::getInstance()->mkdir(param);
		}
		static void mkdirSync(const char* dirPath, bool recursive)
		{
			JSFileSystemManager::getInstance()->mkdirSync(dirPath, recursive);
		}
		static void getFileInfo(JSValueAsParam param)
		{
			JSFileSystemManager::getInstance()->getFileInfo(param);
		}
		static JsValue writeFileSync(const char *filePath, JSValueAsParam data, const char *encoding)
    	{
			return JSFileSystemManager::getInstance()->writeFileSync(filePath, data, encoding);
		}
		static void writeFile(JSValueAsParam param)
		{
			JSFileSystemManager::getInstance()->writeFile(param);
		}
		static void rmdir(JSValueAsParam param)
		{
			JSFileSystemManager::getInstance()->rmdir(param);
		}
		static void copyFile(JSValueAsParam param)
		{
			JSFileSystemManager::getInstance()->copyFile(param);
		}
		static JsValue readFileSync(const char *filePath, const char *encoding/*, const char *position, const char *length*/)
		{
			return JSFileSystemManager::getInstance()->readFileSync(filePath, encoding);
		}
		static void readFile(JSValueAsParam param)
		{
			return JSFileSystemManager::getInstance()->readFile(param);
		}
		static void readDir(JSValueAsParam param)
		{
			JSFileSystemManager::getInstance()->readDir(param);
		}
		static JsValue readdirSync(const char* dirPath)
		{
			return JSFileSystemManager::getInstance()->readdirSync(dirPath);
		}
		static JsValue statSync(const char* path/*, bool recursive*/)
		{
			return JSFileSystemManager::getInstance()->statSync(path);
		}
		static void stat(JSValueAsParam param)
		{
			JSFileSystemManager::getInstance()->stat(param);
		}
	};
	void JSFileSystemManager::exportJS(Module& module)
    {
#ifdef JS_V8
		class_<FileSystemManager> class_binding;
		class_binding.class_function("unlink", &FileSystemManager::unlink);
		class_binding.class_function("unlinkSync", &FileSystemManager::unlinkSync);
		class_binding.class_function("mkdir", &FileSystemManager::mkdir);
		class_binding.class_function("mkdirSync", &FileSystemManager::mkdirSync);
		class_binding.class_function("getFileInfo", &FileSystemManager::getFileInfo);
		class_binding.class_function("writeFileSync", &FileSystemManager::writeFileSync);
		class_binding.class_function("writeFile", &FileSystemManager::writeFile);
		class_binding.class_function("rmdir", &FileSystemManager::rmdir);
		class_binding.class_function("copyFile", &FileSystemManager::copyFile);
		class_binding.class_function("readFileSync", &FileSystemManager::readFileSync);
		class_binding.class_function("readFile", &FileSystemManager::readFile);
		class_binding.class_function("readdir", &FileSystemManager::readDir);
		class_binding.class_function("readdirSync", &FileSystemManager::readdirSync); 
		class_binding.class_function("statSync", &FileSystemManager::statSync);
		class_binding.class_function("stat", &FileSystemManager::stat);
		module.add_class_("FileSystemManager", class_binding);
#endif
	}
}

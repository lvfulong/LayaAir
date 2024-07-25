#include "JSFileSystem.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCFileSystem.h>
#include <ctime>

namespace laya
{
	bool JSFileSystem::rmDir(const char* p_pszPath, JSValueAsParam onprogress, JSValueAsParam oncomplete, JSValueAsParam onerror) 
    {
		return true;
	}
	bool JSFileSystem::rmDirSync(const char* p_pszPath) 
    {
		return true;
	}
	JsValue JSFileSystem::readdirSync(const std::string &path) 
    {
		if (!FileSystem::exists(path))
			return JSP_TO_JS_NULL;
		std::vector<std::string> paths = FileSystem::readdirSync(path);
		return Converter<std::vector<std::string> >::ToJs(paths);
	}
	JsValue JSFileSystem::lstatSync(const std::string &path) 
    {
		std::vector<std::string> paths;
		if (!FileSystem::exists(path))
			return JSP_TO_JS_NULL;
		//try {
			std::time_t wtime;
//#ifdef OS_WINDOWS
            wtime = FileSystem::last_write_time(path);
//#else
//            wtime = FileSystem::last_write_time(path);
//#endif
			bool isDir = FileSystem::is_directory(path);
			bool isFile = FileSystem::is_regular_file(path);
			int sz = 0;
			if (!isDir)sz = (int)FileSystem::file_size(path);
#ifdef JS_V8
			//st.type;
			v8::Isolate* pIso = v8::Isolate::GetCurrent();
			v8::Local<v8::Context> context = pIso->GetCurrentContext();
			//v8::HandleScope scope(pIso); 不用了，还得想办法escape
			v8::Local<v8::Object> retobj = v8::Object::New(pIso);
			retobj->Set(context, Js_Str(pIso, "isDirectory"), v8::Boolean::New(pIso, isDir));
			retobj->Set(context, Js_Str(pIso, "isFile"), v8::Boolean::New(pIso, isFile));
			retobj->Set(context, Js_Str(pIso, "size"), v8::Number::New(pIso, sz));
			retobj->Set(context, Js_Str(pIso, "mtime"), v8::Date::New(context, (double)(wtime*1000)).ToLocalChecked());
			return retobj;
#elif defined(JS_JSC)
            JSContextRef ctx = laya::__TlsData::GetInstance()->GetCurContext();
			JSObjectRef retobj = JSObjectMake(ctx, nullptr, nullptr);
			JSObjectSetProperty(ctx, retobj, JSStringCreateWithUTF8CString("isDirectory"), JSValueMakeBoolean(ctx, isDir), kJSPropertyAttributeNone, nullptr);
			JSObjectSetProperty(ctx, retobj, JSStringCreateWithUTF8CString("isFile"), JSValueMakeBoolean(ctx,isFile), kJSPropertyAttributeNone, nullptr);
			JSObjectSetProperty(ctx, retobj, JSStringCreateWithUTF8CString("size"), JSValueMakeNumber(ctx,sz), kJSPropertyAttributeNone, nullptr);
			JSObjectSetProperty(ctx, retobj, JSStringCreateWithUTF8CString("mtime"), laya::__TransferToJs<long>::ToJsDate(wtime*1000), kJSPropertyAttributeNone, nullptr);
			return retobj;
#endif
		//}
		//catch (...)
        //{
		//	JSP_THROW("lstatSync error!");
		//}
		return JSP_TO_JS_NULL;
	}
    bool JSFileSystem::JSWriteFileSync(const char* p_sUrl, JSValueAsParam args)
    {
        if (!p_sUrl) return false;
        char* pABPtr = NULL;
        int nABLen = 0;
        bool bisab = extractJSAB(args, pABPtr, nABLen);
        bool bret = false;
        if (bisab) 
        {
            if (pABPtr && nABLen > 0)
            {
                bret = writeFileSync1(p_sUrl, pABPtr, nABLen, 0);
            }
        }
        else 
        {
			Local value(args);
            if (value.isString())
            {
                std::string pData = Converter<std::string>::ToCpp(args);
                if (!pData.empty())
                {
                    int len = pData.length();
                    JCBuffer buf((char*)pData.c_str(), len, false, false);
                    bret = writeFileSync( p_sUrl, buf, JCBuffer::utf8);
                }
            }
        }
        return bret;
    }
	JsValue JSFileSystem::readBinFileSync(const char* p_pszFile) 
    {
        JCBuffer buf;
		if (readFileSync(p_pszFile, buf, JCBuffer::raw))
        {
			return laya::createJSAB(buf.m_pPtr, buf.m_nLen);
		}
		else 
        {
			return JSP_TO_JS_NULL;
		}
    }
};

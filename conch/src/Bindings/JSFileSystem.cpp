#include "JSFileSystem.h"
#include <binder/JSBind.h>
#include <utils/Log.h>
#include <utils/JCFileSystem.h>
#include <ctime>

namespace laya
{
	bool JSFileSystem::rmDir(const char* p_pszPath, jsvm::Value onprogress, jsvm::Value oncomplete, jsvm::Value onerror) 
    {
		return true;
	}
	bool JSFileSystem::rmDirSync(const char* p_pszPath) 
    {
		return true;
	}
	jsvm::Value JSFileSystem::readdirSync(const std::string &path) 
    {
		if (!FileSystem::exists(path))
			return jsbind::MakeNull();
		std::vector<std::string> paths = FileSystem::readdirSync(path);
		return jsbind::Make<std::vector<std::string> >(paths);
	}
	jsvm::Value JSFileSystem::lstatSync(const std::string &path) 
    {
		std::vector<std::string> paths;
		if (!FileSystem::exists(path))
			return jsbind::MakeNull();
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
#if 0
//#ifdef JS_V8
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
#endif
#if 0
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
		return jsbind::MakeNull();
	}
    bool JSFileSystem::JSWriteFileSync(const char* p_sUrl, jsbind::Local args)
    {
        if (!p_sUrl) return false;
      
        bool bisab = args.isArrayBuffer() || args.isArrayBufferView();
        bool bret = false;
        if (bisab) 
		{
			auto arrayBuffer = args.as<jsbind::ArrayBuffer>();
			char* pABPtr = reinterpret_cast<char*>(arrayBuffer.getData());
			int nABLen = arrayBuffer.getLength();
            if (pABPtr && nABLen > 0)
            {
                bret = writeFileSync1(p_sUrl, pABPtr, nABLen, 0);
            }
        }
        else 
        {
            if (args.isString())
            {
                std::string pData = args.as<std::string>();
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
	jsvm::Value JSFileSystem::readBinFileSync(const char* p_pszFile) 
    {
        JCBuffer buf;
		if (readFileSync(p_pszFile, buf, JCBuffer::raw))
        {
			return jsbind::ArrayBuffer::MakeArrayBuffer((uint8_t*)buf.m_pPtr, buf.m_nLen).getHandle();
		}
		else 
        {
			return jsbind::MakeNull();
		}
    }
};

#include "JSConsole.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>

namespace laya 
{
    void JSConsole::log(int p_nType,const char* p_sBuffer )
    {
        LogLevel logLevel = (LogLevel)p_nType;
    #ifdef OS_WINDOWS
	    if( p_sBuffer==NULL)
		    return;
	    int nLen = strlen( p_sBuffer ) + 3;
        if (nLen>3) {
            unsigned short* ucStr = new unsigned short[nLen];
            int nlen = UTF8StrToUnicodeStr((unsigned char*)p_sBuffer, ucStr, nLen);
            switch (logLevel)
            {
            case LogLevel::Warn:
                wprintf(L"warn:%s\n", (wchar_t *)ucStr);
                break;
            case LogLevel::Error:
                wprintf(L"error:%s\n", (wchar_t *)ucStr);
                break;
            default:
                wprintf(L"%s\n", (wchar_t *)ucStr);
                break;
            }
            delete[] ucStr;
            ucStr = NULL;
        }
    #elif OS_IOS
        switch (logLevel)
        {
            case LogLevel::Warn:
                LOGIExt(p_sBuffer);
                break;
            case LogLevel::Error:
                LOGIExt(p_sBuffer);
                break;
            default:
                LOGIExt(p_sBuffer);
                break;
        }
    #else
        switch (logLevel)
        {
        case LogLevel::Warn:
            LOGI(" %s", p_sBuffer);
            break;
        case LogLevel::Error:
            LOGI(" %s", p_sBuffer);
            break;
        default:
            LOGI(" %s", p_sBuffer);
            break;
        }
    #endif
    #ifdef JS_V8
	    if (gLayaLogNoParam) 
        {
            auto isolate = v8::Isolate::GetCurrent();
		    v8::HandleScope hs(isolate);
		    int flags = v8::StackTrace::kLineNumber | v8::StackTrace::kScriptNameOrSourceURL | v8::StackTrace::kFunctionName;
		    int wantcount = 1;
		    v8::Local<v8::StackTrace> curstack = v8::StackTrace::CurrentStackTrace(isolate, wantcount, (v8::StackTrace::StackTraceOptions)flags);
		    int count = curstack->GetFrameCount();
		    for (int i = 0; i < count; i++ ){
			    v8::Local<v8::StackFrame> curfrm = curstack->GetFrame(isolate, i);

			    v8::Local<v8::String> fname = curfrm->GetFunctionName();
			    //std::string fnamestr = *v8::String::Utf8Value(isolate, fname->ToString(isolate->GetCurrentContext()).ToLocalChecked());
			    int ln = curfrm->GetLineNumber();
			    v8::Local<v8::String> scname = curfrm->GetScriptName();
			    std::string srcfile;
			    if (!scname.IsEmpty()) {
				    srcfile = *v8::String::Utf8Value(isolate, scname->ToString(isolate->GetCurrentContext()).ToLocalChecked());
			    }
			    v8::Local<v8::String> srcurl = curfrm->GetScriptNameOrSourceURL();
			    if (!srcurl.IsEmpty()) {
				    srcfile = *v8::String::Utf8Value(isolate, srcurl->ToString(isolate->GetCurrentContext()).ToLocalChecked());
			    }
			    gLayaLogNoParam(static_cast<int>(LogLevel::Info), srcfile.c_str(), ln, p_sBuffer);
		    }
		    //gLayaLog(Info,
	    }
    #endif
    }
    void JSConsole::exportJS(Context& context)
    {
        class_<JSConsole> class_binding;
        class_binding.class_function("log", &JSConsole::log);
        context.class_("_console", class_binding);
    }
}

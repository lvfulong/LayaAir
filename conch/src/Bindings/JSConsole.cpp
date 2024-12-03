#include "JSConsole.h"
#include <jsbind/JSBind.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include <utils/JCCommonMethod.h>

namespace laya 
{
    void JSConsole::log(int p_nType,const char* p_sBuffer )
    {
        LogLevel logLevel = (LogLevel)p_nType;
    #if defined(OS_WINDOWS)
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
    #elif defined(OS_IOS)
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
    }
    void JSConsole::exportJS(jsbind::Object& context)
    {
        jsbind::global_class_<JSConsole> class_binding;
        class_binding.class_function("log", &JSConsole::log);
        context.global_class_("_console", class_binding);
    }
}

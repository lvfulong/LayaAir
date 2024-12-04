#include "JSConsole.h"
#include <jsbind/JSBind.h>
#include <utils/JCCommonMethod.h>
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>

namespace laya
{
void JSConsole::log(int p_nType, const char *p_sBuffer)
{
    LogType logType = (LogType)p_nType;
    
#if defined(OS_WINDOWS)
    if (p_sBuffer == NULL)
        return;
    int nLen = strlen(p_sBuffer) + 3;
    if (nLen > 3)
    {
        unsigned short *ucStr = new unsigned short[nLen];
        int nlen = UTF8StrToUnicodeStr((unsigned char *)p_sBuffer, ucStr, nLen);
        switch (logType)
        {
        case LogType::Warn:
            wprintf(L"warn:%s\n", (wchar_t *)ucStr);
            break;
        case LogType::Error:
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
    switch (logType)
    {
    case LogType::Warn:
        LOGIExt(p_sBuffer);
        break;
    case LogType::Error:
        LOGIExt(p_sBuffer);
        break;
    default:
        LOGIExt(p_sBuffer);
        break;
    }
#else
    switch (logType)
    {
    case LogType::Warn:
        LOGW(" %s", p_sBuffer);
        break;
    case LogType::Error:
        LOGE(" %s", p_sBuffer);
        break;
    case LogType::Debug:
        LOGD(" %s", p_sBuffer);
        break;
    case LogType::Info:
        LOGI(" %s", p_sBuffer);
        break;
    case LogType::Fatal:
        LOGF(" %s", p_sBuffer);
        break;
    default:
        LOGI(" %s", p_sBuffer);
        break;
    }
#endif
}
void JSConsole::exportJS(jsbind::Object &context)
{
    jsbind::global_class_<JSConsole> class_binding;
    class_binding.class_function("log", &JSConsole::log);
    context.global_class_("_console", class_binding);
}
} // namespace laya

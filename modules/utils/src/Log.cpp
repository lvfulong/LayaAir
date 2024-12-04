#include <cstdarg>
#include <iostream>
#include <utils/Log.h>
#include <vector>

int g_nDebugLevel = 3;

#if defined(OS_OHOS)
void logMessage(laya::LogType logType, const char *file, int line, const char *fmt, ...)
{
    va_list args0;
    va_list args1;

    va_start(args0, fmt);
    va_copy(args1, args0);
    ssize_t const s = vsnprintf(nullptr, 0, fmt, args0);
    va_end(args0);

    std::vector<char> message;
    message.resize(s + 1);
    vsnprintf(message.data(), s, fmt, args1);

    va_end(args1);

    switch (logType)
    {
    case laya::LogType::Warn:
        if (g_nDebugLevel >= 2)
        {
            OH_LOG_Print(LOG_APP, LOG_WARN, LOG_DOMAIN, LOG_TAG, "%{public}s", message.data());
        }
        if (g_nDebugLevel >= 5)
        {
            alert(message.data());
        }
        break;
    case laya::LogType::Error:
        if (g_nDebugLevel >= 1)
        {
            OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, LOG_TAG, "%{public}s", message.data());
        }
        if (g_nDebugLevel >= 4)
        {
            alert(message.data());
        }
        break;
    case laya::LogType::Debug:
        break;
    case laya::LogType::Info:
        if (g_nDebugLevel >= 3)
        {
            OH_LOG_Print(LOG_APP, LOG_INFO, LOG_DOMAIN, LOG_TAG, "%{public}s", message.data());
        }
        break;
    default:
        break;
    }

}
#endif
#include <cstdarg>
#include <iostream>
#include <utils/Log.h>
#include <vector>

int g_nLogLevel = static_cast<int>(laya::LogLevel::Debug);

#if defined(OS_OHOS)
void logMessage(laya::LogType logType, const char *file, int line, const char *fmt, ...)
{
    if (!fmt)
        return;

    va_list args0;
    va_list args1;

    va_start(args0, fmt);
    va_copy(args1, args0);
    ssize_t const s = vsnprintf(nullptr, 0, fmt, args0);
    va_end(args0);

    if (s < 0)
    {
        va_end(args1);
        return;
    }
    std::string message;
    message.resize(s);

    if (vsnprintf(&message[0], s + 1, fmt, args1) < 0)
    {
        va_end(args1);
        return;
    }

    va_end(args1);

    switch (logType)
    {
    case laya::LogType::Fatal:
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Fatal))
        {
            OH_LOG_Print(LOG_APP, LOG_FATAL, LOG_DOMAIN, LOG_TAG, "%{public}s", message.c_str());
        }
        break;
    case laya::LogType::Warn:
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Warn))
        {
            OH_LOG_Print(LOG_APP, LOG_WARN, LOG_DOMAIN, LOG_TAG, "%{public}s", message.c_str());
        }
        break;
    case laya::LogType::Error:
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Error))
        {
            OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, LOG_TAG, "%{public}s", message.c_str());
        }
        break;
    case laya::LogType::Debug:
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Debug))
        {
            OH_LOG_Print(LOG_APP, LOG_DEBUG, LOG_DOMAIN, LOG_TAG, "%{public}s", message.c_str());
        }
        break;
    case laya::LogType::Info:
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Info))
        {
            OH_LOG_Print(LOG_APP, LOG_INFO, LOG_DOMAIN, LOG_TAG, "%{public}s", message.c_str());
        }
        break;
    default:
        break;
    }
}
#endif
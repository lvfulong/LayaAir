#include <cstdarg>
#include <iostream>
#include <utils/Log.h>
#include <vector>

int g_nDebugLevel = 3;

void (*gLayaLog)(int level, const char *file, int line, const char *fmt, ...) = 0;
void (*gLayaLogNoParam)(int level, const char *file, int line, const char *msg) = 0;
void (*gLayaLogBin)(int level, const char *file, int line, void *pData, int len) = 0;

std::string vformat(const char *fmt, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    // 使用一个足够大的固定大小数组来尝试格式化字符串
    std::vector<char> buf(1024);
    int needed = vsnprintf(buf.data(), buf.size(), fmt, args_copy);
    va_end(args_copy);

    // 检查是否足够，并重新尝试
    if (needed < 0 || needed >= static_cast<int>(buf.size()))
    {
        buf.resize(needed + 1);
        vsnprintf(buf.data(), buf.size(), fmt, args);
    }

    return std::string(buf.data());
}
#if OHOS
void logMessage(laya::LogType logType, const char *file, int line, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string message = vformat(fmt, args);
    switch (logType)
    {
    case laya::LogType::Warn:
        if (g_nDebugLevel >= 2)
        {
            if (gLayaLog)
            {
                gLayaLog(static_cast<int>(laya::LogLevel::Warn), file, line, message.c_str());
            }
            else
            {
                OH_LOG_Print(LOG_APP, LOG_WARN, LOG_DOMAIN, LOG_TAG, "%{public}s", message.c_str());
            }
        }
        if (g_nDebugLevel >= 5)
        {
            alert(message.c_str());
        }
        break;
    case laya::LogType::Error:
        if (g_nDebugLevel >= 1)
        {
            if (gLayaLog)
            {
                gLayaLog(static_cast<int>(laya::LogLevel::Error), file, line, message.c_str());
            }
            else
            {
                OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, LOG_TAG, "%{public}s", message.c_str());
            }
        }
        if (g_nDebugLevel >= 4)
        {
            alert(message.c_str());
        }
        break;
    case laya::LogType::Debug:
        break;
    case laya::LogType::Info:
        if (g_nDebugLevel >= 3)
        {
            if (gLayaLog)
            {
                gLayaLog(static_cast<int>(laya::LogLevel::Info), file, line, message.c_str());
            }
            else
            {

                OH_LOG_Print(LOG_APP, LOG_INFO, LOG_DOMAIN, LOG_TAG, "%{public}s", message.c_str());
            }
        }
        break;
    default:
        break;
    }
    va_end(args);
}
#endif
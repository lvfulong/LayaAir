#ifndef __Log_H__
#define __Log_H__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)
#ifdef OS_WINDOWS
#pragma execution_character_set("utf-8")
#endif
extern int g_nLogLevel;

#define LOG_TAG "LayaBox"
#if defined(OS_ANDROID)
#include <android/log.h>
#include <jni.h>
#elif defined(OS_OHOS)
#include <hilog/log.h>
#endif
namespace laya
{
enum class LogType
{
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
};
enum class LogLevel
{
    Debug = 5,
    Info = 4,
    Warn = 3,
    Error = 2,
    Fatal = 1,
    Close = 0,
};
} // namespace laya
#if defined(OS_OHOS)
void logMessage(laya::LogType logType, const char *file, int line, const char *fmt, ...);
#endif

#define LOG_TAG "LayaBox"

#ifdef OS_IOS
void CToObjectCLogD(const char *szFormat, ...);
void CToObjectCLogI(const char *szFormat, ...);
void CToObjectCLogW(const char *szFormat, ...);
void CToObjectCLogE(const char *szFormat, ...);
void CToObjectCLogF(const char *szFormat, ...);
void CToObjectCLogIExt(const char *str);
#define LOGIExt(str)                                                                                                   \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Info))                                                     \
        {                                                                                                              \
            CToObjectCLogIExt(str);                                                                                    \
        }                                                                                                              \
    }
#define LOGD(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Debug))                                                    \
        {                                                                                                              \
            CToObjectCLogD(__VA_ARGS__);                                                                               \
        }                                                                                                              \
    }
#define LOGI(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Info))                                                     \
        {                                                                                                              \
            CToObjectCLogI(__VA_ARGS__);                                                                               \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Warn))                                                     \
        {                                                                                                              \
            CToObjectCLogW(__VA_ARGS__);                                                                               \
        }                                                                                                              \
    }
#define LOGE(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Error))                                                    \
        {                                                                                                              \
            CToObjectCLogE(__VA_ARGS__);                                                                               \
        }                                                                                                              \
    }
#define LOGF(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Fatal))                                                    \
        {                                                                                                              \
            CToObjectCLogF(__VA_ARGS__);                                                                               \
        }                                                                                                              \
    }
#elif OS_ANDROID
#define LOGD(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Debug))                                                    \
        {                                                                                                              \
            __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);                                              \
        }                                                                                                              \
    }
#define LOGI(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Info))                                                     \
        {                                                                                                              \
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);                                               \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Warn))                                                     \
        {                                                                                                              \
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);                                               \
        }                                                                                                              \
    }
#define LOGE(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Error))                                                    \
        {                                                                                                              \
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);                                              \
        }                                                                                                              \
        \                                                                                                              \
    }
#define LOGF(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Fatal))                                                    \
        {                                                                                                              \
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);                                              \
        }                                                                                                              \
        \                                                                                                              \
    }
#elif OS_OHOS
#define LOGD(...) logMessage(laya::LogType::Debug, __FILE__, __LINE__, __VA_ARGS__);
#define LOGI(...) logMessage(laya::LogType::Info, __FILE__, __LINE__, __VA_ARGS__);
#define LOGW(...) logMessage(laya::LogType::Warn, __FILE__, __LINE__, __VA_ARGS__);
#define LOGE(...) logMessage(laya::LogType::Error, __FILE__, __LINE__, __VA_ARGS__);
#define LOGF(...) logMessage(laya::LogType::Fatal, __FILE__, __LINE__, __VA_ARGS__);
#elif defined(OS_WINDOWS) || defined(OS_LINUX)
#define LOGD(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Debug))                                                    \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    }
#define LOGI(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Info))                                                     \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Warn))                                                     \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    }
#define LOGE(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Error))                                                    \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    }
#define LOGF(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nLogLevel >= static_cast<int>(laya::LogLevel::Fatal))                                                    \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    }
#endif

#define ABORT(msg)                                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        LOGF("%s:%d: fatal error: \"%s\"\n", __FILE__, __LINE__, #msg);                                                \
        ::abort();                                                                                                     \
    } while (false)

#define CHECK(assertion)                                                                                               \
    if (!(assertion))                                                                                                  \
    {                                                                                                                  \
        ABORT(#assertion);                                                                                             \
    }
#if _DEBUG

#define DEBUG_CHECK(assertion) CHECK(assertion)

#else

#define DEBUG_CHECK(assertion)

#endif
#endif //__Log_H__

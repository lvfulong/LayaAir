#ifndef __Log_H__
#define __Log_H__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)
#ifdef OS_WINDOWS
#pragma execution_character_set("utf-8")
#endif
extern int g_nDebugLevel;

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
    Warn,
    Error,
    Debug,
    Info,
};
// 通用的日志函数。
enum class LogLevel
{
    Warn,
    Error,
    Debug,
    Info,
    Runtime,
};
} // namespace laya

void alert(const char *fmt, ...);
#if defined(OS_OHOS)
void logMessage(laya::LogType logType, const char *file, int line, const char *fmt, ...);
#endif

#define LOG_TAG "LayaBox"

#ifdef OS_IOS
void CToObjectCLogI(const char *szFormat, ...);
void CToObjectCLogW(const char *szFormat, ...);
void CToObjectCLogE(const char *szFormat, ...);
void CToObjectCLogIExt(const char *str);
#define LOGIExt(str)                                                                                                   \
    {                                                                                                                  \
        if (g_nDebugLevel >= 3)                                                                                        \
        {                                                                                                              \
            CToObjectCLogIExt(str);                                                                                    \
        }                                                                                                              \
    }
#define LOGI(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 3)                                                                                        \
        {                                                                                                              \
            CToObjectCLogI(__VA_ARGS__);                                                                               \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 2)                                                                                        \
        {                                                                                                              \
            CToObjectCLogW(__VA_ARGS__);                                                                               \
        }                                                                                                              \
        if (g_nDebugLevel >= 5)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }
#define LOGE(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 1)                                                                                        \
        {                                                                                                              \
            CToObjectCLogE(__VA_ARGS__);                                                                               \
        }                                                                                                              \
        if (g_nDebugLevel >= 4)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }
#elif OS_LINUX
#define LOGI(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 3)                                                                                        \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 2)                                                                                        \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
        if (g_nDebugLevel >= 5)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }
#define LOGE(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 1)                                                                                        \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
        if (g_nDebugLevel >= 4)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }
#elif OS_ANDROID
#define LOGI(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 3)                                                                                        \
        {                                                                                                              \
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);                                               \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 2)                                                                                        \
        {                                                                                                              \
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);                                               \
        }                                                                                                              \
        if (g_nDebugLevel >= 5)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }
#define LOGE(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 1)                                                                                        \
        {                                                                                                              \
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);                                              \
        }                                                                                                              \
        if (g_nDebugLevel >= 4)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }
#elif OS_OHOS
#define LOGI(...) logMessage(laya::LogType::Info, __FILE__, __LINE__, __VA_ARGS__);
#define LOGW(...) logMessage(laya::LogType::Warn, __FILE__, __LINE__, __VA_ARGS__);
#define LOGE(...) logMessage(laya::LogType::Error, __FILE__, __LINE__, __VA_ARGS__);
#elif OS_WINDOWS
#define LOGI(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 3)                                                                                        \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 2)                                                                                        \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
        if (g_nDebugLevel >= 5)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }
#define LOGE(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 1)                                                                                        \
        {                                                                                                              \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
        if (g_nDebugLevel >= 4)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }

#endif

#define ABORT(msg)                                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        LOGE("%s:%d: fatal error: \"%s\"\n", __FILE__, __LINE__, #msg);                                                \
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
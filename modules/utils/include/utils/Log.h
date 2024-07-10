#ifndef __Log_H__
#define __Log_H__
#include <stdint.h>
#include <stdio.h>
#pragma warning(disable : 4996)
#ifdef OS_WINDOWS
#pragma execution_character_set("utf-8")
#endif
extern int g_nDebugLevel;

#define LOG_TAG "LayaBox"
#if OS_ANDROID
#include <android/log.h>
#include <jni.h>
#elif OS_OHOS
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
extern void (*gLayaLog)(int level, const char *file, int line, const char *fmt, ...);
// 如果知道没有参数就不用做%的转换了。
extern void (*gLayaLogNoParam)(int level, const char *file, int line, const char *msg);
extern void (*gLayaLogBin)(int level, const char *file, int line, void *pData, int len);

void alert(const char *fmt, ...);
#if OS_OHOS
void logMessage(laya::LogType logType, const char *file, int line, const char *fmt, ...);
#endif
#ifdef WEBASM
#define LOG_TAG "LayaBox"
#define LOGI(...)                                                                                                      \
    {                                                                                                                  \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n");                                                                                                  \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n");                                                                                                  \
    }
#define LOGE(...)                                                                                                      \
    {                                                                                                                  \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n");                                                                                                  \
    }
#else
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
            if (gLayaLog)                                                                                              \
            {                                                                                                          \
                gLayaLog(static_cast<int>(laya::LogLevel::Info), __FILE__, __LINE__, __VA_ARGS__);                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                printf(__VA_ARGS__);                                                                                   \
                printf("\n");                                                                                          \
            }                                                                                                          \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 2)                                                                                        \
        {                                                                                                              \
            if (gLayaLog)                                                                                              \
            {                                                                                                          \
                gLayaLog(static_cast<int>(laya::LogLevel::Warn), __FILE__, __LINE__, __VA_ARGS__);                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                printf(__VA_ARGS__);                                                                                   \
                printf("\n");                                                                                          \
            }                                                                                                          \
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
            if (gLayaLog)                                                                                              \
            {                                                                                                          \
                gLayaLog(static_cast<int>(laya::LogLevel::Error), __FILE__, __LINE__, __VA_ARGS__);                    \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                printf(__VA_ARGS__);                                                                                   \
                printf("\n");                                                                                          \
            }                                                                                                          \
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
            if (gLayaLog)                                                                                              \
            {                                                                                                          \
                gLayaLog(static_cast<int>(laya::LogLevel::Info), __FILE__, __LINE__, __VA_ARGS__);                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);                                           \
            }                                                                                                          \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 2)                                                                                        \
        {                                                                                                              \
            if (gLayaLog)                                                                                              \
            {                                                                                                          \
                gLayaLog(static_cast<int>(laya::LogLevel::Warn), __FILE__, __LINE__, __VA_ARGS__);                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);                                           \
            }                                                                                                          \
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
            if (gLayaLog)                                                                                              \
            {                                                                                                          \
                gLayaLog(static_cast<int>(laya::LogLevel::Error), __FILE__, __LINE__, __VA_ARGS__);                    \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);                                          \
            }                                                                                                          \
        }                                                                                                              \
        if (g_nDebugLevel >= 4)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }
#elif OS_OHOS
#define LOGI(...)   \                                                                                                   
    logMessage(laya::LogType::Info, __FILE__, __LINE__, __VA_ARGS__);                                                  
    #define LOGW(...)\
     logMessage(laya::LogType::Warn, __FILE__, __LINE__, __VA_ARGS__);                               
    #define LOGE(...)\ 
    logMessage(laya::LogType::Error, __FILE__, __LINE__, __VA_ARGS__);
#elif OS_WINDOWS
#define LOGI(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 3)                                                                                        \
        {                                                                                                              \
            if (gLayaLog)                                                                                              \
            {                                                                                                          \
                gLayaLog(static_cast<int>(laya::LogLevel::Info), __FILE__, __LINE__, __VA_ARGS__);                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                printf(__VA_ARGS__);                                                                                   \
                printf("\n");                                                                                          \
            }                                                                                                          \
        }                                                                                                              \
    }
#define LOGW(...)                                                                                                      \
    {                                                                                                                  \
        if (g_nDebugLevel >= 2)                                                                                        \
        {                                                                                                              \
            if (gLayaLog)                                                                                              \
            {                                                                                                          \
                gLayaLog(static_cast<int>(laya::LogLevel::Warn), __FILE__, __LINE__, __VA_ARGS__);                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                printf(__VA_ARGS__);                                                                                   \
                printf("\n");                                                                                          \
            }                                                                                                          \
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
            if (gLayaLog)                                                                                              \
            {                                                                                                          \
                gLayaLog(static_cast<int>(laya::LogLevel::Error), __FILE__, __LINE__, __VA_ARGS__);                    \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                printf(__VA_ARGS__);                                                                                   \
                printf("\n");                                                                                          \
            }                                                                                                          \
        }                                                                                                              \
        if (g_nDebugLevel >= 4)                                                                                        \
        {                                                                                                              \
            alert(__VA_ARGS__);                                                                                        \
        }                                                                                                              \
    }

#endif
#endif
#endif //__Log_H__
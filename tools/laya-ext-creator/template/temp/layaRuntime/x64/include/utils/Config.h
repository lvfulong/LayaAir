#ifndef __CONCH_CONFIG_H__
#define __CONCH_CONFIG_H__

#if defined(__ANDROID__)
#define CONCH_OS_ANDROID 1
#elif defined(__APPLE__)
#define CONCH_OS_IOS 1
#elif defined(__linux__)
#define CONCH_OS_LINUX 1
#elif defined(_WIN32)
#define CONCH_OS_WIN 1
#elif defined(__OHOS__)
#define CONCH_OS_OHOS 1
#endif

#if defined(BUILDING_CONCH_SHARED) && defined(USING_CONCH_SHARED)
#error both BUILDING_CONCH_SHARED and USING_CONCH_SHARED are set please check the build configuration to ensure that at most one of these is set
#endif

#if defined(CONCH_OS_WIN)
#ifdef BUILDING_CONCH_SHARED
#define CONCH_EXPORT __declspec(dllexport)
#elif USING_CONCH_SHARED
#define CONCH_EXPORT __declspec(dllimport)
#else
#define CONCH_EXPORT
#endif

#elif defined(CONCH_OS_LINUX) || defined(CONCH_OS_IOS) || defined(CONCH_OS_ANDROID) || defined(CONCH_OS_OHOS)
#ifdef BUILDING_CONCH_SHARED
#define CONCH_EXPORT __attribute__((visibility("default")))
#else
#define CONCH_EXPORT
#endif
#endif

#ifdef _MSC_VER
#define CONCH_CDECL __cdecl
#else
#define CONCH_CDECL
#endif

#ifdef __cplusplus
#define CONCH_C_START                                                                                                  \
    extern "C"                                                                                                         \
    {
#define CONCH_C_END }
#else
#define CONCH_C_START
#define CONCH_C_END
#endif

#endif
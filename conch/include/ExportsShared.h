#pragma once

#if defined(OS_WINDOWS)
#include <tchar.h>
#include <wtypes.h>
#define CONCH_API_EXTERN __declspec(dllexport)
#define CONCH_API_CDECL __cdecl
#elif defined(OS_LINUX)
#define CONCH_API_EXTERN
#define CONCH_API_CDECL
#endif

#ifdef __cplusplus
#define EXTERN_C_START                                                                                                 \
    extern "C"                                                                                                         \
    {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

typedef void(CONCH_API_CDECL *handleSyncMessageCallback)(const char *eventName, const char *data);
typedef void(CONCH_API_CDECL *handleAsyncMessageCallback)(const char *eventName, const char *data);

#if defined(OS_WINDOWS)
EXTERN_C_START
CONCH_API_EXTERN int CONCH_API_CDECL conchMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine,
                                               int nShowCmd);
CONCH_API_EXTERN void CONCH_API_CDECL conchSetHandleMessageCallback(handleSyncMessageCallback handleSyncMessageCb,
                                                                    handleAsyncMessageCallback handleAsyncMessageCb);
CONCH_API_EXTERN void CONCH_API_CDECL conchSendHandleMessageResult(const char *eventName, const char *result);
CONCH_API_EXTERN void CONCH_API_CDECL conchRunJS(const char *sourceCode);
EXTERN_C_END
#elif defined(OS_LINUX)
extern int conchMain(int argc, char *argv[]);
extern void conchSetHandleMessageCallback(handleSyncMessageCallback handleSyncMessageCb,
                                          handleAsyncMessageCallback handleAsyncMessageCb);
extern void conchSendHandleMessageResult(const char *eventName, const char *result);
extern void conchRunJS(const char *sourceCode);
#endif

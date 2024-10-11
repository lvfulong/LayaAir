#pragma once

#if defined(OS_WINDOWS)
#include <tchar.h>
#include <windows.h>
#include <wtypes.h>
#elif defined(OS_LINUX)
#endif

#if defined(OS_WINDOWS)
extern int conchMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);
#elif defined(OS_LINUX)
extern int conchMain(int argc, char *argv[]);
#endif
typedef void (*handleSyncMessageCallback)(const char *eventName, const char *data);
typedef void (*handleAsyncMessageCallback)(const char *eventName, const char *data);
extern void conchSetHandleMessageCallback(handleSyncMessageCallback handleSyncMessageCb,
                                          handleAsyncMessageCallback handleAsyncMessageCb);
extern void conchSendHandleMessageResult(const char *eventName, const char *result);
extern void conchRunJS(const char *sourceCode);
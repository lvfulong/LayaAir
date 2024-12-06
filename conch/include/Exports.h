#pragma once

#include <utils/Config.h>

#if defined(CONCH_OS_WIN)
#include <tchar.h>
#include <wtypes.h>
#endif
typedef void(CONCH_CDECL *handleSyncMessageCallback)(const char *eventName, const char *data);
typedef void(CONCH_CDECL *handleAsyncMessageCallback)(const char *eventName, const char *data);

CONCH_C_START
#if defined(CONCH_OS_WIN)
CONCH_EXPORT int CONCH_CDECL conchMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);
#elif defined(CONCH_OS_LINUX)
CONCH_EXPORT int CONCH_CDECL conchMain(int argc, char *argv[]);
#endif
CONCH_EXPORT void CONCH_CDECL conchSetHandleMessageCallback(handleSyncMessageCallback handleSyncMessageCb,
                                                            handleAsyncMessageCallback handleAsyncMessageCb);
CONCH_EXPORT void CONCH_CDECL conchSendHandleMessageResult(const char *eventName, const char *result);
CONCH_EXPORT void CONCH_CDECL conchRunJS(const char *sourceCode);
CONCH_C_END

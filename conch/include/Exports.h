#pragma once

#if defined(OS_WINDOWS)
#include <functional>
#include <string>
#include <tchar.h>
#include <windows.h>
#include <wtypes.h>
#elif defined(OS_LINUX)
#include <functional>
#include <string>
#endif

#if defined(OS_WINDOWS)
extern int conchMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);
#elif defined(OS_LINUX)
extern int conchMain(int argc, char *argv[]);
#endif

typedef std::function<void(const std::string &result)> handleResultCallback;
typedef std::function<std::string(const std::string &eventName, const std::string &data)> handleSyncMessageCallback;
typedef std::function<void(const std::string &eventName, const std::string &data, handleResultCallback resultCallback)> handleAsyncMessageCallback;
extern void conchSetHandleMessageCallback(handleSyncMessageCallback handleSyncMessageCb, handleAsyncMessageCallback handleAsyncMessageCb);
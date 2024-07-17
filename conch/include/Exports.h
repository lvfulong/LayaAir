#pragma once

#if OS_WINDOWS
#include <functional>
#include <string>
#include <tchar.h>
#include <windows.h>
#include <wtypes.h>
#elif OS_LINUX
#include <functional>
#include <string>
#endif

#if OS_WINDOWS
extern int conchMainConsole(int argc, _TCHAR *argv[]);
extern int conchMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);
#elif OS_LINUX
extern int conchMain(int argc, char *argv[]);
#endif

typedef std::function<void(const std::string &result)> handleResultCallback;
typedef std::function<std::string(const std::string &eventName, const std::string &data)> handleSyncMessageCallback;
typedef std::function<void(const std::string &eventName, const std::string &data, handleResultCallback resultCallback)> handleAsyncMessageCallback;
extern void conchSetHandleMessageCallback(handleSyncMessageCallback handleSyncMessageCb, handleAsyncMessageCallback handleAsyncMessageCb);
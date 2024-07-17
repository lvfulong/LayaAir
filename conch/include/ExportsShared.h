#pragma once

#if OS_WINDOWS
#include <functional>
#include <string>
#include <tchar.h>
#include <wtypes.h>
#if BUILDING_CONCH_SHARED
#define CONCH_API __declspec(dllexport)
#else
#define CONCH_API __declspec(dllimport)
#endif
#elif OS_LINUX
#include <functional>
#include <string>
#endif

typedef std::function<void(const std::string &result)> handleResultCallback;
typedef std::function<std::string(const std::string &eventName, const std::string &data)> handleSyncMessageCallback;
typedef std::function<void(const std::string &eventName, const std::string &data, handleResultCallback resultCallback)>
    handleAsyncMessageCallback;

#if OS_WINDOWS
extern "C" CONCH_API int conchMainConsole(int argc, WCHAR *argv[]);
extern "C" CONCH_API int conchMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);
extern "C" CONCH_API void conchSetHandleMessageCallback(handleSyncMessageCallback handleSyncMessageCb,
                                                        handleAsyncMessageCallback handleAsyncMessageCb);
#elif OS_LINUX
#include <functional>
#include <string>
extern int conchMain(int argc, char *argv[]);
extern void conchSetHandleMessageCallback(handleSyncMessageCallback handleSyncMessageCb,
                                          handleAsyncMessageCallback handleAsyncMessageCb);
#endif

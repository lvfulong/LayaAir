#pragma once

#if WIN32
#include <wtypes.h>
#include <tchar.h>
#if BUILDING_CONCH_SHARED
#define CONCH_API __declspec(dllexport)
#else
#define CONCH_API __declspec(dllimport)
#endif
extern "C" CONCH_API int conchMainConsole(int argc, _TCHAR * argv[]);
extern "C" CONCH_API int conchMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);
#elif __LINUX__
extern int conchMain(int argc, char *argv[]);
#endif
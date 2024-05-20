#pragma once

#if WIN32
#include <wtypes.h>
#include <tchar.h>
#include <windows.h>

extern int conchMainConsole(int argc, _TCHAR * argv[]);
extern int conchMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);
#elif __LINUX__
extern int conchMain(int argc, char *argv[]);
#endif
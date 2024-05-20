#pragma once

#if WIN32
#include <wtypes.h>
#include <tchar.h>
extern int conchMain(int argc, _TCHAR* argv[]);
#elif __LINUX__
extern int conchMain(int argc, char *argv[]);
#endif
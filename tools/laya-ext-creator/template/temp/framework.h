#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define DEBUG_CHECK(assertion)
#ifndef SIZE_MAX
// SIZE_MAX definition must match exactly with limits.h for modules support.
#ifdef _WIN64
#define SIZE_MAX 0xffffffffffffffffui64
#else
#define SIZE_MAX 0xffffffffui32
#endif
#endif


#define LAYAEXTAPI __declspec(dllexport)


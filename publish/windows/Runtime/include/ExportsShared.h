#pragma once

#if WIN32
#include <wtypes.h>
#include <tchar.h>
#if BUILDING_CONCH_SHARED
#define CONCH_API __declspec(dllexport)
#else
#define CONCH_API __declspec(dllimport)
#endif
/*
用法：
    conch6.exe [options] url
参数：
    -test
        只执行c的测试用例。这时候不再使用url
    -perf
        执行一个url进行统计
    -perf_count
        update的次数
    -opath    path
        指定效率测试的输出目录
    -debug=""
*/
extern "C" CONCH_API int conchMain(int argc, _TCHAR* argv[]);
#elif __LINUX__
//todo
#endif
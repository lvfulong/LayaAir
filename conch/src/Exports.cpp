#if BUILDING_CONCH_FROME_SOURCE
#include "Exports.h"
#else
#include "ExportsShared.h"
#endif
#include "JCConch.h"
#include "JCSystemConfig.h"
#include <Application/App.h>
#include <downloadCache/JCIosFileSource.h>
#include <filesystem>
#include <string>
#include <utils/JCCommonMethod.h>
#if WIN32
#include <windows.h>
#endif
extern std::string gRedistPath;
extern std::string gAssetRootPath;
namespace fs = std::filesystem;

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
#if WIN32
int conchMain(int argc, _TCHAR *argv[])
{
    bool bRunTest = false;
    char *pRunTestCase = NULL;
    // 解析参数
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            if (laya::g_kSystemConfig.m_strStartURL.length() <= 0)
            {
                laya::g_kSystemConfig.m_strStartURL = argv[i];
            }
            else
            {
                printf(" 参数错误！只能有一个启动url，否则就是参数，需要以‘-’开头!\n ");
                return 1;
            }
            if (memcmp(argv[i], "http", 4) != 0)
            {
                laya::g_kSystemConfig.m_strStartURL = "http://127.0.0.1:9988/";
                laya::g_kSystemConfig.m_strStartURL += argv[i];
            }
        }
        else
        {
            char *cargv = (char *)argv[i] + 1;
            if (memcmp(cargv, "test", 4) == 0)
            {
                bRunTest = true;
                if (strlen(cargv) > 4)
                {
                    pRunTestCase = cargv + 4;
                }
            }
            else if (memcmp(cargv, "perf", 4) == 0)
            {
                laya::g_kSystemConfig.m_bPerfStat = true;
            }
            else if (memcmp(cargv, "opath", 5) == 0)
            {
                laya::g_kSystemConfig.m_strPerfOut = cargv + 5;
            }
            else if (memcmp(cargv, "perf_count", 10) == 0)
            {
                laya::g_kSystemConfig.m_nPerf_UpdateNum = atoi(cargv + 10);
            }
            else if (memcmp(cargv, "debug_port", 10) == 0)
            {
                // Conch::s_nDebugPort = atoi(cargv + 10);
            }
            else
            {
                printf("Unknown param:%s\n", cargv);
            }
        }
    }
#elif __LINUX__
int conchMain(int argc, char *argv[])
{
#endif
    fs::path exePath = laya::getExePath();
    LOGE("start exe path %s", exePath.c_str());
    gRedistPath = exePath.remove_filename().string();
    gAssetRootPath = gRedistPath;
    laya::JCIosFileSource *pAssets = new laya::JCIosFileSource();
    pAssets->Init(gRedistPath.c_str());
    laya::JCConch::s_pAssetsFiles = pAssets;

    if (bRunTest)
    {
        // JSMemorySurvey::DelInstance();
        // svFileCache::delInstance();
        // delete pAssets;
        return 0;
    }
    // if (g_kSystemConfig.m_bPerfStat) {
    //     gRunStat.strTestID = g_kSystemConfig.m_strStartURL;
    // }
    laya::App app;
    Config config;
    config.title = "LayaNative3.0";
    app.run(config);
    // app.handleMessage();
    // app.exitApp();
    // delete pAssets;
    return 0;
}
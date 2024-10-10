#ifdef BUILDING_CONCH_FROME_SOURCE
#include "Exports.h"
#else
#include "ExportsShared.h"
#endif
#include "JCConch.h"
#include "JCSystemConfig.h"
#include <Application/App.h>
#include <downloadCache/JCIosFileSource.h>
#include <string>
#include <utils/JCCommonMethod.h>
#include <utils/JCFileSystem.h>

extern std::string gRedistPath;
extern std::string gAssetRootPath;

// conch6.exe [options] url
handleSyncMessageCallback g_handleSyncMessageCb;
handleAsyncMessageCallback g_handleAsyncMessageCb;

void conchSetHandleMessageCallback(handleSyncMessageCallback handleSyncMessageCb,
                                   handleAsyncMessageCallback handleAsyncMessageCb)
{
    g_handleSyncMessageCb = handleSyncMessageCb;
    g_handleAsyncMessageCb = handleAsyncMessageCb;
}
std::unordered_map<std::string, std::function<void(const char *)>> g_sendHandleMessageResultMap;
static std::mutex s_sendHandleMessageResultMapLock;
void conchSendHandleMessageResult(const char *eventName, const char *result)
{
    std::unique_lock<std::mutex> lock(s_sendHandleMessageResultMapLock);
    auto it = g_sendHandleMessageResultMap.find(eventName);
    if (it != g_sendHandleMessageResultMap.end())
    {
        it->second(result);
        g_sendHandleMessageResultMap.erase(it);
    }
    else
    {
        LOGE("conchSendHandleMessageResult find no event %s handler", eventName);
    }
}
void conchRegisterHandleMessageHandler(const char *eventName, std::function<void(const char *)> cb)
{
    std::unique_lock<std::mutex> lock(s_sendHandleMessageResultMapLock);
    auto it = g_sendHandleMessageResultMap.insert(std::make_pair(eventName, cb));
    if (!it.second)
    {
        LOGE("event with name %s already existed", eventName);
    }
}
int mainImpl()
{
    std::string exePath = laya::getExePath();
    std::string exeName = laya::removeFileExtension(laya::FileSystem::filename(exePath));
    gRedistPath = laya::FileSystem::remove_filename(exePath);
    gAssetRootPath = gRedistPath;
    laya::JCIosFileSource *pAssets = new laya::JCIosFileSource();
    pAssets->Init(gRedistPath.c_str());
    laya::JCConch::s_pAssetsFiles = pAssets;
    laya::App app;
    laya::App::Config config;
    config.title = exeName;

    app.run(config);
    return 0;
}
#if defined(OS_WINDOWS)
int conchMainConsole(int argc, WCHAR *argv[])
{
    bool bRunTest = false;
    char *pRunTestCase = NULL;
    // 解析参数
    /*for (int i = 1; i < argc; i++)
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
            char* cargv = (char*)argv[i] + 1;
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
    }*/
    return mainImpl();
}

int conchMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    int argc;
    LPWSTR *strCmdLineWide = CommandLineToArgvW(lpCmdLine, &argc);

    if (argc > 0)
    {
        std::string strCmdLineUtf8 = laya::wideToUtf8(strCmdLineWide[0]);
        if (strstr(strCmdLineUtf8.c_str(), "http"))
        {
            laya::g_kSystemConfig.m_strStartURL = strCmdLineUtf8;
        }
    }
    int ret = mainImpl();
    LocalFree(strCmdLineWide);
    return ret;
}
#elif defined(OS_LINUX)
int conchMain(int argc, char *argv[])
{
    return mainImpl();
}
#endif
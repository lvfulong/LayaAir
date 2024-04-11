#include "stdafx.h"
#include <windows.h>
#include <string>
#include "JCConch.h"
#include "JCSystemConfig.h"
#include <filesystem>
#include <app/App.h>
#include <downloadCache/JCIosFileSource.h>
#include <utils/JCCommonMethod.h>
extern std::string gRedistPath;
namespace  fs = std::filesystem;
/*
�÷���
    conch6.exe [options] url
������
    -test
        ִֻ��c�Ĳ�����������ʱ����ʹ��url
    -perf
        ִ��һ��url����ͳ��
    -perf_count
        update�Ĵ���
    -opath    path
        ָ��Ч�ʲ��Ե����Ŀ¼
    -debug=""
*/
std::string gAssetRootPath = "";
int main(int argc, _TCHAR* argv[])
{
    bool bRunTest = false;
    char* pRunTestCase = NULL;
    //��������
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
                printf(" ��������ֻ����һ������url��������ǲ�������Ҫ�ԡ�-����ͷ!\n ");
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
                if (strlen(cargv) > 4) {
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
                //Conch::s_nDebugPort = atoi(cargv + 10);
            }
            else 
            {
                printf("Unknown param:%s\n", cargv);
            }
        }
    }
    fs::path exePath = laya::getExePath();
    LOGE("start exe path %s", exePath.c_str());
    gRedistPath = exePath.remove_filename().string();
    gAssetRootPath = gRedistPath;
    laya::JCIosFileSource* pAssets = new laya::JCIosFileSource();
    pAssets->Init(gRedistPath.c_str());
    laya::JCConch::s_pAssetsFiles = pAssets;
    laya::g_kSystemConfig.loadConfigIniFile();
    if (bRunTest) 
    {
        //JSMemorySurvey::DelInstance();
        //svFileCache::delInstance();
        //delete pAssets;
        return 0;
    }
    //if (g_kSystemConfig.m_bPerfStat) {
    //    gRunStat.strTestID = g_kSystemConfig.m_strStartURL;
    //}
    laya::App app;
    Config config;
    config.title = "LayaNative3.0";
    app.run(config);
    //app.handleMessage();
    //app.exitApp();
    //delete pAssets;
    return 0;
}

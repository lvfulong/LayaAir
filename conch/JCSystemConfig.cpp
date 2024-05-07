#include "JCSystemConfig.h"
#include <utils/JCCommonMethod.h>
#include <utils/JCFileSystem.h>
#include <utils/Log.h>
#include <utils/IniFile.h>
#include <utils/JCBuffer.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "JCConch.h"
#include <utils/JCFileSource.h>

extern std::string gAssetRootPath;
extern std::string gRedistPath;
extern int g_nInnerWidth;
extern int g_nInnerHeight;

namespace laya
{
JCSystemConfig g_kSystemConfig;
JCSystemConfig::JCSystemConfig()
{
    reset();
}
bool JCSystemConfig::s_bIsPlug = true; // �����������ͬһ����Ҳ����ı䡣
bool JCSystemConfig::s_bLocalizable = false;
void JCSystemConfig::reset()
{
    m_bPerfStat = false;
    m_nFrameType = FT_FAST;
    m_nFrameThreshold = 2000;
    m_nSleepTime = 0;
    m_strStartURL = "";
    m_nPerf_UpdateNum = 500;
    m_jsonparamExt = "";
    s_bLocalizable = false;
    m_bShowInternalPerBar = false;
    m_bConchWebGL = true;
}


void JCSystemConfig::loadConfigIniFile()
{
    // ���������ļ����ÿ���
    std::string configpath = gAssetRootPath; 
    configpath += "/config.ini";
#if __APPLE__||__ANDROID__
    std::string content = JCConch::s_pAssetsFiles->readTextAsset("config.ini");
    JCBuffer buf((char*)content.c_str(), strlen(content.c_str()), false, false);
    std::string tempFilePath = gRedistPath + "appCache" +  std::string("/tmp_config.ini");
    writeFileSync(tempFilePath.c_str(), buf, JCBuffer::utf8); 
    configpath = tempFilePath;
#endif
    std::error_code error;
    if (!fs::exists(configpath, error))
    {
        LOGE("No config.ini file found!");
    }
    IniFile configIni(configpath.c_str());
#if WIN32 || __LINUX__
    int defaultWidth = 1280;
    if (configIni.hasEntry("common:width"))
    {
        g_nInnerWidth = configIni.getIntOrDefault("common:width", defaultWidth);
    }
    else
    {
        LOGW("Warning: can not find common:width use default %d", defaultWidth);
    }

    int defaultHeight = 720;
    if (configIni.hasEntry("common:height"))
    {
        g_nInnerHeight = configIni.getIntOrDefault("common:height", defaultHeight);
    }
    else
    {
        LOGW("Warning: can not find common:height use default %d", defaultHeight);
    }
#endif
#ifdef __APPLE__
    if (configIni.hasEntry("ios:orientation"))
    {
        m_nOrientationType = configIni.getUIntOrDefault("ios:orientation", 24);
    }
    else
    {
        LOGW("Warning: can not find ios:orientation use default %d", 24);
    }
#endif
    m_nJSDebugMode = configIni.getIntOrDefault("common:JSDebugMode", 0);
    m_nJSDebugPort = configIni.getIntOrDefault("common:JSDebugPort", 5959);

    m_bConchWebGL = configIni.getBoolOrDefault("common:ConchWebGL", true);
}
} // namespace laya

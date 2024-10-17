#include "JCSystemConfig.h"
#include <utils/IniFile.h>
#include <utils/JCBuffer.h>
#include <utils/JCCommonMethod.h>
#include <utils/JCFileSystem.h>
#include <utils/Log.h>
#ifdef OS_WINDOWS
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
static WindowMode stringToWindowMode(const std::string &mode)
{
    if (compareStrings(mode, "Window", false))
    {
        return WindowMode::WM_Window;
    }
    else if (compareStrings(mode, "WindowResizable", false))
    {
        return WindowMode::WM_WindowResizable;
    }
    else if (compareStrings(mode, "FullScreen", false))
    {
        return WindowMode::WM_FullScreen;
    }
    else
    {
        return WindowMode::WM_Window;
    }
}
JCSystemConfig g_kSystemConfig;
JCSystemConfig::JCSystemConfig()
{
    reset();
}
void JCSystemConfig::reset()
{
    m_nFrameType = FT_FAST;
    m_nFrameThreshold = 2000;
    m_nSleepTime = 0;
    m_strStartURL = "";
}

void JCSystemConfig::loadConfigIniFile()
{
    // ���������ļ����ÿ���
    std::string configpath = gAssetRootPath;
    configpath += "config.ini";
#if defined(OS_IOS) || defined(OS_ANDROID) || defined(OS_OHOS)
    std::string content = JCConch::s_pAssetsFiles->readTextAsset("config.ini");
    JCBuffer buf((char *)content.c_str(), strlen(content.c_str()), false, false);
    std::string tempFilePath = gRedistPath + "appCache" + std::string("/tmp_config.ini");
    writeFileSync(tempFilePath.c_str(), buf, JCBuffer::utf8);
    configpath = tempFilePath;
#endif
    if (!FileSystem::exists(configpath))
    {
        LOGE("No config.ini file found!");
    }
    IniFile configIni(configpath.c_str());

#if defined(OS_WINDOWS) || defined(OS_LINUX)
    int defaultWidth = 1280;
    if (configIni.hasEntry("desktop:width"))
    {
        g_nInnerWidth = configIni.getIntOrDefault("desktop:width", defaultWidth);
    }
    else
    {
        LOGW("Warning: can not find desktop:width use default %d", defaultWidth);
    }

    int defaultHeight = 720;
    if (configIni.hasEntry("desktop:height"))
    {
        g_nInnerHeight = configIni.getIntOrDefault("desktop:height", defaultHeight);
    }
    else
    {
        LOGW("Warning: can not find desktop:height use default %d", defaultHeight);
    }

    std::string defaultWindowMode = "";
    if (configIni.hasEntry("desktop:WindowMode"))
    {
        defaultWindowMode = configIni.getStringOrDefault("desktop:WindowMode", defaultWindowMode);
        m_windowMode = stringToWindowMode(defaultWindowMode);
    }
    else
    {
        LOGW("Warning: can not find desktop:WindowMode use default %s", defaultWindowMode.c_str());
    }

    bool showDebugConsole = false;
    if (configIni.hasEntry("desktop:ShowDebugConsole"))
    {
        m_showDebugConsole = configIni.getBoolOrDefault("desktop:ShowDebugConsole", showDebugConsole);
    }
    else
    {
        LOGW("Warning: can not find desktop:ShowDebugConsole use default %d", showDebugConsole);
    }

#endif
#ifdef OS_IOS
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

    m_graphicsAPI = toGraphicsAPI(configIni.getStringOrDefault("common:GraphicsAPI", "OpenGLES"));
}
} // namespace laya

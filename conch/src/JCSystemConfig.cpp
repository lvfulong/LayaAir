#include "JCSystemConfig.h"
#include <utils/IniFile.h>
#include <utils/JCBuffer.h>
#include <utils/JCCommonMethod.h>
#include <utils/JCFileSystem.h>
#include <utils/Log.h>
#include <platform/OS.h>
#ifdef OS_WINDOWS
#include <windows.h>
#endif
#include "JCConch.h"
#include <utils/JCFileSource.h>
#if defined(USE_SWAPPY)
#include <swappy/swappyGL.h>
#endif

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
std::string toString(GraphicsAPI api)
{
    switch (api)
    {
    case GraphicsAPI::Invalid:
        return "Invalid";
        break;
    case GraphicsAPI::OpenGLES:
        return "OpenGLES";
        break;
    case GraphicsAPI::WebGL:
        return "WebGL";
        break;
    default:
        return "Invalid";
        break;
    }

}
GraphicsAPI toGraphicsAPI(const std::string &str)
{
    if (compareStrings(str, "OpenGLES", false))
    {
        return GraphicsAPI::OpenGLES;
    }
    else if (compareStrings(str, "WebGL", false))
    {
        return GraphicsAPI::WebGL;
    }
    else
    {
        return GraphicsAPI::Invalid;
    }
}
JCSystemConfig g_kSystemConfig;
JCSystemConfig::JCSystemConfig()
{
    reset();
}
void JCSystemConfig::reset()
{
    m_strStartURL = "";
}
#if defined(USE_SWAPPY)
bool JCSystemConfig::isSwappyEnabled()
{
    return m_useSwappy && SwappyGL_isEnabled(); 
}
#endif
void JCSystemConfig::loadConfigIniFile()
{
    // ���������ļ����ÿ���
    std::string configPath = OS::getAssetFullPath("config.ini");

    if (!FileSystem::exists(configPath))
    {
        LOGE("No config.ini file found!");
    }
    IniFile configIni(configPath.c_str());

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

    bool runInBackground = false;
    if (configIni.hasEntry("desktop:RunInBackground"))
    {
        m_runInBackground = configIni.getBoolOrDefault("desktop:RunInBackground", runInBackground);
    }
    else
    {
        LOGW("Warning: can not find desktop:RunInBackground use default %d", runInBackground);
    }
#endif
#if defined(OS_ANDROID)
    m_useSwappy = true;
    if (configIni.hasEntry("android:UseSwappy"))
    {
        m_useSwappy = configIni.getBoolOrDefault("android:UseSwappy", true);
    }
    else
    {
        LOGW("Warning: can not find android:UseSwappy use default %d", true);
    }

#endif
    m_nJSDebugMode = configIni.getIntOrDefault("common:JSDebugMode", 0);
    m_nJSDebugPort = configIni.getIntOrDefault("common:JSDebugPort", 5959);

    m_graphicsAPI = toGraphicsAPI(configIni.getStringOrDefault("common:GraphicsAPI", "OpenGLES"));
}
} // namespace laya

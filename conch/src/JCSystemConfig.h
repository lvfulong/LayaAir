
#ifndef _LAYA_CONFIG_H__
#define _LAYA_CONFIG_H__

#include <string>
#include <utils/Types.h>

namespace laya
{

enum FRAME_TYPE
{
    FT_SLOW = 0,
    FT_FAST,
    FT_MOUSE,
};
enum class WindowMode
{
    WM_Window,
    WM_WindowResizable,
    WM_FullScreen
};
class JCSystemConfig
{
  public:
    JCSystemConfig();
    void reset();

  public:
    std::string m_strStartURL;
    FRAME_TYPE m_nFrameType;
    double m_nFrameThreshold;
    int m_nSleepTime;
    std::string m_strPerfOut;
    static bool s_bIsPlug;
    GraphicsAPI m_graphicsAPI = GraphicsAPI::OpenGLES;
    int m_nOrientationType = 24;
    void loadConfigIniFile();
    int m_nJSDebugMode = 0;
    int m_nJSDebugPort = 5959;
    WindowMode m_windowMode{WindowMode::WM_WindowResizable};
    bool m_showDebugConsole = false;
};
extern JCSystemConfig g_kSystemConfig;
} // namespace laya

#endif

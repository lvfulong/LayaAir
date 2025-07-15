
#ifndef _LAYA_CONFIG_H__
#define _LAYA_CONFIG_H__

#include <string>

namespace laya
{

enum class WindowMode
{
    WM_Window,
    WM_WindowResizable,
    WM_FullScreen
};
enum class GraphicsAPI
{
    Invalid = 0,
    OpenGLES = 1,
    WebGL = 2,
};
std::string toString(GraphicsAPI api);
GraphicsAPI toGraphicsAPI(const std::string &str);
class JCSystemConfig
{
  public:
    JCSystemConfig();
    void reset();
#if defined(USE_SWAPPY)
    bool isSwappyEnabled();
#endif
  public:
    std::string m_strStartURL;
    std::string m_strPerfOut;
    static bool s_bIsPlug;
    GraphicsAPI m_graphicsAPI = GraphicsAPI::OpenGLES;
    int m_nOrientationType = 24;
    void loadConfigIniFile();
    int m_nJSDebugMode = 0;
    int m_nJSDebugPort = 5959;
    WindowMode m_windowMode{WindowMode::WM_WindowResizable};
    bool m_showDebugConsole = false;
    uint64_t m_frameIntervalInMs = (uint64_t)(1000.f / 60);
    bool m_useSwappy = true;
    bool m_runInBackground = false;
    int m_audioStreamThreshold = 1024 * 1024; // 1MB
    float m_maxPumpMessageLoopTime = 0.016f;//(单位： 秒)用于设置一帧内V8的PumpMessageLoop的执行时间
};
extern JCSystemConfig g_kSystemConfig;
} // namespace laya

#endif

#include "JCConch.h"
#include <SDL2/SDL_syswm.h>
#include <app/App.h>
#include <assert.h>
#include <utils/IniFile.h>
#include <filesystem>
#include "JCSystemConfig.h"
namespace  fs = std::filesystem;
extern std::string gRedistPath;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
#ifdef WIN32
#include <Windows.h>
HWND g_hWnd;
#elif LINUX
#include <SDL2/SDL_syswm.h>
#include <X11/Xlib.h>
Display *g_X11_display;
Window g_X11_window;
#endif

namespace laya
{
App::App()
{
    SDL_Init(SDL_INIT_EVENTS);
    loadConfigIniFile();
}
App::~App()
{
    SDL_DestroyWindow(m_sdlWindow);
    SDL_Quit();
}
#if  LINUX
std::string getExePath()
{
    char buf[256];
	memset(buf, 0, 256);
	ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf));
	if (len <= 0) {
		printf("getExePath failed");
        return "";
	}
	std::string ret(buf);
	return ret;
}
#endif
void App::loadConfigIniFile()
{
    fs::path exePath;
#ifdef WIN32
    TCHAR szPath[MAX_PATH];
    ::GetModuleFileName(NULL, szPath, MAX_PATH);
    exePath = szPath;
#elif  LINUX
    exePath = getExePath();
#endif
    gRedistPath = exePath.remove_filename().string();
    LOGE("start exe path %s", gRedistPath.c_str());
    // ���������ļ����ÿ���
    fs::path configpath(szPath);
    configpath.remove_filename();
    configpath /= "config.ini";
    if (!fs::exists(configpath))
    {
        LOGE("No config.ini file found!");
    }
    IniFile configIni(configpath.string().c_str());

#ifdef WIN32 || LINUX
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
        g_kSystemConfig.m_nOrientation = configIni.getUIntOrDefault("ios:orientation", 24);
    }
    else
    {
        LOGW("Warning: can not find ios:orientation use default %d", 24);
    }
#endif
    m_nJSDebugMode = configIni.getIntOrDefault("common:JSDebugMode", 0);
    m_nJSDebugPort = configIni.getIntOrDefault("common:JSDebugPort", 5959);


    laya::g_kSystemConfig.m_bConchWebGL = configIni.getBoolOrDefault("common:ConchWebGL", true);

}
void App::run(const Config &config)
{
    const int x = SDL_WINDOWPOS_CENTERED;
    const int y = SDL_WINDOWPOS_CENTERED;
    uint32_t windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;
    if (config.resizeable)
    {
        windowFlags |= SDL_WINDOW_RESIZABLE;
    }

    if (config.headless)
    {
        windowFlags |= SDL_WINDOW_HIDDEN;
    }
    m_sdlWindow = SDL_CreateWindow(config.title.c_str(), x, y, (int)g_nInnerWidth, (int)g_nInnerHeight, windowFlags);

    SDL_SysWMinfo sys;
    SDL_VERSION(&sys.version);
    if (SDL_FALSE != SDL_GetWindowWMInfo(m_sdlWindow, &sys))
    {
#ifdef WIN32
        g_hWnd = sys.info.win.window;
        // HINSTANCE hInstance = sys.info.win.hinstance;
#elif LINUX
        g_X11_display = sys.info.x11.display;
        g_X11_window = sys.info.x11.window;
#endif
    }
    else
    {
        assert(true && "get hWnd failed");
    }

    laya::BackendOptions options;
#ifdef WIN32
    options.nativeLayer = g_hWnd;
#elif LINUX
    options.nativeLayer = m_sdlWindow;
#endif
    laya::JCConch::s_pConch.reset(new laya::JCConch((laya::JS_DEBUG_MODE)m_nJSDebugMode, m_nJSDebugPort));
    laya::JCConch::s_pConchRender->createBackend(options);
    laya::JCConch::s_pConchRender->createScreenSurface(options.nativeLayer);
    laya::JCConch::s_pConch->onAppStart();

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

    SDL_Event event;
    while (!m_closed)
    {

        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                m_closed = true;
                break;
            case SDL_KEYDOWN: {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    m_closed = true;
                }
                inputEvent e;
                e.nTouchType = e.nType = E_ONKEYDOWN;
                strncpy(e.type, "keydown", 256);
                e.keyCode = event.key.keysym.scancode;

                JCConch::s_pConch->dispatchInputEvent(e);
            }
            break;
            case SDL_KEYUP: {
                inputEvent e;
                e.nTouchType = e.nType = E_ONKEYUP;
                strncpy(e.type, "keyup", 256);
                e.keyCode = event.key.keysym.scancode;

                JCConch::s_pConch->dispatchInputEvent(e);
            }
            break;
            case SDL_MOUSEWHEEL: {
                inputEvent e;
                e.nTouchType = e.nType = E_ONMOUSEWHEEL;
                strncpy(e.type, "mousewheel", 256);
                e.posX = event.button.x;
                e.posY = event.button.y;
                e.nWheel = event.wheel.y;

                JCConch::s_pConch->dispatchInputEvent(e);
                break;
            }

            case SDL_MOUSEBUTTONDOWN: {
                inputEvent e;
                if (SDL_BUTTON_LEFT == event.button.button)
                {
                    e.nTouchType = e.nType = E_ONMOUSEDOWN;
                    e.posX = event.button.x;
                    e.posY = event.button.y;
                    strncpy(e.type, "mousedown", 256);

                    JCConch::s_pConch->dispatchInputEvent(e);
                }
                else if (SDL_BUTTON_RIGHT == event.button.button)
                {
                    inputEvent e;
                    e.nTouchType = e.nType = E_ONRIGHTMOUSEDOWN;
                    e.posX = event.button.x;
                    e.posY = event.button.y;
                    strncpy(e.type, "rightmousedown", 256);

                    JCConch::s_pConch->dispatchInputEvent(e);
                }

                break;
            }
            case SDL_MOUSEBUTTONUP: {
                inputEvent e;
                if (SDL_BUTTON_LEFT == event.button.button)
                {
                    inputEvent e;
                    e.nTouchType = e.nType = E_ONMOUSEUP;
                    e.posX = event.button.x;
                    e.posY = event.button.y;
                    strncpy(e.type, "mouseup", 256);

                    JCConch::s_pConch->dispatchInputEvent(e);
                }
                else if (SDL_BUTTON_RIGHT == event.button.button)
                {
                    inputEvent e;
                    e.nTouchType = e.nType = E_ONRIGHTMOUSEUP;
                    e.posX = event.button.x;
                    e.posY = event.button.y;
                    strncpy(e.type, "rightmouseup", 256);

                    JCConch::s_pConch->dispatchInputEvent(e);
                }

                break;
            }
            case SDL_MOUSEMOTION: {
                inputEvent e;
                e.nTouchType = e.nType = E_ONMOUSEMOVE;
                strncpy(e.type, "mousemove", 256);
                e.posX = event.motion.x;
                e.posY = event.motion.y;

                JCConch::s_pConch->dispatchInputEvent(e);
                break;
            }
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    g_nInnerWidth = event.window.data1;
                    g_nInnerHeight = event.window.data2;
                    g_bGLCanvasSizeChanged = true;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        laya::JCConch::s_pConch->update();
    }
}
} // namespace laya
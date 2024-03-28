#include <app/App.h>
#include "JCConch.h"
#include <assert.h>
#include <SDL2/SDL_syswm.h>
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
#ifdef WIN32
#include <Windows.h>
HWND g_hWnd;
#elif LINUX
#include <X11/Xlib.h>
#include <SDL2/SDL_syswm.h>
Display *g_X11_display;
Window g_X11_window;
#endif

namespace laya
{
App::App()
{
    SDL_Init(SDL_INIT_EVENTS);
}
App::~App()
{
    SDL_DestroyWindow(m_sdlWindow);
    SDL_Quit();
}
void App::run(const Config &config, size_t width, size_t height, int nJSDebugMode, int nJSDebugPort)
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
    g_nInnerWidth = width;
    g_nInnerHeight = height;
    m_sdlWindow = SDL_CreateWindow(config.title.c_str(), x, y, (int)width, (int)height, windowFlags);

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
    laya::JCConch::s_pConch.reset(new laya::JCConch((laya::JS_DEBUG_MODE)nJSDebugMode, nJSDebugPort));
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
                    e.posX = x;
                    e.posY = y;
                    strncpy(e.type, "mouseup", 256);

                    JCConch::s_pConch->dispatchInputEvent(e);
                }
                else if (SDL_BUTTON_RIGHT == event.button.button)
                {
                    inputEvent e;
                    e.nTouchType = e.nType = E_ONRIGHTMOUSEUP;
                    e.posX = x;
                    e.posY = y;
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
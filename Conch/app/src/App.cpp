#include "JCConch.h"
#include <app/App.h>
#ifdef WIN32
#include <SDL2/SDL_syswm.h>
#include <Windows.h>
#endif
#include <assert.h>
extern int g_nInnerWidth;
extern int g_nInnerHeight;
#ifdef WIN32
HWND g_hWnd;
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
#ifdef WIN32
    SDL_SysWMinfo sys;
    SDL_VERSION(&sys.version);
    if (SDL_FALSE != SDL_GetWindowWMInfo(m_sdlWindow, &sys))
    {
        g_hWnd = sys.info.win.window;
        // HINSTANCE hInstance = sys.info.win.hinstance;
    }
    else
    {
        assert(true && "get hWnd failed");
    }
#endif

    laya::BackendOptions options;
#ifdef WIN32
    options.nativeLayer = g_hWnd;
#else LINUX
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
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    m_closed = true;
                }
                // window->keyDown(event.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                // window->keyUp(event.key.keysym.scancode);
                break;
            case SDL_MOUSEWHEEL:
                // if (!io || !io->WantCaptureMouse)
                //     window->mouseWheel(event.wheel.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                // if (!io || !io->WantCaptureMouse)
                //     window->mouseDown(event.button.button, event.button.x, event.button.y);
                break;
            case SDL_MOUSEBUTTONUP:
                // if (!io || !io->WantCaptureMouse)
                //     window->mouseUp(event.button.x, event.button.y);
                break;
            case SDL_MOUSEMOTION:
                // if (!io || !io->WantCaptureMouse)
                //     window->mouseMoved(event.motion.x, event.motion.y);
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    // window->resize();
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
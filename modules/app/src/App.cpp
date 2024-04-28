#include "JCConch.h"
#include <SDL2/SDL_syswm.h>
#include <app/App.h>
#include <assert.h>
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

#ifdef WIN32
    HHOOK hKeyboardHook = NULL;

    LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        if (nCode == HC_ACTION)
        {
            KBDLLHOOKSTRUCT* pKeyboardHook = (KBDLLHOOKSTRUCT*)lParam;

            bool isAltPressed = (pKeyboardHook->flags & LLKHF_ALTDOWN) != 0;
            bool isCtrlPressed = GetAsyncKeyState(VK_CONTROL);
            bool isShiftPressed = GetAsyncKeyState(VK_SHIFT);
            //printf("%d,%d,%d\n", isAltPressed, isCtrlPressed, isShiftPressed);
            if (wParam == WM_KEYDOWN)
            {
                inputEvent e;
                e.nTouchType = e.nType = E_ONKEYDOWN;
                strncpy(e.type, "keydown", 256);
                e.keyCode = pKeyboardHook->vkCode;

                e.bCtrl = isCtrlPressed;
                e.bShift = isShiftPressed;
                e.bAlt = isAltPressed;

                JCConch::s_pConch->dispatchInputEvent(e);
            }
            else if (wParam == WM_KEYUP) {
                inputEvent e;
                e.nTouchType = e.nType = E_ONKEYUP;
                strncpy(e.type, "keyup", 256);
                e.keyCode = pKeyboardHook->vkCode;

                e.bCtrl = isCtrlPressed;
                e.bShift = isShiftPressed;
                e.bAlt = isAltPressed;

                JCConch::s_pConch->dispatchInputEvent(e);
            }
        }

        return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
    }
#elif LINUX
#endif // WIN32

App::App()
{
    SDL_Init(SDL_INIT_EVENTS);
}
App::~App()
{
    SDL_DestroyWindow(m_sdlWindow);
    SDL_Quit();
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
        hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, sys.info.win.hinstance, 0);

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
    laya::JCConch::s_pConch.reset(new laya::JCConch());
    laya::JCConch::s_pConchRender->createBackend(options);
    laya::JCConch::s_pConchRender->createScreenSurface(options.nativeLayer);
    laya::JCConch::s_pConch->onAppStart();

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    // 启用SDL_SYSWMEVENT
    //SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    //SDL_StopTextInput();

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
                break;
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
            case SDL_SYSWMEVENT:{
                break;
                SDL_SysWMmsg* sysMsg = event.syswm.msg;
                switch (sysMsg->subsystem){
                case SDL_SYSWM_WINDOWS: {
                    UINT msg = sysMsg->msg.win.msg;
                    WPARAM wParam = sysMsg->msg.win.wParam;
                    LPARAM lParam = sysMsg->msg.win.lParam;
                    switch (msg) {
                    case WM_KEYDOWN: {
                        inputEvent e;
                        e.nTouchType = e.nType = E_ONKEYDOWN;
                        strncpy(e.type, "keydown", 256);
                        UINT keyCode = wParam;
                        e.keyCode = keyCode;

                        e.bCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
                        e.bShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
                        e.bAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;

                        JCConch::s_pConch->dispatchInputEvent(e);

                    }
                        break;
                    case WM_KEYUP: {
                        inputEvent e;
                        e.nTouchType = e.nType = E_ONKEYUP;
                        strncpy(e.type, "keydown", 256);
                        e.keyCode = event.key.keysym.scancode;

                        JCConch::s_pConch->dispatchInputEvent(e);

                    }
                        break;
                    }
                    if (msg == WM_KEYDOWN) {
                        int a = 10;
                    }
                    break;
                }
                case SDL_SYSWM_X11: {
                    break;
                }
                    
                }

            }
            break;
            /*
            case SDL_TEXTEDITING: {
                inputEvent e;
                auto aa = event.text.text;
                printf("Editing text: %s\n", event.edit.text);
                printf("Cursor position: %d\n", event.edit.start);
                printf("Selection length: %d\n", event.edit.length);
                int a = 1;
            }
                break;
            case SDL_TEXTINPUT: {
                inputEvent e;
                auto aa = event.text.text;
                printf("SDL_TEXTINPUT text: %s\n", event.edit.text);


                int a = 1;
            }
                break;
            case SDL_TEXTEDITING_EXT: {
                inputEvent e;
            }
                break;
*/
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

#ifdef WIN32
    // 卸载键盘钩子
    UnhookWindowsHookEx(hKeyboardHook);
#elif LINUX
#endif
}
} // namespace laya
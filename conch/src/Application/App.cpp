#include "JCConch.h"
#include "JCSystemConfig.h"
#include <Application/App.h>
#include <SDL2/SDL_syswm.h>
#include <assert.h>
extern std::string gRedistPath;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
#if defined(OS_WINDOWS)
#include <Windows.h>
#include <io.h>
#include <objidlbase.h>
#include <gdiplus.h>
extern HWND g_hWnd;
#elif defined(OS_LINUX)
#include <SDL2/SDL_syswm.h>
#include <X11/Xlib.h>
Display *g_X11_display;
Window g_X11_window;
#endif

namespace laya
{
#if defined(OS_WINDOWS)
static void createAndAttachConsole()
{
    if (!::AttachConsole(ATTACH_PARENT_PROCESS) && (::IsDebuggerPresent() || laya::g_kSystemConfig.m_showDebugConsole))
    {
        if (::AllocConsole())
        {
            FILE *unused;
            if (freopen_s(&unused, "CONOUT$", "w", stdout))
            {
                _dup2(_fileno(stdout), 1);
            }
            if (freopen_s(&unused, "CONOUT$", "w", stderr))
            {
                _dup2(_fileno(stdout), 2);
            }
            std::ios::sync_with_stdio();
        }
    }
}
#endif
App::App()
{
    SDL_Init(SDL_INIT_EVENTS);
}
App::~App()
{
    SDL_DestroyWindow(m_sdlWindow);
    SDL_Quit();
}

std::array<int, 512> keycodeMap = {
    0,   0,   0,   0,   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
};

void App::run(const Config &config)
{
    keycodeMap[SDL_SCANCODE_RETURN] = 0x0d;       // VK_RETURN;
    keycodeMap[SDL_SCANCODE_ESCAPE] = 0x1b;       // VK_ESCAPE;
    keycodeMap[SDL_SCANCODE_BACKSPACE] = 0x08;    // VK_BACK;
    keycodeMap[SDL_SCANCODE_TAB] = 0x09;          // VK_TAB;
    keycodeMap[SDL_SCANCODE_SPACE] = 0x20;        // VK_SPACE;
    keycodeMap[SDL_SCANCODE_MINUS] = 0xBD;        // VK_OEM_MINUS;
    keycodeMap[SDL_SCANCODE_MINUS] = 0x92;        // VK_OEM_NEC_EQUAL
    keycodeMap[SDL_SCANCODE_LEFTBRACKET] = 0xDB;  // VK_OEM_4;//{
    keycodeMap[SDL_SCANCODE_RIGHTBRACKET] = 0xDD; // VK_OEM_6//}
    keycodeMap[SDL_SCANCODE_BACKSLASH] = 0xBF;    // VK_OEM_2///  反斜杠
    keycodeMap[SDL_SCANCODE_SEMICOLON] = 0xBA;    // VK_OEM_1
    keycodeMap[SDL_SCANCODE_COMMA] = 0xBC;        // VK_OEM_COMMA
    keycodeMap[SDL_SCANCODE_PERIOD] = 0xBE;       // VK_OEM_PERIOD
    keycodeMap[SDL_SCANCODE_SLASH] = 0xDC;        // VK_OEM_5
    keycodeMap[SDL_SCANCODE_CAPSLOCK] = 0x14;     // VK_CAPITAL
    keycodeMap[SDL_SCANCODE_F1] = 0x70;           // VK_F1
    keycodeMap[SDL_SCANCODE_F2] = 0x71;           // VK_F2
    keycodeMap[SDL_SCANCODE_F3] = 0x72;           // VK_F3
    keycodeMap[SDL_SCANCODE_F4] = 0x73;           // VK_F4
    keycodeMap[SDL_SCANCODE_F5] = 0x74;           // VK_F5
    keycodeMap[SDL_SCANCODE_F6] = 0x75;           // VK_F6
    keycodeMap[SDL_SCANCODE_F7] = 0x76;           // VK_F7
    keycodeMap[SDL_SCANCODE_F8] = 0x77;           // VK_F8
    keycodeMap[SDL_SCANCODE_F9] = 0x78;           // VK_F9
    keycodeMap[SDL_SCANCODE_F10] = 0x79;          // VK_F10
    keycodeMap[SDL_SCANCODE_F11] = 0x7A;          // VK_F11
    keycodeMap[SDL_SCANCODE_F12] = 0x7B;          // VK_F12
    keycodeMap[SDL_SCANCODE_PRINTSCREEN] = 0x2C;  // VK_SNAPSHOT
    keycodeMap[SDL_SCANCODE_SCROLLLOCK] = 0x91;   // VK_SCROLL
    keycodeMap[SDL_SCANCODE_PAUSE] = 0x13;        // VK_PAUSE
    keycodeMap[SDL_SCANCODE_INSERT] = 0x2D;       // VK_INSERT
    keycodeMap[SDL_SCANCODE_HOME] = 0x24;         // VK_HOME
    keycodeMap[SDL_SCANCODE_PAGEUP] = 0x21;       // VK_PRIOR
    keycodeMap[SDL_SCANCODE_DELETE] = 0x2E;       // VK_DELETE
    keycodeMap[SDL_SCANCODE_END] = 0x23;          // VK_END
    keycodeMap[SDL_SCANCODE_PAGEDOWN] = 0x22;     // VK_NEXT
    keycodeMap[SDL_SCANCODE_RIGHT] = 0x27;        // VK_RIGHT
    keycodeMap[SDL_SCANCODE_LEFT] = 0x25;         // VK_LEFT
    keycodeMap[SDL_SCANCODE_DOWN] = 0x28;         // VK_DOWN
    keycodeMap[SDL_SCANCODE_UP] = 0x26;           // VK_UP
    keycodeMap[SDL_SCANCODE_NUMLOCKCLEAR] = 0x90; // VK_NUMLOCK
    keycodeMap[SDL_SCANCODE_KP_DIVIDE] = 0x6F;    // VK_DIVIDE
    keycodeMap[SDL_SCANCODE_KP_MULTIPLY] = 0x6A;  // VK_MULTIPLY
    keycodeMap[SDL_SCANCODE_KP_MINUS] = 0x6D;     // VK_SUBTRACT
    keycodeMap[SDL_SCANCODE_KP_PLUS] = 0x6B;      // VK_ADD
    keycodeMap[SDL_SCANCODE_KP_ENTER] = 0x0D;     // VK_RETURN
    keycodeMap[SDL_SCANCODE_KP_1] = 0x61;         // VK_NUMPAD1
    keycodeMap[SDL_SCANCODE_KP_2] = 0x62;         // VK_NUMPAD2
    keycodeMap[SDL_SCANCODE_KP_3] = 0x63;         // VK_NUMPAD3
    keycodeMap[SDL_SCANCODE_KP_4] = 0x64;         // VK_NUMPAD4
    keycodeMap[SDL_SCANCODE_KP_5] = 0x65;         // VK_NUMPAD5
    keycodeMap[SDL_SCANCODE_KP_6] = 0x66;         // VK_NUMPAD6
    keycodeMap[SDL_SCANCODE_KP_7] = 0x67;         // VK_NUMPAD7
    keycodeMap[SDL_SCANCODE_KP_8] = 0x68;         // VK_NUMPAD8
    keycodeMap[SDL_SCANCODE_KP_9] = 0x69;         // VK_NUMPAD9
    keycodeMap[SDL_SCANCODE_KP_0] = 0x60;         // VK_NUMPAD0
    keycodeMap[SDL_SCANCODE_KP_PERIOD] = 0x6E;    // VK_DECIMAL //小键盘的‘.’

    laya::JCConch::s_pConch.reset(new laya::JCConch());

#if defined(OS_WINDOWS)
    createAndAttachConsole();
#endif

    const int x = SDL_WINDOWPOS_CENTERED;
    const int y = SDL_WINDOWPOS_CENTERED;
    SDL_Init(SDL_INIT_AUDIO);

    uint32_t windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;
    if (laya::g_kSystemConfig.m_windowMode == laya::WindowMode::WM_Window)
    {
    }
    else if (laya::g_kSystemConfig.m_windowMode == laya::WindowMode::WM_WindowResizable)
    {
        windowFlags |= SDL_WINDOW_RESIZABLE;
    }
    else if (laya::g_kSystemConfig.m_windowMode == laya::WindowMode::WM_FullScreen)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }

    m_sdlWindow = SDL_CreateWindow(config.title.c_str(), x, y, (int)g_nInnerWidth, (int)g_nInnerHeight, windowFlags);

    SDL_SysWMinfo sys;
    SDL_VERSION(&sys.version);
    if (SDL_FALSE != SDL_GetWindowWMInfo(m_sdlWindow, &sys))
    {
#if defined(OS_WINDOWS)
        g_hWnd = sys.info.win.window;
        // HINSTANCE hInstance = sys.info.win.hinstance;

#elif defined(OS_LINUX)
        g_X11_display = sys.info.x11.display;
        g_X11_window = sys.info.x11.window;
#endif
    }
    else
    {
        assert(true && "get hWnd failed");
    }

    laya::BackendOptions options;
#if defined(OS_WINDOWS)
    options.nativeLayer = g_hWnd;
#elif defined(OS_LINUX)
    options.nativeLayer = m_sdlWindow;
#endif

    laya::JCConch::s_pConchRender->createBackend(options);
    laya::JCConch::s_pConchRender->createScreenSurface(options.nativeLayer);
    laya::JCConch::s_pConch->onAppStart();

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    // 启用SDL_SYSWMEVENT
    // SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    SDL_StopTextInput();

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
                // e.keyCode = event.key.keysym.scancode;
                e.keyCode = keycodeMap[event.key.keysym.scancode];
                // e.keyCode = event.key.keysym.sym;
                auto mod = event.key.keysym.mod;
                bool bCtrl = mod & (KMOD_LCTRL | KMOD_RCTRL);
                bool bAlt = mod & KMOD_ALT;
                bool bShift = mod & KMOD_SHIFT;
                e.bAlt = bAlt;
                e.bShift = bShift;
                e.bCtrl = bCtrl;

                // printf("alt:%d ctrl:%d shift:%d\n", bAlt, bCtrl, bShift);

                JCConch::s_pConch->dispatchInputEvent(e);
            }
            break;
            case SDL_KEYUP: {
                inputEvent e;
                e.nTouchType = e.nType = E_ONKEYUP;
                strncpy(e.type, "keyup", 256);
                // e.keyCode = event.key.keysym.scancode;
                e.keyCode = keycodeMap[event.key.keysym.scancode];
                auto mod = event.key.keysym.mod;
                bool bCtrl = mod & (KMOD_LCTRL | KMOD_RCTRL);
                bool bAlt = mod & KMOD_ALT;
                bool bShift = mod & KMOD_SHIFT;
                e.bAlt = bAlt;
                e.bShift = bShift;
                e.bCtrl = bCtrl;

                JCConch::s_pConch->dispatchInputEvent(e);
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
                strncpy(e.type, "wheel", 256);
//                e.posX = event.wheel.mouseX;
//                e.posY = event.wheel.mouseY;
                SDL_GetMouseState(&e.posX, &e.posY);
                e.deltaMode = 0;
                e.deltaX = -event.wheel.x * 100.0f; // 凑的
                e.deltaY = -event.wheel.y * 100.0f; // 凑的
                e.deltaZ = 0.0f;                    // 凑的
                e.nWheel = 1;

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
                    laya::JCConch::s_pConchRender->onScreenSurfaceResize(g_nInnerWidth, g_nInnerHeight);
                    break;
                case SDL_WINDOWEVENT_MINIMIZED:
                    if (!m_min)
                    {
                        m_min = true;
                        laya::JCConch::s_pConch->onAppPause();
                    }
                    m_activate = false;
                    break;
                case SDL_WINDOWEVENT_MAXIMIZED:
                    if (m_min)
                    {
                        m_min = false;
                        laya::JCConch::s_pConch->onAppResume();
                    }
                    m_activate = true;
                    break;
                case SDL_WINDOWEVENT_RESTORED:
                    if (m_min)
                    {
                        m_min = false;
                        laya::JCConch::s_pConch->onAppResume();
                    }
                    m_activate = true;
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    m_activate = true;
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    m_activate = false;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        int delay = 8;
        if (m_min)
            delay = 100;
        else if (!m_activate)
        {
            delay = 33;
        }
        SDL_Delay(delay);
        laya::JCConch::s_pConch->update();
    }
    laya::JCConch::s_pConch->onAppDestroy();
    SDL_DestroyWindow(m_sdlWindow);
    SDL_Quit();
}
} // namespace laya

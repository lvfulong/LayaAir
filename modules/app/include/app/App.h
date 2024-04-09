#ifndef __APP_H__
#define __APP_H__

#include <app/Config.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string>

namespace laya
{
class App
{
  public:
    App();
    ~App();
    void run(const Config &config);

  private:
    void loadConfigIniFile();
    bool m_closed = false;
    SDL_Window *m_sdlWindow;
    int m_nJSDebugMode = 0;
    int m_nJSDebugPort = 5959;
};
} // namespace laya
#endif

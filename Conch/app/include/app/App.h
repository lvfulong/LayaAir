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
    void run(const Config &config, size_t width, size_t height, int nJSDebugMode, int nJSDebugPort);

  private:
    bool m_closed = false;
    SDL_Window *m_sdlWindow;
};
} // namespace laya
#endif

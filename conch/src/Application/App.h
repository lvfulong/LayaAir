#ifndef __APP_H__
#define __APP_H__

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string>

namespace laya
{
class App
{
  public:
    struct Config
    {
        std::string title;
    };
    App();
    ~App();
    void run(const Config &config);

  private:
    bool m_closed{false};
    SDL_Window *m_sdlWindow;
    bool m_min{false};
    bool m_activate{true};
};
} // namespace laya
#endif

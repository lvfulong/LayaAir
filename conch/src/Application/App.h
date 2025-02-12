#ifndef __APP_H__
#define __APP_H__

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <functional>
#include <string>

namespace laya
{
extern Uint32 CUSTOM_EVENT_EDITBOX;

struct EditBoxEventData
{
    // int data1;
    // std::string data2;
    std::function<void()> callback;
};
void SendEditBoxCustomEvent(std::function<void()> &&callback);
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

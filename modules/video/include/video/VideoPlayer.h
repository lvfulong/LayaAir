#ifndef __VideoPlayer_H__
#define __VideoPlayer_H__

#include <functional>
#include <memory>
#include <string>

namespace ffplay
{
class VideoPlayerImpl;
class VideoPlayer
{

  public:
    VideoPlayer();
    ~VideoPlayer();
    void setMedia(const std::string &url);
    void setMedia(unsigned char* buffer, int length);
    void setRenderCallback(std::function<void(unsigned char *data, int width, int height, int bufferSize)> cb);
    void renderVideo();
    int getState();
    void setEmit(std::function<void(const char *)> emitFun);
    void setLoop(bool loop);
    bool getLoop();
    void play();
    void pause();
    bool getPause();
    bool getAutoplay();
    void setAutoplay(bool value);
    void stop();
    double getVideoWidth();
    double getVideoHeight();
    double getCurrentPosition();
    double getDuration();
    double getVolume();
    void setVolume(double volume);
    void seekTo(double time);
    static void init();
    static void destroy();

  private:
    std::unique_ptr<VideoPlayerImpl> m_impl;
};
} // namespace ffplay
#endif
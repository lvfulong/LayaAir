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
    void setRenderCallback(std::function<void(unsigned char *data, int width, int height, int bufferSize)> cb);
    void renderVideo();
    static void init();
    static void destroy();

  private:
    std::unique_ptr<VideoPlayerImpl> m_impl;
};
} // namespace ffplay
#endif
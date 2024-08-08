#ifndef __VideoPlayer_H__
#define __VideoPlayer_H__

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
    static void init();
    static void destroy();

  private:
    std::unique_ptr<VideoPlayerImpl> m_impl;
};
} // namespace ffplay
#endif
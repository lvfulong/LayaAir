#include "ffplay.h"
#include <stdio.h>
#include <string.h>
#include <video/VideoPlayer.h>

namespace ffplay
{

struct VideoPlayerImpl
{
    VideoState *m_is{nullptr};
};
VideoPlayer::VideoPlayer() : m_impl(new VideoPlayerImpl())
{
}
VideoPlayer::~VideoPlayer()
{
}
void VideoPlayer::init()
{
#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
    avformat_network_init();
}
void VideoPlayer::destroy()
{
}
void VideoPlayer::setMedia(const std::string &url)
{
    if (m_impl->m_is == nullptr)
    {
        // todo
        AVInputFormat *iformat = nullptr;
        m_impl->m_is = stream_open(url.c_str(), iformat);
        if (!m_impl->m_is)
        {
            av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
            do_exit(NULL);
        }
    }
}
void VideoPlayer::renderVideo()
{
}
void VideoPlayer::setRenderCallback(std::function<void(unsigned char *data, int width, int height, int bufferSize)> cb)
{
    m_impl->m_is->render_callback = cb;
}
} // namespace ffplay
#include "ffplay.h"
#include <stdio.h>
#include <string.h>
#include <video/VideoPlayer.h>

namespace ffplay
{

struct VideoPlayerImpl
{
    VideoState m_is;
};
VideoPlayer::VideoPlayer() : m_impl(new VideoPlayerImpl())
{
}
VideoPlayer::~VideoPlayer()
{
}
void VideoPlayer::init()
{
    // #if CONFIG_AVDEVICE
    avdevice_register_all();
    // #endif
    avformat_network_init();
}
void VideoPlayer::destroy()
{
}
void VideoPlayer::setMedia(const std::string &url)
{

    // todo
    AVInputFormat *iformat = nullptr;
    if (!stream_open(&m_impl->m_is, url.c_str(), iformat))
    {
        av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
        do_exit(NULL);
    }
}
void VideoPlayer::renderVideo()
{
    double remaining_time = 0.0;
    if (m_impl->m_is.show_mode != VideoState::SHOW_MODE_NONE && (!m_impl->m_is.paused || m_impl->m_is.force_refresh))
    {
        video_refresh(&m_impl->m_is, &remaining_time);
    }
}
void VideoPlayer::setRenderCallback(std::function<void(unsigned char *data, int width, int height, int bufferSize)> cb)
{
    m_impl->m_is.render_callback = cb;
}
int VideoPlayer::getState()
{
    return (int)m_impl->m_is.m_videoState;
}
void VideoPlayer::setEmit(std::function<void(const char *)> emitFun)
{
    m_impl->m_is.m_emitFunc = emitFun;
}
void VideoPlayer::setLoop(bool loop)
{
    m_impl->m_is.m_loop = loop;
}
bool VideoPlayer::getLoop()
{
    return m_impl->m_is.m_loop;
}

} // namespace ffplay
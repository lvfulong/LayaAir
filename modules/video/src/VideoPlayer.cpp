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
    stop();
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
    if (m_impl)
    {
        // todo
        AVInputFormat *iformat = nullptr;
        if (!stream_open(&m_impl->m_is, url.c_str(), iformat))
        {
            av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
            do_exit(NULL);
        }
    }
}
void VideoPlayer::setMedia(unsigned char *buffer, int length)
{
    if (m_impl)
    {
        // todo
        AVInputFormat *iformat = nullptr;
        if (!stream_open(&m_impl->m_is, buffer, length, iformat))
        {
            av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
            do_exit(NULL);
        }
    }
}
void VideoPlayer::renderVideo()
{
    if (m_impl)
    {
        double remaining_time = 0.0;
        if (m_impl->m_is.show_mode != VideoState::SHOW_MODE_NONE &&
            (!m_impl->m_is.paused || m_impl->m_is.force_refresh))
        {
            video_refresh(&m_impl->m_is, &remaining_time);
        }
    }
}
void VideoPlayer::setRenderCallback(std::function<void(unsigned char *data, int width, int height, int bufferSize)> cb)
{
    if (m_impl)
    {
        m_impl->m_is.render_callback = cb;
    }
}
int VideoPlayer::getState()
{
    if (m_impl)
    {
        return (int)m_impl->m_is.m_videoState;
    }
    return 0;
}
void VideoPlayer::setEmit(std::function<void(const char *)> emitFun)
{
    if (m_impl)
    {
        m_impl->m_is.m_emitFunc = emitFun;
    }
}
void VideoPlayer::setLoop(bool loop)
{
    if (m_impl)
    {
        m_impl->m_is.m_loop = loop;
    }
}
bool VideoPlayer::getLoop()
{
    if (m_impl)
    {
        return m_impl->m_is.m_loop;
    }
    return false;
}
void VideoPlayer::play()
{
    if (m_impl)
    {
        do_play(&m_impl->m_is);
    }
}
void VideoPlayer::pause()
{
    if (m_impl)
    {
        do_pause(&m_impl->m_is);
    }
}
bool VideoPlayer::getPause()
{
    if (m_impl)
    {
        if (m_impl->m_is.paused)
        {
            return true;
        }
    }
    return false;
}
bool VideoPlayer::getAutoplay()
{
    if (m_impl)
    {
        return m_impl->m_is.m_autoplay;
    }
    return false;
}
void VideoPlayer::setAutoplay(bool value)
{
    if (m_impl)
    {
        m_impl->m_is.m_autoplay = value;
    }
}
void VideoPlayer::stop()
{
    if (m_impl)
    {
        do_exit(&m_impl->m_is);
        m_impl.reset();
    }
}
double VideoPlayer::getVideoWidth()
{
    if (m_impl)
    {
        return m_impl->m_is.videoWidth;
    }
    return 0;
}
double VideoPlayer::getVideoHeight()
{
    if (m_impl)
    {
        return m_impl->m_is.videoHeight;
    }
    return 0;
}
} // namespace ffplay
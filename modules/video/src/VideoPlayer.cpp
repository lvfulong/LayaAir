#include "ffplay.h"
#include <stdio.h>
#include <string.h>
#include <video/VideoPlayer.h>

namespace ffplay
{

struct VideoPlayerImpl
{
    std::unique_ptr<VideoState> m_is;
    std::string m_url;
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
bool VideoPlayer::isValid()
{
    return m_impl->m_is.get() != nullptr;
}
void VideoPlayer::setMedia(const std::string &url)
{
    if(url.empty())
    {
        return;
    }
    if (isValid())
    {
        stop();
    }
    m_impl->m_url = url;
    AVInputFormat *iformat = nullptr;
    m_impl->m_is = std::make_unique<VideoState>();
    m_impl->m_is->m_emitFunc = m_emitFunc;
    if (!stream_open(m_impl->m_is.get(), url.c_str(), iformat))
    {
        m_impl->m_is.reset();
        av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
        do_exit(NULL);
    }
}
void VideoPlayer::setMedia(unsigned char *buffer, int length)
{
    if (buffer == nullptr || length <= 0)
    {
        return;
    }
    if (isValid())
    {
        stop();
    }
    AVInputFormat* iformat = nullptr;
    m_impl->m_is = std::make_unique<VideoState>();
    m_impl->m_is->m_emitFunc = m_emitFunc;
    if (!stream_open(m_impl->m_is.get(), buffer, length, iformat))
    {
        m_impl->m_is.reset();
        av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
        do_exit(NULL);
    }
 
}
void VideoPlayer::renderVideo()
{
    if (isValid())
    {
        double remaining_time = 0.0;
        if (m_impl->m_is->show_mode != VideoState::SHOW_MODE_NONE &&
            (!m_impl->m_is->paused || m_impl->m_is->force_refresh))
        {
            video_refresh(m_impl->m_is.get(), &remaining_time);
        }
    }
}
void VideoPlayer::setRenderCallback(std::function<void(unsigned char *data, int width, int height, int bufferSize)> cb)
{
    if (isValid())
    {
        m_impl->m_is->render_callback = cb;
    }
}
int VideoPlayer::getState()
{
    if (isValid())
    {
        return (int)m_impl->m_is->m_videoState;  
    }
    return 0;
}
void VideoPlayer::setEmit(std::function<void(const char *)> emitFun)
{
    m_emitFunc = emitFun;
}
void VideoPlayer::setLoop(bool loop)
{
    if (isValid())
    {
        m_impl->m_is->m_loop = loop;
    }
}
bool VideoPlayer::getLoop()
{
    if (isValid())
    {
        return m_impl->m_is->m_loop;
    }
    return false;
}
void VideoPlayer::play()
{
    if (isValid())
    {
        do_play(m_impl->m_is.get());
    }
}
void VideoPlayer::pause()
{
    if (isValid())
    {
        do_pause(m_impl->m_is.get());
    }
}
bool VideoPlayer::getPause()
{
    if (isValid())
    {
        if (m_impl->m_is->paused)
        {
            return true;
        }
    }
    return false;
}
bool VideoPlayer::getAutoplay()
{
    if (isValid())
    {
        return m_impl->m_is->m_autoplay;
    }
    return false;
}
void VideoPlayer::setAutoplay(bool value)
{
    if (isValid())
    {
        m_impl->m_is->m_autoplay = value;
    }
}
void VideoPlayer::stop()
{
    if (isValid())
    {
        do_exit(m_impl->m_is.get());
        m_impl->m_is.reset();
    }
}
double VideoPlayer::getVideoWidth()
{
    if (isValid())
    {
        return m_impl->m_is->videoWidth;
    }
    return 0;
}
double VideoPlayer::getVideoHeight()
{
    if (isValid())
    {
        return m_impl->m_is->videoHeight;
    }
    return 0;
}
double VideoPlayer::getCurrentPosition()
{
    if (isValid()) 
    {
        return m_impl->m_is->audio_clock;
    }
    return 0.0;
}

double VideoPlayer::getDuration()
 {
    if (isValid())
    {
        if (m_impl->m_is->duration == AV_NOPTS_VALUE)
        { 
            return 0.0;
        }
        return m_impl->m_is->duration / (double)AV_TIME_BASE;
    }
    return 0.0;
}

double VideoPlayer::getVolume() 
{
    if (isValid()) 
    {
        return m_impl->m_is->audio_volume;
    }
    return 0.0;
}

void VideoPlayer::setVolume(double volume)
 {
    if (isValid())
    {
        m_impl->m_is->audio_volume = volume;
    }
}

void VideoPlayer::seekTo(double time) 
{
    if (isValid())
    {
        double duration = getDuration();
        if (time >= duration)
        {
            time = 0.0;
        }
        stream_seek(m_impl->m_is.get(), (int64_t)(time * AV_TIME_BASE), 0, 0);
    }
}
} // namespace ffplay

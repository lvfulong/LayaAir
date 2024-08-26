#include "JSVideo.h"
#include "Video/VideoPlayer.h"
namespace laya
{
std::set<std::string> JSVideo::ms_supportFormat = {
    //"video/mp4",
    //"video/ogg",
    //"video/webm"
};

class Win32VideoHandler final : public IVideoHandler
{
  public:
    Win32VideoHandler()
    {
    }

    virtual ~Win32VideoHandler()
    {
    }

    virtual bool isFrameUpdated()
    {
        return true;
    }

    virtual void updateBitmapData(BitmapData *bitmapData)
    {
        m_videoPlayer.setRenderCallback([bitmapData](unsigned char *data, int width, int height, int bufferSize) {
            bitmapData->reconfigure(width, height, sizeof(int) * 8, laya::ImgType_unknow);

            memcpy(bitmapData->m_pImageData, data, bufferSize);
        });
        m_videoPlayer.renderVideo();
    }

    ffplay::VideoPlayer m_videoPlayer;
};

JSVideo::JSVideo()
{
    Init();
    m_CallbackRef.reset(new int(1));
    AdjustAmountOfExternalAllocatedMemory(sizeof(JSVideo));
    JCMemorySurvey::GetInstance()->newClass("JSVideo", sizeof(JSVideo), this);

    m_pVideoHandler = new Win32VideoHandler;
    m_pJCVideo->setVideoHandler(m_pVideoHandler);

    ((Win32VideoHandler *)m_pVideoHandler)
        ->m_videoPlayer.setEmit(std::bind(&JSVideo::CallHandle, this, std::placeholders::_1));
}

JSVideo::~JSVideo()
{
    Dispose();

    JCMemorySurvey::GetInstance()->releaseClass("JSVideo", this);
}

void JSVideo::_releaseHandler()
{
    ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.stop();
}

void JSVideo::LoadInternal(const std::string &path)
{
    ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.setMedia(path);
    // CallHandle("loadedmetadata");
}
void JSVideo::LoadInternal(char *buffer, int length)
{
    ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.setMedia((unsigned char *)buffer, length);
}
void JSVideo::Play()
{
    ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.play();
}

void JSVideo::Pause()
{
    ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.pause();
}

void JSVideo::Stop()
{
    ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.stop();
}

bool JSVideo::GetPaused()
{
    return ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.getPause();
}

bool JSVideo::GetLoop()
{
    return ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.getLoop();
}
void JSVideo::SetLoop(bool value)
{
    ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.setLoop(value);
}
void JSVideo::SetAutoplay(bool value)
{
    ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.setAutoplay(value);
}

bool JSVideo::GetAutoplay()
{
    return ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.getAutoplay();
}

double JSVideo::GetVideoWidth()
{
    return ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.getVideoWidth();
}

double JSVideo::GetVideoHeight()
{
    return ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.getVideoHeight();
}

double JSVideo::GetWidth()
{
    NOT_IMPLEMENT_RET(0);
}

void JSVideo::SetWidth(double val)
{
    NOT_IMPLEMENT();
}

double JSVideo::GetHeight()
{
    NOT_IMPLEMENT_RET(0);
}

void JSVideo::SetHeight(double val)
{
}

double JSVideo::GetCurrentTime()
{
    NOT_IMPLEMENT_RET(0);
}

void JSVideo::SetCurrentTime(double val)
{
    NOT_IMPLEMENT();
}

double JSVideo::GetDuration()
{
    NOT_IMPLEMENT_RET(0);
}

double JSVideo::GetVolume()
{
    NOT_IMPLEMENT_RET(0);
}

void JSVideo::SetVolume(double val)
{
    NOT_IMPLEMENT();
}

void JSVideo::SetX(double val)
{
    NOT_IMPLEMENT();
}

double JSVideo::GetX()
{
    NOT_IMPLEMENT_RET(0);
}

void JSVideo::SetY(double val)
{
    NOT_IMPLEMENT();
}

double JSVideo::GetY()
{
    NOT_IMPLEMENT_RET(0);
}

int32_t JSVideo::GetReadyState()
{
    return ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.getState();
}

}; // namespace laya

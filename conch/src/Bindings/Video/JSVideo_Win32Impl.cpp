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
}

void JSVideo::LoadInternal(const std::string &path)
{
    ((Win32VideoHandler *)m_pVideoHandler)->m_videoPlayer.setMedia(path);
    // CallHandle("loadedmetadata");
}

void JSVideo::Play()
{
    NOT_IMPLEMENT();
}

void JSVideo::Pause()
{
    NOT_IMPLEMENT();
}

void JSVideo::Stop()
{
    NOT_IMPLEMENT();
}

bool JSVideo::GetPaused()
{
    NOT_IMPLEMENT_RET(true);
}

bool JSVideo::GetLoop()
{
    return ((Win32VideoHandler*)m_pVideoHandler)->m_videoPlayer.getLoop();
}
void JSVideo::SetLoop(bool value)
{
    ((Win32VideoHandler*)m_pVideoHandler)->m_videoPlayer.setLoop(value);
}
void JSVideo::SetAutoplay(bool value)
{
    NOT_IMPLEMENT();
}

bool JSVideo::GetAutoplay()
{
    NOT_IMPLEMENT_RET(false);
}

double JSVideo::GetVideoWidth()
{
    NOT_IMPLEMENT_RET(0);
}

double JSVideo::GetVideoHeight()
{
    NOT_IMPLEMENT_RET(0);
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

#include "../../JCConch.h"
#include "../../JCScriptRuntime.h"
#include "../../JCSystemConfig.h"
#include "JSVideo.h"
#include <LayaGL/JCLayaGLDispatch.h>

namespace laya
{
int JSVideo::ms_handleIdx = 0;

void JSVideo::Init()
{
    m_pJCVideo = std::make_shared<JCVideo>();
    m_nID = JCConch::s_pConchRender->m_pImageManager->getImageID();

    //		LOGI("[Debug][Video] ID is %d", m_nID);

    m_pJCVideo->setManager(JCConch::s_pConchRender->m_pFileResManager, JCConch::s_pConchRender->m_pImageManager);

    m_isDownloadWaitLoad = false;
    m_isDownloadWaitPlay = false;
    m_src = "";

    int nID = this->m_nID;
    auto pJCVideo = this->m_pJCVideo;
    JCConch::s_pConch->s_pConchRender->postTaskFromJSToRenderAsync(
        [nID, pJCVideo]() { JCConch::s_pConchRender->m_pImageManager->setImage(nID, pJCVideo); });
}

void JSVideo::Dispose()
{
    int nID = m_nID;
    JCConch::s_pConch->s_pConchRender->postTaskFromJSToRenderAsync([nID]() {
        auto pImage = JCConch::s_pConchRender->m_pImageManager->getImage(nID);
        if (pImage)
        {
            pImage->releaseBitmapData();
        }
    });
}

void JSVideo::Load(const char *url)
{
    if (strstr(url, "http://stand.alone.version") != nullptr)
    {
        std::weak_ptr<int> cbref(m_CallbackRef);
        JCFileRes *pRes = JCConch::s_pScriptRuntime->m_pFileResMgr->getRes(url);
        pRes->setOnReadyCB(std::bind(&JSVideo::onDownloadOK, this, std::placeholders::_1, false, cbref));
        pRes->setOnErrorCB(
            std::bind(&JSVideo::onDownloadError, this, std::placeholders::_1, std::placeholders::_2, cbref));
    }
    else
    {
        LoadInternal(url);
    }
}
void JSVideo::onDownloadOK(JCResStateDispatcher *p_pRes, bool p_bDecodeSync, std::weak_ptr<int> &callbackref)
{
    if (!callbackref.lock())
    {
        return;
    }
    // m_nDownloadState = 0;
    JCFileRes *pFileRes = (JCFileRes *)p_pRes;
    if (pFileRes->m_pBuffer.get())
    {
        m_buffer = pFileRes->m_pBuffer;
        LoadInternal(pFileRes->m_pBuffer.get(), pFileRes->m_nLength);
    }
    else
    {
        onDownloadError(p_pRes, 1, callbackref);
    }
}
void JSVideo::onDownloadError(JCResStateDispatcher *, int e, std::weak_ptr<int> &callbackref)
{
    if (!callbackref.lock())
        return;

    // std::function<void(void)> pFunction = std::bind(&JSImage::onErrorCallJSFunction, this, e, callbackref);
    // postToJS(pFunction);
}
bool JSVideo::CanPlayType(const char *type)
{
    auto it = ms_supportFormat.find(type);
    return it != ms_supportFormat.end();
}

const char *JSVideo::GetSrc()
{
    return m_src.c_str();
}

void JSVideo::SetSrc(const char *url)
{
    if (!m_src.empty())
    {
        return;
    }
    m_src = url;
    Load(url);
}

int JSVideo::getImageID()
{
    return m_nID;
}

void JSVideo::AddEvent(const char *evtName)
{
    auto it = m_evtMap.find(evtName);
    if (it == m_evtMap.end())
        m_evtMap.insert(std::make_pair(evtName, 1));
    else
        it->second++;
}

void JSVideo::RemoveEvent(const char *evtName)
{
    auto it = m_evtMap.find(evtName);
    if (it != m_evtMap.end())
    {
        it->second--;
        if (it->second == 0)
            m_evtMap.erase(it);
    }
}

void JSVideo::_setDispatchEventFunc(JSValueAsParam emitFunc)
{
    m_dispatchHandle.reset(emitFunc);
}

void JSVideo::CallHandle(const char *evtName)
{
    std::string evt(evtName);
    //		LOGI("[Debug][Video]Call handle %s", evtName);
    auto it = m_evtMap.find(evtName);
    if (it != m_evtMap.end())
    {
        if (JCConch::s_pScriptRuntime)
        {
            JCConch::s_pScriptRuntime->m_pScriptThread->post(std::bind(&JSVideo::DispatchEvtToJS, this, evt));
        }
    }
}

void JSVideo::DispatchEvtToJS(const std::string evtName)
{
    m_dispatchHandle.call<void>(toLocal(this), evtName.c_str());
}

void JSVideo::exportJS(Context &context)
{
    class_<JSVideo> class_binding;
    class_binding.constructor<>();
    class_binding.function("load", &JSVideo::Load);
    class_binding.function("play", &JSVideo::Play);
    class_binding.function("pause", &JSVideo::Pause);
    class_binding.function("stop", &JSVideo::Stop);
    class_binding.function("addEvent", &JSVideo::AddEvent);
    class_binding.function("removeEvent", &JSVideo::RemoveEvent);
    class_binding.function("_setDispatchEventFunc", &JSVideo::_setDispatchEventFunc);
    class_binding.function("_releaseHandler", &JSVideo::_releaseHandler);
    class_binding.function("canPlayType", &JSVideo::CanPlayType);
    class_binding.property("paused", &JSVideo::GetPaused);
    class_binding.property("loop", &JSVideo::GetLoop, &JSVideo::SetLoop);
    class_binding.property("x", &JSVideo::GetX, &JSVideo::SetX);
    class_binding.property("y", &JSVideo::GetY, &JSVideo::SetY);
    class_binding.property("videoWidth", &JSVideo::GetVideoWidth);
    class_binding.property("videoHeight", &JSVideo::GetVideoHeight);
    class_binding.property("readyState", &JSVideo::GetReadyState);
    class_binding.property("conchImgId", &JSVideo::getImageID);
    class_binding.property("autoplay", &JSVideo::GetAutoplay, &JSVideo::SetAutoplay);
    class_binding.property("width", &JSVideo::GetWidth, &JSVideo::SetWidth);
    class_binding.property("height", &JSVideo::GetHeight, &JSVideo::SetHeight);
    class_binding.property("currentTime", &JSVideo::GetCurrentTime, &JSVideo::SetCurrentTime);
    class_binding.property("duration", &JSVideo::GetDuration);
    class_binding.property("src", &JSVideo::GetSrc, &JSVideo::SetSrc);
    class_binding.property("currentSrc", &JSVideo::GetSrc, &JSVideo::SetSrc);
    class_binding.property("volume", &JSVideo::GetVolume, &JSVideo::SetVolume);
    context.class_("ConchVideo", class_binding);
}

} // namespace laya

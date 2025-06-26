#ifndef __JsVideo_h__
#define __JsVideo_h__

#include <jsbind/JSBind.h>

#include <Image/JCVideo.h>
#include <map>
#include <set>
#include <string>
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif
namespace laya
{

// #define SHOW_NOT_IMPLEMENT_LOG

#ifdef SHOW_NOT_IMPLEMENT_LOG
#define NOT_IMPLEMENT()                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        LOGW("%s is not implemented", __FUNCTION__);                                                                   \
    } while (false);
#define NOT_IMPLEMENT_RET(value)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        LOGW("%s is not implemented", __FUNCTION__);                                                                   \
    } while (false);                                                                                                   \
    return value;
#else
#define NOT_IMPLEMENT()
#define NOT_IMPLEMENT_RET(value) return value;
#endif

class JSVideo
{
  public:
    static void exportJS(jsbind::Object &context);

    JSVideo();
    ~JSVideo();

    void AddEvent(const char *evtName);
    void RemoveEvent(const char *evtName);
    void _setDispatchEventFunc(jsvm_value emitFunc);

    void Load(const char *url);
    void Play();
    void Pause();
    void Stop();
    bool CanPlayType(const char *type);

    void SetAutoplay(bool val);
    bool GetAutoplay();

    void SetX(double val);
    double GetX();

    void SetY(double val);
    double GetY();

    bool GetPaused();

    bool GetLoop();
    void SetLoop(bool value);

    const char *GetSrc();
    void SetSrc(const char *url);

    double GetVideoWidth();
    double GetVideoHeight();

    double GetWidth();
    void SetWidth(double val);

    double GetHeight();
    void SetHeight(double val);

    double GetCurrentTime();
    void SetCurrentTime(double val);

    double GetDuration();

    double GetVolume();
    void SetVolume(double val);

    void CallHandle(const char *evtName);

    int32_t GetReadyState();

    int getImageID();

    void LoadJS();

    void _releaseHandler();

    static void executeVideoCallback(int index, int event);
    static void executeLoadedmetadata(int index, const char *evtName);
    static void executeDurationBack(int index, int duration);
    static void executeCurrentTimeBack(int index, int currentTime);

  private:
    void Init();
    void Dispose();
    void LoadInternal(const std::string &path);
    void LoadInternal(char *buffer, int length);
    void DispatchEvtToJS(const std::string path);

    void onDownloadOK(JCResStateDispatcher *p_pRes, bool p_bDecodeSync, std::weak_ptr<int> &callbackref);
    void onDownloadError(JCResStateDispatcher *, int e, std::weak_ptr<int> &callbackref);

  public:
    int m_tag;
    static int ms_handleIdx;
    static std::set<std::string> ms_supportFormat;

    std::map<std::string, int> m_evtMap;
    jsbind::Persistent m_dispatchHandle;
    std::string m_src;

    bool m_isDownloadWaitLoad;
    bool m_isDownloadWaitPlay;

    IVideoHandler *m_pVideoHandler; /// !! released it by JCVideo

    std::shared_ptr<JCVideo> m_pJCVideo;
    int m_nID;
    std::shared_ptr<int> m_CallbackRef;
    std::shared_ptr<Data> m_data;
};
}; // namespace laya
#endif

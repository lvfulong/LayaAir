#include "JCConch.h"
#include <aki/jsbind.h>
#include <hilog/log.h>

class NapiHelper
{
  public:
    ~NapiHelper()
    {
    }
    static NapiHelper *GetInstance()
    {
        return &NapiHelper::help_;
    }
    std::string getDeviceInfo();
    void startVibration(float duration);
    void playBackgroundMusic(const char *p_sUrl, int p_nTimes, float nCurrentTime);
    void pauseBackgroundMusic();
    void stopBackgroundMusic();
    void setBackgroundMusicVolume(float p_nVolume);
    void resumeBackgroundMusic();
    void setCurrentTime(double nCurrentTime);
    double getCurrentTime();
    double getDuration();
    std::string getAppVersion();
    std::string getAppLocalVersion();

    void createVideoPlayer(int m_tag);
    void removeVideoPlayer(int m_tag);
    void setURL(int m_tag, std::string path, int isurl);
    void play(int m_tag);
    void pause(int m_tag);
    void stop(int m_tag);
    void setLooping(int m_tag, bool value);
    void setAutoplay(int m_tag, bool value);
    void setX(int m_tag, double value);
    void setY(int m_tag, double value);
    void setWidth(int m_tag, double value);
    void setHeight(int m_tag, double value);
    void setVideoCurrentTime(int m_tag, double value);
    void setVolume(int m_tag, double value);
    int getVolume(int m_tag);
    void showDialog(const char *p_sBuffer);
    void setPreferredOrientation(int orientation);
    float getScreenInch();
    static inline std::string Utf8Value(napi_env _env, napi_value _value)
    {
        size_t length;
        napi_status status = napi_get_value_string_utf8(_env, _value, nullptr, 0, &length);

        // NAPI_THROW_IF_FAILED(_env, status, "");

        std::string value;
        value.reserve(length + 1);
        value.resize(length);
        status = napi_get_value_string_utf8(_env, _value, &value[0], value.capacity(), &length); // cjh add &length
        // NAPI_THROW_IF_FAILED(_env, status, "");
        return value;
    }

  private:
    static NapiHelper help_;
};
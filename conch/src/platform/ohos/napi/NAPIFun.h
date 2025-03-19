#ifndef _NAPIFUN_
#define _NAPIFUN_

#include "Audio/JCAudioManager.h"
#include "JCConch.h"
#include "JCScriptRuntime.h"
#include "JCSystemConfig.h"
#include "downloadCache/JCOHOSFileSource.h"
#include "platform/ohos/napi/plugin_manager.h"
#include "utils/JCZipFile.h"
#include "utils/Log.h"
#include <aki/jsbind.h>
#include <rawfile/raw_file_manager.h>
#include <utils/thread/JCLayaThreadPool.h>

extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
extern std::string gRedistPath;

extern NativeResourceManager *g_pAssetManager;

class NAPIFun
{
  public:
    ~NAPIFun()
    {
    }
    static NAPIFun *GetInstance()
    {
        return &NAPIFun::fun_;
    }
    static void ConchNAPI_configSetURL(std::string p_strUrl);
    static void ConchNAPI_InitDLib(napi_value assetManager, std::string p_strCachePath);
    static void ConchNAPI_audioMusicPlayEnd();
    static void ConchNAPI_networkChanged(int nNetworkType);
    static void ConchNAPI_inputChange(int keycode);
    static void ConchNAPI_RunJS(const std::string &js);
    static void ConchNAPI_handleKeyboardInput(const std::string &value);
    static void ConchNAPI_handleKeyboardConfirm(const std::string &value);
    static void ConchNAPI_handleKeyboardComplete(const std::string &value);

  private:
    static NAPIFun fun_;
};

#endif
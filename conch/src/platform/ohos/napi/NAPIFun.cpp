#include "./NAPIFun.h"
#include "JCScriptRuntime.h"
#include <Bindings/JSDevice.h>
using namespace laya;

NAPIFun NAPIFun::fun_;
bool NAPIFun::m_bTakeScreenshot = false;


void NAPIFun::ConchNAPI_configSetURL(std::string p_strUrl)
{
    g_kSystemConfig.m_strStartURL = p_strUrl;
}
void NAPIFun::ConchNAPI_InitDLib(napi_value assetManager, int nThreadNum, std::string p_strAssetRootPath, std::string p_strCachePath)
{
    auto fun = NAPIFun::GetInstance();
    LOGI("NAPI InitDLib");
    DEBUG_CHECK(!laya::JCConch::s_pConch);

    //fun->g_nInitTime = tmGetCurms();

    gRedistPath = p_strCachePath;
    gRedistPath += "/";
    gAssetRootPath = p_strAssetRootPath;

    napi_env env = aki::JSBind::GetScopedEnv();
    g_pAssetManager = OH_ResourceManager_InitNativeResourceManager(env, assetManager);

    laya::JCOHOSFileSource *pAssets = new laya::JCOHOSFileSource();
    pAssets->Init(g_pAssetManager, "");
    JCConch::s_pAssetsFiles = pAssets;
    laya::JCConch::s_pConch.reset(new laya::JCConch());
    
}
void NAPIFun::ConchNAPI_audioMusicPlayEnd()
{
    laya::JCMp3Interface *pMp3Player = laya::JCAudioManager::GetInstance()->m_pMp3Player;
    if (pMp3Player)
    {
        pMp3Player->onPlayEnd();
    }
}
void NAPIFun::ConchNAPI_networkChanged(int nNetworkType)
{
    JCConch::s_pScriptRuntime->onNetworkChanged(nNetworkType);
}
void NAPIFun::ConchNAPI_inputChange(int keycode)
{
    if (JCConch::s_pScriptRuntime->m_pCurEditBox)
    {
        JCConch::s_pScriptRuntime->m_pCurEditBox->onInput();
    }
}
void NAPIFun::ConchNAPI_RunJS(const std::string &js)
{
     JCConch::s_pScriptRuntime->callJSString(js);
}
void NAPIFun::ConchNAPI_handleKeyboardInput(const std::string &value)
{
    JSDevice::handleKeyboardInput(value.c_str());
}
void NAPIFun::ConchNAPI_handleKeyboardConfirm(const std::string &value)
{
    JSDevice::handleKeyboardConfirm(value.c_str());
}
void NAPIFun::ConchNAPI_handleKeyboardComplete(const std::string &value)
{   
    JSDevice::handleKeyboardComplete(value.c_str());
}
JSBIND_GLOBAL()
{
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_configSetURL, "ConchNAPI_configSetURL");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_InitDLib, "ConchNAPI_InitDLib");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_audioMusicPlayEnd, "ConchNAPI_audioMusicPlayEnd");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_networkChanged, "ConchNAPI_networkChanged");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_inputChange, "ConchNAPI_inputChange");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_RunJS, "ConchNAPI_RunJS");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_handleKeyboardInput, "ConchNAPI_handleKeyboardInput");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_handleKeyboardConfirm, "ConchNAPI_handleKeyboardConfirm");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_handleKeyboardComplete, "ConchNAPI_handleKeyboardComplete");
}

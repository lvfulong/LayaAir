#include "./NAPIFun.h"
#include "JCScriptRuntime.h"
#include <Bindings/JSDevice.h>
using namespace laya;

NAPIFun NAPIFun::fun_;
NativeResourceManager* g_pAssetManager = nullptr;


void NAPIFun::ConchNAPI_init(napi_value assetManager, std::string url, std::string filesDir, std::string cacheDir)   
{
    auto fun = NAPIFun::GetInstance();
    LOGI("NAPI init %s %s %s", url.c_str(), filesDir.c_str(), cacheDir.c_str());  
    DEBUG_CHECK(!laya::JCConch::s_pConch);

    OS::setFilesDir(filesDir);  
    OS::setCacheDir(cacheDir);  
    g_kSystemConfig.m_strStartURL = url;

    napi_env env = aki::JSBind::GetScopedEnv();
    g_pAssetManager = OH_ResourceManager_InitNativeResourceManager(env, assetManager);

    laya::JCOHOSFileSource *pAssets = new laya::JCOHOSFileSource();
    pAssets->Init(g_pAssetManager, "");
    JCConch::s_pAssetsFiles = pAssets;

    
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
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_init, "ConchNAPI_init");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_audioMusicPlayEnd, "ConchNAPI_audioMusicPlayEnd");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_networkChanged, "ConchNAPI_networkChanged");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_inputChange, "ConchNAPI_inputChange");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_RunJS, "ConchNAPI_RunJS");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_handleKeyboardInput, "ConchNAPI_handleKeyboardInput");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_handleKeyboardConfirm, "ConchNAPI_handleKeyboardConfirm");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_handleKeyboardComplete, "ConchNAPI_handleKeyboardComplete");
}

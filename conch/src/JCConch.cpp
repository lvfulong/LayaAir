/**
@file			JCConch.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "JCConch.h"
#include <algorithm>
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include <utils/JCFileSystem.h>
#include <utils/JCFileSource.h>
#include <resource/JCFileResManager.h>
#include "JCConch.h"
#include <jsbind/JSBind.h>
#include <Bindings/JSFileReader.h>
#include <Bindings/JSGlobalExportCFun.h>
#include "JCScriptRuntime.h"
#include <downloadMgr/JCDownloadMgr.h>
#include "JCSystemConfig.h"
#include <LayaGL/JCLayaGL.h>
#include <Audio/JCAudioManager.h>
#include <Bindings/JSInput.h>
#include <thread>
#include <platform/OS.h>
#if defined(OS_ANDROID)
    #include "WebSocket/WebSocket.h"
    #include "CToJavaBridge.h"
    #include <dlfcn.h>
    #include <pthread.h>
    #include <sys/types.h>
    #include <unistd.h>
#elif defined(OS_IOS)
    #include "CToObjectC.h"
    #include "pthread.h"
#elif defined(OS_WINDOWS)
    #include <windows.h>
    HWND g_hWnd;
#elif defined(OS_OHOS)
#include "aki/jsbind.h"
#include <string>
#include "platform/ohos/napi/helper/NapiHelper.h"
#endif

std::string gRedistPath = "";
std::string gResourcePath = "";
std::string gAssetRootPath = "";

int g_nInnerWidth = 1024;
int g_nInnerHeight = 768;
bool g_bGLCanvasSizeChanged = false;

namespace laya
{
    extern std::shared_ptr<JCWorkerThread> g_DecThread;
	JCWorkerThread* g_FileIOThread = nullptr;
    std::shared_ptr<JCConch> JCConch::s_pConch;
    int64_t JCConch::s_nUpdateTime = 0;
    JCFileSource* JCConch::s_pAssetsFiles = NULL;
    std::shared_ptr<JCConchRender> JCConch::s_pConchRender;
    std::shared_ptr<JCScriptRuntime> JCConch::s_pScriptRuntime;
    void _vibrate()
    {
#if defined(OS_ANDROID)
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "vibrate", kRet);
#elif defined(OS_OHOS)
        NapiHelper::GetInstance()->startVibration(0.1f);
#endif
    }
    JCConch::JCConch()
    {
        m_sCachePath = gRedistPath + "/appCache";
        if (!FileSystem::exists(m_sCachePath))
        {
            FileSystem::mkdir(m_sCachePath);
        }
        laya::g_kSystemConfig.loadConfigIniFile();
#ifdef OS_APPLE
#elif defined(OS_WINDOWS)
        HMODULE libHandle = LoadLibrary(L"libGLESv2.dll");
#elif defined(OS_ANDROID)
        //void *libhandle = dlopen("libGLESv2.so", RTLD_LAZY);
#endif
        m_nUrlHistoryPos = -1;
        

        g_DecThread = std::make_shared<JCWorkerThread>(new JCWorkerThread(true));
        g_DecThread->setThreadName("image decode");
		g_FileIOThread = new JCWorkerThread(true);
		g_FileIOThread->setThreadName("file io");
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        int nDownloadThreadNum = 3;
        LOGI("download thread num = %d", nDownloadThreadNum);
        pdmgr->init(nDownloadThreadNum);
        m_pFileResMgr = new JCFileResManager(pdmgr);


       
        LOGI("Graphics API %s", toString(g_kSystemConfig.m_graphicsAPI).c_str());


        s_pConchRender.reset(new JCConchRender(m_pFileResMgr));
        s_pConchRender->init();

        s_pScriptRuntime.reset(new JCScriptRuntime());
        //------------------------------------------------------------------------------

        m_pAssetsRes = JCConch::s_pAssetsFiles;
        m_strStartJS = "scripts/apploader.js";

        JCConch::s_pScriptRuntime->init(m_pFileResMgr, m_pAssetsRes, this);


        //onAppStart();
        m_strLocalStoragePath = gRedistPath + "/localstorage/";

        if (!FileSystem::exists(m_strLocalStoragePath))
        {
            FileSystem::mkdir(m_strLocalStoragePath);
        }
    
        initializeCurrentThreadAsScriptThread();
        JCConch::s_pScriptRuntime->m_scriptThreadMessageLoop = &MessageLoop::getCurrent();
        
	}
    JCConch::~JCConch() {


    }
	void JCConch::onAppDestroy()
    { 
        DEBUG_CHECK(isScriptThread());
        m_isAppStarted = false;
  
        //关闭下载线程
        //删除所有的下载任务
        JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
        pNetLoader->stopCurTask();
        pNetLoader->clearAllAsyncTask();
      
        g_DecThread.reset();
        delete g_FileIOThread;

        JCConch::s_pScriptRuntime->stop();
          
        JCDownloadMgr::delInstance();
        
        s_pScriptRuntime.reset();
        delete JCAudioManager::GetInstance();
        s_pConchRender.reset();

        LOGI("onAppDestroy...");
	}

    void JCConch::onAppStart()
    {   
        DEBUG_CHECK(isScriptThread());
        if (m_isAppStarted)
        {
            return;
        }
        JCConch::s_pScriptRuntime->start(m_strStartJS.c_str());
        JCAudioManager::GetInstance();
        m_isAppStarted = true;
        JCConch::s_pScriptRuntime->loadJSScript();
	}
    void JCConch::reload() 
    {
        DEBUG_CHECK(isScriptThread());
        LOGI("JCConch::reload start...");
        //先通知消息管理器，关闭各个线程之间的post
        //lvtodo m_ThreadCmdMgr.stop();
        JCConch::s_pScriptRuntime->reload();
        LOGI("JCConch::reload end.");
        
    }
    int JCConch::urlHistoryLength() 
    {
        return m_vUrlHistory.size();
    }
    void JCConch::urlBack() 
    {
        urlGo(-1);
    }
    void JCConch::urlGo(int s) 
    {
#ifdef OS_IOS
        CToObjectCRunStopJSLoop();
#endif
        int sz = m_vUrlHistory.size();
        m_nUrlHistoryPos += s;
        if (m_nUrlHistoryPos >= sz) m_nUrlHistoryPos = sz - 1;
        if (m_nUrlHistoryPos < 0) m_nUrlHistoryPos = 0;
        if ((size_t)m_nUrlHistoryPos < m_vUrlHistory.size())
        {
            g_kSystemConfig.m_strStartURL = m_vUrlHistory[m_nUrlHistoryPos];
        }
        reload();
    }
    void JCConch::urlForward() 
    {
        urlGo(1);
    }
    void JCConch::urlHistoryPush(const char* sUrl) 
    {
        m_nUrlHistoryPos++; //位置
        int nsz = m_nUrlHistoryPos+1; //希望的大小
        m_vUrlHistory.resize(nsz);
        m_vUrlHistory[m_nUrlHistoryPos] = sUrl;
        int sz = m_vUrlHistory.size();
        if (sz>1) 
        {
            if (m_vUrlHistory[sz - 1] == m_vUrlHistory[sz - 2]) 
            {
                m_vUrlHistory.resize(sz - 1);
                m_nUrlHistoryPos--;
            }
        }
    }
    void JCConch::update() {
        DEBUG_CHECK(isScriptThread());
        auto pScriptRuntime = JCConch::s_pScriptRuntime;
        if (pScriptRuntime)
        {
            pScriptRuntime->update();
        }
    }
    void JCConch::dispatchInputEvent(inputEvent e)
    {
         JSInput::getInstance()->activeCall(e);
    }
    void JCConch::dispatchInputEvent(DeviceOrientationEvent e)
    {
         JSInput::getInstance()->activeCall(e);
    }
    void JCConch::dispatchInputEvent(DeviceMotionEvent e)
    {
         JSInput::getInstance()->activeCall(e);
    }
    void JCConch::dispatchEngineEvent(const EngineEventBase&  e)
    {
         switch(e.m_type)
         {
            case EngineEventType::Reload:
                reload();
                break;
            case EngineEventType::UrlBack:
                urlBack();
                break;
            default:
                break;
         }
    }
    void JCConch::onAppPause() {
        DEBUG_CHECK(isScriptThread());
#if defined(OS_ANDROID) || defined(OS_OHOS)  
            if( laya::JCAudioManager::GetInstance()->getMp3Mute() == false && laya::JCAudioManager::GetInstance()->getMp3Stopped() == false)
            {
                #if defined(OS_ANDROID)
                JCAudioManager::GetInstance()->pauseMp3();
                #endif
                #if defined(OS_OHOS)
                NapiHelper::GetInstance()->pauseBackgroundMusic();
                #endif
            }
            laya::JCAudioManager::GetInstance()->m_pWavPlayer->pause();
#endif
            auto pScriptRuntime = JCConch::s_pScriptRuntime;
            if (pScriptRuntime)
            {
                pScriptRuntime->onBlur();
            }
    }
    void JCConch::onAppResume() {
        DEBUG_CHECK(isScriptThread());
#if defined(OS_ANDROID) || defined(OS_OHOS)  
            //继续声音
            if( laya::JCAudioManager::GetInstance()->getMp3Mute() == false && laya::JCAudioManager::GetInstance()->getMp3Stopped() == false)
            {
                #if defined(OS_ANDROID)
                laya::JCAudioManager::GetInstance()->resumeMp3();
                #endif
                #if defined(OS_OHOS)
                NapiHelper::GetInstance()->resumeBackgroundMusic();
                #endif
            }
            laya::JCAudioManager::GetInstance()->m_pWavPlayer->resume();
#endif
            auto pScriptRuntime = JCConch::s_pScriptRuntime;
            if (pScriptRuntime)
            {
                pScriptRuntime->onFocus();
            }
    }
}

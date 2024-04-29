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
#include <binder/JSInterface.h>
#include <Bindings/JSFileReader.h>
#include <Bindings/JSGlobalExportCFun.h>
#include "JCScriptRuntime.h"
#include <downloadMgr/JCDownloadMgr.h>
#include "JCSystemConfig.h"
#include <LayaGL/JCLayaGL.h>
#include <Audio/JCAudioManager.h>
#include <Bindings/JSInput.h>

#ifdef ANDROID
    #include "WebSocket/WebSocket.h"
    #include "CToJavaBridge.h"
    #include <dlfcn.h>
    #include <pthread.h>
    #include <sys/types.h>
    #include <unistd.h>
#elif __APPLE__
    #include "CToObjectC.h"
    #include "pthread.h"
#elif WIN32
    #include <windows.h>
#endif
#ifdef __APPLE__
#include "OpenGLBackendiOS.h"
#elif ANDROID
#include "OpenGLBackendAndroidEGL.h"
#elif WIN32
#include "OpenGLBackendWinEGL.h"
#endif

std::string gRedistPath = "";
std::string gResourcePath = "";
#ifdef __APPLE__
    std::string gAssetRootPath = "";
#endif
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
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "vibrate", kRet);
#endif
    }
    JCConch::JCConch()
    {
#ifdef __APPLE__
#elif WIN32
        HMODULE libHandle = LoadLibrary("libGLESv2.dll");
#elif ANDROID
        //void *libhandle = dlopen("libGLESv2.so", RTLD_LAZY);
#endif
        m_nUrlHistoryPos = -1;
        m_sCachePath = gRedistPath + "/appCache";

        g_DecThread = std::make_shared<JCWorkerThread>(new JCWorkerThread(true));
        g_DecThread->setThreadName("image decode");
		g_FileIOThread = new JCWorkerThread(true);
		g_FileIOThread->setThreadName("file io");
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        int nDownloadThreadNum = 3;
        LOGI("download thread num = %d", nDownloadThreadNum);
        pdmgr->init(nDownloadThreadNum);
        m_pFileResMgr = new JCFileResManager(pdmgr);

        //m_pScriptThread = new JSMulThread();//m_pScriptThread = new JSSingleThread();
       
#ifdef WEBGL_THREAD
        LOGI("WebGL mode %s, WebGL thread on", g_kSystemConfig.m_bConchWebGL ? "on" : "off");
#else
        LOGI("WebGL mode %s, WebGL thread off", g_kSystemConfig.m_bConchWebGL ? "on" : "off");
#endif

        s_pConchRender.reset(new JCConchRender(m_pFileResMgr));
        s_pConchRender->init();

        s_pScriptRuntime.reset(new JCScriptRuntime());
        //------------------------------------------------------------------------------

        m_pAssetsRes = JCConch::s_pAssetsFiles;
        m_strStartJS = "scripts/apploader.js";

        JCConch::s_pScriptRuntime->init(m_pFileResMgr, m_pAssetsRes, this);

#ifdef JS_V8_DEBUGGER
        m_pDbgAgent = NULL;
        if (g_kSystemConfig.m_nJSDebugMode != JS_DEBUG_MODE_OFF)
        {
            LOGI("open js debug port at %d", g_kSystemConfig.m_nJSDebugPort);
            m_pDbgAgent = new DebuggerAgent("layabox", g_kSystemConfig.m_nJSDebugPort);
            JCConch::s_pScriptRuntime->m_pDbgAgent = m_pDbgAgent;
        }
        else
        {
            m_pDbgAgent = NULL;
            JCConch::s_pScriptRuntime->m_pDbgAgent = NULL;
        }
#endif
        //onAppStart();
        m_strLocalStoragePath = gRedistPath + "/localstorage/";
        //try
        //{
        std::error_code error;
        if (!fs::exists(m_strLocalStoragePath, error))
        {
            fs::create_directories(m_strLocalStoragePath, error);
        }
        //}
        //catch (...)
        //{
        //    if (global_onCreateFileError)
        //    {
        //        global_onCreateFileError();
        //    }
        //    return;
        //}
        JCConch::s_pScriptRuntime->start(m_strStartJS.c_str(), g_kSystemConfig.m_nJSDebugPort);
	}
    JCConch::~JCConch() {

    }
	void JCConch::onAppDestroy()
    { 
        m_isAppStarted = false;
  
        //关闭下载线程
        //删除所有的下载任务
        JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
        pNetLoader->stopCurTask();
        pNetLoader->clearAllAsyncTask();
      
        g_DecThread.reset();
        delete g_FileIOThread;
        m_semaphoreFramePacer.notifyAllWait();
        m_semaphore.notifyAllWait();

        JCConch::s_pScriptRuntime->stop();
          
        JCDownloadMgr::delInstance();
        
        s_pScriptRuntime.reset();
        delete JCAudioManager::GetInstance();
        s_pConchRender.reset();
#ifdef JS_V8_DEBUGGER
        if (m_pDbgAgent) 
        {
            m_pDbgAgent->Shutdown();
            delete m_pDbgAgent;
            m_pDbgAgent = NULL;
        }
#endif
        LOGI("onAppDestroy...");
	}

    void JCConch::onAppStart()
    {   
        if (m_isAppStarted)
        {
            return;
        }
        JCAudioManager::GetInstance();
        m_isAppStarted = true;
        JCConch::s_pScriptRuntime->loadJSScript();
	}
    void JCConch::reload() 
    {
        LOGI("JCConch::reload start...");
        //先通知消息管理器，关闭各个线程之间的post
        //lvtodo m_ThreadCmdMgr.stop();
        postToPlatform([this](){
            JCConch::s_pScriptRuntime->reload();
            LOGI("JCConch::reload end.");
        });
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
#ifdef __APPLE__
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
        m_semaphoreFramePacer.setDataNum(1);
        //todo
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            for (int i = 0, size = m_tasks.size(); i < size; i++ )
            {
                m_tasks[i]();
            }
            m_tasks.clear();
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
    void JCConch::postToPlatform(std::function<void(void)> task) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_tasks.emplace_back(task);
    }

    void postToPlatform(std::function<void(void)> task) {
        auto pConch = JCConch::s_pConch;
        if (pConch) {
            pConch->postToPlatform(task);
        }
    }

	void postToJS(std::function<void(void)> task) {
        auto pScriptRuntime = JCConch::s_pScriptRuntime;
        if (pScriptRuntime) {
            pScriptRuntime->m_pScriptThread->post(task);
        }
    }
    void JCConch::onAppPause() {
        m_semaphore.setDataNum(0);
        m_semaphoreFramePacer.stop();
        postToJS([]() {
#ifdef ANDROID
            if( laya::JCAudioManager::GetInstance()->getMp3Mute() == false && laya::JCAudioManager::GetInstance()->getMp3Stopped() == false)
            {
                JCAudioManager::GetInstance()->pauseMp3();
            }
            laya::JCAudioManager::GetInstance()->m_pWavPlayer->pause();
#endif
            auto pScriptRuntime = JCConch::s_pScriptRuntime;
            if (pScriptRuntime)
            {
                pScriptRuntime->onBlur();
            }
            auto pConch = JCConch::s_pConch;
            if (pConch) {
                pConch->m_semaphore.waitUntilHasData();
            }
        });
    }
    void JCConch::onAppResume() {
        m_semaphore.setDataNum(1);
        m_semaphoreFramePacer.resume();
        postToJS([]() {
#ifdef ANDROID
            //继续声音
            if( laya::JCAudioManager::GetInstance()->getMp3Mute() == false && laya::JCAudioManager::GetInstance()->getMp3Stopped() == false)
            {
                laya::JCAudioManager::GetInstance()->resumeMp3();
            }
            laya::JCAudioManager::GetInstance()->m_pWavPlayer->resume();
#endif
            auto pScriptRuntime = JCConch::s_pScriptRuntime;
            if (pScriptRuntime)
            {
                pScriptRuntime->onFocus();
            }
        });
    }
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------

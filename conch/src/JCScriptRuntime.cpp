/**
@file			JCScriptRuntime.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JCScriptRuntime.h"
#include <algorithm>
#include <utils/Log.h>
#include <jsbind/JSBind.h>
#include <Bindings/JSFileReader.h>
#include <Bindings/JSGlobalExportCFun.h>
#include <Bindings/JSInput.h>
#include <utils/JCFileSource.h>
#include <resource/JCFileResManager.h>
#include "Audio/JCAudioManager.h"
#include "JCSystemConfig.h"
#include "JCConch.h"
#include <downloadMgr/JCDownloadMgr.h>
#include <inttypes.h>
#include <Bindings/JSLayaGL.h>
#include <LayaGL/JCLayaGLDispatch.h>
#include <zip/JCZip.h>
#include <Bindings/JSLaunchOptions.h>
#include <Bindings/JSPromiseRejectionEvent.h>
//#include "btBulletDynamicsCommon.h"
#include <cstdarg>
#include "2D/FontManager.h"
#if defined(OS_WINDOWS)
#include "video/VideoPlayer.h"
#endif
#include "Extention/LayaExtWin.h"
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
extern laya::JCZip *g_ZipPackage;
extern std::string gRedistPath;

namespace laya 
{
    JCScriptRuntime::JCScriptRuntime()
    {
        m_pScriptThread = std::make_shared<jsvm::ScriptThread>();
        m_bHasJSThread = false;
        m_pFileResMgr = NULL;
        m_pAssetsRes = NULL;
        m_bIsExit = false;
        m_pUrl = new JCUrl();
        //m_nThreadState = 0;
        m_pArrayBufferManager = new JCArrayBufferManager();
        m_pRenderCmd = new JCCommandEncoderBuffer(102400, 1280);
        //事件现在有问题，只能添加不能删除。所以不要调用多次。
        m_pScriptThread->on(JCWorkerThread::Event_threadStart, std::bind(&JCScriptRuntime::onThreadInit, this, std::placeholders::_1));
        m_pScriptThread->on(JCWorkerThread::Event_threadStop, std::bind(&JCScriptRuntime::onThreadExit, this, std::placeholders::_1));
        m_nUpdateCount = 0;
#ifdef JS_V8
        m_pDbgAgent = nullptr;  
#endif
#if !defined(OS_LINUX) && !defined(OS_WINDOWS)
		m_pCurEditBox = NULL;
#endif
    }
    JCScriptRuntime::~JCScriptRuntime() 
    {

        m_pFileResMgr = NULL;
        m_pAssetsRes = NULL;
        if (m_pUrl)
        {
            delete m_pUrl;
            m_pUrl = NULL;
        }
        if (m_pRenderCmd)
        {
            delete m_pRenderCmd;
            m_pRenderCmd = NULL;
        }
        if (m_pArrayBufferManager)
        {
            delete m_pArrayBufferManager;
            m_pArrayBufferManager = nullptr;
        }
    }
    void  JCScriptRuntime::init(JCFileResManager* pFileMgr, JCFileSource* pAssetRes, JCConch* pConch)
    {
        m_pConch = pConch;
        m_pFileResMgr = pFileMgr;
        m_pAssetsRes = pAssetRes;
        FontManager::init();
#if defined(OS_WINDOWS)
        ffplay::VideoPlayer::init();
#endif
    }
    static void onUnhandledRejection(jsvm::Value pPromise, jsvm::Value pReason, const char* type)
    {
#ifdef JS_V8
        JSPromiseRejectionEvent* event = new JSPromiseRejectionEvent;
        event->setPromise(pPromise);
        event->setReason(pReason);
        event->setType(type);
        JCConch::s_pScriptRuntime->m_pJSOnUnhandledRejectionFunction.call<void>(jsvm::global(), jsbind::Make<JSPromiseRejectionEvent*>(event));
#endif
    }
    void JCScriptRuntime::start(const char* pStartJS) 
    {
        LOGI("Start js %s", pStartJS);
        if (pStartJS)m_strStartJS = pStartJS;

#ifdef JS_V8_DEBUGGER
        // m_pDbgAgent = NULL;
        // if (g_kSystemConfig.m_nJSDebugMode != JS_DEBUG_MODE_OFF)
        // {
        //     jsvm::OpenInspector(env, g_kSystemConfig.m_nJSDebugPort);
        //     LOGI("open js debug port at %d", g_kSystemConfig.m_nJSDebugPort);
        //     m_pDbgAgent = new DebuggerAgent("layabox", g_kSystemConfig.m_nJSDebugPort);
        //     JCConch::s_pScriptRuntime->m_pDbgAgent = m_pDbgAgent;
        // }
        // else
        // {
        //     m_pDbgAgent = NULL;
        //     JCConch::s_pScriptRuntime->m_pDbgAgent = NULL;
        // }
#endif

        m_debugPort = g_kSystemConfig.m_nJSDebugMode;
        //m_pScriptThread->initialize(m_debugPort, std::bind(&onUnhandledRejection, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        //m_nThreadState = 1;
        m_pScriptThread->setLoopFunc(std::bind(&JCScriptRuntime::onUpdate, JCConch::s_pScriptRuntime.get()));
        m_pScriptThread->start();
    }
    void JCScriptRuntime::stop()
    {
        LOGI("Stop js start...");

#ifdef JS_V8_DEBUGGER
        // if (m_pDbgAgent)
        // {
        //     m_pDbgAgent->Shutdown();
        //     delete m_pDbgAgent;
        //     m_pDbgAgent = NULL;
        // }
#endif
        //while (m_nThreadState==1)
        {
            //LOGI("stop: wait for thread to start...");
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        m_pScriptThread->stop();
        //m_pScriptThread->uninitialize();
        LOGI("Stop js end.");
    }
    void JCScriptRuntime::reload() 
    {
        stop();
        FontManager::destroy();
        //为了避免上个js的下载的影响，去掉下载任务
        //这个必须在stop之后做，且保证stop确实会停止线程，包括还没有启动的线程。
        //如果线程还没有启动，stop会等待线程先启动。这样就可能会执行脚本，所以，清理必须放在stop之后。
        JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
        pNetLoader->stopCurTask();
        pNetLoader->clearAllAsyncTask();
        pNetLoader->resetDownloadTail();	//防止第二次进入的时候，下载错误的dcc等
        pNetLoader->resetFinalReplacePath();
        pNetLoader->resetDownloadReplaceExt();
        //文件资源不能跨js环境使用，所以必须clear
        // 例如一个资源正在下载，则可能的问题：1.可能会上个线程取消了，不会再回调， 2. 自己希望回调的是上个js环境，也无法传给新的js环境。
        // 所以需要clear。
        m_pFileResMgr->clear();
        start(m_strStartJS.c_str());
        loadJSScript();
    }
    void JCScriptRuntime::onThreadInit(JCEventEmitter::evtPtr evt) 
    {
        LOGI("js thread started.");
        GET_ENV;
        env;
        //m_nThreadState = 2;
        //JSObjNode::s_pListJSObj = new JCSimpList();
#ifdef JS_V8_DEBUGGER
        if (g_kSystemConfig.m_nJSDebugMode != JS_DEBUG_MODE_OFF)
        {
            jsvm::OpenInspector(env, g_kSystemConfig.m_nJSDebugPort,m_pScriptThread);
        }

        // if (m_pDbgAgent) 
        // {
        //     m_pDbgAgent->onJSStart(m_pScriptThread,(g_kSystemConfig.m_nJSDebugMode == JS_DEBUG_MODE_WAIT) ? true : false);
        //     LOGI("js debug open mode: %d port %d", g_kSystemConfig.m_nJSDebugMode, g_kSystemConfig.m_nJSDebugPort);
        // }
        // else
        // {
        //     LOGI("js debug closed");
        // }
#endif
        JCConch::s_pConchRender->m_pImageManager->resetJSThread();

        //JS线程的数据清空一下
        m_pArrayBufferManager->clearAll();

        //给渲染线程发开始指令
        m_pRenderCmd->clearData();
         
        JCConch::s_pConchRender->postTaskFromJSToRenderSync([]()->bool {
            JCConch::s_pConchRender->clearAllData();
            return true;
        }).get();
        

        JSGlobalExportC();
        importAllDynaLib(gRedistPath,nullptr);

    }
    void JCScriptRuntime::loadJSScript() {
        postToJS([this](){
        //设置js的一些环境。必须在所有导出之后，执行其他脚本之前。
        {
            char* sJSRuntime = NULL;
            int nSize = 0;
            if (m_pAssetsRes->loadFileContent("scripts/runtimeInit.js", sJSRuntime, nSize))
            {
                jsbind::runScript(sJSRuntime);
                delete[] sJSRuntime;
            }
        }
        char* sJCBuffer = NULL;
        int nJSSize = 0;
        if (m_pAssetsRes->loadFileContent(m_strStartJS.c_str(), sJCBuffer, nJSSize))
        {
            std::string kBuf = "(function(window){\n'use strict'\n";
            kBuf += sJCBuffer;
            kBuf += "\n})(window);\n//@ sourceURL=apploader.js";
#ifdef JS_V8
            //v8::Isolate* isolate = v8::Isolate::GetCurrent();
            //v8::HandleScope handle_scope(isolate);
            //v8::TryCatch try_catch(isolate);
            jsbind::runScript(kBuf);
            //if (try_catch.HasCaught())
            //{
            //    __JSRun::ReportException(isolate, &try_catch);
            //}
#else
            jsbind::runScript(kBuf);
#endif
            delete[] sJCBuffer;
            sJCBuffer = NULL;
        }
        jsbind::runScript("gc();gc();gc();");
        });
    }
    void JCScriptRuntime::onThreadExit(JCEventEmitter::evtPtr evt)
    {
        GET_ENV;
        //if (m_nThreadState == 0)
        //{
        //    return;
        //}
        LOGI("js thread exiting...");
        //m_nThreadState = 0;
        m_pJSOnFrameFunction.reset();
        m_pJSOnResizeFunction.reset();
        m_pJSOnFocusFunction.reset();
        m_pJSOnBlurFunction.reset();
        m_pJSMouseEvtFunction.reset();
        m_pJSKeyEvtFunction.reset();
        m_pJSTouchEvtFunction.reset();
        m_pJSDeviceMotionEvtFunction.reset();
        m_pJSNetworkEvtFunction.reset();
        m_pJSOnceOtherEvtFuction.reset();
        m_pJSOnDrawFunction.reset();
        m_bJSBulletGetWorldTransformHandle.reset();
        m_bJSBulletSetWorldTransformHandle.reset();
		m_bJSBulletDrawLineHandle.reset();
		m_bJSBulletClearLineHandle.reset();
        m_pJSZipPackage.reset();
        m_pJSOnUnhandledRejectionFunction.reset();
		m_pJSOnScreenOrientationChanged.reset();
		m_pJSSetGlobalRepaintFunction.reset();
		g_ZipPackage = NULL;
#if !defined(OS_LINUX) && !defined(OS_WINDOWS)
        m_pCurEditBox = NULL;
#endif
        jsbind::runDeinitializers();

        JSGlobalDisExportC();
#ifdef JS_V8
#ifdef JS_V8_DEBUGGER
        jsvm::CloseInspector(env);
#endif
#elif JS_JSC
        JSP_RESET_GLOBAL_FUNCTION;
#endif

        JCAudioManager::ClearAllWork();
        JCAudioManager::GetInstance()->stopMp3();
        JCAudioManager::GetInstance()->pauseMp3();
    
        m_pArrayBufferManager->clearAll();
        
        JCConch::s_pConchRender->postTaskFromJSToRenderSync([]()->bool {
            JCConch::s_pConchRender->clearAllData();
            return true;
        }).get();
    }
    bool JCScriptRuntime::onUpdate() 
    {
        if (!JCConch::s_pConch)
        {
            return true;
        }
        bool stop = JCConch::s_pConch->m_semaphoreFramePacer.waitUntilHasData();
        if (stop)
        {
             return true;
        }
        JCConch::s_pConch->m_semaphoreFramePacer.setDataNum(0);
        
        JCConch::s_pConchRender->postTaskFromJSToRenderSync([this]()->bool {
            if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::OpenGLES) {
                JCConch::s_pConchRender->start();
            }
            
            return true;
        }).get();
        
        //PERF_INITVAR(nBenginTime);
#ifdef JS_V8_DEBUGGER
        //auto xxx = m_pScriptThread.get();
        m_pScriptThread->runDbgFuncs();
#endif
        m_nUpdateCount++;
        bool bRunOnDraw = false;
        double nTime = tmGetCurms();

        if (m_pJSOnDrawFunction.isValid())
        {
			
            //JS_TRY;
            m_pJSOnDrawFunction.call<void>(jsvm::global(), nTime);
            //JS_CATCH;

            JCConch::s_pConchRender->postTaskFromJSToRenderSync([this]()->bool {
                this->dispatchLayaGLBuffer(true);
                return true;
            }).get();
			
        }
        JSInput* pInput = JSInput::getInstance();
        if ( pInput->m_bTouchMode && m_pJSTouchEvtFunction.isValid() )
        {
            pInput->swapCurrentTouchEvent();
            if( pInput->m_vInputEventsJS.size() > 0 )
            {
                pInput->m_nTouchFrame = 120;
                for (int i = 0, nSize = (int)pInput->m_vInputEventsJS.size(); i < nSize; i++ )
                {
                    TouchEventInfo* touchEvent = &pInput->m_vInputEventsJS[i];
                    m_pJSTouchEvtFunction.call<void>(jsvm::global(), touchEvent->nType, touchEvent->nID,"type",touchEvent->x, touchEvent->y);
                }
            }
            if( pInput->m_nTouchFrame > 0 )
            {
                pInput->m_nTouchFrame--;
            }
        }

        if (g_bGLCanvasSizeChanged && m_pJSOnResizeFunction.isValid())
        {
            m_pJSOnResizeFunction.call<void>(jsvm::global(), g_nInnerWidth, g_nInnerHeight);
            //m_pRootCanvas->size( g_nInnerWidth,g_nInnerHeight );
            g_bGLCanvasSizeChanged = false;
        }
        int nUpdateNum = m_nUpdateCount % 3;
        switch (nUpdateNum)
        {
        case 0:
            JCAudioManager::GetInstance()->update();
            break;
        case 1:
            //如果有需要清理的或者update可以放到这 
            JCAudioManager::GetInstance()->m_pWavPlayer->autoGarbageCollection();
            break;
        case 2:
            //如果有需要清理的或者update可以放到这
            break;
        }
		
        //JS_TRY;
        if (m_pJSOnFrameFunction.isValid())
        {
            m_pJSOnFrameFunction.call<void>(jsvm::global());
        }
        //JS_CATCH;
		
        //float dt = tmGetCurms() - nBenginTime;
        //PERF_UPDATE_DATA(JCPerfHUD::PHUD_JS_DELAY, (float)dt);

		{
//lvtodo
        GET_ENV
            jsvm::Value script;
        jsvm::Status status;

        status = jsvm::ReportException(env);
        DEBUG_CHECK(status == jsvm::Status::OK);
        DEBUG_CHECK(status == jsvm::Status::OK);
		}
        JCConch::s_pConchRender->postTaskFromJSToRenderSync([this]()->bool {
            JCConch::s_pScriptRuntime->dispatchLayaGLBuffer(false);
            JCConch::s_pConchRender->update();
            if (g_kSystemConfig.m_graphicsAPI == GraphicsAPI::OpenGLES) {
                JCConch::s_pConchRender->end();
            }
            JCConch::s_pConchRender->swapBuffer();
            return true;
        }).get();
        
        return true;
    }

    void JCScriptRuntime::dispatchLayaGLBuffer(bool bDispatchGC)
    {
        JCArrayBufferManager::ArrayBufferContent* pRootCommandEncoder = JSLayaGL::getInstance()->m_pRootCommandEncoder;
        if (!pRootCommandEncoder)return;
        char* pBuffer = pRootCommandEncoder->m_pBuffer;
        int nLen = (*(int*)pBuffer - 1) * 4;
        m_pRenderCmd->setShareBuffer(pBuffer + 4, nLen);
        ((int*)pBuffer)[0] = 1;
        JCLayaGLDispatch::dispatchAllCmds(m_pRenderCmd);
        m_pRenderCmd->clearData();

        JCArrayBufferManager::ArrayBufferContent* frmcnt = JCConch::s_pScriptRuntime->m_pArrayBufferManager->getArrayBuffer(JSLayaGL::getInstance()->m_nFrameAndSyncCountABListID);    
        if(frmcnt){
            char* frmCntBuff = frmcnt->m_pBuffer;
            ((int*)frmCntBuff)[0] ++;
        }
        
    }
    void JCScriptRuntime::clearCmdBuffer()
    {
        JCArrayBufferManager::ArrayBufferContent* pRootCommandEncoder = JSLayaGL::getInstance()->m_pRootCommandEncoder;
        if (!pRootCommandEncoder)return;
        char* pBuffer = pRootCommandEncoder->m_pBuffer;
        ((int*)pBuffer)[0] = 1;
        JCLayaGLDispatch::dispatchAllCmds(m_pRenderCmd);
        m_pRenderCmd->clearData();
    }
    void JCScriptRuntime::onNetworkChanged(int nType)
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::onNetworkChangedCallJSFunction, this, nType);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::onNetworkChangedCallJSFunction(int nType)
    {
        m_pJSNetworkEvtFunction.call<void>(jsvm::global(), nType);
    }
    void JCScriptRuntime::jsGC()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsGCCallJSFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsGCCallJSFunction()
    {
        jsbind::runScript("gc()");
    }
    void JCScriptRuntime::callJC(std::string sFunctionName, std::string sJsonParam, std::string sCallbackFunction)
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::callJSFuncton, this, sFunctionName, sJsonParam, sCallbackFunction );
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::callJSString( std::string sBuffer )
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::callJSStringFunction, this,sBuffer);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::callJSStringFunction( std::string sBuffer )
    {
        jsbind::runScript(sBuffer);
    }
    void JCScriptRuntime::callJSFuncton(std::string sFunctionName, std::string sJsonParam, std::string sCallbackFunction)
    {
        std::string sBuffer = sFunctionName;
        sBuffer += "(\"";
        sBuffer += sJsonParam;
        sBuffer += "\",\"";
        sBuffer += sCallbackFunction;
        sBuffer += "\");";
        LOGI("JCScriptRuntime::callJSFuncton buffer=%s",sBuffer.c_str() );
        jsbind::runScript( sBuffer);
    }
    void JCScriptRuntime::restoreAudio()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsRestoreAudioFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsRestoreAudioFunction()
    {
        if(JCAudioManager::GetInstance()->getMp3Mute() == false && JCAudioManager::GetInstance()->getMp3Stopped() == false)
        {
            JCAudioManager::GetInstance()->resumeMp3();
        }
    }
    void JCScriptRuntime::jsReloadUrl()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsReloadUrlJSFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsReloadUrlJSFunction()
    {
        jsbind::runScript("reloadJS(true)");
    }
    void JCScriptRuntime::jsUrlback()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsUrlbackJSFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsUrlbackJSFunction()
    {
        jsbind::runScript("history.back()");
    }
    void JCScriptRuntime::postToJS(const std::function<void(void)>& func)
    {
        m_pScriptThread->post(func);
    }
    /*void JCScriptRuntime::postToDownload(const std::function<void(void)>& funcf)
    {

    }
    void JCScriptRuntime::postToDecoder(const std::function<void(void)>& func)
    {

    }*/
	void JCScriptRuntime::onBlur()
	{
        if (this->m_pJSOnBlurFunction.isValid())
        {
            this->m_pJSOnBlurFunction.call<void>(jsvm::global());
			
        }
	}
	void JCScriptRuntime::onFocus()
	{
        if (this->m_pJSOnFocusFunction.isValid())
        {
            this->m_pJSOnFocusFunction.call<void>(jsvm::global(), jsbind::Make<JSLaunchOptions*>(new JSLaunchOptions()));
        }
	}

}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------

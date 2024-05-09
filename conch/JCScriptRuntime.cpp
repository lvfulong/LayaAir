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
#include <binder/JSInterface.h>
#include <Bindings/JSFileReader.h>
#include <Bindings/JSGlobalExportCFun.h>
#include <Bindings/JSInput.h>
#include <utils/JCFileSource.h>
#include <resource/JCFileResManager.h>
#include "Audio/JCAudioManager.h"
#include "JCSystemConfig.h"
#include "JCConch.h"
//#include <Performance/JCPerfHUD.h>
#include <downloadMgr/JCDownloadMgr.h>
#include <inttypes.h>
#include <Bindings/JSLayaGL.h>
#include <LayaGL/JCLayaGLDispatch.h>
#include <zip/JCZip.h>
#include <Bindings/JSLaunchOptions.h>
#include <Bindings/JSPromiseRejectionEvent.h>
#include "LayaAir/2D/RenderState2D.h"
#include "LayaAir/2D/Context2D.h"
#include "LayaAir/2D/RenderTexture2D.h"
//#include "btBulletDynamicsCommon.h"
#include <cstdarg>
std::string g_ConfigJS = "";
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
extern laya::JCZip *g_ZipPackage;

namespace laya 
{
#ifdef JS_V8_DEBUGGER
    bool g_bSendLogToDbg = true;

    void mygLayaLog(int level, const char* file, int line, const char* fmt, ...) {
        if (!JCConch::s_pScriptRuntime)
            return;
        DebuggerAgent* pDbgAgent = JCConch::s_pScriptRuntime->m_pDbgAgent;
        if (!g_bSendLogToDbg || !pDbgAgent) {
            va_list args;
            va_start(args, fmt);
            vprintf(fmt, args);
            va_end(args);
            return;
        }
        char buf[1024];
        char* pBuf = NULL;
        va_list args;
        va_start(args, fmt);
        int len = vsnprintf(buf, 1024, fmt, args);
        if (len < 0) {
            printf("log error! \n");
            return;
        }
        if (len > 1024) {
            pBuf = new char[len + 1];
            len = vsnprintf(pBuf, len + 1, fmt, args);
            if (len < 0)
                return;
        }
        va_end(args);
        const char* pTypes[] = { "warning","error", "debug", "log","runtime" };
        int sz = sizeof(pTypes) / sizeof(const char*);
        pDbgAgent->sendToDbgConsole(pBuf ? pBuf : buf, file, line, 0, level < sz ? pTypes[level] : "unknown");
        if (pBuf) {
            delete[] pBuf;
        }
    }

    void mygLayaLogSimp(int level, const char* file, int line, const char* msg) {
        if (!JCConch::s_pScriptRuntime)
            return;
        DebuggerAgent* pDbgAgent = JCConch::s_pScriptRuntime->m_pDbgAgent;
        if (!g_bSendLogToDbg || !pDbgAgent) {
            printf("%s", msg);
            return;
        }
        const char* pTypes[] = { "warning","error", "debug", "log","runtime" };
        int sz = sizeof(pTypes) / sizeof(const char*);
        pDbgAgent->sendToDbgConsole((char*)msg, file, line, 0, level < sz ? pTypes[level] : "unknown");
    }

#endif
    JCScriptRuntime::JCScriptRuntime()
    {
        m_pScriptThread = new JSMulThread();
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
#if !defined(__LINUX__) && !defined(WIN32)
		m_pCurEditBox = NULL;
#endif
    }
    JCScriptRuntime::~JCScriptRuntime() 
    {
        delete m_pScriptThread;
        m_pScriptThread = NULL;

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
    }
    static void onUnhandledRejection(JSValueAsParam pPromise, JSValueAsParam pReason, const char* type)
    {
#ifdef JS_V8
        JSPromiseRejectionEvent* event = new JSPromiseRejectionEvent;
        event->setPromise(pPromise);
        event->setReason(pReason);
        event->setType(type);
        JCConch::s_pScriptRuntime->m_pJSOnUnhandledRejectionFunction.call<void>(getCurrentContext().global(), JSP_TO_JS(JSPromiseRejectionEvent*, event));
#endif
    }
    void JCScriptRuntime::start(const char* pStartJS, int debugPort) 
    {
        LOGI("Start js %s", pStartJS);
        if (pStartJS)m_strStartJS = pStartJS;
        m_debugPort = debugPort;
        m_pScriptThread->initialize(m_debugPort, std::bind(&onUnhandledRejection, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        //m_nThreadState = 1;
        m_pScriptThread->setLoopFunc(std::bind(&JCScriptRuntime::onUpdate, JCConch::s_pScriptRuntime.get()));
        m_pScriptThread->start();
    }
    void JCScriptRuntime::stop()
    {
        LOGI("Stop js start...");
        //while (m_nThreadState==1)
        {
            //LOGI("stop: wait for thread to start...");
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        m_pScriptThread->stop();
        m_pScriptThread->uninitialize();
        LOGI("Stop js end.");
    }
    void JCScriptRuntime::reload() 
    {
        stop();
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
        start(m_strStartJS.c_str(), m_debugPort);
        loadJSScript();
    }
    void JCScriptRuntime::onThreadInit(JCEventEmitter::evtPtr evt) 
    {
        LOGI("js thread started.");

        //m_nThreadState = 2;
        //JCPerfHUD::resetFrame();
#ifdef JS_V8
        //JSObjNode::s_pListJSObj = new JCSimpList();
#ifdef JS_V8_DEBUGGER
        if (m_pDbgAgent) 
        {
            m_pDbgAgent->onJSStart(m_pScriptThread,(g_kSystemConfig.m_nJSDebugMode == JS_DEBUG_MODE_WAIT) ? true : false,[]{
                //gLayaLog = mygLayaLog;
                //gLayaLogNoParam = mygLayaLogSimp;
            },[]{
                //gLayaLog = nullptr;
                //gLayaLogNoParam = nullptr;
            });
            LOGI("js debug open mode: %d port %d", g_kSystemConfig.m_nJSDebugMode, g_kSystemConfig.m_nJSDebugPort);

            gLayaLog = mygLayaLog;
            gLayaLogNoParam = mygLayaLogSimp;
        }
        else
        {
            LOGI("js debug closed");
        }
#endif
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
        

    }
    void JCScriptRuntime::loadJSScript() {
        postToJS([this](){
        //设置js的一些环境。必须在所有导出之后，执行其他脚本之前。
        {
            char* sJSRuntime = NULL;
            int nSize = 0;
            if (m_pAssetsRes->loadFileContent("scripts/runtimeInit.js", sJSRuntime, nSize))
            {
                JSP_RUN_SCRIPT(sJSRuntime);
                delete[] sJSRuntime;
            }
        }
        if (!g_ConfigJS.empty())
        {
            JSP_RUN_SCRIPT(g_ConfigJS.c_str());
        }
        char* sJCBuffer = NULL;
        int nJSSize = 0;
        if (m_pAssetsRes->loadFileContent(m_strStartJS.c_str(), sJCBuffer, nJSSize))
        {
            std::string kBuf = "(function(window){\n'use strict'\n";
            kBuf += sJCBuffer;
            kBuf += "\n})(window);\n//@ sourceURL=apploader.js";
#ifdef JS_V8
            v8::Isolate* isolate = v8::Isolate::GetCurrent();
            v8::HandleScope handle_scope(isolate);
            v8::TryCatch try_catch(isolate);
            JSP_RUN_SCRIPT(kBuf.c_str());
            if (try_catch.HasCaught())
            {
                __JSRun::ReportException(isolate, &try_catch);
            }
#else
            JSP_RUN_SCRIPT(kBuf.c_str());
#endif
            delete[] sJCBuffer;
            sJCBuffer = NULL;
        }
        JSP_RUN_SCRIPT("gc();gc();gc();");
        });
    }
    void JCScriptRuntime::onThreadExit(JCEventEmitter::evtPtr evt)
    {
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
#if !defined(__LINUX__) && !defined(WIN32)
        m_pCurEditBox = NULL;
#endif
        internal::runDeinitializers();

        JSGlobalDisExportC();
#ifdef JS_V8
#ifdef JS_V8_DEBUGGER
        if (m_pDbgAgent)
        {
            m_pDbgAgent->onJSExit();
        }
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
            JCConch::s_pConchRender->start();
            return true;
        }).get();
        
        //PERF_INITVAR(nBenginTime);
#ifdef JS_V8
        m_pScriptThread->runDbgFuncs();
#endif
        m_nUpdateCount++;
        bool bRunOnDraw = false;
        double nTime = tmGetCurms();

        if (!m_pJSOnDrawFunction.isEmpty())
        {
			
            JS_TRY;
            m_pJSOnDrawFunction.call<void>(getCurrentContext().global(), nTime);
            JS_CATCH;

            JCConch::s_pConchRender->postTaskFromJSToRenderSync([this]()->bool {
                this->dispatchLayaGLBuffer(true);
                return true;
            }).get();
			
        }
        JSInput* pInput = JSInput::getInstance();
        if ( pInput->m_bTouchMode )
        {
            pInput->swapCurrentTouchEvent();
            if( pInput->m_vInputEventsJS.size() > 0 )
            {
                pInput->m_nTouchFrame = 120;
                for (int i = 0, nSize = (int)pInput->m_vInputEventsJS.size(); i < nSize; i++ )
                {
                    TouchEventInfo* touchEvent = &pInput->m_vInputEventsJS[i];
                    m_pJSTouchEvtFunction.call<void>(getCurrentContext().global(), touchEvent->nType, touchEvent->nID,"type",touchEvent->x, touchEvent->y);
                }
            }
            if( pInput->m_nTouchFrame > 0 )
            {
                pInput->m_nTouchFrame--;
            }
        }

        if (g_bGLCanvasSizeChanged)
        {
            m_pJSOnResizeFunction.call<void>(getCurrentContext().global(), g_nInnerWidth, g_nInnerHeight);
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
		
        JS_TRY;
            m_pJSOnFrameFunction.call<void>(getCurrentContext().global());
        JS_CATCH;
		
        //float dt = tmGetCurms() - nBenginTime;
        //PERF_UPDATE_DATA(JCPerfHUD::PHUD_JS_DELAY, (float)dt);

        JCConch::s_pConchRender->postTaskFromJSToRenderSync([this]()->bool {
            JCConch::s_pScriptRuntime->dispatchLayaGLBuffer(false);
            JCConch::s_pConchRender->update();
            JCConch::s_pConchRender->end();
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
        m_pJSNetworkEvtFunction.call<void>(getCurrentContext().global(), nType);
    }
    void JCScriptRuntime::jsGC()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsGCCallJSFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsGCCallJSFunction()
    {
        JSP_RUN_SCRIPT("gc()");
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
        JSP_RUN_SCRIPT(sBuffer.c_str());
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
        JSP_RUN_SCRIPT( sBuffer.c_str() );
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
        JSP_RUN_SCRIPT("reloadJS(true)");
    }
    void JCScriptRuntime::jsUrlback()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsUrlbackJSFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsUrlbackJSFunction()
    {
        JSP_RUN_SCRIPT("history.back()");
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
        if (!this->m_pJSOnBlurFunction.isEmpty())
        {
            this->m_pJSOnBlurFunction.call<void>(getCurrentContext().global());
			
        }
	}
	void JCScriptRuntime::onFocus()
	{
        if (!this->m_pJSOnFocusFunction.isEmpty())
        {
            this->m_pJSOnFocusFunction.call<void>(getCurrentContext().global(), JSP_TO_JS(JSLaunchOptions*, new JSLaunchOptions()));
        }
	}

}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------

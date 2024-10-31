/**
@file			JCScriptRuntime.h
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/
 
#ifndef __JCScriptRuntime_H__
#define __JCScriptRuntime_H__

#include <binder/JSInterface.h>
#include <utils/JCLayaUrl.h>
#include <manager/JCArrayBufferManager.h>
#include <mutex>
#include <utils/JCCommonMethod.h>
#include <vector>
#include "ScriptThread/ScriptThread.h"
#if defined(OS_ANDROID)
    #include <Bindings/JSAndroidEditBox.h>
#elif defined(OS_OHOS)
    #include <Bindings/JSOHOSEditBox.h>   
#elif defined(OS_IOS)
    #include <Bindings/JSIOSEditBox.h>
#endif

namespace laya
{
    enum NetworkType 
    {
        NET_NO = 0,
        NET_WIFI = 1,
        NET_2G = 2,
        NET_3G = 3,
        NET_4G = 4,
        NET_YES = 5,
    };

#ifdef JS_V8
    class DebuggerAgent;
#endif
    class JCConch;
	class JSThreadInterface;
    class JSLayaGL;
    class JCFileResManager;
    class JCFileSource;
    class JCScriptRuntime
    {
    public:
        
        JCScriptRuntime();

        ~JCScriptRuntime();

        /** @brief
                初始化js环境。
         *  @param[in] pFileMgr
         *  @param[in] pAssetRes
         *  @param[in] pThreadCmdSender 跨线程发送消息的统一管理。避免各个线程直接发送消息导，以简化退出流程。
         *  @return void
        */
        void init(JCFileResManager* pFileMgr, JCFileSource* pAssetRes, JCConch* pConch);

        void start(const char* pStartJS);

        void stop();

        void reload();

        void onThreadInit(JCEventEmitter::evtPtr evt);

        bool onUpdate();
        
        void onUpdateTimer();

        void clearCmdBuffer();

        void dispatchLayaGLBuffer(bool bDispatchGC);

        //输入事件触发js
        void onUpdateInput();

        void onThreadExit(JCEventEmitter::evtPtr evt);

        void loadJSScript();

    public:

        void jsGC();
        void jsGCCallJSFunction();

        void callJSString( std::string sBuffer );
        void callJSStringFunction( std::string sBuffer );

        void jsReloadUrl();
        void jsReloadUrlJSFunction();

        void jsUrlback();
        void jsUrlbackJSFunction();

        void restoreAudio();
        void jsRestoreAudioFunction();

        void onNetworkChanged(int nType);
        void onNetworkChangedCallJSFunction(int nType);

        void callJC( std::string sFunctionName,std::string sJsonParam,std::string sCallbackFunction );
        void callJSFuncton(std::string sFunctionName, std::string sJsonParam, std::string sCallbackFunction);

		void onBlur();

		void onFocus();

    public:

        void postToJS(const std::function<void(void)>& func);

        //void postToDownload(const std::function<void(void)>& funcf);

        //void postToDecoder(const std::function<void(void)>& func);
    public:
        JCConch*                            m_pConch;
        std::unique_ptr<ScriptThread>       m_pScriptThread;
        JCCommandEncoderBuffer*				m_pRenderCmd;                   
		bool                                m_bHasJSThread;	                //js线程是否在工作
        jsbind::Persistent                         m_pJSOnFrameFunction;
        jsbind::Persistent                         m_pJSOnDrawFunction;            //垂直回扫同步
        jsbind::Persistent                         m_pJSOnResizeFunction;
        jsbind::Persistent                         m_pJSOnBlurFunction;
        jsbind::Persistent                         m_pJSOnFocusFunction;
        jsbind::Persistent                         m_pJSMouseEvtFunction;          //鼠标事件回调
        jsbind::Persistent                         m_pJSKeyEvtFunction;
        jsbind::Persistent                         m_pJSTouchEvtFunction;
        jsbind::Persistent                         m_pJSDeviceMotionEvtFunction;   //重力感应
        jsbind::Persistent                         m_pJSOnceOtherEvtFuction;       //注册一次的事件 如截屏
        jsbind::Persistent					        m_pJSNetworkEvtFunction;        //网络事件的监听
        jsbind::Persistent                         m_pJSOnUnhandledRejectionFunction;
        jsbind::Persistent                         m_bJSBulletGetWorldTransformHandle;
        jsbind::Persistent                         m_bJSBulletSetWorldTransformHandle;
        jsbind::Persistent                         m_bJSBulletDrawLineHandle;
        jsbind::Persistent                         m_bJSBulletClearLineHandle;
        jsbind::Persistent                         m_pJSZipPackage;
        jsbind::Persistent                         m_pJSOnScreenOrientationChanged;
        jsbind::Persistent                         m_pJSSetGlobalRepaintFunction;
        std::string                         m_strStartJS;
        JCFileResManager*                   m_pFileResMgr;                  //外部设置的。本地不允许删除
        JCFileSource*                       m_pAssetsRes;	                //外部设置的。本地不允许删除
        bool                                m_bIsExit;
        char*                               m_pOtherBufferSharedWidthJS;
        int							        m_nUpdateCount;                 //update次数
        JCUrl*                              m_pUrl;
        int                                 m_debugPort;
        /*
         * 确保线程已经开始了。有时候想结束线程，但是实际线程还没起来（结束请求太快）
         * 确保线程已经开始了。有时候想结束线程，但是实际线程还没起来（结束请求太快）
         * 需要靠这个状态来判断是否已经起来了。防止stop之后，线程才起来，导致混乱。
        */
        //int                                 m_nThreadState;                 
        JCArrayBufferManager*               m_pArrayBufferManager;          ///<ArrayBufferManager
#if defined(OS_ANDROID)
        JSAndroidEditBox*                   m_pCurEditBox;
#elif defined(OS_OHOS)
        JSOHOSEditBox*                      m_pCurEditBox;
#elif defined(OS_IOS)
        JSIOSEditBox *                      m_pCurEditBox;
#endif
#ifdef JS_V8
        DebuggerAgent*				        m_pDbgAgent;
#endif
    };
}

//------------------------------------------------------------------------------


#endif //__JCScriptRuntime_H__

//-----------------------------END FILE--------------------------------

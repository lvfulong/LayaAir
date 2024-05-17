/**
@file			JCConch.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JCConch_H__
#define __JCConch_H__

#include <vector>
#include <utils/JCCommonMethod.h>
#include "JCConchRender.h"
#include "WebGL/WebGLThread.h"
#include "Backend.h"
#include <utils/InputTypes.h>

namespace laya
{


    enum JS_DEBUG_MODE
    {
        JS_DEBUG_MODE_OFF = 0,      //关闭
        JS_DEBUG_MODE_NORMAL,       //正常
        JS_DEBUG_MODE_WAIT,         //等待
    };
    class JSThreadInterface;
    class JCFileResManager;
	class JCFileSource;
	class DebuggerAgent;
    class JSMulThread;
    class JCScriptRuntime;
	class JCConch
	{
	public:

		JCConch();

		~JCConch();
        
		void onAppStart();

        void onAppDestroy();
        
        void reload();

        int urlHistoryLength();

        void urlBack();

        void urlGo(int s);

        void urlForward();

        void urlHistoryPush(const char* sUrl);

		void exit();

		const char* getLocalStoragePath() 
        {
            return m_strLocalStoragePath.c_str(); 
        }
        void update();

        void postToPlatform(std::function<void(void)> task);

		void onAppPause();

		void onAppResume();

        void dispatchInputEvent(inputEvent e);

        void dispatchInputEvent(DeviceOrientationEvent e);

        void dispatchInputEvent(DeviceMotionEvent e);

    public:

        static std::shared_ptr<JCConch>         s_pConch;
        static int64_t			                s_nUpdateTime;
        static std::shared_ptr<JCConchRender>	s_pConchRender;
        std::string				                m_strLocalStoragePath;
        static std::shared_ptr<JCScriptRuntime> s_pScriptRuntime;
        static JCFileSource*	                s_pAssetsFiles;
        JCFileSource*			                m_pAssetsRes;
        std::string				                m_strStartJS;
        std::string                             m_sCachePath;
    #ifdef JS_V8
        DebuggerAgent*			                m_pDbgAgent;
    #endif
        JCFileResManager*	                    m_pFileResMgr;
    protected:
        bool					                m_bDestroying;
        std::vector<std::string>                m_vUrlHistory;
        int                                     m_nUrlHistoryPos;
    public:

        std::vector<std::function<void(void)>>  m_tasks;
        std::mutex                              m_mutex;
        bool                                    m_isAppStarted = { false };
        JCWorkSemaphore                         m_semaphoreFramePacer;
        JCWorkSemaphore                         m_semaphore;
	};

    void postToPlatform(std::function<void(void)> task);

    bool isInJSThread();
	void postToJS(std::function<void(void)> task);
};
//------------------------------------------------------------------------------


#endif //__JCConch_H__

//-----------------------------END FILE--------------------------------

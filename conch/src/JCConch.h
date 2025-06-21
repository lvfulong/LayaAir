#ifndef __JCConch_H__
#define __JCConch_H__

#include <vector>
#include <utils/JCCommonMethod.h>
#include "JCConchRender.h"
#include "WebGL/WebGLThread.h"
#include <utils/InputTypes.h>
#include "EngineEvent.h"
#include <memory>
#include <utils/MessageLoop.h>
#include <core/Thread.h>
#include <platform/OS.h>
#include <webstorage/WebStorage.h>
#include <audio/AudioPlayer.h>

namespace laya
{


    enum JS_DEBUG_MODE
    {
        JS_DEBUG_MODE_OFF = 0,      //关闭
        JS_DEBUG_MODE_NORMAL,       //正常
        JS_DEBUG_MODE_WAIT,         //等待
    };
    class JCFileResManager;
	class JCFileSource;
    class JCScriptRuntime;
    class OS;
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

		static const std::string& getLocalStoragePath()
        {
            return s_localStoragePath;
        }
        static const std::string& getAppCachePath()
        {
            return s_cachePath;
        }
        void update();

		void onAppPause();

		void onAppResume();

        void dispatchInputEvent(inputEvent e);

        void dispatchInputEvent(DeviceOrientationEvent e);

        void dispatchInputEvent(DeviceMotionEvent e);

        void dispatchEngineEvent(const EngineEventBase&  e);

        LocalStorage& getLocalStorage()
        {
            //惰性初始化
            if (!m_pLocalStorage)
            {
                m_pLocalStorage = std::make_unique<LocalStorage>();
                m_pLocalStorage->initialize(getLocalStoragePath());
            }   
            return *m_pLocalStorage;
        }
        audio::AudioPlayer& getAudioPlayer()
        {
            if (!m_pAudioPlayer)
            {
                m_pAudioPlayer = std::make_unique<audio::AudioPlayer>();
            }
            return *m_pAudioPlayer;
        }
    public:

        static std::shared_ptr<JCConch>         s_pConch;
        static int64_t			                s_nUpdateTime;
        static std::shared_ptr<JCConchRender>	s_pConchRender;
        static std::string				        s_localStoragePath;
        static std::shared_ptr<JCScriptRuntime> s_pScriptRuntime;
        static JCFileSource*	                s_pAssetsFiles;
        JCFileSource*			                m_pAssetsRes;
        std::string				                m_strStartJS;
        static std::string                      s_cachePath;
        JCFileResManager*	                    m_pFileResMgr;
    protected:
        bool					                m_bDestroying;
        std::vector<std::string>                m_vUrlHistory;
        int                                     m_nUrlHistoryPos;
        std::unique_ptr<LocalStorage>           m_pLocalStorage{nullptr};
        std::unique_ptr<audio::AudioPlayer>            m_pAudioPlayer{nullptr};
    public:

        std::vector<std::function<void(void)>>  m_tasks;
        std::mutex                              m_mutex;
        bool                                    m_isAppStarted = { false };
    private:
        void _realReload();
	};
};
#endif

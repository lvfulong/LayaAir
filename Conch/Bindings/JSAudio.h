#ifndef __JSAudio_H__
#define __JSAudio_H__


#include <stdio.h>
#include <binder/JSInterface.h>
#include "resource/Audio/JCAudioInterface.h"
#include <utils/JCBuffer.h>


namespace laya 
{
    class OpenALSourceInfo;
    class JSAudio : public JCAudioInterface
    {
    public:
		enum EXT_TYPE
		{
			EXT_INVALID = -1,
			EXT_OGG = 0,
			EXT_WAV,
			EXT_MP3,
		};

		enum EXT_STATE
		{
			EXT_STATE_INVALID = -1,
			EXT_STATE_PLAY = 0,
			EXT_STATE_PAUSE,
			EXT_STATE_STOP,
			EXT_STATE_PLAY_END,
		};

	    static void exportJS(Context& context);

	    JSAudio();

	    ~JSAudio();

    public:

	    void setAutoPlay( bool p_bAutoPlay );

	    bool getAutoPlay();

	    void setLoop( bool p_bLoop );

	    bool getLoop();

	    void setMuted( bool p_bMuted );

	    bool getMuted();

	    void setSrc( const char* p_sSrc );

	    const char* getSrc();

	    void setVolume( float p_nVolume );

	    float getVolume();

        void setCurrentTime(float nCurrentTime);

        float getCurrentTime();

		void setIsBackgroundMusic(bool p_bIsBackgroundMusic);

		bool getIsBackgroundMusic();

    public:

	    void addEventListener( const char* p_sName, JSValueAsParam p_pFunction );

	    void play();

	    void pause();

	    void stop();

	    //下载线程的回调
	    bool onDownloaded( void* p_pRes, std::weak_ptr<int> callbackref );

	    bool onDownloadErr(void* p_pRes, int p_nErrCode, std::weak_ptr<int> callbackref);

    public:

	    void onPlayEnd();

        void onCanplayCallJSFunction(std::weak_ptr<int> callbackref);

	    void onPlayEndCallJSFunction(std::weak_ptr<int> callbackref);

	    void onErrorCallJSFunction(int p_nErrCode,std::weak_ptr<int> callbackref);

    public:

        bool			        m_bDownloaded;			//是否下载完成	

		EXT_TYPE				m_nType = EXT_INVALID;	//类型 

        bool			        m_bAutoPlay;			//是否自动播放

        bool			        m_bLoop;				//是否循环

        bool			        m_bMuted;				//是否静音

        float                   m_nCurrentTime;         //播放到的时间

        std::string		        m_sSrc;					//src

        float                   m_nVolume;				//音量

        std::string		        m_sLocalFileName;

	    std::shared_ptr<int>	m_CallbackRef;

    public:

        Persistent		    m_pJSFunctionAudioEnd;	//JS的回调
        Persistent         m_pJSFunctionCanPlay;   //JS的回调
        Persistent         m_pJSFunctionError;     //JS的回调
        OpenALSourceInfo*   m_pOpenALInfo;          //openAL的指针

    private:

        bool			        m_bNeedHandlePlay;

		bool					m_bIsBackgroundMusic = false;

		int						m_nState = EXT_STATE_INVALID;
    };
}

#endif
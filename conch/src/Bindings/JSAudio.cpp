#include "JSAudio.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include <downloadMgr/JCDownloadMgr.h>
#include <utils/JCMemorySurvey.h>
#include <utils/JCFileSystem.h>
#include <resource/JCFileResManager.h>
#include <utils/JCLayaUrl.h>
#include <utils/JCCommonMethod.h>
#include "JSFile.h"
#include "../../JCScriptRuntime.h"
#include <functional>
#include "../../Audio/JCAudioManager.h"
#include "../../JCSystemConfig.h"
#include "JCConch.h"

namespace laya
{
	//为了确保mp3只被写入文件一次
    std::map<std::string,std::string>	ms_vSaveMp3File;
    //------------------------------------------------------------------------------
    JSAudio::JSAudio()
    {
        m_nCurrentTime = 0;
	    m_bNeedHandlePlay = false;
	    m_bAutoPlay = false;
	    m_bLoop = false;
	    m_bMuted = false;
	    m_nVolume = 1;
	    m_sSrc = "";
	    m_sLocalFileName = "";
	    m_bDownloaded = false;
        m_pOpenALInfo = NULL;
		m_fDuration = std::numeric_limits<double>::quiet_NaN();
	    AdjustAmountOfExternalAllocatedMemory( 534 );
	    JCMemorySurvey::GetInstance()->newClass( "audio",534,this );
	    m_CallbackRef.reset(new int(1));
    }
    //------------------------------------------------------------------------------
    JSAudio::~JSAudio()
    {
        JCMemorySurvey::GetInstance()->releaseClass( "audio",this );
	    JCAudioManager::GetInstance()->delWav(this);
	    JCAudioManager::GetInstance()->delMp3Obj(this);
    }
    //------------------------------------------------------------------------------
    void JSAudio::addEventListener( const char* p_sName, JSValueAsParam p_pFunction )
    {
	    if( strcmp( p_sName,"ended" ) == 0 )
	    {
		    m_pJSFunctionAudioEnd.reset(p_pFunction);
	    }
	    else if(strcmp(p_sName,"canplaythrough")==0)
	    {
		    m_pJSFunctionCanPlay.reset(p_pFunction);
	    }
	    else if (strcmp(p_sName, "error") == 0)
	    {
		    m_pJSFunctionError.reset(p_pFunction);
	    }
        else
        {
            LOGW("JSAudio::addEventListener(%s)尚未支持",p_sName );
        }
    }
    //------------------------------------------------------------------------------
    void JSAudio::setAutoPlay( bool p_bAutoPlay )
    {
	    m_bAutoPlay = p_bAutoPlay;
    }
    //------------------------------------------------------------------------------
    bool JSAudio::getAutoPlay()
    {
	    return m_bAutoPlay;
    }
    //------------------------------------------------------------------------------
    void JSAudio::setLoop( bool p_bLoop )
    {
	    m_bLoop = p_bLoop;
    }
    //------------------------------------------------------------------------------
    bool JSAudio::getLoop()
    {
	    return m_bLoop;
    }
    //------------------------------------------------------------------------------
    void JSAudio::setMuted( bool p_bMuted )
    {
	    m_bMuted = p_bMuted;
		if (m_nType == EXT_INVALID)
		{
			return;
		}
		/*if (m_nType == EXT_MP3 && m_bIsBackgroundMusic)
	    {
		    JCAudioManager::GetInstance()->setMp3Mute( m_bMuted );
	    }
	    else*/
	    {
            if (m_pOpenALInfo && m_pOpenALInfo->m_pAudio == this)
            {
                JCAudioManager::GetInstance()->setWavVolume(m_pOpenALInfo, m_bMuted ? 0 : m_nVolume);
            }
	    }
    }
    //------------------------------------------------------------------------------
    bool JSAudio::getMuted()
    {
	    return m_bMuted;
    }
    //------------------------------------------------------------------------------
    void JSAudio::setSrc( const char* p_sSrc )
    {
	    //如果和原来播放的一样，直接播放就行了
        std::string sSrc = p_sSrc;
	    if( m_sSrc == sSrc )
	    {
            std::weak_ptr<int> cbref(m_CallbackRef);
            auto pFunction = std::bind(&JSAudio::onCanplayCallJSFunction,this, cbref);
            postToJS( pFunction );
		    if( m_bAutoPlay == true )
		    {
			    //直接播放
			    play();
		    }
		    return;
	    }
	    //这段是第一次播放此音乐，先下载走正规流程
	    m_sSrc = sSrc;
	    //为了android
	    m_sSrc.at(0) = m_sSrc.at(0);


		//去掉?后面的，因为可能增加版本号
		std::string sTemp = m_sSrc;
		int p3 = sTemp.rfind('?');
		//去下载文件
		char* sT = (char*)(sTemp.c_str());
		if (p3 != -1)
		{
			sT[p3] = 0;
		}
		char* sExtName = (char*)(getExtName(LayaStrlwr(sT)));
		if (strcmp(sExtName, "mp3") == 0)
		{
			m_nType = EXT_MP3;
		}
		else if (strcmp(sExtName, "wav") == 0)
		{
			m_nType = EXT_WAV;
		}
		else if (strcmp(sExtName, "ogg") == 0)
		{
			m_nType = EXT_OGG;
		}
		else
		{
			m_nType = EXT_INVALID;
			LOGW("JSAudio::setSrc extname != mp3 && extname != wav && exname != ogg");
			return;
		}


	    //这段代码是查找，是否在缓存中已经有此音乐了，直接找到播放
	    JCWaveInfo* pWavInfo = JCAudioManager::GetInstance()->FindWaveInfo( p_sSrc );
	    if( pWavInfo != NULL )
	    {
			//设置一下这个值，防止pWavInfo被垃圾 回收以后无法重建
			m_sLocalFileName = pWavInfo->m_sLocalFile;
		    m_bDownloaded = true;
			m_fDuration = pWavInfo->m_fDuration;
            std::weak_ptr<int> cbref(m_CallbackRef);
            std::function<void(void)> pFunction = std::bind(&JSAudio::onCanplayCallJSFunction,this, cbref);
            postToJS( pFunction );
		    if( m_bAutoPlay == true )
		    {
			    play();
		    }
		    return;
	    }
	    
	    if(m_nType == EXT_MP3)
	    {
		    //必须确保 如果是mp3必须每次都new JSAudio这个类，否则会出问题
		    //临时用的这种办法，现在不好，因为有二次写文件的事情。。
		    //TODO以后得修改
		    std::map<std::string,std::string>::iterator iter = ms_vSaveMp3File.find( m_sSrc );
		    if( iter == ms_vSaveMp3File.end() )
		    {
			    laya::JCFileRes* res = JCConch::s_pScriptRuntime->m_pFileResMgr->getRes(m_sSrc);
			    std::weak_ptr<int> cbref(m_CallbackRef);
			    res->setOnReadyCB( std::bind(&JSAudio::onDownloaded,this, std::placeholders::_1,cbref));
			    res->setOnErrorCB(std::bind(&JSAudio::onDownloadErr, this, std::placeholders::_1, std::placeholders::_2, cbref));
		    }
		    else
		    {
			    m_sLocalFileName = iter->second;
				std::error_code error;
			    if(fs::exists(m_sLocalFileName.c_str(), error))
                {
				    m_bDownloaded = true;
                    std::weak_ptr<int> cbref(m_CallbackRef);
                    auto pFunction = std::bind(&JSAudio::onCanplayCallJSFunction,this, cbref);
                    postToJS( pFunction );
				    //if( m_nType == 0 )
				    {
					    if( m_bAutoPlay || m_bNeedHandlePlay == true )
					    {
						    m_bNeedHandlePlay = false;
							if(!m_bShouldStop)
								play();
					    }
				    }
			    }
                else
                {
				    LOGW("JSAudio::setSrc music 当前文件不存在,%s",m_sLocalFileName.c_str());
				    ms_vSaveMp3File.erase(iter);
				    laya::JCFileRes* res = JCConch::s_pScriptRuntime->m_pFileResMgr->getRes(m_sSrc);

				    std::weak_ptr<int> cbref(m_CallbackRef);
				    res->setOnReadyCB( std::bind(&JSAudio::onDownloaded,this, std::placeholders::_1,cbref));
				    res->setOnErrorCB(std::bind(&JSAudio::onDownloadErr, this, std::placeholders::_1, std::placeholders::_2, cbref));
			    }
		    }
	    }
    }
    //------------------------------------------------------------------------------
    bool JSAudio::onDownloadErr(void* p_pRes, int p_nErrCode,std::weak_ptr<int> callbackref)
    {
	    if (!callbackref.lock())return false;
	    auto pFunction = std::bind(&JSAudio::onErrorCallJSFunction, this, p_nErrCode,callbackref);
        postToJS(pFunction);
	    return true;
    }
    bool JSAudio::onDownloaded(void* p_pRes, std::weak_ptr<int> callbackref)
    {
	    if( !callbackref.lock() )return false;
	    laya::JCResStateDispatcher* pRes = (laya::JCResStateDispatcher*)p_pRes;
	    laya::JCFileRes* pFileRes = (laya::JCFileRes*)pRes;
	    if( pFileRes->m_pBuffer.get()==NULL || pFileRes->m_nLength==0 )
        {
		    return false;
	    }
	    JCBuffer p_buf;
	    p_buf.m_pPtr=pFileRes->m_pBuffer.get();
	    p_buf.m_nLen = pFileRes->m_nLength;
	    m_bDownloaded = true;
       // std::weak_ptr<int> cbref(m_CallbackRef);
        //std::function<void(void)> pFunction = std::bind(&JSAudio::onCanplayCallJSFunction,this, callbackref);
		if( m_nType == EXT_MP3)
	    {

                //必须确保 如果是mp3必须每次都new JSAudio这个类，否则会出问题
                //临时用的这种办法，现在不好，因为有二次写文件的事情。。
                //TODO以后得修改
                std::map<std::string, std::string>::iterator iter = ms_vSaveMp3File.find(m_sSrc);
                if (iter != ms_vSaveMp3File.end())
                {
                    m_sLocalFileName = iter->second;
                }
                else
                {
                    int p1 = m_sSrc.rfind('/');
                    int p2 = m_sSrc.rfind('\\');
                    int pos = std::max<int>(p1, p2);
                    std::string audiofile = m_sSrc.substr(pos + 1, m_sSrc.length());
                    //去掉?后面的，因为可能增加版本号
                    int p3 = audiofile.rfind('?');
                    char* sT = (char*)(audiofile.c_str());
                    if (p3 != -1)
                    {
                        sT[p3] = 0;
                    }
                    //unsigned int hash = JCCachedFileSys::hashRaw(m_sSrc.c_str());
                    //char tmpBuf[32];
                    //sprintf(tmpBuf, "%x_", hash);
                    //m_sLocalFileName = JCConch::s_pScriptRuntime->m_pFileResMgr->m_pFileCache->getAppPath() + "/" + tmpBuf + audiofile;
#ifdef WIN32
					//windows下可以直接使用这个文件
					// 不行，需要扩展名
					//m_sLocalFileName = pFileRes->m_strLocalPath;
#else
#endif
					if (m_sLocalFileName.length() <= 0) {
						char tmpBuf[32];
						sprintf(tmpBuf, "%x_%x",pFileRes->m_nLength, rand());
						const char* local = tmpBuf;
						m_sLocalFileName = JCFileResManager::getAppCachePath() + "/" + local + audiofile;
						writeFileSync(m_sLocalFileName.c_str(), p_buf);
					}
					
                    ms_vSaveMp3File[m_sSrc] = m_sLocalFileName;
                }
	    }

		JCWaveInfo* info=nullptr;
	    /*if( m_nType == EXT_MP3 && m_bIsBackgroundMusic)
	    {
		    if( m_bAutoPlay || m_bNeedHandlePlay == true )
		    {
			    m_bNeedHandlePlay = false;
			    play();
		    }
	    }
		else */
		if (m_nType == EXT_MP3){
			info = JCAudioManager::GetInstance()->AddWaveInfoMp3(m_sSrc, m_sLocalFileName.c_str(), this);
		}else{
		    info = JCAudioManager::GetInstance()->AddWaveInfo( m_sSrc,p_buf,(int)(p_buf.m_nLen),this, m_nType == EXT_OGG);
	    }
		if(info){
			m_fDuration = info->m_fDuration;

			auto pFunction = std::bind(&JSAudio::onCanplayCallJSFunction, this, callbackref);
			postToJS( pFunction );

		    if( m_bAutoPlay || m_bNeedHandlePlay == true ){
			    m_bNeedHandlePlay = false;
				if(!m_bShouldStop)
					play();
		    }
		}
	    return true;
    }
    //------------------------------------------------------------------------------
    const char* JSAudio::getSrc()
    {
	    return m_sSrc.c_str();
    }
    //------------------------------------------------------------------------------
    void JSAudio::setVolume( float p_nVolume )
    {
        if (p_nVolume > 1)p_nVolume = 1;
        if (p_nVolume < 0)p_nVolume = 0;
	    m_nVolume = p_nVolume;
		if (m_nType == EXT_INVALID)
		{
			return;
		}
		/*if (m_nType == EXT_MP3 && m_bIsBackgroundMusic)
	    {
		    JCAudioManager::GetInstance()->setMp3Volume( m_nVolume );
	    }
	    else*/
	    {
            if (m_pOpenALInfo && m_pOpenALInfo->m_pAudio == this)
            {
                JCAudioManager::GetInstance()->setWavVolume(m_pOpenALInfo,m_nVolume);
            }
	    }
    }
    //------------------------------------------------------------------------------
    float JSAudio::getVolume()
    {
	    return m_nVolume;
    }
    //------------------------------------------------------------------------------

	float JSAudio::getDuration(){
		return m_fDuration;
	}

    void JSAudio::play()
    {
        //if (m_bMuted)return;
	    if( m_bDownloaded == false )
	    {
		    m_bNeedHandlePlay = true;
		    return;
	    }
		if (m_nType == EXT_INVALID)
		{
			return;
		}
		if (m_nState == EXT_STATE_PLAY)
		{
			return;
		}
		m_nState = EXT_STATE_PLAY;
		/*if (m_nType == EXT_MP3 && m_bIsBackgroundMusic)
        {
            JCAudioManager::GetInstance()->stopMp3();
            if (g_kSystemConfig.m_bUseDcc)
            {
                if (m_sLocalFileName.length() > 0)
                {
                    JCAudioManager::GetInstance()->playMp3(m_sLocalFileName.c_str(), m_bLoop ? -1 : 0, (int)m_nCurrentTime, this);
                }
                else
                {
                    JCAudioManager::GetInstance()->playMp3(m_sSrc.c_str(), m_bLoop ? -1 : 0, (int)m_nCurrentTime, this);
                }
            }
            else
            {
                JCAudioManager::GetInstance()->playMp3(m_sLocalFileName.c_str(), m_bLoop ? -1 : 0, (int)m_nCurrentTime, this);
            }
	    }
	    else */if (m_nType == EXT_MP3/* && !m_bIsBackgroundMusic*/)
	    {			
#if !defined(__LINUX__)
		    m_pOpenALInfo = JCAudioManager::GetInstance()->playWavMp3( this, m_sSrc, m_sLocalFileName.c_str(), m_nCurrentTime);
#endif
	    }
		else
		{
			m_pOpenALInfo = JCAudioManager::GetInstance()->playWav(this, m_sSrc, m_nType == EXT_OGG, m_nCurrentTime);
		}

		if (m_pOpenALInfo){
			if (m_bMuted) {
				JCAudioManager::GetInstance()->setWavVolume(m_pOpenALInfo, 0);
			}
			else {
				JCAudioManager::GetInstance()->setWavVolume(m_pOpenALInfo, m_nVolume);
			}
		}

    }
    //------------------------------------------------------------------------------
    void JSAudio::pause()
    {


			
		if (m_nType == EXT_INVALID)
		{
			return;
		}
		
		/*if (m_nType == EXT_MP3 && m_bIsBackgroundMusic)
	    {
		    JCAudioManager::GetInstance()->pauseMp3();
	    }*/
        else
        {
			if (m_nState != EXT_STATE_PLAY)
			{
				return;
			}

			m_nState = EXT_STATE_PAUSE;

            if (m_pOpenALInfo && m_pOpenALInfo->m_pAudio == this)
            {
				m_nCurrentTime = getCurrentTime();
                JCAudioManager::GetInstance()->stopWav(m_pOpenALInfo);
                m_pOpenALInfo = NULL;
            }
        }
    }
    //------------------------------------------------------------------------------
    void JSAudio::stop()
    {
		if (m_nType == EXT_INVALID)
		{
			return;
		}
		/*if (m_nType == EXT_MP3 && m_bIsBackgroundMusic)
	    {
		    JCAudioManager::GetInstance()->stopMp3();
	    }*/
        else
        {
			if (m_nState != EXT_STATE_PLAY)
			{
				m_bShouldStop = true;
				return;
			}
			m_bShouldStop = false;//正确执行stop了，不需要记录了
			m_nState = EXT_STATE_STOP;
            if (m_pOpenALInfo && m_pOpenALInfo->m_pAudio == this)
            {
				m_nCurrentTime = getCurrentTime();
                JCAudioManager::GetInstance()->stopWav(m_pOpenALInfo);
                m_pOpenALInfo = NULL;
            }
        }
    }
    void JSAudio::setCurrentTime(float nCurrentTime)
    {
        m_nCurrentTime = nCurrentTime;
    }
    float JSAudio::getCurrentTime()
    {
		if (m_pOpenALInfo && m_pOpenALInfo->m_pAudio == this)
		{
            return JCAudioManager::GetInstance()->getCurrentTime(m_pOpenALInfo);
        }
		else if (m_nState == EXT_STATE_STOP || m_nState == EXT_STATE_PAUSE)
		{
			return m_nCurrentTime;
		}
        return 0;
    }
    //------------------------------------------------------------------------------
    void JSAudio::onPlayEnd()
    {
		m_nState = EXT_STATE_PLAY_END;
	    std::weak_ptr<int> cbref(m_CallbackRef);
	    auto pFunction = std::bind(&JSAudio::onPlayEndCallJSFunction,this, cbref);
        postToJS( pFunction );
    }
    //------------------------------------------------------------------------------
    void JSAudio::onPlayEndCallJSFunction( std::weak_ptr<int> callbackref)
    {
	    if( !callbackref.lock())return;
	    m_pJSFunctionAudioEnd.call<void>(toLocal(this));
    }
    //------------------------------------------------------------------------------
    void JSAudio::onCanplayCallJSFunction( std::weak_ptr<int> callbackref)
    {
        if( !callbackref.lock())
            return;
        m_pJSFunctionCanPlay.call<void>(toLocal(this));
    }
    void JSAudio::onErrorCallJSFunction(int p_nErrorCode,std::weak_ptr<int> callbackref)
    {
	    if (!callbackref.lock())return;
	    m_pJSFunctionError.call<void>(toLocal(this), p_nErrorCode);
    }
	void JSAudio::setIsBackgroundMusic(bool p_bIsBackgroundMusic)
	{
		m_bIsBackgroundMusic = p_bIsBackgroundMusic;
	}

	bool JSAudio::getIsBackgroundMusic()
	{
		return m_bIsBackgroundMusic;
	}
    void JSAudio::exportJS(Context& context) 
    {
		class_<JSAudio> class_binding;
        class_binding.property("autoplay", &JSAudio::getAutoPlay, &JSAudio::setAutoPlay);
        class_binding.property("loop", &JSAudio::getLoop, &JSAudio::setLoop);
        class_binding.property("muted", &JSAudio::getMuted, &JSAudio::setMuted);
        class_binding.property("src", &JSAudio::getSrc, &JSAudio::setSrc);
        class_binding.property("volume", &JSAudio::getVolume, &JSAudio::setVolume);
		class_binding.property("duration",&JSAudio::getDuration);
        class_binding.property("currentTime", &JSAudio::getCurrentTime, &JSAudio::setCurrentTime);
		class_binding.property("isBackgroundMusic", &JSAudio::getIsBackgroundMusic, &JSAudio::setIsBackgroundMusic);
	    class_binding.function("setLoop", &JSAudio::setLoop);
	    class_binding.function("play", &JSAudio::play);
	    class_binding.function("pause", &JSAudio::pause);
	    class_binding.function("stop", &JSAudio::stop);
	    class_binding.function("addEventListener", &JSAudio::addEventListener);
		class_binding.constructor<>();
		context.class_("ConchAudio", class_binding);
    }
}
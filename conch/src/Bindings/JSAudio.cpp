#include "JSAudio.h"
#include <jsbind/JSBind.h>
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
#include "../../JCSystemConfig.h"
#include "JCConch.h"
#include <audio/AudioDecoderCache.h>
#include <audio/StreamDecoder.h>
#include <profiler/Profiler.h>
namespace laya
{
    //------------------------------------------------------------------------------
    JSAudio::JSAudio()
    {
	    reset();
	    jsbind::AdjustAmountOfExternalAllocatedMemory( 534 );
	    JCMemorySurvey::GetInstance()->newClass( "audio",534,this );
	    m_CallbackRef.reset(new int(1));
		m_audioPlayer = &JCConch::s_pConch->getAudioPlayer();
    }
    //------------------------------------------------------------------------------
    JSAudio::~JSAudio()
    {
        JCMemorySurvey::GetInstance()->releaseClass( "audio",this );
	    //JCAudioManager::GetInstance()->delWav(this, m_sSrc);
	    //JCAudioManager::GetInstance()->delMp3Obj(this);
    }
	void JSAudio::reset()
	{
		m_nCurrentTime = 0;
	    m_bNeedHandlePlay = false;
	    m_bAutoPlay = false;
	    m_bLoop = false;
	    m_bMuted = false;
	    m_nVolume = 1;
	    m_sSrc = "";
	    m_bDownloaded = false;
        m_audioRenderInfo = NULL;
		if (m_audio)
		{
			m_audioPlayer->stop(m_audio);
			m_decoder.reset();
			m_audio.reset();
		}
	}
    //------------------------------------------------------------------------------
    void JSAudio::addEventListener( const char* p_sName, jsvm_value p_pFunction )
    {
	    if( strcmp( p_sName,"ended" ) == 0 )
	    {
		    m_pJSFunctionAudioEnd = jsbind::Persistent(p_pFunction);
	    }
	    else if(strcmp(p_sName,"canplaythrough")==0)
	    {
		    m_pJSFunctionCanPlay = jsbind::Persistent(p_pFunction);
	    }
		else if (strcmp(p_sName, "canplay") == 0)
		{
			//todo
		}
	    else if (strcmp(p_sName, "error") == 0)
	    {
		    m_pJSFunctionError = jsbind::Persistent(p_pFunction);
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
    void JSAudio::setMuted(bool p_bMuted)
    {
	    m_bMuted = p_bMuted;
		if (m_nType == EXT_INVALID)
		{
			return;
		}
	    {
            if (m_audio)
            {
                m_audioPlayer->setVolume(m_audio, m_bMuted ? 0 : m_nVolume);
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
		if (sSrc == "")
		{
			reset();
			return;
		}
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

		auto decoder = audio::AudioDecoderCache::get(m_sSrc);
	    if(decoder)
	    {
		    m_bDownloaded = true;
			m_decoder = decoder;
			DEBUG_CHECK(m_decoder);
            std::weak_ptr<int> cbref(m_CallbackRef);
            std::function<void(void)> pFunction = std::bind(&JSAudio::onCanplayCallJSFunction, this, cbref);
            postToJS( pFunction );
		    if(m_bAutoPlay == true)
		    {
			    play();
		    }
		    return;
	    }
	    
	    
		laya::JCFileRes* res = JCConch::s_pScriptRuntime->m_pFileResMgr->getRes(m_sSrc);
		std::weak_ptr<int> cbref(m_CallbackRef);
		res->setOnReadyCB( std::bind(&JSAudio::onDownloaded,this, std::placeholders::_1,cbref));
		res->setOnErrorCB( std::bind(&JSAudio::onDownloadErr,this,std::placeholders::_1,std::placeholders::_2,cbref));
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
	    if( !pFileRes->m_data || pFileRes->m_data->size() == 0)
        {
		    return false;
	    }
	    m_bDownloaded = true;
       // std::weak_ptr<int> cbref(m_CallbackRef);
        //std::function<void(void)> pFunction = std::bind(&JSAudio::onCanplayCallJSFunction,this, callbackref);

		if (pFileRes->m_data->size() >= g_kSystemConfig.m_audioStreamThreshold && (m_nType == EXT_MP3 || m_nType == EXT_OGG))
		{
			Profiler_ZoneScoped("audio::AudioDecoderCache::createStreamDecoder", 0x00ff00);
			m_decoder = audio::AudioDecoderCache::createStreamDecoder(m_sSrc, pFileRes->m_data);
			//流式解码
			//m_decoder = audio::AudioDecoderCache::createDecoder(m_sSrc, (uint8_t*)p_buf.m_pPtr, p_buf.m_nLen);
		}
		else
		{
			Profiler_ZoneScoped("audio::AudioDecoderCache::createStaticDecoder", 0xff00);
			m_decoder = audio::AudioDecoderCache::createStaticDecoder(m_sSrc, pFileRes->m_data);
		}
		if(m_decoder) 
		{
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
	  	if(m_audio)
	  	{
			m_audioPlayer->setVolume(m_audio, m_nVolume);
	  	}
    }
    float JSAudio::getVolume()
    {
	    return m_nVolume;
    }
	double JSAudio::getDuration()
	{
		if (m_audio)
		{
			return m_audioPlayer->getDuration(m_audio);
		}
		return std::numeric_limits<double>::quiet_NaN();
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
		if (m_decoder)
		{
			Profiler_ZoneScoped("SAudio::play()", 0x0000ff);
			if (!m_audio)
			{
				m_audio = m_audioPlayer->createAudio(m_decoder);
			}
			std::weak_ptr<int> cbref(m_CallbackRef);
			m_audio->setOnPlayEnd([this, cbref](){
				if(!cbref.lock())
				{
					return;
				}
				m_nState = EXT_STATE_PLAY_END;
				auto pFunction = std::bind(&JSAudio::onPlayEndCallJSFunction,this, cbref);
				postToJS( pFunction );
			});
			m_audioPlayer->play(m_audio);

			//audioPlayer.setCurrentTime(m_audio, m_nCurrentTime);
			if (m_bMuted) 
			{
				//m_audio->setVolume(0);
			}
			else
			{
				//m_audio->setVolume(m_nVolume);
			}
		}

#if 0
		if (m_nType == EXT_MP3)
	    {			
#if !defined(OS_LINUX)
		    m_audioRenderInfo = JCAudioManager::GetInstance()->playWavMp3( this, m_sSrc, m_nCurrentTime);
#endif
	    }
		else
		{
			m_audioRenderInfo = JCAudioManager::GetInstance()->playWav(this, m_sSrc, m_nType == EXT_OGG, m_nCurrentTime);
		}
		if (m_audioRenderInfo)
		{
			if (m_bMuted) 
			{
				JCAudioManager::GetInstance()->setWavVolume(m_audioRenderInfo, 0);
			}
			else 
			{
				JCAudioManager::GetInstance()->setWavVolume(m_audioRenderInfo, m_nVolume);
			}
		}
#endif
    }
    //------------------------------------------------------------------------------
    void JSAudio::pause()
    {
		if (m_nType == EXT_INVALID)
		{
			return;
		}
        else
        {
			if (m_nState != EXT_STATE_PLAY)
			{
				return;
			}

			m_nState = EXT_STATE_PAUSE;
            if (m_audio)
            {
				m_nCurrentTime = getCurrentTime();
                m_audioPlayer->pause(m_audio);
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
        else
        {
			if (m_nState != EXT_STATE_PLAY)
			{
				m_bShouldStop = true;
				return;
			}
			m_bShouldStop = false;//正确执行stop了，不需要记录了
			m_nState = EXT_STATE_STOP;
            if (m_audio)
            {
				m_nCurrentTime = getCurrentTime();
				m_audioPlayer->stop(m_audio);
				m_audio.reset();
            }
        }
    }
    void JSAudio::setCurrentTime(double nCurrentTime)
    {
        m_nCurrentTime = nCurrentTime;
		if (m_audio)
		{
			m_audioPlayer->seek(m_audio, nCurrentTime);
		}
    }
    double JSAudio::getCurrentTime()
    {
		if (m_audio)
		{
			return m_audioPlayer->tell(m_audio);
		}

		/*if (m_audioRenderInfo && m_audioRenderInfo->m_pAudio == this)
		{
			return JCAudioManager::GetInstance()->getCurrentTime(m_audioRenderInfo);
        }
		else if (m_nState == EXT_STATE_STOP || m_nState == EXT_STATE_PAUSE)
		{
			return m_nCurrentTime;
		}*/
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
	    m_pJSFunctionAudioEnd.call<void>(jsbind::toLocal(this));
    }
    //------------------------------------------------------------------------------
    void JSAudio::onCanplayCallJSFunction( std::weak_ptr<int> callbackref)
    {
        if( !callbackref.lock())
            return;
        m_pJSFunctionCanPlay.call<void>(jsbind::toLocal(this));
    }
    void JSAudio::onErrorCallJSFunction(int p_nErrorCode,std::weak_ptr<int> callbackref)
    {
	    if (!callbackref.lock())return;
	    m_pJSFunctionError.call<void>(jsbind::toLocal(this), p_nErrorCode);
    }
    void JSAudio::exportJS(jsbind::Object& context)
    {
		jsbind::class_<JSAudio> class_binding;
        class_binding.property("autoplay", &JSAudio::getAutoPlay, &JSAudio::setAutoPlay);
        class_binding.property("loop", &JSAudio::getLoop, &JSAudio::setLoop);
        class_binding.property("muted", &JSAudio::getMuted, &JSAudio::setMuted);
        class_binding.property("src", &JSAudio::getSrc, &JSAudio::setSrc);
        class_binding.property("volume", &JSAudio::getVolume, &JSAudio::setVolume);
		class_binding.property("duration",&JSAudio::getDuration);
        class_binding.property("currentTime", &JSAudio::getCurrentTime, &JSAudio::setCurrentTime);
	    class_binding.function("play", &JSAudio::play);
	    class_binding.function("pause", &JSAudio::pause);
	    class_binding.function("stop", &JSAudio::stop);
	    class_binding.function("addEventListener", &JSAudio::addEventListener);
		class_binding.constructor<>();
		context.class_("ConchAudio", class_binding);
    }
}
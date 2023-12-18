#include "JSNotify.h"
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#ifdef ANDROID
	#include <jni.h>
	#include "CToJavaBridge.h"
#elif __APPLE__
    #include "CToObjectC.h"
#endif


namespace laya 
{
    void JSNotify::setRepeatNotify( int p_nID,int p_nStartTime,int p_nRepeatType,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc )
    {
        LOGI("JSNotify::setRepeatNotify id=%d,startTime=%ld,type=%d,tickerText=%s,titleText=%s,desc=%s",p_nID,p_nStartTime,p_nRepeatType,p_sTickerText,p_sTitleText,p_sDesc );
        #ifdef ANDROID
            std::vector<intptr_t> params;
            params.push_back(p_nID);
            params.push_back((long)p_nStartTime);
            params.push_back(p_nRepeatType);
            params.push_back((long)p_sTickerText);
            params.push_back((long)p_sTitleText);
            params.push_back((long)p_sDesc);
            CToJavaBridge::JavaRet kRet;
            CToJavaBridge::GetInstance()->callMethod("laya.game.Notifycation.LayaNotifyManager", "setRepeatingNotify", p_nID, p_nStartTime,p_nRepeatType,p_sTickerText, p_sTitleText, p_sDesc, kRet);
        #elif __APPLE__
            CToObjectCSetRepeatNotify( p_nID,p_nStartTime,p_nRepeatType,p_sTickerText,p_sTickerText,p_sDesc );
        #elif WIN32
            
        #endif
    }
    //------------------------------------------------------------------------------
    void JSNotify::setOnceNotify( int p_nID,int p_nStartTime,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc )
    {
        LOGI("JSNotify::setOnceNotify id=%d,startTime=%ld,tickerText=%s,titleText=%s,desc=%s",p_nID,p_nStartTime,p_sTickerText,p_sTitleText,p_sDesc );
        #ifdef ANDROID
            std::vector<intptr_t> params;
            params.push_back(p_nID);
            params.push_back((long)p_nStartTime);
            params.push_back((long)p_sTickerText);
            params.push_back((long)p_sTitleText);
            params.push_back((long)p_sDesc);
            CToJavaBridge::JavaRet kRet;
            CToJavaBridge::GetInstance()->callMethod("layaair.game.Notifycation.LayaNotifyManager", "setOnceNotify", p_nID,p_nStartTime,p_sTickerText,p_sTitleText,p_sDesc,kRet);
        #elif __APPLE__
            CToObjectCSetOnceNotify( p_nID,p_nStartTime,p_sTickerText,p_sTitleText,p_sDesc );
        #elif WIN32
            
        #endif
    }
    //------------------------------------------------------------------------------
    void JSNotify::deleteOnceNotify( int p_nID )
    {
        LOGI("JSNotify::deleteOnceNotify id=%d",p_nID );
        #ifdef ANDROID
            CToJavaBridge::JavaRet kRet;
            CToJavaBridge::GetInstance()->callMethod("layaair.game.Notifycation.LayaNotifyManager", "removeNotify", p_nID, kRet);
        #elif __APPLE__
            CToObjectCDeleteOnceNotify( p_nID );
        #elif WIN32
            
        #endif
    }
    //------------------------------------------------------------------------------
    void JSNotify::deleteAllNotify()
    {
        LOGI("JSNotify::deleteAllNotify" );
        #ifdef ANDROID
            CToJavaBridge::JavaRet kRet;
            CToJavaBridge::GetInstance()->callMethod("layaair.game.Notifycation.LayaNotifyManager", "removeAllNotify", kRet);
        #elif __APPLE__
            CToObjectCDeleteAllNotify();
        #elif WIN32
            
        #endif
    }
    //------------------------------------------------------------------------------
    void JSNotify::exportJS(Context& context) 
    {
        class_<JSNotify> class_binding;
        class_binding.class_function("setRepeatNotify", &JSNotify::setRepeatNotify);
        class_binding.class_function("setOnceNotify", &JSNotify::setOnceNotify);
        class_binding.class_function("deleteOnceNotify", &JSNotify::deleteOnceNotify);
        class_binding.class_function("deleteAllNotify", &JSNotify::deleteAllNotify);
        context.class_("conchNotify", class_binding);
    }
}
/**
@file			JNIFun.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_19
*/

#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/native_window_jni.h>
#ifdef OS_ANDROID
    #include <sys/syscall.h>
    #define gettidv1() syscall(__NR_gettid)
    #define gettidv2() syscall(SYS_gettid)
#endif
#include <downloadCache/JCAndroidFileSource.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <downloadMgr/JCDownloadMgr.h>
#include <utils/Log.h>
#include "JCConch.h"
#include <Bindings/JSConchConfig.h>
#include "CToJavaBridge.h"
#include "Audio/JCAudioManager.h"
#include <Bindings/JSInput.h>
#include <utils/JCZipFile.h>
#include "JCSystemConfig.h"
#include <Bindings/JSGlobalExportCFun.h>
#include "JCConchRender.h"
#include "JCScriptRuntime.h"
#include <Bindings/Video/JSVideo.h>
#include <imageLib/JCImageRW.h>
#include "network/HttpClientAndroid.h"
#include "2D/CanvasRenderingContext2DAndroid.h"
#include "HandleAsyncMessageMethodRecord.h"

extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
extern std::string gRedistPath;
//------------------------------------------------------------------------------

extern AAssetManager* g_pAssetManager;
extern std::string gAssetRootPath;
extern std::string gAPKExpansionMainPath;
extern std::string gAPKExpansionPatchPath;
int64_t g_nInitTime = 0;


using namespace laya;

//------------------------------------------------------------------------------
extern "C"
{
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_configSetParamExt(JNIEnv * env, jobject obj,jstring p_strParamExt);//extparam
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_configSetURL(JNIEnv * env, jobject obj,jstring p_strUrl);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_InitDLib(JNIEnv * env, jobject obj,jobject assetManager,jint nDownloadThreadNum,jstring p_strAssetRootPath,jstring p_strCachePath, jstring p_strAPKExpansionMainPath, jstring p_strAPKExpansionPatchPath);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_SetLocalStoragePath(JNIEnv * env, jobject obj,jstring p_strLocalStorage );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleTouch(JNIEnv * env, jobject obj,jint type,jint id,jint x,jint y );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyEvent(JNIEnv * env, jobject obj,jint keyCode,jint actionType);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleJoystickEvent(JNIEnv * env, jobject obj,float THUMBL_xOffset,float THUMBL_yOffset,float THUMBR_xOffset,float THUMBR_yOffset,float LT_Offset,float RT_Offset);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceMotionEvent(JNIEnv * env, jobject obj, float ax, float ay, float az, float agx, float agy, float agz, float ra, float rb, float rg, float interval);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceOrientationEvent(JNIEnv * env, jobject obj, float ra, float rb, float rg);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_ReleaseDLib(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppDestroy(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppPause(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSurfaceCreated(JNIEnv * env, jobject obj, jobject surface);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnSurfaceResize(JNIEnv * env, jobject obj, int width,int height);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnSurfaceDestroy(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppResume(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onDrawFrame(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exportStaticMethodToC(JNIEnv * env, jobject obj, jstring packcls);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_alertCallback(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSensorChanged(JNIEnv * env, jobject obj,float arc );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_RunJS(JNIEnv* env, jobject obj, jstring jsstr );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_postMsgToRuntime(JNIEnv* env, jobject obj, jstring msg, jstring params);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exitToPlatform( JNIEnv * env, jobject obj );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_audioMusicPlayEnd( JNIEnv * env, jobject obj );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_networkChanged(JNIEnv* env, jobject obj, jint nNetworkType );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_inputChange(JNIEnv* env, jobject obj, jint keycode );	
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_setLocalizable(JNIEnv * env, jobject obj, jboolean p_bIsLocalPackage);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_callConchJSFunction(JNIEnv* env, jobject obj, jstring sFunctionName,jstring sJsonParam,jstring sCallbackFunction);
    JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_emit(JNIEnv* env, jobject obj, jlong ptr, jstring str);
	JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_transferBitmap(JNIEnv* env, jobject obj, jobject bitmap, jlong dataPtr);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardInput(JNIEnv* env, jobject obj, jstring strValue);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardConfirm(JNIEnv* env, jobject obj, jstring strValue);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardComplete(JNIEnv* env, jobject obj, jstring strValue);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_reloadJS(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_urlBack(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleAsyncMessageMethodNative(JNIEnv* env, jobject obj, jlong nativeHandle, jstring result);
};
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_configSetURL(JNIEnv * env, jobject obj,jstring p_strUrl)
{
    char* pstrUrl =(char*) env->GetStringUTFChars( p_strUrl, NULL ); 
	g_kSystemConfig.m_strStartURL = pstrUrl;
	LOGI("JNI seturl: %s", pstrUrl);
	env->ReleaseStringUTFChars(p_strUrl, pstrUrl);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_InitDLib(JNIEnv * env, jobject obj,jobject assetManager,int nThreadNum,jstring p_strAssetRootPath, jstring p_strCachePath , jstring p_strAPKExpansionMainPath, jstring p_strAPKExpansionPatchPath)
{
	LOGI("JNI InitDLib tid=%x", std::this_thread::get_id());
	if(laya::JCConch::s_pConch)
    {
        return;//下面导致线上crash
        LOGI("JNI has an old conch object! delete it");
		//如果上次不正常退出，如果时间太短，可能有的线程还在创建过程中。所以等待一会儿。
		//例如g_pConch突然为null，可能有人还在用。
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //必须走完整流程，例如有的线程还在创建，完了后就正常跑，然后这里g_pConch又被删了
        Java_layaair_game_browser_ConchJNI_ReleaseDLib(env,obj);
        LOGI("JNI del old end");        
	}
	
	HttpClientAndroid::addStaticMethod(env, "layaair/game/browser/LayaHttpClient");
	CanvasRenderingContext2DAndroid::addStaticMethod(env, "layaair/game/browser/LayaCanvasRenderingContext2D");

    //这个不要放到开始，以影响面上面的异常处理
    g_nInitTime = tmGetCurms();
    char* pAssetRootPath =(char*) env->GetStringUTFChars( p_strAssetRootPath, NULL ); 
	char* pCachePath = (char*)env->GetStringUTFChars( p_strCachePath, NULL);
	char* pAPKExpansionMain =(char*) env->GetStringUTFChars( p_strAPKExpansionMainPath, NULL ); 
	char* pAPKExpansionPatch = (char*)env->GetStringUTFChars( p_strAPKExpansionPatchPath, NULL);
	LOGI( "JNI InitDownLoadManager CachePath=%s, assetroot=%s, APKExpansionMain=%s, APKExpansionPatch=%s ", pCachePath, pAssetRootPath, pAPKExpansionMain, pAPKExpansionPatch);
	gRedistPath = pCachePath;
	gRedistPath +="/";
	gAssetRootPath = pAssetRootPath;
	gAPKExpansionMainPath= pAPKExpansionMain;
	gAPKExpansionPatchPath = pAPKExpansionPatch;
	if( assetManager==0 || !(g_pAssetManager = AAssetManager_fromJava(env,assetManager)))
    {
		LOGI("JNI Warning! AssetManager =NULL!! 下面要采用jar流程了。"); 
		JCZipFile* pZip = new laya::JCZipFile();
		if( strstr(pAssetRootPath,".jar" ) ||strstr(pAssetRootPath,".JAR" )||strstr(pAssetRootPath,".zip" )||strstr(pAssetRootPath,".apk")||strstr(pAssetRootPath,".APK") ){
			if(pZip->open(pAssetRootPath))
            {
                LOGI("JNI 打开jar成功。");
				pZip->InitDir("assets");
			}
			JCConch::s_pAssetsFiles = pZip;
		}
        else
        {
            LOGI("JNI 没有设置assetMgr，也没有传入jar包。");
		} 
	}
    else
    {
		laya::JCAndroidFileSource* pAssets = new laya::JCAndroidFileSource();
		pAssets->Init(g_pAssetManager, "","", "", "");
		JCConch::s_pAssetsFiles = pAssets;
	}
	env->ReleaseStringUTFChars(p_strAssetRootPath, pAssetRootPath);
	env->ReleaseStringUTFChars(p_strCachePath, pCachePath);
	env->ReleaseStringUTFChars(p_strAPKExpansionMainPath, pAPKExpansionMain);
	env->ReleaseStringUTFChars(p_strAPKExpansionPatchPath, pAPKExpansionPatch);
	laya::JCConch::s_pConch.reset(new laya::JCConch());
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_SetLocalStoragePath(JNIEnv * env, jobject obj,jstring p_strLocalStorage )
{
    char* pLocalStoragePath =(char*) env->GetStringUTFChars( p_strLocalStorage, NULL ); 
	LOGI( "JNI localStoragePath=%s", pLocalStoragePath);
	env->ReleaseStringUTFChars(p_strLocalStorage, pLocalStoragePath);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleTouch( JNIEnv * env, jobject obj,jint type,jint id,jint x,jint y )
{
	enum java_motion_Action
    {
		ACTION_DOWN=0,
		ACTION_UP=1,
		ACTION_MOVE=2,
		ACTION_POINTER_DOWN=5,
		ACTION_POINTER_UP=6,
	};
	switch(type)
    {
	case java_motion_Action::ACTION_DOWN:
        {
		    inputEvent e;
		    e.nType =E_ONTOUCHSTART;
		    e.nTouchType = type;
		    e.posX = x;
		    e.posY = y;
		    e.id = id;
		    strncpy(e.type, "touchstart", 256 );
			JCConch::s_pConch->dispatchInputEvent(e);
            break;
	    }
	case java_motion_Action::ACTION_UP:
        {
		    inputEvent e;
		    e.nType = E_ONTOUCHEND;
		    e.nTouchType = type;
		    e.posX = x;
		    e.posY = y;
		    e.id = id;
		    strncpy(e.type, "touchend", 256 );
			JCConch::s_pConch->dispatchInputEvent(e);
            break;
	    }                             
    case java_motion_Action::ACTION_MOVE: 
    {
        inputEvent e;
        e.nType = E_ONTOUCHMOVE;
        e.nTouchType = type;
        e.posX = x;
        e.posY = y;
        e.id = id;
        strncpy(e.type, "touchmove", 256);
		JCConch::s_pConch->dispatchInputEvent(e);
        break;
    }
	case java_motion_Action::ACTION_POINTER_DOWN:
        {
		    inputEvent e;
		    e.nType = E_ONACTION_POINTER_DOWN;
		    e.nTouchType = type;
		    e.posX = x;
		    e.posY = y;
		    e.id = id;
		    strncpy(e.type, "touchstart", 256 );
			JCConch::s_pConch->dispatchInputEvent(e);
            break;
	    }
	case java_motion_Action::ACTION_POINTER_UP:
        {
		    inputEvent e;
		    e.nType = E_ONACTION_POINTER_UP;
		    e.nTouchType = type;
		    e.posX = x;
		    e.posY = y;
		    e.id = id;
		    strncpy(e.type, "touchend", 256 );
			JCConch::s_pConch->dispatchInputEvent(e);
            break;
	    }
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyEvent(JNIEnv * env, jobject obj,jint keyCode,jint actionType)
{
	enum android_key_Code
	{
		KEYCODE_BACK = 4,
		KEYCODE_DPAD_UP = 19,
		KEYCODE_DPAD_DOWN = 20,
		KEYCODE_DPAD_LEFT = 21,
		KEYCODE_DPAD_RIGHT = 22,
		KEYCODE_DPAD_CENTER = 23,
		KEYCODE_VOLUME_UP = 24,
		KEYCODE_VOLUME_DOWN = 25,
		KEYCODE_ENTER = 66,
		KEYCODE_MENU = 82,
		KEYCODE_BUTTON_A = 96,
		KEYCODE_BUTTON_B = 97,
		KEYCODE_BUTTON_X = 99,
		KEYCODE_BUTTON_Y = 100,
		KEYCODE_BUTTON_L1 = 102,
		KEYCODE_BUTTON_R1 = 103,
		KEYCODE_BUTTON_L2 = 104,
		KEYCODE_BUTTON_R2 = 105,
		KEYCODE_BUTTON_THUMBL = 106,
		KEYCODE_BUTTON_THUMBR = 107,
		KEYCODE_BUTTON_START = 108,
		KEYCODE_BUTTON_SELECT = 109,
	};
	enum android_key_Action
	{
		ACTION_DOWN = 0,
		ACTION_UP = 1,
	};
	if(actionType == android_key_Action::ACTION_DOWN)
	{
		inputEvent e;
		e.nType = E_ONKEYDOWN;
		e.keyCode = keyCode;
		strncpy(e.type, "keydown", 256 );
		JCConch::s_pConch->dispatchInputEvent(e);
	}
	else if(actionType == android_key_Action::ACTION_UP)
	{
		inputEvent e;
		e.nType =E_ONKEYUP;
		e.keyCode = keyCode;
		strncpy(e.type, "keyup", 256 );
		JCConch::s_pConch->dispatchInputEvent(e);
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleJoystickEvent( JNIEnv * env, jobject obj,float THUMBL_xOffset,float THUMBL_yOffset,float THUMBR_xOffset,float THUMBR_yOffset,float LT_Offset,float RT_Offset )
{
	inputEvent e;
	e.nType = E_JOYSTICK;
	e.fTHUMBL_xOffset = THUMBL_xOffset;
	e.fTHUMBL_yOffset = THUMBL_yOffset;
	e.fTHUMBR_xOffset = THUMBR_xOffset;
	e.fTHUMBR_yOffset = THUMBR_yOffset;
	e.fLT_Offset = LT_Offset;
	e.fRT_Offset = RT_Offset;
	strncpy(e.type, "onjoystick", 256 );
	JCConch::s_pConch->dispatchInputEvent(e);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceMotionEvent(JNIEnv * env, jobject obj, float ax, float ay, float az, float agx, float agy, float agz, float ra, float rb, float rg, float interval)
{
    DeviceMotionEvent e;
    e.nType = E_DEVICEMOTION;
    e.ax = ax;
    e.ay = ay;
    e.az = az;
    e.agx = agx;
    e.agy = agy;
    e.agz = agz;
    e.ra = ra;
    e.rb = rb;
    e.rg = rg;
    e.interval = interval;
    strncpy(e.type, "devicemotion", 256);
    JCConch::s_pConch->dispatchInputEvent(e);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceOrientationEvent(JNIEnv * env, jobject obj, float ra, float rb, float rg)
{
    DeviceOrientationEvent e;
    e.nType = E_DEVICEORIENTATION;
    e.ra = ra;
    e.rb = rb;
    e.rg = rg;
    strncpy(e.type, "deviceorientation", 256);
    JCConch::s_pConch->dispatchInputEvent(e);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_ReleaseDLib(JNIEnv * env, jobject obj )
{
	LOGI("JNI del engine tid=%x", std::this_thread::get_id());
	JCAudioManager::GetInstance()->stopMp3();
	laya::JCConch::s_pConch->onAppDestroy();
	laya::JCConch::s_pConch.reset();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppDestroy(JNIEnv * env, jobject obj )
{
	LOGI("JNI OnAppDestroy tid=%x", std::this_thread::get_id());
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppPause(JNIEnv * env, jobject obj )
{
    LOGI("JNI OnAppPause tid=%x", std::this_thread::get_id());
	laya::JCConch::s_pConch->onAppPause();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppResume(JNIEnv * env, jobject obj )
{
    LOGI("JNI OnAppResume tid=%x", std::this_thread::get_id());
    laya::JCConch::s_pConch->onAppResume();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSurfaceCreated(JNIEnv *env, jobject obj, jobject surface)
{
	LOGI("JNI OnCreated tid=%x", std::this_thread::get_id());
    ANativeWindow* aNativeWindow = ANativeWindow_fromSurface(env, surface);
    laya::BackendOptions options;
    laya::JCConch::s_pConchRender->createBackend(options);
    laya::JCConch::s_pConchRender->createScreenSurface(aNativeWindow);
	laya::JCConch::s_pConch->onAppStart();
}

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnSurfaceResize(JNIEnv *env, jobject obj, int width,int height)
{
	LOGI("JNI OnResize tid=%x", std::this_thread::get_id());
	
	if( g_nInnerWidth!=width || g_nInnerHeight != height )
    {
        LOGI("JNI surface innersize changed : g_nInnerWidth=%d,g_nInnerHeight=%d",width,height);
		g_nInnerWidth = width;
		g_nInnerHeight = height;
		g_bGLCanvasSizeChanged = true;

	}
    laya::JCConch::s_pConchRender->onScreenSurfaceResize(width, height);
    LOGI("JNI init dev w=%d,h=%d",width,height);
    //pRender->onGLDeviceLosted();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnSurfaceDestroy(JNIEnv * env, jobject obj)
{
	LOGI("JNI OnDestroy tid=%x", std::this_thread::get_id());
	laya::JCConch::s_pConchRender->destroyScreenSurface();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onDrawFrame(JNIEnv * env, jobject obj )
{
    laya::JCConch::s_pConch->update();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_alertCallback(JNIEnv * env, jobject obj )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exportStaticMethodToC(JNIEnv * env, jobject obj, jstring packcls)
{
	LOGI("JNI exportStaticMethodToC tid=%x", std::this_thread::get_id());
    const char* rawPackCls = env->GetStringUTFChars(packcls, NULL);
	CToJavaBridge::GetInstance()->addStaticMethod(env,rawPackCls);
    env->ReleaseStringUTFChars(packcls, rawPackCls);
}

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSensorChanged(JNIEnv * env, jobject obj,float arc )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_RunJS(JNIEnv* env, jobject obj, jstring jsstr )
{
	LOGI("JNI RunJS tid=%x", std::this_thread::get_id());
    if (jsstr)
    {
        const char* rawString = env->GetStringUTFChars(jsstr, NULL);
        JCConch::s_pScriptRuntime->callJSString(rawString);
        env->ReleaseStringUTFChars(jsstr, rawString);
    }
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_postMsgToRuntime(JNIEnv* env, jobject obj, jstring msg, jstring params)
{
	LOGI("JNI postMsgToRuntime tid=%x", std::this_thread::get_id());
    const char* rawMsg = env->GetStringUTFChars(msg, NULL);
    const char* rawParams = env->GetStringUTFChars(params, NULL);
    env->ReleaseStringUTFChars(msg, rawMsg);
    env->ReleaseStringUTFChars(params, rawParams);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exitToPlatform( JNIEnv * env, jobject obj )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_audioMusicPlayEnd( JNIEnv * env, jobject obj )
{
	LOGI("JNI audioMusicPlayEnd tid=%x", std::this_thread::get_id());
	laya::JCMp3Interface* pMp3Player = laya::JCAudioManager::GetInstance()->m_pMp3Player;
	if( pMp3Player )
	{
		pMp3Player->onPlayEnd();
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_networkChanged(JNIEnv* env, jobject obj, jint nNetworkType )
{
	LOGI("JNI networkChanged tid=%x", std::this_thread::get_id());
    JCConch::s_pScriptRuntime->onNetworkChanged(nNetworkType);
}

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_inputChange(JNIEnv* env, jobject obj, jint keycode)
{
	LOGI("JNI inputChange tid=%x", std::this_thread::get_id());
	
    if (JCConch::s_pScriptRuntime->m_pCurEditBox)
    {
        JCConch::s_pScriptRuntime->m_pCurEditBox->onInput();
    }
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_setLocalizable(JNIEnv * env, jobject obj, jboolean p_bIsLocalPackage)
{
	LOGI("JNI setLocalizable tid=%x", std::this_thread::get_id());
    JCSystemConfig::s_bLocalizable = p_bIsLocalPackage;
    LOGI("setLocalizable: %d", p_bIsLocalPackage);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_callConchJSFunction(JNIEnv* env, jobject obj, jstring p_sFunctionName,jstring p_sJsonParam,jstring p_sCallbackFunction)
{
	LOGI("JNI callConchJSFunction tid=%x", std::this_thread::get_id());
    const char* sFunctionName = env->GetStringUTFChars(p_sFunctionName, NULL);
    const char* sJsonParam = env->GetStringUTFChars(p_sJsonParam, NULL);
    const char* sCallbackFunction = env->GetStringUTFChars(p_sCallbackFunction, NULL);
    LOGI(">>>>>>>>Java_layaair_game_browser_ConchJNI_callConchJSFunction functionName=%s, jsonParam=%s, callbackFuncton=%s",sFunctionName, sJsonParam,sCallbackFunction );
    JCConch::s_pScriptRuntime->callJC( sFunctionName,sJsonParam,sCallbackFunction );
	env->ReleaseStringUTFChars(p_sFunctionName, sFunctionName);
    env->ReleaseStringUTFChars(p_sJsonParam, sJsonParam);
    env->ReleaseStringUTFChars(p_sCallbackFunction, sCallbackFunction);
}

JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_emit(JNIEnv* env, jobject obj, jlong ptr, jstring str)
{
	LOGI("JNI LayaVideoPlayer_emit tid=%x", std::this_thread::get_id());
	
	JSVideo* pVideo = reinterpret_cast<JSVideo*>(ptr);

	if(!pVideo)
		return;

	const char* evtName = env->GetStringUTFChars(str, NULL);
	
//	LOGI("[DEBUG][Video]Call emit function %s", evtName);

	pVideo->CallHandle(evtName);
	env->ReleaseStringUTFChars(str, evtName);
}

JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_transferBitmap(JNIEnv* env, jobject obj, jobject bitmap, jlong dataPtr)
{
	LOGI("JNI LayaVideoPlayer_transferBitmap tid=%x", std::this_thread::get_id());
	
    AndroidBitmapInfo bmpInfo={0};
	if(AndroidBitmap_getInfo(env, bitmap, &bmpInfo) < 0)
	{
//		LOGE("[Debug][Video]bitmap: Error getInfo");
		return;
	}

	char** dataFromBmp=NULL;
    if(AndroidBitmap_lockPixels(env,bitmap,(void**)&dataFromBmp))
    {
//		LOGE("[Debug][Video]bitmap: Error lockPixels ");
		return;
	}

//	LOGI("[Debug][Video]Call native transferBitmap %ld", dataPtr);

	BitmapData* pBitmapData = reinterpret_cast<BitmapData*>(dataPtr);
	if(!pBitmapData)
	{
//		LOGE("[Debug][Video]Error pBitmapData");
		AndroidBitmap_unlockPixels(env,bitmap);
		return;
	}

	pBitmapData->reconfigure(bmpInfo.width, bmpInfo.height, 32, ImgType_unknow);
	memcpy(pBitmapData->m_pImageData, dataFromBmp, sizeof(int32_t) * pBitmapData->m_nWidth * pBitmapData->m_nHeight);

	AndroidBitmap_unlockPixels(env,bitmap);
}

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardInput(JNIEnv* env, jobject obj, jstring strValue)
{
	const char* value = env->GetStringUTFChars(strValue, NULL);
	//JSLayaNative::getInstance()->handleKeyboardInput(value);
	env->ReleaseStringUTFChars(strValue, value);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardConfirm(JNIEnv* env, jobject obj, jstring strValue)
{
	const char* value = env->GetStringUTFChars(strValue, NULL);
	//JSLayaNative::getInstance()->handleKeyboardConfirm(value);
	env->ReleaseStringUTFChars(strValue, value);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardComplete(JNIEnv* env, jobject obj, jstring strValue)
{
	const char* value = env->GetStringUTFChars(strValue, NULL);
	//JSLayaNative::getInstance()->handleKeyboardComplete(value);
	env->ReleaseStringUTFChars(strValue, value);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_reloadJS(JNIEnv* env, jobject obj)
{
	EngineEventReload e;
    JCConch::s_pConch->dispatchEngineEvent(e);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_urlBack(JNIEnv* env, jobject obj)
{
	EngineEventUrlBack e;
    JCConch::s_pConch->dispatchEngineEvent(e);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleAsyncMessageMethodNative(JNIEnv* env, jobject obj, jlong nativeHandle, jstring result)
{
	const char* strResult = env->GetStringUTFChars(result, NULL);
	HandleAsyncMessageMethodRecord* pHandleAsyncMessageMethodRecord = reinterpret_cast<HandleAsyncMessageMethodRecord*>(nativeHandle);
	if(pHandleAsyncMessageMethodRecord)
	{
		pHandleAsyncMessageMethodRecord->m_callback(strResult);
	}
	env->ReleaseStringUTFChars(result, strResult);
}


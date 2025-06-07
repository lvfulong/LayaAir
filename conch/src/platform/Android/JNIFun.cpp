#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/native_window_jni.h>
#include <Bindings/JSDevice.h>
#include <sys/syscall.h>
#define gettidv1() syscall(__NR_gettid)
#define gettidv2() syscall(SYS_gettid)
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
#include "network/src/HttpClientAndroid.h"
#include "2D/src/CanvasRenderingContext2DAndroid.h"
#include "HandleAsyncMessageMethodRecord.h"
#if defined(USE_SWAPPY)
#include <swappy/swappyGL.h>
#include <swappy/swappyGL_extra.h>
#endif
#include "network/src/UploadTaskAndroid.h"
#include <Bindings/JSMemory.h>


extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;

using namespace laya;

//------------------------------------------------------------------------------
extern "C"
{
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_init(JNIEnv* env, jobject obj, jobject activity, jobject options, jobject surface);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_uninit(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleTouch(JNIEnv* env, jobject obj, jint type, jint id,jint x, jint y);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyEvent(JNIEnv* env, jobject obj,jint keyCode,jint actionType);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleJoystickEvent(JNIEnv* env, jobject obj,float THUMBL_xOffset,float THUMBL_yOffset,float THUMBR_xOffset,float THUMBR_yOffset,float LT_Offset,float RT_Offset);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceMotionEvent(JNIEnv* env, jobject obj, float ax, float ay, float az, float agx, float agy, float agz, float ra, float rb, float rg, float interval);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceOrientationEvent(JNIEnv* env, jobject obj, float ra, float rb, float rg);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onAppPause(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSurfaceCreated(JNIEnv * env, jobject obj, jobject surface);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSurfaceResize(JNIEnv * env, jobject obj, int width,int height);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSurfaceDestroy(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onAppResume(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_performUpdates(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exportStaticMethodToC(JNIEnv * env, jobject obj, jstring packcls);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_alertCallback(JNIEnv * env, jobject obj );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_RunJS(JNIEnv* env, jobject obj, jstring jsstr );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_audioMusicPlayEnd( JNIEnv * env, jobject obj );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_networkChanged(JNIEnv* env, jobject obj, jint nNetworkType );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_inputChange(JNIEnv* env, jobject obj, jint keycode );	
    JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_emit(JNIEnv* env, jobject obj, jlong ptr, jstring str);
	JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_transferBitmap(JNIEnv* env, jobject obj, jobject bitmap, jlong dataPtr);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardInput(JNIEnv* env, jobject obj, jstring strValue);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardConfirm(JNIEnv* env, jobject obj, jstring strValue);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardComplete(JNIEnv* env, jobject obj, jstring strValue);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_reloadJS(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_urlBack(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleAsyncMessageMethodNative(JNIEnv* env, jobject obj, jlong nativeHandle, jstring result);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onTrimMemory(JNIEnv* env, jobject obj, jint level);
};
static std::string getStringField(JNIEnv *env, jobject obj, const char *fieldName) {
	jclass cls = env->GetObjectClass(obj);
	DEBUG_CHECK(cls != NULL && "getStringField cls is NULL");
	jfieldID id = env->GetFieldID(cls, fieldName, "Ljava/lang/String;");
	DEBUG_CHECK(id != NULL && "getStringField id is NULL");
	jstring jstr = (jstring)env->GetObjectField(obj, id);
	const char *strValue = env->GetStringUTFChars(jstr, NULL);
	std::string result(strValue);
	env->ReleaseStringUTFChars(jstr, strValue);
	return strValue;
}
static jobject getObjectField(JNIEnv *env, jobject obj, const char *fieldName) {
	jclass cls = env->GetObjectClass(obj);
	DEBUG_CHECK(cls != NULL && "getStringField cls is NULL");
	jfieldID id = env->GetFieldID(cls, fieldName, "Ljava/lang/Object;");
	//DEBUG_CHECK(id != NULL && "getStringField id is NULL");
	jobject jobj = (jobject)env->GetObjectField(obj, id);
	DEBUG_CHECK(jobj != NULL && "getObjectField jobj is NULL");
	return jobj;
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_init(JNIEnv * env, jobject obj, jobject activity, jobject joptions, jobject surface)
{
	LOGI("JNI init");
	if(laya::JCConch::s_pConch)
    {
        return;//下面导致线上crash
        LOGI("JNI has an old conch object! delete it");
		//如果上次不正常退出，如果时间太短，可能有的线程还在创建过程中。所以等待一会儿。
		//例如g_pConch突然为null，可能有人还在用。
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //必须走完整流程，例如有的线程还在创建，完了后就正常跑，然后这里g_pConch又被删了
        Java_layaair_game_browser_ConchJNI_uninit(env,obj);
        LOGI("JNI del old end");        
	}
	jobject jAssetManager = getObjectField(env, joptions, "assetManager");
	DEBUG_CHECK(jAssetManager != NULL && "assetManager is NULL");
	HttpClientAndroid::addStaticMethod(env, "layaair/game/browser/LayaHttpClient");
	UploadTaskAndroid::addStaticMethod(env, "layaair/game/browser/LayaUploadTask");
	CanvasRenderingContext2DAndroid::addStaticMethod(env, "layaair/game/browser/LayaCanvasRenderingContext2D");

	std::string pFilesDir = getStringField(env, joptions, "filesDir");
	std::string pCacheDir = getStringField(env, joptions, "cacheDir");
	std::string pAPKExpansionMain = getStringField(env, joptions, "apkExpansionMainPath");
	std::string pAPKExpansionPatch = getStringField(env, joptions, "apkExpansionPatchPath");
	std::string pUrl = getStringField(env, joptions, "url");

	g_kSystemConfig.m_strStartURL = pUrl;

	laya::OS::setFilesDir(pFilesDir);	
	laya::OS::setCacheDir(pCacheDir);

	LOGD( "JNI Init FilesDir = %s, CacheDir = %s, APKExpansionMain = %s, APKExpansionPatch = %s ", pFilesDir.c_str(), pCacheDir.c_str(), pAPKExpansionMain.c_str(), pAPKExpansionPatch.c_str());


	AAssetManager* assetManager = AAssetManager_fromJava(env, jAssetManager);
	laya::JCAndroidFileSource* pAssets = new laya::JCAndroidFileSource();
	pAssets->Init(assetManager, "", pAPKExpansionMain, pAPKExpansionPatch);
	JCConch::s_pAssetsFiles = pAssets;
	


	laya::JCConch::s_pConch.reset(new laya::JCConch());

#if defined(USE_SWAPPY)
	if (g_kSystemConfig.m_useSwappy)
	{
		SwappyGL_init(env, activity);
		//推荐的模式 “自动模式 + 流水线”模式
		//https://developer.android.google.cn/games/sdk/frame-pacing?hl=zh-cn
        SwappyGL_setAutoSwapInterval(false);
        SwappyGL_setAutoPipelineMode(false);
        SwappyGL_enableStats(false);
		SwappyGL_setSwapIntervalNS(SWAPPY_SWAP_60FPS);
	}
#endif

	ANativeWindow* aNativeWindow = ANativeWindow_fromSurface(env, surface);
    laya::BackendOptions options;
    laya::JCConch::s_pConchRender->createBackend(options);
    laya::JCConch::s_pConchRender->createScreenSurface(aNativeWindow);
	laya::JCConch::s_pConch->onAppStart();
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
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_uninit(JNIEnv * env, jobject obj )
{
	LOGI("JNI uninit");
	DEBUG_CHECK(isScriptThread());
	JCAudioManager::GetInstance()->stopMp3();
	laya::JCConch::s_pConch->onAppDestroy();
	laya::JCConch::s_pConch.reset();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onAppPause(JNIEnv * env, jobject obj )
{
    LOGI("JNI onAppPause");
	DEBUG_CHECK(isScriptThread());
	if (laya::JCConch::s_pConch) {
		laya::JCConch::s_pConch->onAppPause();
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onAppResume(JNIEnv * env, jobject obj )
{
    LOGI("JNI onAppResume");
	DEBUG_CHECK(isScriptThread());
	if (laya::JCConch::s_pConch) {
    	laya::JCConch::s_pConch->onAppResume();
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSurfaceCreated(JNIEnv *env, jobject obj, jobject surface)
{
	LOGI("JNI onSurfaceCreated");
	DEBUG_CHECK(isScriptThread());
    ANativeWindow* aNativeWindow = ANativeWindow_fromSurface(env, surface);
    laya::BackendOptions options;
    laya::JCConch::s_pConchRender->createBackend(options);
    laya::JCConch::s_pConchRender->createScreenSurface(aNativeWindow);
	laya::JCConch::s_pConch->onAppStart();
}

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSurfaceResize(JNIEnv *env, jobject obj, int width,int height)
{
	LOGI("JNI onSurfaceResize(%d, %d)", width, height);
	DEBUG_CHECK(isScriptThread());
    laya::JCConch::s_pConchRender->onScreenSurfaceResize(width, height);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSurfaceDestroy(JNIEnv * env, jobject obj)
{
	LOGI("JNI onSurfaceDestroy");
	DEBUG_CHECK(isScriptThread());
	laya::JCConch::s_pConchRender->destroyScreenSurface();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_performUpdates(JNIEnv * env, jobject obj )
{
    laya::JCConch::s_pConch->update();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_alertCallback(JNIEnv * env, jobject obj )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exportStaticMethodToC(JNIEnv * env, jobject obj, jstring packcls)
{
	LOGI("JNI exportStaticMethodToC");
    const char* rawPackCls = env->GetStringUTFChars(packcls, NULL);
	CToJavaBridge::GetInstance()->addStaticMethod(env,rawPackCls);
    env->ReleaseStringUTFChars(packcls, rawPackCls);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_RunJS(JNIEnv* env, jobject obj, jstring jsstr )
{
	LOGI("JNI RunJS");
    if (jsstr)
    {
        const char* rawString = env->GetStringUTFChars(jsstr, NULL);
        JCConch::s_pScriptRuntime->callJSString(rawString);
        env->ReleaseStringUTFChars(jsstr, rawString);
    }
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_audioMusicPlayEnd( JNIEnv * env, jobject obj )
{
	LOGI("JNI audioMusicPlayEnd");
	laya::JCMp3Interface* pMp3Player = laya::JCAudioManager::GetInstance()->m_pMp3Player;
	if( pMp3Player )
	{
		pMp3Player->onPlayEnd();
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_networkChanged(JNIEnv* env, jobject obj, jint nNetworkType)
{
	LOGI("JNI networkChanged");
	if (JCConch::s_pScriptRuntime)
	{
    	JCConch::s_pScriptRuntime->onNetworkChanged(nNetworkType);
	}
}

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_inputChange(JNIEnv* env, jobject obj, jint keycode)
{
	LOGI("JNI inputChange");
	
    if (JCConch::s_pScriptRuntime->m_pCurEditBox)
    {
        JCConch::s_pScriptRuntime->m_pCurEditBox->onInput();
    }
}
JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_emit(JNIEnv* env, jobject obj, jlong ptr, jstring str)
{
	LOGI("JNI LayaVideoPlayer_emit");
	
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
	LOGI("JNI LayaVideoPlayer_transferBitmap");
	
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
	JSDevice::handleKeyboardInput(value);
	env->ReleaseStringUTFChars(strValue, value);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardConfirm(JNIEnv* env, jobject obj, jstring strValue)
{
	const char* value = env->GetStringUTFChars(strValue, NULL);
	JSDevice::handleKeyboardConfirm(value);
	env->ReleaseStringUTFChars(strValue, value);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyboardComplete(JNIEnv* env, jobject obj, jstring strValue)
{
	const char* value = env->GetStringUTFChars(strValue, NULL);
	JSDevice::handleKeyboardComplete(value);
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

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onTrimMemory(JNIEnv* env, jobject obj, jint level)
{
	JSMemory::fireMemoryWarning(level);
}
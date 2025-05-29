#include "JSConchConfig.h"
#include "JSInput.h"
#include "resource/JCFileResManager.h"
#include "../../JCScriptRuntime.h"
#include "JCConch.h"
#if defined(OS_ANDROID)
    #include "CToJavaBridge.h"
#elif defined(OS_WINDOWS)
    #include <Windows.h>
#elif defined(OS_IOS)
    #include "CToObjectC.h"
#elif defined(OS_OHOS)
    #include "aki/jsbind.h"
    #include <string>
    #include "platform/ohos/napi/helper/NapiHelper.h"
#endif
#include "platform/OS.h"
#include "downloadMgr/JCDownloadMgr.h"
#include <utils/Log.h>
#include "../../JCSystemConfig.h"
#include "../../JCConchRender.h"
#include "../../JCConch.h"
#include "../../WebSocket/WebSocket.h"
#include <resource/Audio/JCAudioWavPlayer.h>
#include "../../Audio/JCAudioManager.h"
#include <LayaGL/JCLayaGL.h>
#include <LayaGL/JCLayaGLDispatch.h>
#include <string>

extern int g_nLogLevel;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;

#ifdef OS_WINDOWS
extern HWND g_hWnd;
#endif

namespace laya
{
	std::string				JSConchConfig::m_sGUID = "unknow";
    std::string				JSConchConfig::m_sDeviceModel = "unknow";
	std::string				JSConchConfig::m_sDeviceInfo= "{\"resolution\":\"unknow\",	\"guid\":\"unknow\",\"imei\":[\"unknow\"],\"imsi\":[\"unknow\"],\"os\":\"unknow\",\"osversion\":\"unknow\",\"phonemodel\":\"unknow\"	}";
    std::string              JSConchConfig::m_sAppVersion;
    std::string             JSConchConfig::m_sAppLocalVersion;
	std::string             JSConchConfig::m_sLanguage;
	std::string             JSConchConfig::m_sIPAddress;
	std::string             JSConchConfig::m_sCountryCode;
	std::string             JSConchConfig::m_sModel;
    float                   JSConchConfig::m_fScreenScaleW = 1.0f;
    float                   JSConchConfig::m_fScreenScaleH = 1.0f;
    float                   JSConchConfig::m_fScreenTx = 0.0f;
    float                   JSConchConfig::m_fScreenTy = 0.0f;


    const std::string& JSConchConfig::getLocalStoragePath()
    {
        return JCConch::getLocalStoragePath();
    }
    float JSConchConfig::getTotalMem()
    {
        return OS::getTotalMem();
    }
    int JSConchConfig::getUsedMem()
    {
        return OS::getUsedMem();
    }
    int JSConchConfig::getAvalidMem()
    {
        return OS::getAvalidMem();
    }
    float JSConchConfig::getScreenInch()
    {
#ifdef OS_ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getScreenInch", kRet, CToJavaBridge::JavaRet::RT_Float))
        {
            return kRet.floatRet;
        }
        return 0;
#elif OS_OHOS
        return NapiHelper::GetInstance()->getScreenInch();
#elif OS_WINDOWS
        return 0;
#elif OS_IOS
        return CToObjectCGetScreenInch();
#endif
        return 0;
    }
    void JSConchConfig::setTouchMoveRange(float p_fMM)
    {
#ifdef OS_ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setTouchMoveRange", p_fMM,kRet);
#elif OS_WINDOWS

#endif
    }

void JSConchConfig::setScreenOrientation(int p_nOrientation)
    {
        enum ori {
            landscape = 0,
            portrait = 1,
            user = 2,
            behind = 3,
            sensor = 4,
            nosensor = 5,
            sensor_landscape = 6,
            sensor_portrait = 7,
            reverse_landscape = 8,
            reverse_portrait = 9,
            full_sensor = 10,
            num
        };
        bool vbLandscapes[] = { true,false,false,false,false,false,true,false,true,false,false };
        if (p_nOrientation >= num) {
            return;
        }
        int w = g_nInnerWidth > g_nInnerHeight ? g_nInnerWidth : g_nInnerHeight;
        int h = g_nInnerWidth > g_nInnerHeight ? g_nInnerHeight : g_nInnerWidth;
#ifdef OS_ANDROID
        if (vbLandscapes[p_nOrientation]) {
            g_nInnerWidth = w;
            g_nInnerHeight = h;
        }
        else {
            g_nInnerWidth = h;
            g_nInnerHeight = w;
        }
#endif
        g_bGLCanvasSizeChanged = true;
#ifdef OS_ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setScreenOrientation", p_nOrientation, kRet);
#elif OS_WINDOWS
        MoveWindow(g_hWnd, 0, 0, g_nInnerWidth, g_nInnerHeight, true);
#elif OS_IOS
        CToObjectCSetScreenOrientation(p_nOrientation);
#elif OS_OHOS
        int orientation = 0;
        if(p_nOrientation == landscape) {
           orientation = 2;
        } else if(p_nOrientation == portrait) {
           orientation = 1;
        } else if(p_nOrientation == user) {
           orientation = 0;
        } else if(p_nOrientation == behind) {
           orientation = 5;
        } else if(p_nOrientation == nosensor) {
           orientation = 11;
        } else if(p_nOrientation == sensor_landscape) {
           orientation = 7;
        } else if(p_nOrientation == sensor_portrait) {
           orientation = 6;
        } else if(p_nOrientation == reverse_landscape) {
           orientation = 4;
        } else if(p_nOrientation == reverse_portrait) {
           orientation = 3;
        } else if(p_nOrientation == sensor || p_nOrientation == full_sensor) {
           orientation = 3;
        }
        NapiHelper::GetInstance()->setPreferredOrientation(orientation);
#endif

    }

    void JSConchConfig::setUrlIgnoreCase(bool b) {
        JCConch::s_pScriptRuntime->m_pFileResMgr->m_bUrlToLowerCase = b;
    }

    bool JSConchConfig::getUrlIgnoreCase() {
        return JCConch::s_pScriptRuntime->m_pFileResMgr->m_bUrlToLowerCase;
    }

    int JSConchConfig::getNetworkType()
    {
        return OS::getNetworkType();
    }

	const char* JSConchConfig::getIPAddress()
	{
#ifdef OS_ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getIPAddress", kRet))
		{
			m_sIPAddress = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
		}
		return m_sIPAddress.c_str();
#elif OS_WINDOWS
		return "";
#elif OS_IOS
		//m_sIPAddress = CToObjectCIPAddress();
		return m_sIPAddress.c_str();
#endif
		return "";
	}

    void JSConchConfig::setDownloadTryNum(int p_nOptTry, int p_nConnTry) {
        JCDownloadMgr* pDmgr = JCDownloadMgr::getInstance();
        if (pDmgr) {
            pDmgr->setOpt_tryNumOnTimeout(p_nOptTry, p_nConnTry);
        }
    }

    void JSConchConfig::setDownloadPathReplace(const char* p_pszPath, const char* p_pszReplace) {
        JCDownloadMgr* pDmgr = JCDownloadMgr::getInstance();
        if (pDmgr) {
            pDmgr->setFinalReplacePath(p_pszPath, p_pszReplace);
        }
    }

    void JSConchConfig::setDownloadTail(int type, const char* p_strTail) {
        JCDownloadMgr* pDmgr = JCDownloadMgr::getInstance();
        if (pDmgr) {
            pDmgr->setDownloadTail(type, p_strTail);
        }
    }

    void JSConchConfig::setDownloadReplaceExt(const char* p_pszOrigin, const char* p_pszNew) {

    }

    void JSConchConfig::setDownloadIgnoreCRLR(bool b) {
       
    }

    void JSConchConfig::resetDownloadIgnoreCRLR() {
       
    }

    void JSConchConfig::addChkIgnoreChksumExt(const char* p_pszExt) {
       
    }

    void JSConchConfig::clearChkIgnoreChksumExt() {
       
    }

    void JSConchConfig::setDownloadUnmask(const char* p_pszExt, unsigned int p_nKey, int p_nLen) {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        if (pdmgr) {
            pdmgr->setDownloadUnmask(p_pszExt, p_nKey, p_nLen);
        }
    }

    void JSConchConfig::resetDownloadUnmask() {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        if (pdmgr) {
            pdmgr->resetDownloadUnmask();
        }
    }

    void JSConchConfig::setDownloadNoResponseTimeout(int p_nDuration) {
        JCDownloadMgr::s_nNoResponseTimeout = p_nDuration;
    }

    void JSConchConfig::resetDownloadNoResponseTimeout() {
        JCDownloadMgr::s_nNoResponseTimeout = 15000;
    }
    const char* JSConchConfig::getOS()
    {
#ifdef OS_IOS
        return "Conch-ios";
#elif OS_ANDROID
        return "Conch-android";
#elif OS_WINDOWS
        return "Conch-window";
#elif OS_OHOS
        return "Conch-ohos";
#elif OS_LINUX
        return "Conch-linux";
#endif
    }
    const char* JSConchConfig::getBrowserInfo()
    {
#ifdef OS_IOS
        return "Conch-ios";
#elif OS_ANDROID
        return "Conch-android";
#elif OS_WINDOWS
        return "Conch-window";
#elif OS_OHOS
        return "Conch-ohos";
#elif OS_LINUX
        return "Conch-linux";
#endif
    }
    const char* JSConchConfig::getGuid()
    {
#ifdef OS_IOS
        m_sGUID = CToObjectCGetGUID();
        return m_sGUID.c_str();
#elif OS_ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getWifiMac", kRet))
        {
            m_sGUID = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
        }
        LOGI("getGuid::get_Value=%s", m_sGUID.c_str());
        return m_sGUID.c_str();
#elif OS_WINDOWS
        return "window";
#elif OS_LINUX
        return "linux";
    #else
    return "";
#endif
    }
    const char* JSConchConfig::getRuntimeVersion()
    {
#ifdef OS_IOS
        return "ios-conch6-release-3.3.0-beta.4";
#elif OS_ANDROID
        return "android-conch6-release-3.3.0-beta.4";
#elif OS_WINDOWS
        return "window-conch6-release-3.3.0-beta.4";
#elif OS_OHOS
        return "ohos-conch6-release-3.3.0-beta.4";
#elif OS_LINUX
        return "linux-conch6-release-3.3.0-beta.4";
#endif
    }
	//机型
	const char* JSConchConfig::getModel()
	{
#ifdef OS_IOS
		m_sModel = CToObjectCGetModel();
		return m_sModel.c_str();
#elif OS_ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getModel", kRet))
		{
			m_sModel = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sModel.c_str();
		}
		return "";
#elif OS_WINDOWS
		return "";
#elif OS_LINUX
        return "";
#endif
	}
	//国家&地区
	const char* JSConchConfig::getCountryCode()
	{
#ifdef OS_IOS
		m_sCountryCode = CToObjectCGetCountryCode();
		return m_sCountryCode.c_str();
#elif OS_ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getCountryCode", kRet))
		{
            m_sCountryCode = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sCountryCode.c_str();
		}
		return "";
#elif OS_WINDOWS
		return "";
#elif OS_LINUX
        return "";
#endif
	}

    //设备国家&地区码
    const char* JSConchConfig::getTelCountryCode()
    {
#ifdef OS_IOS
        m_sCountryCode = CToObjectCGetTelCountryCode();
		return m_sCountryCode.c_str();
#elif OS_ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getTelCountryCode", kRet))
        {
            m_sCountryCode = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
            return m_sCountryCode.c_str();
        }
        return "";
#elif OS_WINDOWS
        return "";
#elif OS_LINUX
        return "";
#endif
    }

	const char* JSConchConfig::getLanguage()
	{
#ifdef OS_IOS
		m_sLanguage = CToObjectCGetLanguage();
		return m_sLanguage.c_str();
#elif OS_ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getLanguage", kRet))
		{
			m_sLanguage = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sLanguage.c_str();
	}
		return "";
#elif OS_WINDOWS
		return "";
#elif OS_LINUX
        return "";
#endif
	}
	void JSConchConfig::setLanguage(const char* pStrLanguage)
	{
#ifdef OS_WINDOWS
#elif OS_ANDROID
		std::string strBuffer = pStrLanguage;
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setLanguage", strBuffer.c_str(), kRet);
#elif OS_IOS
		CToObjectCSetLanguage(pStrLanguage);
#endif
	}
    const char* JSConchConfig::getAppVersion()
    {
#ifdef OS_IOS
        m_sAppVersion = CToObjectCGetAppVersion();
        return m_sAppVersion.c_str();
#elif OS_ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getAppVersion", kRet))
        {
			m_sAppVersion = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sAppVersion.c_str();
        }
        return "";
#elif OS_OHOS
        m_sAppVersion = NapiHelper::GetInstance()->getAppVersion();
        return m_sAppVersion.c_str();
#elif OS_WINDOWS
        return "3.0";
#elif OS_LINUX
        return "3.0";
#endif
    }
    const char* JSConchConfig::getAppLocalVersion()
    {
#ifdef OS_IOS
        m_sAppLocalVersion = CToObjectCGetAppLocalVersion();
        return m_sAppLocalVersion.c_str();
#elif OS_ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getAppLocalVersion", kRet))
        {
			m_sAppLocalVersion = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sAppLocalVersion.c_str();
        }
        return "";
#elif OS_OHOS
        m_sAppLocalVersion = NapiHelper::GetInstance()->getAppLocalVersion();
        return m_sAppLocalVersion.c_str();
#elif OS_WINDOWS
        return "3.0";
#elif OS_LINUX
        return "3.0";
#endif
    }
    const char* JSConchConfig::getDeviceInfo()
    {
#ifdef OS_IOS
        m_sDeviceInfo = CToObjectCGetDeviceInfo();
        return m_sDeviceInfo.c_str();
#elif OS_ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "GetDeviceInfo", kRet))
        {
            m_sDeviceInfo = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
        }
        LOGI("getDeviceInfo::get_Value=%s", m_sDeviceInfo.c_str());
        return m_sDeviceInfo.c_str();
#elif OS_OHOS
        m_sDeviceInfo = NapiHelper::GetInstance()->getDeviceInfo();
        return m_sDeviceInfo.c_str();
#elif OS_WINDOWS || OS_LINUX
        return "{\"resolution\":\"1920*1080\",	\"guid\":\"xxxxxxxxx\",\"imei\":[\"imeixxx\"],\"imsi\":[\"imsixxx\"],\"os\":\"windows\",\"osversion\":\"windows7 64\",\"phonemodel\":\"Wintel\"	}";
#endif
    }
    float JSConchConfig::getCurrentDeviceSystemVersion()
    {
#ifdef OS_IOS
        return CToObjectCGetDeviceSystemVersion();
#else
        return 0.0f;
#endif
    }
    void JSConchConfig::setScreenScale(float fScaleW, float fScaleH, float ftx, float fty) 
    {
        m_fScreenTx = ftx;
        m_fScreenTy = fty;
        m_fScreenScaleW = fScaleW;
        m_fScreenScaleH = fScaleH;
        JCLayaGL::s_fMainCanvasScaleX = fScaleW;
        JCLayaGL::s_fMainCanvasScaleY = fScaleH;
        JCLayaGL::s_fMainCanvasTX = ftx;
        JCLayaGL::s_fMainCanvasTY = fty;
    }
    float JSConchConfig::getScreenScaleW() 
    {
        return m_fScreenScaleW;
    }
    float JSConchConfig::getScreenScaleH() 
    {
        return m_fScreenScaleH;
    }
    void JSConchConfig::setCurlProxy(const char* pProxy) {
        if (pProxy) {
            LOGI("setCurlProxy %s", pProxy);
            JCDownloadMgr::s_curlProxyString = pProxy;
            JCDownloadMgr::getInstance()->setProxyString(pProxy);
        }
    }

    void JSConchConfig::setWebsocketProxy(const char* pProxy) {
        if (pProxy) {
            WebSocket::s_strProxy = pProxy;
            LOGI("setWebsocketProxy:%s", pProxy);
        }
    }
    void JSConchConfig::setTouchMode(bool bMode)
    {
        JSInput::getInstance()->setTouchMode(bMode);
    }
    bool JSConchConfig::getTouchMode()
    {
        return JSInput::getInstance()->getTouchMode();
    }
    void JSConchConfig::setLogLevel(int nLevel)
    {
        g_nLogLevel = nLevel;
    }
    void JSConchConfig::setImageReleaseSpaceTime(int nSpaceTime)
    {
        JCConch::s_pConch->s_pConchRender->postTaskFromJSToRenderAsync([nSpaceTime]() {
			JCConch::s_pConchRender->m_pImageManager->setReleaseSpaceTime(nSpaceTime);
        });
    }
    void JSConchConfig::enableMemorySurvey(bool bEnable)
    {
        JCMemorySurvey::GetInstance()->setEnable(bEnable);
    }
    void JSConchConfig::setDownloadConnTimeout(int tm) 
    {
        JCDownloadMgr::s_nConnTimeout = tm;
    }
    void JSConchConfig::setDownloadOptTimeout(int tm) 
    {
        JCDownloadMgr::s_nOptTimeout = tm;
    }
    void JSConchConfig::printAllMemorySurvey(const char* sPath)
    {
        std::string sWritePath = "";
        if (sPath == NULL || strlen(sPath) <= 0)
        {
            sWritePath = JCConch::getAppCachePath();
        }
        else
        {
            sWritePath = sPath;
        }
        JCMemorySurvey::GetInstance()->printAll(sWritePath.c_str());
    }
    void JSConchConfig::enableEncodeURI(bool b) 
    {
        JCDownloadMgr::s_bEncodeURI = b;
    }
    void JSConchConfig::setJSDebugMode(int nMode)
    {
        g_kSystemConfig.m_nJSDebugMode = (JS_DEBUG_MODE)nMode;
    }
    int JSConchConfig::getJSDebugMode()
    {
        return g_kSystemConfig.m_nJSDebugMode;
    }
    void JSConchConfig::setJSDebugPort(int nPort)
    {
        g_kSystemConfig.m_nJSDebugPort = nPort;
    }
    int JSConchConfig::getJSDebugPort()
    {
        return g_kSystemConfig.m_nJSDebugPort;
    }
    void JSConchConfig::setSoundGarbageCollectionTime(int nTime)
    {
        JCAudioWavPlayer::s_nGarbageCollectionTime = nTime;
    }
	bool JSConchConfig::getConchWebGL()
	{
		return g_kSystemConfig.m_graphicsAPI == GraphicsAPI::WebGL;//lv todo delete
	}
    GraphicsAPI JSConchConfig::getGraphicsAPI()
    {
        return g_kSystemConfig.m_graphicsAPI;
    }
    int JSConchConfig::getMemoryUsageInByte()
    {
        return OS::getMemoryUsageInByte();
    }
    void JSConchConfig::exportJS(jsbind::Object& context)
    {
        jsbind::global_class_<JSConchConfig> class_binding;

		class_binding.class_function("getStoragePath", &JSConchConfig::getLocalStoragePath);
		class_binding.class_function("getTotalMem", &JSConchConfig::getTotalMem);
		class_binding.class_function("getUsedMem", &JSConchConfig::getUsedMem);
		class_binding.class_function("getAvalidMem", &JSConchConfig::getAvalidMem);
		class_binding.class_function("getScreenInch", &JSConchConfig::getScreenInch);
		class_binding.class_function("setTouchMoveRange", &JSConchConfig::setTouchMoveRange);
		class_binding.class_function("setScreenOrientation", &JSConchConfig::setScreenOrientation);
		class_binding.class_function("setScreenScale", &JSConchConfig::setScreenScale);
		class_binding.class_function("getScreenScaleW", &JSConchConfig::getScreenScaleW);
		class_binding.class_function("getScreenScaleH", &JSConchConfig::getScreenScaleH);
		class_binding.class_function("setUrlIgnoreCase", &JSConchConfig::setUrlIgnoreCase);
		class_binding.class_function("getUrlIgnoreCase", &JSConchConfig::getUrlIgnoreCase);
		class_binding.class_function("getNetworkType", &JSConchConfig::getNetworkType);
		class_binding.class_function("getRuntimeVersion", &JSConchConfig::getRuntimeVersion);
		class_binding.class_function("setDownloadTryNum", &JSConchConfig::setDownloadTryNum);
		class_binding.class_function("setDownloadPathReplace", &JSConchConfig::setDownloadPathReplace);
		class_binding.class_function("setDownloadTail", &JSConchConfig::setDownloadTail);
		 class_binding.class_function("setDownloadNoResponseTimeout", &JSConchConfig::setDownloadNoResponseTimeout);
		 class_binding.class_function("setDownloadReplaceExt", &JSConchConfig::setDownloadReplaceExt);
		 class_binding.class_function("setDownloadIgnoreCRLR", &JSConchConfig::setDownloadIgnoreCRLR);
		 class_binding.class_function("addChkIgnoreChksumExt", &JSConchConfig::addChkIgnoreChksumExt);
		 class_binding.class_function("clearChkIgnoreChksumExt", &JSConchConfig::clearChkIgnoreChksumExt);
		 class_binding.class_function("setDownloadUnmask", &JSConchConfig::setDownloadUnmask);
		 class_binding.class_function("resetDownloadUnmask", &JSConchConfig::resetDownloadUnmask);
		 class_binding.class_function("getOS", &JSConchConfig::getOS);
		 class_binding.class_function("getAppVersion", &JSConchConfig::getAppVersion);
		 class_binding.class_function("getAppLocalVersion", &JSConchConfig::getAppLocalVersion);
		 class_binding.class_function("getBrowserInfo", &JSConchConfig::getBrowserInfo);
		 class_binding.class_function("getGuid", &JSConchConfig::getGuid);
		 class_binding.class_function("getDeviceInfo", &JSConchConfig::getDeviceInfo);
		 class_binding.class_function("setCurlProxy", &JSConchConfig::setCurlProxy);
		 class_binding.class_function("setWebsocketProxy", &JSConchConfig::setWebsocketProxy);
		 class_binding.class_function("setTouchMode", &JSConchConfig::setTouchMode);
		 class_binding.class_function("getTouchMode", &JSConchConfig::getTouchMode);
		 class_binding.class_function("setLogLevel", &JSConchConfig::setLogLevel);
		 class_binding.class_function("setImageReleaseSpaceTime", &JSConchConfig::setImageReleaseSpaceTime);
		 class_binding.class_function("enableMemorySurvey", &JSConchConfig::enableMemorySurvey);
		 class_binding.class_function("getIPAddress", &JSConchConfig::getIPAddress);
		 class_binding.class_function("setDownloadConnTimeout", &JSConchConfig::setDownloadConnTimeout);
		 class_binding.class_function("setDownloadOptTimeout", &JSConchConfig::setDownloadOptTimeout);
		 class_binding.class_function("printAllMemorySurvey", &JSConchConfig::printAllMemorySurvey);
		 class_binding.class_function("enableEncodeURI", &JSConchConfig::enableEncodeURI);
         class_binding.class_function("getGraphicsAPI", &JSConchConfig::getGraphicsAPI);
		 class_binding.class_function("setSoundGarbageCollectionTime", &JSConchConfig::setSoundGarbageCollectionTime);
		 class_binding.class_function("setLanguage", &JSConchConfig::setLanguage);
		 class_binding.class_function("getLanguage", &JSConchConfig::getLanguage);
         class_binding.class_function("getModel", &JSConchConfig::getModel);
        class_binding.class_function("getCountryCode", &JSConchConfig::getCountryCode);
        class_binding.class_function("getTelCountryCode", &JSConchConfig::getTelCountryCode);
        class_binding.class_function("getMemoryUsageInByte", &JSConchConfig::getMemoryUsageInByte);
         //class_property必须在下面，否则导不出class_function
        class_binding.class_property("JSDebugMode", &JSConchConfig::getJSDebugMode, &JSConchConfig::setJSDebugMode);
        class_binding.class_property("JSDebugPort", &JSConchConfig::getJSDebugPort, &JSConchConfig::setJSDebugPort);
        class_binding.class_property("conchWebGL", &JSConchConfig::getConchWebGL);
        class_binding.class_property("urlIgnoreCase", &JSConchConfig::getUrlIgnoreCase, &JSConchConfig::setUrlIgnoreCase);
        context.global_class_("conchConfig", class_binding);
    }
}
#include "JSConchConfig.h"
#include "JSInput.h"
#include "resource/JCFileResManager.h"
#include "../../JCScriptRuntime.h"
#include "JCConch.h"
#ifdef ANDROID
    #include "CToJavaBridge.h"
#elif WIN32
    #include <Windows.h>
#elif __APPLE__
    #include "CToObjectC.h"
#endif
#include "downloadMgr/JCDownloadMgr.h"
#include <Utils/Log.h>
#include "../../JCSystemConfig.h"
#include "../../JCConchRender.h"
#include "../../JCConch.h"
#include "../../WebSocket/WebSocket.h"
#include <resource/Audio/JCAudioWavPlayer.h>
#include "../../Audio/JCAudioManager.h"
#include <LayaGL/JCLayaGL.h>
#include <LayaGL/JCLayaGLDispatch.h>
#include <string>

extern int g_nDebugLevel;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;

#ifdef WIN32
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


    const char* JSConchConfig::getLocalStoragePath()
    {
        if (JCConch::s_pConch)
            return JCConch::s_pConch->getLocalStoragePath();
        return "";
    }
    float JSConchConfig::getTotalMem()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getTotalMem", kRet, CToJavaBridge::JavaRet::RT_Float))
        {
            return kRet.floatRet;
        }
        return 0;
#elif WIN32
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);
        return (float)(statex.ullTotalPhys / 1024);
#elif __APPLE__
        return CToObjectCGetTotalMem();
#endif
        return 0;
    }
    int JSConchConfig::getUsedMem()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getUsedMem", kRet, CToJavaBridge::JavaRet::RT_Float))
        {
            return (int)(kRet.floatRet);
        }
        return 0;
#elif __APPLE__
        return CToObjectCGetUsedMem();
#elif WIN32
        return getAppUsedMem();
#endif
        return 0;
    }
    int JSConchConfig::getAvalidMem()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getAvalidMem", kRet, CToJavaBridge::JavaRet::RT_Float))
        {
            return (int)(kRet.floatRet);
        }
        return 0;
#elif WIN32
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);
        return (int)(statex.ullAvailPhys / 1024);
#elif __APPLE__
        return CToObjectCGetAvalidMem();
#endif
        return 0;
    }
    float JSConchConfig::getScreenInch()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getScreenInch", kRet, CToJavaBridge::JavaRet::RT_Float))
        {
            return kRet.floatRet;
        }
        return 0;
#elif WIN32
        return 0;
#elif __APPLE__
        return CToObjectCGetScreenInch();
#endif
        return 0;
    }
    void JSConchConfig::setTouchMoveRange(float p_fMM)
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setTouchMoveRange", p_fMM,kRet);
#elif WIN32

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
#ifdef ANDROID
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
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setScreenOrientation", p_nOrientation, kRet);
#elif WIN32
        MoveWindow(g_hWnd, 0, 0, g_nInnerWidth, g_nInnerHeight, true);
#elif __APPLE__
        CToObjectCSetScreenOrientation(p_nOrientation);
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
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getContextedType", kRet, CToJavaBridge::JavaRet::RT_Int))
        {
            return kRet.intRet;
        }
        return 1;
#elif WIN32
        return 1;
#elif __APPLE__
        return CToObjectCGetNetworkType();
#endif
        return 0;
    }

	const char* JSConchConfig::getIPAddress()
	{
#ifdef ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getIPAddress", kRet))
		{
			m_sIPAddress = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
		}
		return m_sIPAddress.c_str();
#elif WIN32
		return "";
#elif __APPLE__
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

    void JSConchConfig::setDownloadVersionString(const char* p_pszVersion) {
        if (p_pszVersion && strlen(p_pszVersion) > 0) {
            JCFileResDCC::s_strExtVersion = p_pszVersion;
            JCFileResDCC::s_strExtVersion += "=";
        }
        else
            JCFileResDCC::s_strExtVersion = "";
    }

    const char* JSConchConfig::getOS()
    {
#ifdef __APPLE__
        return "Conch-ios";
#elif ANDROID
        return "Conch-android";
#elif WIN32
        return "Conch-window";
#endif
    }
    const char* JSConchConfig::getBrowserInfo()
    {
#ifdef __APPLE__
        return "Conch-ios";
#elif ANDROID
        return "Conch-android";
#elif WIN32
        return "Conch-window";
#endif
    }
    const char* JSConchConfig::getGuid()
    {
#ifdef __APPLE__
        m_sGUID = CToObjectCGetGUID();
        return m_sGUID.c_str();
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getWifiMac", kRet))
        {
            m_sGUID = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
        }
        LOGI("getGuid::get_Value=%s", m_sGUID.c_str());
        return m_sGUID.c_str();
#elif WIN32
        return "window";
#endif
    }
    const char* JSConchConfig::getRuntimeVersion()
    {
#ifdef __APPLE__
        return "ios-conch6-release-3.1.0-beta.1";
#elif ANDROID
        return "android-conch6-release-3.1.0-beta.1";
#elif WIN32
        return "window-conch6-release-3.1.0-beta.1";
#endif
    }
	//机型
	const char* JSConchConfig::getModel()
	{
#ifdef __APPLE__
		m_sModel = CToObjectCGetModel();
		return m_sModel.c_str();
#elif ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getModel", kRet))
		{
			m_sModel = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sModel.c_str();
		}
		return "";
#elif WIN32
		return "";
#endif
	}
	//国家&地区
	const char* JSConchConfig::getCountryCode()
	{
#ifdef __APPLE__
		m_sCountryCode = CToObjectCGetCountryCode();
		return m_sCountryCode.c_str();
#elif ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getCountryCode", kRet))
		{
            m_sCountryCode = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sCountryCode.c_str();
		}
		return "";
#elif WIN32
		return "";
#endif
	}

    //设备国家&地区码
    const char* JSConchConfig::getTelCountryCode()
    {
#ifdef __APPLE__
        m_sCountryCode = CToObjectCGetTelCountryCode();
		return m_sCountryCode.c_str();
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getTelCountryCode", kRet))
        {
            m_sCountryCode = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
            return m_sCountryCode.c_str();
        }
        return "";
#elif WIN32
        return "";
#endif
    }

	const char* JSConchConfig::getLanguage()
	{
#ifdef __APPLE__
		m_sLanguage = CToObjectCGetLanguage();
		return m_sLanguage.c_str();
#elif ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getLanguage", kRet))
		{
			m_sLanguage = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sLanguage.c_str();
	}
		return "";
#elif WIN32
		return "";
#endif
	}
	void JSConchConfig::setLanguage(const char* pStrLanguage)
	{
#ifdef WIN32
#elif ANDROID
		std::string strBuffer = pStrLanguage;
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setLanguage", strBuffer.c_str(), kRet);
#elif __APPLE__
		CToObjectCSetLanguage(pStrLanguage);
#endif
	}
    const char* JSConchConfig::getAppVersion()
    {
#ifdef __APPLE__
        m_sAppVersion = CToObjectCGetAppVersion();
        return m_sAppVersion.c_str();
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getAppVersion", kRet))
        {
			m_sAppVersion = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sAppVersion.c_str();
        }
        return "";
#elif WIN32
        return "2.0";
#endif
    }
    const char* JSConchConfig::getAppLocalVersion()
    {
#ifdef __APPLE__
        m_sAppLocalVersion = CToObjectCGetAppLocalVersion();
        return m_sAppLocalVersion.c_str();
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getAppLocalVersion", kRet))
        {
			m_sAppLocalVersion = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
			return m_sAppLocalVersion.c_str();
        }
        return "";
#elif WIN32
        return "2.0";
#endif
    }
    bool JSConchConfig::getIsPlug() 
    {
        return JCSystemConfig::s_bIsPlug != 0;
    }

    const char* JSConchConfig::getJsonparamExt()
    {
        return g_kSystemConfig.m_jsonparamExt.c_str();
    }
    const char* JSConchConfig::getDeviceInfo()
    {
#ifdef __APPLE__
        m_sDeviceInfo = CToObjectCGetDeviceInfo();
        return m_sDeviceInfo.c_str();
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "GetDeviceInfo", kRet))
        {
            m_sDeviceInfo = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
        }
        LOGI("getDeviceInfo::get_Value=%s", m_sDeviceInfo.c_str());
        return m_sDeviceInfo.c_str();
#elif WIN32
        return "{\"resolution\":\"1920*1080\",	\"guid\":\"xxxxxxxxx\",\"imei\":[\"imeixxx\"],\"imsi\":[\"imsixxx\"],\"os\":\"windows\",\"osversion\":\"windows7 64\",\"phonemodel\":\"Wintel\"	}";
#endif
    }
    float JSConchConfig::getCurrentDeviceSystemVersion()
    {
#ifdef __APPLE__
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
    bool JSConchConfig::getLocalable()
    {
        return JCSystemConfig::s_bLocalizable;
    }
    void JSConchConfig::setLocalable(bool isLocalPackage)
    {
        JCSystemConfig::s_bLocalizable = isLocalPackage;
    }
    void JSConchConfig::setMouseFrame(double thredholdms)
    {
        g_kSystemConfig.m_nFrameType = FT_MOUSE;
        g_kSystemConfig.m_nFrameThreshold = thredholdms;
        g_kSystemConfig.m_nSleepTime = 32;
    }
    void JSConchConfig::setSlowFrame(bool p_bIsSlow)
    {
        g_kSystemConfig.m_nFrameType = p_bIsSlow ? FT_SLOW : FT_FAST;
        g_kSystemConfig.m_nSleepTime = 32;
    }
    void JSConchConfig::setLimitFPS(int nFpsNum)
    {
        if( nFpsNum >= 60 )
        {
            g_kSystemConfig.m_nFrameType = FT_FAST;
        }
        else
        {
            g_kSystemConfig.m_nFrameType = FT_SLOW;
            g_kSystemConfig.m_nSleepTime = 1000 / nFpsNum - 1;
        }
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
    void JSConchConfig::setDebugLevel(int nLevel)
    {
        g_nDebugLevel = nLevel;
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
    void JSConchConfig::showInternalPerfBar(int b, float scale) 
    {
        if (scale == 0.0f)scale = 10.0f;

        if (b > 0) {
            g_kSystemConfig.m_bShowInternalPerBar = true;

            //JCPerfHUD::addData(new perfBarData(JCPerfHUD::PHUD_BAR_JS_ONDRAW, 0x6600ff00, "jsbar", scale));
            //JCPerfHUD::addData(new perfBarData(JCPerfHUD::PHUD_BAR_RENDER, 0x66ff0000, "renderbar", scale));
            //JCPerfHUD::addData(new perfBarData(JCPerfHUD::PHUD_BAR_JSWAIT, 0x66003300, "jswait", scale));
            //JCPerfHUD::addData(new perfBarData(JCPerfHUD::PHUD_BAR_GLWAIT, 0x66330000, "glwait", scale));
        }
        else {
            g_kSystemConfig.m_bShowInternalPerBar = false;
            //JCPerfHUD::delData(JCPerfHUD::PHUD_BAR_JS_ONDRAW);
            //JCPerfHUD::delData(JCPerfHUD::PHUD_BAR_RENDER);
            //JCPerfHUD::delData(JCPerfHUD::PHUD_BAR_JSWAIT);
            //JCPerfHUD::delData(JCPerfHUD::PHUD_BAR_GLWAIT);
        }
    }
    void JSConchConfig::testSleep(int tm) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(tm));
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
            sWritePath = JCConch::s_pConch->m_sCachePath.c_str();
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
        JCConch::s_pConch->m_nJSDebugMode = (JS_DEBUG_MODE)nMode;
    }
    int JSConchConfig::getJSDebugMode()
    {
        return JCConch::s_pConch->m_nJSDebugMode;
    }
    void JSConchConfig::setJSDebugPort(int nPort)
    {
        JCConch::s_pConch->m_nJSDebugPort = nPort;
    }
    int JSConchConfig::getJSDebugPort()
    {
        return JCConch::s_pConch->m_nJSDebugPort;
    }
    void JSConchConfig::setSoundGarbageCollectionTime(int nTime)
    {
        JCAudioWavPlayer::s_nGarbageCollectionTime = nTime;
    }
    bool JSConchConfig::getUseDcc()
    {
        return g_kSystemConfig.m_bUseDcc;
    }
	bool JSConchConfig::getConchWebGL()
	{
		return g_kSystemConfig.m_bConchWebGL;
	}
    int JSConchConfig::getMemoryUsageInByte()
    {
#ifdef __APPLE__
        return CToObjectCGetMemoryUsageInByte();
#elif ANDROID
		CToJavaBridge::JavaRet kRet;
		if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getMemoryUsageInByte", kRet, CToJavaBridge::JavaRet::RT_Float))
		{
			return (int)(kRet.floatRet);
		}
		return 0;
#elif WIN32
        return 0;
#endif
    }
    void JSConchConfig::exportJS(Context& context)
    {
        class_<JSConchConfig> class_binding;

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
		 class_binding.class_function("setDownloadVersionString", &JSConchConfig::setDownloadVersionString);
		 class_binding.class_function("getOS", &JSConchConfig::getOS);
		 class_binding.class_function("getAppVersion", &JSConchConfig::getAppVersion);
		 class_binding.class_function("getAppLocalVersion", &JSConchConfig::getAppLocalVersion);
		 class_binding.class_function("getBrowserInfo", &JSConchConfig::getBrowserInfo);
		 class_binding.class_function("getGuid", &JSConchConfig::getGuid);
		 class_binding.class_function("getDeviceInfo", &JSConchConfig::getDeviceInfo);
		 class_binding.class_function("getIsPlug", &JSConchConfig::getIsPlug);
		 class_binding.class_function("setLimitFPS", &JSConchConfig::setLimitFPS);
		 class_binding.class_function("setMouseFrame", &JSConchConfig::setMouseFrame);
		 class_binding.class_function("setSlowFrame", &JSConchConfig::setSlowFrame);
		 class_binding.class_function("setCurlProxy", &JSConchConfig::setCurlProxy);
		 class_binding.class_function("setWebsocketProxy", &JSConchConfig::setWebsocketProxy);
		 class_binding.class_function("setTouchMode", &JSConchConfig::setTouchMode);
		 class_binding.class_function("getTouchMode", &JSConchConfig::getTouchMode);
		 class_binding.class_function("setDebugLevel", &JSConchConfig::setDebugLevel);
		 class_binding.class_function("setImageReleaseSpaceTime", &JSConchConfig::setImageReleaseSpaceTime);
		 class_binding.class_function("enableMemorySurvey", &JSConchConfig::enableMemorySurvey);
		 class_binding.class_function("showInternalPerfBar", &JSConchConfig::showInternalPerfBar);
		 class_binding.class_function("getIPAddress", &JSConchConfig::getIPAddress);
		 class_binding.class_function("test_sleep", &JSConchConfig::testSleep);
		 class_binding.class_function("setDownloadConnTimeout", &JSConchConfig::setDownloadConnTimeout);
		 class_binding.class_function("setDownloadOptTimeout", &JSConchConfig::setDownloadOptTimeout);
		 class_binding.class_function("printAllMemorySurvey", &JSConchConfig::printAllMemorySurvey);
		 class_binding.class_function("enableEncodeURI", &JSConchConfig::enableEncodeURI);

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
        class_binding.class_property("paramExt", &JSConchConfig::getJsonparamExt);
        class_binding.class_property("urlIgnoreCase", &JSConchConfig::getUrlIgnoreCase, &JSConchConfig::setUrlIgnoreCase);
        class_binding.class_property("localizable", &JSConchConfig::getLocalable, &JSConchConfig::setLocalable);
        class_binding.class_property("useDCC", &JSConchConfig::getUseDcc);
        context.class_("conchConfig", class_binding);
    }
}
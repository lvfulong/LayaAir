#include "JSRuntime.h"
#include <utils/JCCrypto.h>
#include "downloadMgr/JCDownloadMgr.h"
#include "../../JCConch.h"
#include <utils/JCFileSource.h>
#ifdef OS_IOS 
    #include "CToObjectC.h"
#endif
#ifdef OS_ANDROID 
#include "CToJavaBridge.h"
#elif defined(OS_OHOS)
    #include "aki/jsbind.h"
    #include <string>
    #include "platform/ohos/napi/helper/NapiHelper.h"
#endif
#include "platform/OS.h"
#include "../../JCSystemConfig.h"
#include "JSInput.h"
#include "JSConchConfig.h"
#include <imageLib/JCImageRW.h>
#include "JSLayaGL.h"
#include "JSZip.h"
#include "JSLaunchOptions.h"
#include "JSArrayBufferRef.h"
#include "2D/FontManager.h"
#include "../downloadCache/DCC2/JSDownloader.h"
#include "JSDevice.h"
#include "JSNetwork.h"

laya::JCZip *g_ZipPackage = NULL;
//------------------------------------------------------------------------------
namespace laya
{
    std::string JSRuntime::m_strReturn;
    void JSRuntime::setOnFrameFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnFrameFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setZipPackage(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSZipPackage = jsbind::Persistent(p_pFunction);
        JSZip* zip = jsbind::as<JSZip*>(JCConch::s_pScriptRuntime->m_pJSZipPackage.getHandle());
		if (zip)
		{
			g_ZipPackage = &zip->m_zip;
		}
    }
    void JSRuntime::setOnDrawFunction(jsvm_value p_pFunction) 
    {
        JCConch::s_pScriptRuntime->m_pJSOnDrawFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setOnResizeFunction(jsvm_value p_onresize)
    {
        JCConch::s_pScriptRuntime->m_pJSOnResizeFunction = jsbind::Persistent(p_onresize);
    }
    void JSRuntime::setOnBlurFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnBlurFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setOnFocusFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnFocusFunction = jsbind::Persistent(p_pFunction);
    }
    void  JSRuntime::setGetWorldTransformFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_bJSBulletGetWorldTransformHandle = jsbind::Persistent(p_pFunction);
    }
    void  JSRuntime::setSetWorldTransformFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_bJSBulletSetWorldTransformHandle = jsbind::Persistent(p_pFunction);
    }
	void  JSRuntime::setBulletDrawLineFunction(jsvm_value p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_bJSBulletDrawLineHandle = jsbind::Persistent(p_pFunction);
	}
	void  JSRuntime::setBulletClearLineFunction(jsvm_value p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_bJSBulletClearLineHandle = jsbind::Persistent(p_pFunction);
	}
    void JSRuntime::setBuffer(jsbind::ArrayBuffer arrayBuffer) 
    {
        bool bIsArrayBuffer = arrayBuffer.isValid();
        if (bIsArrayBuffer)
        {
            JCConch::s_pScriptRuntime->m_pOtherBufferSharedWidthJS = reinterpret_cast<char*>(arrayBuffer.getData());
        }
        else {
            LOGE("JSRuntime::setCmdBuffer param is not an ArrayBuffer!");
        }
    }
    void JSRuntime::setHref(jsvm_value p_sHref)
    {
        jsbind::Local href(p_sHref);
        if (href.isString())
        {
            std::string strHef = href.as<std::string>();
            if (!strHef.empty())
            {
                std::string url = JCConch::s_pScriptRuntime->m_pUrl->resolve(strHef.c_str());
                g_kSystemConfig.m_strStartURL = url;
                g_kSystemConfig.m_strStartURL.at(0) = g_kSystemConfig.m_strStartURL.at(0);
                JCConch::s_pConch->urlHistoryPush(url.c_str());
                JCDownloadMgr* pdm = JCDownloadMgr::getInstance();
                if (pdm) 
                {
                    pdm->resetFinalReplacePath();
                    pdm->resetDownloadTail();
                    pdm->resetDownloadReplaceExt();
                }
                JCConch::s_pScriptRuntime->m_pUrl->parse(url.c_str());
                std::string tempurl = JCConch::s_pScriptRuntime->m_pUrl->m_Host;
                JCEncrypt::getpassCode(tempurl);

                std::string ss = JCConch::s_pScriptRuntime->m_pUrl->m_Host;
                int n = ss.find(':');
                if (n>0)
                    ss.at(n) = '.';

                std::string cookiefile = JSConchConfig::getLocalStoragePath() + "/" + ss + "_curlcookie.txt";
                pdm->setCookieFile(cookiefile.c_str());
            }
        }
    }
    void JSRuntime::setMouseEvtFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSMouseEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setTouchEvtFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSTouchEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setDeviceMotionEvtFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSDeviceMotionEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setKeyEvtFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSKeyEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setNetworkEvtFunction(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSNetworkEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::captureScreen(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnceOtherEvtFuction = jsbind::Persistent(p_pFunction);
        JCConch::s_pConchRender->requestCaptureScreen();
    }
    std::string JSRuntime::getCachePath() 
    {
        return JCConch::getAppCachePath();
    }
    unsigned char* _readAssetAlloc(int sz, void* pUserData) 
    {
        auto ret = new unsigned char[sz];
        *(unsigned char**)pUserData = ret;
        return ret;
    }
    jsvm_value JSRuntime::readFileFromAsset(const char* file, const char* encode) 
    {
        if (!JCConch::s_pScriptRuntime->m_pAssetsRes)
        {
            return jsbind::MakeNull();
        }
        int sz = 0;
        unsigned char* pBuff = NULL;
        if (JCConch::s_pScriptRuntime->m_pAssetsRes->loadFileContent(file, _readAssetAlloc, &pBuff, sz))
        {
            if (strcmp(encode, "utf8") == 0)
            {
                std::string str;
                str.assign((char*)pBuff, sz);
                delete[] pBuff;
                return jsbind::Make<std::string>(str);
            }
            else 
            {
                //TODO 写一个不用自己保留的AB
                auto ab = jsbind::ArrayBuffer::MakeArrayBuffer((uint8_t*)pBuff, sz);
                //JSArrayBuffer* pab = JSArrayBuffer::create(sz);
                //memcpy(pab->getPtr(), pBuff, sz);
                delete[] pBuff;
                //return (pab->toLocal());;
                return ab.getHandle();
            }
        }
        return jsbind::MakeNull();
    }
    void JSRuntime::setScreenWakeLock(bool bWakeLock)
    {
        OS::setScreenWakeLock(bWakeLock);
    }
    void JSRuntime::setSensorAble(bool bSensorAble)
    {
        OS::setSensorAble(bSensorAble);
    }
    jsvm_value JSRuntime::strTobufer(const char* s)
    {
        int size = (strlen(s) + 1);
        int alignedSize = (size + 3) & 0xfffffffc;
        return jsbind::ArrayBuffer::MakeArrayBuffer((uint8_t*)s, alignedSize).getHandle();
    }
    const char* JSRuntime::getPresetUrl()
    {
        return g_kSystemConfig.m_strStartURL.c_str();
    }
    void JSRuntime::printCorpseImages()
    {
        JCImageManager* pImageManger = JCConch::s_pConchRender->m_pImageManager;
        if (pImageManger == NULL) return;
        std::string sFilePath = JCConch::getLocalStoragePath();
        sFilePath += "/imagesLog.txt";
        pImageManger->printCorpseImages(sFilePath.c_str());
    }
    const char* JSRuntime::callMethod(int objid,bool isSyn,const char*clsName, const char* methodName, const char* paramStr)
    {
#ifdef OS_ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethodRefection(objid, isSyn, clsName, methodName, paramStr, kRet))
        {
            m_strReturn = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
            LOGI("JSRuntime::callMethod %s %s %s", m_strReturn.c_str(), clsName , methodName);
            return m_strReturn.c_str();
        }
        return "";
#elif OS_WINDOWS
        
#elif OS_IOS
        m_strReturn = CToObjectCCallMethod( objid, isSyn, clsName, methodName,paramStr);
        LOGI("JSRuntime::callMethod %s", m_strReturn.c_str());
        return m_strReturn.c_str();
#endif
        return "";
    }
    bool JSRuntime::saveAsPng(jsbind::ArrayBuffer arrayBuffer, int w, int h, const char* p_pszFile)
    {
        bool bIsArrayBuffer = arrayBuffer.isValid();
        if (bIsArrayBuffer)
        { 
            return laya::saveAsPng(reinterpret_cast<char*>(arrayBuffer.getData()), w, h, p_pszFile);
        }
        return false;
    }
    bool JSRuntime::saveAsJpeg(jsbind::ArrayBuffer arrayBuffer, int w, int h, const char* p_pszFile)
    {
        bool bIsArrayBuffer = arrayBuffer.isValid();
        if (bIsArrayBuffer)
        {
            ImageBaseInfo info;
            info.m_nBpp = 32;
            info.m_nWidth = w;
            info.m_nHeight = h;
            return laya::saveAsJpeg(reinterpret_cast<char*>(arrayBuffer.getData()), info, p_pszFile);
        }
        return false;
    }
    jsvm_value JSRuntime::convertBitmapToPng(jsbind::ArrayBuffer arrayBuffer, int w, int h)
    {
        bool bIsArrayBuffer = arrayBuffer.isValid();
        if (bIsArrayBuffer)
        {
            std::pair<unsigned char*, unsigned long> ret = laya::convertBitmapToPng(reinterpret_cast<const char*>(arrayBuffer.getData()), w, h, 8);
            if (ret.first != nullptr)
            {
                return jsbind::ArrayBuffer::MakeArrayBuffer(reinterpret_cast<uint8_t*>(ret.first), ret.second).getHandle();
            }
        }
        return jsbind::MakeNull();
    }
    jsvm_value JSRuntime::convertBitmapToJpeg(jsbind::ArrayBuffer arrayBuffer, int w, int h)
    {
        bool bIsArrayBuffer = arrayBuffer.isValid();
        if (bIsArrayBuffer)
        {
            std::pair<unsigned char*, unsigned long> ret = laya::convertBitmapToJpeg(reinterpret_cast<const char*>(arrayBuffer.getData()), w, h, 32);
            if (ret.first != nullptr)
            {
                return jsbind::ArrayBuffer::MakeArrayBuffer(reinterpret_cast<uint8_t*>(ret.first), ret.second).getHandle();
            }
        }
        return jsbind::MakeNull();
    }
	void JSRuntime::exit()
    {
        OS::exit();
    }
	int JSRuntime::getSafeInsetTop()
	{
        return OS::getSafeInsetTop();
	}
	int JSRuntime::GetSafeInsetLeft()
	{
        return OS::getSafeInsetLeft();
	}
	int JSRuntime::GetSafeInsetBottom()
	{
        return OS::getSafeInsetBottom();
	}
	int JSRuntime::GetSafeInsetRight()
	{
        return OS::getSafeInsetRight();
	}

	jsvm_value JSRuntime::getLaunchOptionsSync()
	{
        return jsbind::Make<JSLaunchOptions*>(new JSLaunchOptions());
	}
    void JSRuntime::setOnUnhandledRejection(jsvm_value p_pFunction)
    {
	    JCConch::s_pScriptRuntime->m_pJSOnUnhandledRejectionFunction = jsbind::Persistent(p_pFunction);
    }
    jsvm_value JSRuntime::getOnUnhandledRejection()
    {
        return JCConch::s_pScriptRuntime->m_pJSOnUnhandledRejectionFunction.getHandle();
    }
	void JSRuntime::setScreenOrientation(const char* p_strOrientation, jsvm_value p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_pJSOnScreenOrientationChanged = jsbind::Persistent(p_pFunction);
		static std::unordered_map<std::string, int> nameToVal = {
			{"landscape", 0},{ "portrait", 1 }, {"user", 2 },{ "behind", 3 }, {"sensor", 4 },{ "nosensor", 5 }, {"sensor_landscape", 6 },{ "sensorLandscape", 6 },
		{"sensor_portrait", 7 }, {"sensorPortrait", 7 },{ "reverse_landscape", 8 }, {"reverseLandscape", 8 },{ "reverse_portrait", 9 }, {"reversePortrait", 9 },{ "full_sensor", 10 }, {"fullSensor", 10 },
		};
		int nOrientation = 0;
		std::unordered_map<std::string, int>::iterator it = nameToVal.find(p_strOrientation);
		if (it != nameToVal.end())
		{
			nOrientation = it->second;
		}
		JSConchConfig::setScreenOrientation(nOrientation);
		JCConch::s_pScriptRuntime->m_pJSOnScreenOrientationChanged.call<void>(jsvm::global());
	}
	void JSRuntime::setGlobalRepaint(jsvm_value p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_pJSSetGlobalRepaintFunction = jsbind::Persistent(p_pFunction);
	}
	void JSRuntime::setGlobalRepaintCall()
	{
		if (JCConch::s_pScriptRuntime->m_pJSSetGlobalRepaintFunction.isValid())
		{
			JCConch::s_pScriptRuntime->m_pJSSetGlobalRepaintFunction.call<void>(jsvm::global());
		}
	}
    void JSRuntime::setOnError(jsvm_value p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnErrorFunction = jsbind::Persistent(p_pFunction);

    }
	struct SkinnedMatrixCache
	{
		int subMeshIndex;
		int batchIndex;
		int batchBoneIndex;
	};
    jsvm_value JSRuntime::createArrayBufferRef(jsbind::ArrayBuffer arrayBuffer, int nType, bool bSyncToRender, int nRefType)
    {
       
        bool bIsArrayBuffer = arrayBuffer.isValid();
        if (bIsArrayBuffer)
        { 
            char* pBuffer = reinterpret_cast<char*>(arrayBuffer.getData());
            int nABLen = arrayBuffer.getByteLength();
            JSArrayBufferRef* pArrayBufferRef = new JSArrayBufferRef();
            pArrayBufferRef->m_bSyncToRender = bSyncToRender;
            {
                pArrayBufferRef->m_nID = JCConch::s_pScriptRuntime->m_pArrayBufferManager->createArrayBuffer(pBuffer, nABLen, (JCArrayBufferManager::ARRAY_BUFFER_TYPE)nType, (JCArrayBufferManager::ARRAY_BUFFER_REF_TYPE)nRefType);
            }
            return jsbind::Make<JSArrayBufferRef*>(pArrayBufferRef);
        }
        LOGE("JSRuntime::createArrayBufferRef type error");
        return jsbind::MakeNull();
    }
    bool JSRuntime::registerFont(const std::string& family, jsbind::Local pathOrArrayBuffer)
    {
        if (pathOrArrayBuffer.isString())
        {
            std::string path = pathOrArrayBuffer.as<std::string>();
            return FontManager::registerFont(family, path);
        }
        else if (pathOrArrayBuffer.isArrayBuffer() || pathOrArrayBuffer.isArrayBufferView())
        {
            auto arrayBuffer = pathOrArrayBuffer.as<jsbind::ArrayBuffer>();
            return FontManager::registerFont(family, arrayBuffer.getData(), arrayBuffer.getByteLength());
        }
        LOGI("registerFont failed");
        return false;
    }
#if defined(USE_DCC)
    void onProgJS(unsigned int now, unsigned int total, float speed, std::shared_ptr<jsbind::Persistent>& jsOnProg)
    {
        auto onProgressLocal = jsOnProg->getLocal();
        if (onProgressLocal.isValid() && onProgressLocal.isFunction())
        {
            onProgressLocal.call<void>(jsvm::global(), now, total, speed);
            //释放持久句柄
            //jsOnProg->reset();
        }
        else 
        {
            // 抛出错误或处理非函数情况
        }
    }
    int onprog(unsigned int now, unsigned int total, float speed, std::shared_ptr<jsbind::Persistent>& jsOnProg) {
        postToJS(std::bind(onProgJS, now, total, speed, jsOnProg));
        return 0;
    }

    void onDownloaded_JS(JCBuffer & p_Buff,
            int pnCurlRet,
            int pnHttpRet,
            std::shared_ptr<jsbind::Persistent>& jsOnComp,
            std::shared_ptr<jsbind::Persistent>& jsOnProg
    ) {
        auto onCompleteLocal = jsOnComp->getLocal();
        if (onCompleteLocal.isValid() && onCompleteLocal.isFunction())
        {
            auto ab = jsbind::ArrayBuffer::MakeArrayBuffer((uint8_t*)p_Buff.m_pPtr, p_Buff.m_nLen);
            onCompleteLocal.call<void>(jsvm::global(), ab, "","");
            //释放持久句柄
            jsOnComp->reset();
            jsOnProg->reset();
        }
        else 
        {
            // 抛出错误或处理非函数情况
        }
        
        //释放buffer
        p_Buff.m_bNeedDel = true;
        p_Buff.free();
    }

    void onDownloaded(
        JCBuffer& p_Buff,
        const std::string& pLocalAddr,
        const std::string& pSvAddr,
        int pnCurlRet,
        int pnHttpRet,
        const std::string& pstrHeader,
        std::shared_ptr<jsbind::Persistent>& jsOnComp,
        std::shared_ptr<jsbind::Persistent>& jsOnProg) {

        //checkIsEncrypted(p_Buff.m_pPtr, p_Buff.m_nLen);
        //if (gHandleDataFunc) {
        //    int nNewlen = m_nLength;
        //    char* pNewData = gHandleDataFunc(m_pBuffer.get(), nNewlen);
        //    if (pNewData) {
        //        m_nLength = nNewlen;
        //        m_pBuffer.reset(pNewData);
        //    }
        //}

        p_Buff.m_bNeedDel = false; //下载线程不要删除，js那边删
        postToJS(std::bind(onDownloaded_JS, p_Buff, pnCurlRet, pnHttpRet, jsOnComp, jsOnProg));
    }

    /**
    *   不带dcc和缓存的下载
    */
    void JSRuntime::downloadFile(const std::string& url, jsvm_value onProgress, jsvm_value onComplete, jsvm_value onError) {
        if (url.empty()) 
            return;

        auto onCompleteP = std::make_shared<jsbind::Persistent>(onComplete);
        auto onProgP = std::make_shared<jsbind::Persistent>(onProgress);

        JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
        auto onComp = std::bind(onDownloaded,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4,
            std::placeholders::_5,
            std::placeholders::_6, onCompleteP,onProgP);
        auto onProg = std::bind(onprog,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            onProgP);

        pNetLoader->download(url.c_str(), 0, onProg, onComp, 0, 0);
    }

    // 下载完成后在 JS 线程调用回调
    void onDownloadComplete(std::function<void(JCBuffer, const std::string&, const std::string&)> callback) {
        // 在这里使用 DirectDownloader 的结果调用 JavaScript 的回调函数
        // 该函数应由 postToJS 在 JS 线程中回调来执行
        //callback();
    }


    void JSRuntime::setDownloader(jsvm_value obj){
        std::shared_ptr<JSDownloader> jsdownloader = std::make_shared<JSDownloader>();
        jsdownloader->setJSDownloader(obj);
        JCFileResManager* pfsMgr = JCConch::s_pScriptRuntime->m_pFileResMgr;
        pfsMgr->m_pDownloader = jsdownloader;
    }
#endif
    //todo 刷新生命周期
    std::shared_ptr<int> callbackRef(new int(1));
    jsvm_value JSRuntime::postAsyncMessage(jsbind::Local eventName, jsbind::Local data)
    {
        if (eventName.isNull() || eventName.isUndefined())
        {
            LOGE("Error: postAsyncMessage eventName is null or undefined");
            return jsbind::MakeUndefined();
        }
        if (!eventName.isString())
        {
            LOGE("Error: postAsyncMessage eventName is not string");
            return jsbind::MakeUndefined();
        }
        if (data.isNull() || data.isUndefined())
        {
            LOGE("Error: postAsyncMessage data is null or undefined");
            return jsbind::MakeUndefined();
        }
        if (!data.isString())
        {
            LOGE("Error: postAsyncMessage data is not string");
            return jsbind::MakeUndefined();
        }
        return OS::postAsyncMessage(callbackRef, eventName.as<std::string>(), data.as<std::string>());
    }
    std::string JSRuntime::postSyncMessage(jsbind::Local eventName, jsbind::Local data)
    {
        if (eventName.isNull() || eventName.isUndefined())
        {
            LOGE("Error: postSyncMessage eventName is null or undefined");
            return "";
        }
        if (!eventName.isString())
        {
            LOGE("Error: postSyncMessage eventName is not string");
            return "";
        }
        if (data.isNull() || data.isUndefined())
        {
            LOGE("Error: postSyncMessage data is null or undefined");
            return "";
        }
        if (!data.isString())
        {
            LOGE("Error: postSyncMessage data is not string");
            return "";
        }
        return OS::postSyncMessage(eventName.as<std::string>(), data.as<std::string>());
    }
    void JSRuntime::setPreferredFramesPerSecond(uint32_t fps)
    {
        OS::setPreferredFramesPerSecond(fps);
    }
    void JSRuntime::exportJS(jsbind::Object& context)
    {
        jsbind::global_class_<JSRuntime> class_binding;
        JSDevice::exportJS(class_binding);
        JSNetwork::exportJS(context, class_binding);
		class_binding.class_function("postAsyncMessage", &JSRuntime::postAsyncMessage);
        class_binding.class_function("postSyncMessage", &JSRuntime::postSyncMessage);
		class_binding.class_function("setGlobalRepaint", &JSRuntime::setGlobalRepaint);
		class_binding.class_function("setScreenOrientation", &JSRuntime::setScreenOrientation);
		class_binding.class_function("getLaunchOptionsSync", &JSRuntime::getLaunchOptionsSync);
		class_binding.class_function("setZipPackage", &JSRuntime::setZipPackage);
		class_binding.class_function("setGetWorldTransformFunction", &JSRuntime::setGetWorldTransformFunction);
		class_binding.class_function("setSetWorldTransformFunction", &JSRuntime::setSetWorldTransformFunction);
		class_binding.class_function("setOnFrame", &JSRuntime::setOnFrameFunction);
		class_binding.class_function("setOnDraw", &JSRuntime::setOnDrawFunction);
		class_binding.class_function("setOnResize", &JSRuntime::setOnResizeFunction);
		class_binding.class_function("setOnBlur", &JSRuntime::setOnBlurFunction);
		class_binding.class_function("setOnFocus", &JSRuntime::setOnFocusFunction);
		class_binding.class_function("onHide", &JSRuntime::setOnBlurFunction);
		class_binding.class_function("onShow", &JSRuntime::setOnFocusFunction);
		class_binding.class_function("setHref", &JSRuntime::setHref);
		class_binding.class_function("setMouseEvtFunction", &JSRuntime::setMouseEvtFunction);
		class_binding.class_function("setKeyEvtFunction", &JSRuntime::setKeyEvtFunction);
		class_binding.class_function("setTouchEvtFunction", &JSRuntime::setTouchEvtFunction);
		class_binding.class_function("setDeviceMotionEvtFunction", &JSRuntime::setDeviceMotionEvtFunction);
		class_binding.class_function("setNetworkEvtFunction", &JSRuntime::setNetworkEvtFunction);
		class_binding.class_function("setBuffer", &JSRuntime::setBuffer);
		class_binding.class_function("setScreenWakeLock", &JSRuntime::setScreenWakeLock);
		class_binding.class_function("setSensorAble", &JSRuntime::setSensorAble);
		class_binding.class_function("readFileFromAsset", &JSRuntime::readFileFromAsset);
		class_binding.class_function("getCachePath", &JSRuntime::getCachePath);
		class_binding.class_function("strTobufer", &JSRuntime::strTobufer);
		class_binding.class_function("callMethod", &JSRuntime::callMethod);
		class_binding.class_function("printCorpseImages", &JSRuntime::printCorpseImages);
		class_binding.class_function("captureScreen", &JSRuntime::captureScreen);
		class_binding.class_function("saveAsPng", &JSRuntime::saveAsPng);
		class_binding.class_function("saveAsJpeg", &JSRuntime::saveAsJpeg);
		class_binding.class_function("convertBitmapToPng", &JSRuntime::convertBitmapToPng);
		class_binding.class_function("convertBitmapToJpeg", &JSRuntime::convertBitmapToJpeg);
		class_binding.class_function("exit", &JSRuntime::exit);
        class_binding.class_function("createArrayBufferRef", &JSRuntime::createArrayBufferRef);
        class_binding.class_function("registerFont", &JSRuntime::registerFont);
        class_binding.class_function("onerror", &JSRuntime::setOnError);
        class_binding.class_function("setPreferredFramesPerSecond", &JSRuntime::setPreferredFramesPerSecond);
#if defined(USE_DCC)
        class_binding.class_function("downloadNoCache", &JSRuntime::downloadFile);
        class_binding.class_function("setDownloader", &JSRuntime::setDownloader);
#endif
        //class_property必须在下面，否则导不出class_function
        class_binding.class_property("onunhandledrejection", &JSRuntime::getOnUnhandledRejection, &JSRuntime::setOnUnhandledRejection);
		class_binding.class_property("safeInsetTop", &JSRuntime::getSafeInsetTop);
		class_binding.class_property("safeInsetLeft", &JSRuntime::GetSafeInsetLeft);
		class_binding.class_property("safeInsetBottom", &JSRuntime::GetSafeInsetBottom);
		class_binding.class_property("safeInsetRight", &JSRuntime::GetSafeInsetRight);
        class_binding.class_property("presetUrl", &JSRuntime::getPresetUrl);
        context.global_class_("conch", class_binding);
    }
}

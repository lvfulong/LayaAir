#include "JSRuntime.h"
#include <utils/JCCrypto.h>
#include "downloadMgr/JCDownloadMgr.h"
#include "../../JCConch.h"
#include <utils/JCFileSource.h>
#ifdef __APPLE__ 
    #include "CToObjectC.h"
#endif
#ifdef __ANDROID__ 
#include "CToJavaBridge.h"
#endif
#include "../../JCSystemConfig.h"
#include "JSInput.h"
#include "JSConchConfig.h"
#include <imageLib/JCImageRW.h>
#include "JSLayaGL.h"
#include "JSZip.h"
#include "JSLaunchOptions.h"
//#include "LayaAir/3D/JSTransform.h"
#include "JSArrayBufferRef.h"
#include "2D/FontManager.h"

laya::JCZip *g_ZipPackage = NULL;
//------------------------------------------------------------------------------
namespace laya
{
    std::string JSRuntime::m_strReturn;
    void JSRuntime::setOnFrameFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnFrameFunction.reset(p_pFunction);
    }
    void JSRuntime::setZipPackage(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSZipPackage.reset(p_pFunction);
        JSZip* zip = (JSZip*)Converter<JSZip*>::ToCpp(JCConch::s_pScriptRuntime->m_pJSZipPackage.toLocal().handle_);
		if (zip)
		{
			g_ZipPackage = &zip->m_zip;
		}
    }
    void JSRuntime::setOnDrawFunction(JSValueAsParam p_pFunction) 
    {
        JCConch::s_pScriptRuntime->m_pJSOnDrawFunction.reset(p_pFunction);
    }
    void JSRuntime::setOnResizeFunction(JSValueAsParam p_onresize)
    {
        JCConch::s_pScriptRuntime->m_pJSOnResizeFunction.reset(p_onresize);
    }
    void JSRuntime::setOnBlurFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnBlurFunction.reset(p_pFunction);
    }
    void JSRuntime::setOnFocusFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnFocusFunction.reset(p_pFunction);
    }
    void  JSRuntime::setGetWorldTransformFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_bJSBulletGetWorldTransformHandle.reset(p_pFunction);
    }
    void  JSRuntime::setSetWorldTransformFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_bJSBulletSetWorldTransformHandle.reset(p_pFunction);
    }
	void  JSRuntime::setBulletDrawLineFunction(JSValueAsParam p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_bJSBulletDrawLineHandle.reset(p_pFunction);
	}
	void  JSRuntime::setBulletClearLineFunction(JSValueAsParam p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_bJSBulletClearLineHandle.reset(p_pFunction);
	}
    void JSRuntime::setBuffer(JSValueAsParam pArrayBuffer) 
    {
        char* pArrayBufferPtr = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBuffer, pArrayBufferPtr, nABLen);
        if (bIsArrayBuffer)
        {
            JCConch::s_pScriptRuntime->m_pOtherBufferSharedWidthJS = pArrayBufferPtr;
        }
        else {
            LOGE("JSRuntime::setCmdBuffer param is not an ArrayBuffer!");
        }
    }
    void JSRuntime::setHref(JSValueAsParam p_sHref)
    {
        Local href(p_sHref);
        if (href.isString())
        {
            std::string strHef = href.as<std::string>();
            if (!strHef.empty())
            {
                std::string url = JCConch::s_pScriptRuntime->m_pUrl->resolve(strHef.c_str());
                g_kSystemConfig.m_strStartURL = url;
                g_kSystemConfig.m_strStartURL.at(0) = g_kSystemConfig.m_strStartURL.at(0);
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

                std::string cookiefile = JSConchConfig::getLocalStoragePath() + ss + "_curlcookie.txt";
                pdm->setCookieFile(cookiefile.c_str());
#ifdef __ANDROID__
                CToJavaBridge::JavaRet kRet;
                CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setHrefToJava", url.c_str(), kRet);
#endif
            }
        }
    }
    void JSRuntime::setMouseEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSMouseEvtFunction.reset(p_pFunction);
    }
    void JSRuntime::setTouchEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSTouchEvtFunction.reset(p_pFunction);
    }
    void JSRuntime::setDeviceMotionEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSDeviceMotionEvtFunction.reset(p_pFunction);
    }
    void JSRuntime::setKeyEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSKeyEvtFunction.reset(p_pFunction);
    }
    void JSRuntime::setNetworkEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSNetworkEvtFunction.reset(p_pFunction);
    }
    void JSRuntime::captureScreen(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnceOtherEvtFuction.reset(p_pFunction);
        JCConch::s_pConchRender->requestCaptureScreen();
    }
    const char* JSRuntime::getCachePath() 
    {
        return JCConch::s_pConch->m_sCachePath.c_str();
    }
    unsigned char* _readAssetAlloc(int sz, void* pUserData) 
    {
        auto ret = new unsigned char[sz];
        *(unsigned char**)pUserData = ret;
        return ret;
    }
    JsValue JSRuntime::readFileFromAsset(const char* file, const char* encode) 
    {
        if (!JCConch::s_pScriptRuntime->m_pAssetsRes)
        {
            return JSP_TO_JS_NULL;
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
                return JSP_TO_JS_STR(str.c_str());
            }
            else 
            {
                //TODO 写一个不用自己保留的AB
                JsValue ab = createJSAB((char*)pBuff, sz);
                //JSArrayBuffer* pab = JSArrayBuffer::create(sz);
                //memcpy(pab->getPtr(), pBuff, sz);
                delete[] pBuff;
                //return (pab->toLocal());;
                return ab;
            }
        }
        return JSP_TO_JS_NULL;
    }
    void JSRuntime::setScreenWakeLock(bool p_bWakeLock)
    {
#ifdef __APPLE__
        CToObjectCSetScreenWakeLock(p_bWakeLock);
#elif __ANDROID__
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setScreenWakeLock", p_bWakeLock, kRet);
#elif WIN32

#endif
    }
    void JSRuntime::setSensorAble(bool p_bSensorAble)
    {
#ifdef __APPLE__
        CToObjectCSetSensorAble(p_bSensorAble);
#elif __ANDROID__
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setSensorAble", p_bSensorAble, kRet);
#elif WIN32

#endif
    }
    JsValue JSRuntime::strTobufer(const char* s)
    {
        return createJSABAligned((char*)s, (strlen(s)+1));
    }
    const char* JSRuntime::getPresetUrl()
    {
        return g_kSystemConfig.m_strStartURL.c_str();
    }
    void JSRuntime::printCorpseImages()
    {
        JCImageManager* pImageManger = JCConch::s_pConchRender->m_pImageManager;
        if (pImageManger == NULL) return;
        std::string sFilePath = JCConch::s_pConch->m_strLocalStoragePath;
        sFilePath += "/imagesLog.txt";
        pImageManger->printCorpseImages(sFilePath.c_str());
    }
    const char* JSRuntime::callMethod(int objid,bool isSyn,const char*clsName, const char* methodName, const char* paramStr)
    {
#ifdef __ANDROID__
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethodRefection(objid, isSyn, clsName, methodName, paramStr, kRet))
        {
            m_strReturn = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
            LOGI("JSRuntime::callMethod %s %s %s", m_strReturn.c_str(), clsName , methodName);
            return m_strReturn.c_str();
        }
        return "";
#elif WIN32
        
#elif __APPLE__
        m_strReturn = CToObjectCCallMethod( objid, isSyn, clsName, methodName,paramStr);
        LOGI("JSRuntime::callMethod %s", m_strReturn.c_str());
        return m_strReturn.c_str();
#endif
        return "";
    }
    bool JSRuntime::saveAsPng(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile)
    {
        char* pArrayBuffer = NULL;
        int nArrayBufferSize = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, pArrayBuffer, nArrayBufferSize);
        if (bIsArrayBuffer)
        {
            return laya::saveAsPng(pArrayBuffer, w, h, p_pszFile);
        }
        return false;
    }
    bool JSRuntime::saveAsJpeg(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile)
    {
        char* pArrayBuffer = NULL;
        int nArrayBufferSize = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, pArrayBuffer, nArrayBufferSize);
        if (bIsArrayBuffer)
        {
            ImageBaseInfo info;
            info.m_nBpp = 32;
            info.m_nWidth = w;
            info.m_nHeight = h;
            return laya::saveAsJpeg(pArrayBuffer, info, p_pszFile);
        }
        return false;
    }
    JsValue JSRuntime::convertBitmapToPng(JSValueAsParam pArrayBufferArgs, int w, int h)
    {
        char* pArrayBuffer = NULL;
        int nArrayBufferSize = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, pArrayBuffer, nArrayBufferSize);
        if (bIsArrayBuffer)
        {
            std::pair<unsigned char*, unsigned long> ret = laya::convertBitmapToPng((const char*)pArrayBuffer, w, h, 8);
            if (ret.first != nullptr)
                return  createJSAB((char*)ret.first, ret.second);
        }
        return JSP_TO_JS_NULL;
    }
    JsValue JSRuntime::convertBitmapToJpeg(JSValueAsParam pArrayBufferArgs, int w, int h)
    {
        char* pArrayBuffer = NULL;
        int nArrayBufferSize = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, pArrayBuffer, nArrayBufferSize);
        if (bIsArrayBuffer)
        {
            std::pair<unsigned char*, unsigned long> ret = laya::convertBitmapToJpeg((const char*)pArrayBuffer, w, h, 32);
            if (ret.first != nullptr)
                return  createJSAB((char*)ret.first, ret.second);
        }
        return JSP_TO_JS_NULL;
    }
	void JSRuntime::exit()
    {
#ifdef __ANDROID__
        CToJavaBridge::JavaRet ret;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "exit", ret);
#elif __APPLE__
#elif WIN32
#endif
    }
	int JSRuntime::getSafeInsetTop()
	{
        int safeInsetTop = 0;
        int safeInsetLeft = 0;
        int safeInsetBottom = 0;
        int safeInsetRight = 0;
#ifdef __ANDROID__
		CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
#elif __APPLE__
        CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
#elif WIN32
#endif
		return safeInsetTop;
	}
	int JSRuntime::GetSafeInsetLeft()
	{
        int safeInsetTop = 0;
        int safeInsetLeft = 0;
        int safeInsetBottom = 0;
        int safeInsetRight = 0;
#ifdef __ANDROID__
		CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
#elif __APPLE__
        CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
#elif WIN32
#endif
		return safeInsetLeft;
	}
	int JSRuntime::GetSafeInsetBottom()
	{
        int safeInsetTop = 0;
        int safeInsetLeft = 0;
        int safeInsetBottom = 0;
        int safeInsetRight = 0;
#ifdef __ANDROID__
		CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
#elif __APPLE__
        CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
#elif WIN32
#endif
		return safeInsetBottom;
	}
	int JSRuntime::GetSafeInsetRight()
	{
        int safeInsetTop = 0;
        int safeInsetLeft = 0;
        int safeInsetBottom = 0;
        int safeInsetRight = 0;
#ifdef __ANDROID__
		CToJavaBridge::GetInstance()->getSafeInsetRect(safeInsetLeft, safeInsetTop, safeInsetRight, safeInsetBottom);
#elif __APPLE__
        CToObjectCGetSafeAreaInsets(&safeInsetTop, &safeInsetLeft, &safeInsetBottom, &safeInsetRight);
#elif WIN32
#endif
		return safeInsetRight;
	}

	JsValue JSRuntime::getLaunchOptionsSync()
	{
		return JSP_TO_JS(JSLaunchOptions*, new JSLaunchOptions());
	}
    void JSRuntime::setOnUnhandledRejection(JSValueAsParam p_pFunction)
    {
	    JCConch::s_pScriptRuntime->m_pJSOnUnhandledRejectionFunction.reset(p_pFunction);
    }
    JsValue JSRuntime::getOnUnhandledRejection()
    {
        return JCConch::s_pScriptRuntime->m_pJSOnUnhandledRejectionFunction.toLocal().handle_;
    }
	void JSRuntime::setScreenOrientation(const char* p_strOrientation, JSValueAsParam p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_pJSOnScreenOrientationChanged.reset(p_pFunction);
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
		JCConch::s_pScriptRuntime->m_pJSOnScreenOrientationChanged.call<void>(getCurrentContext().global());
	}
	void JSRuntime::setGlobalRepaint(JSValueAsParam p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_pJSSetGlobalRepaintFunction.reset(p_pFunction);
	}
	void JSRuntime::setGlobalRepaintCall()
	{
		if (!JCConch::s_pScriptRuntime->m_pJSSetGlobalRepaintFunction.isEmpty())
		{
			JCConch::s_pScriptRuntime->m_pJSSetGlobalRepaintFunction.call<void>(getCurrentContext().global());
		}
	}
	struct SkinnedMatrixCache
	{
		int subMeshIndex;
		int batchIndex;
		int batchBoneIndex;
	};
	/*void JSRuntime::computeSubSkinnedDataForNative(JSValueAsParam inverseBindPosesBuffer, JSValueAsParam boneIndices, JSValueAsParam subData, JSValueAsParam skinnedMatrixCaches, JSValueAsParam bonesTransform, JSValueAsParam skinnedDataLoopMarks, JSValueAsParam skinnedData)
	{
		char* pInverseBindPosesBuffer = NULL;
		int nInverseBindPosesBufferSize = 0;
		if (!extractJSAB(inverseBindPosesBuffer, pInverseBindPosesBuffer, nInverseBindPosesBufferSize))
		{
			LOGE("computeSubSkinnedDataForNative BindPoses error");
			return;
		}

		char* pBoneIndices = NULL;
		int nBoneIndicesSize = 0;
		if (!extractJSAB(boneIndices, pBoneIndices, nBoneIndicesSize))
		{
			LOGE("computeSubSkinnedDataForNative bone indices error");
			return;
		}


		char* pSubData = NULL;
		int nSubDataSize = 0;
		if (!extractJSAB(subData, pSubData, nSubDataSize))
		{
			LOGE("computeSubSkinnedDataForNative SubData error");
			return;
		}

		char* pSkinnedMatrixCaches = NULL;
		int nSkinnedMatrixCachesSize = 0;
		if (!extractJSAB(skinnedMatrixCaches, pSkinnedMatrixCaches, nSkinnedMatrixCachesSize))
		{
			LOGE("computeSubSkinnedDataForNative SkinnedMatrixCaches error");
			return;

		}
		char* pSkinnedDataLoopMarks = NULL;
		int nSkinnedDataLoopMarksSize = 0;
		if (!extractJSAB(skinnedDataLoopMarks, pSkinnedDataLoopMarks, nSkinnedDataLoopMarksSize))
		{
			LOGE("computeSubSkinnedDataForNative SkinnedDataLoopMarks error");
			return;
		}
		uint32_t* _skinnedDataLoopMarks = (uint32_t*)pSkinnedDataLoopMarks;
		float* data = (float*)pSubData;
	
		v8::Isolate* isolate = Isolate::GetCurrent();
		v8::Local< v8::Context> context = isolate->GetCurrentContext();

		for (int k = 0, q = nBoneIndicesSize / sizeof(uint16_t); k < q; k++)
		{
			uint16_t index = ((uint16_t*)pBoneIndices)[k];
			if (_skinnedDataLoopMarks[index] == RenderInfo::loopCount)
			{
				SkinnedMatrixCache& c = ((SkinnedMatrixCache*)pSkinnedMatrixCaches)[index];

				v8::Local<Array> __array = skinnedData.As<Array>();
				v8::Local<Array> __subArray = __array->Get(context, c.subMeshIndex).ToLocalChecked().As<Array>();
				JsValue ab =  __subArray->Get(context, c.batchIndex).ToLocalChecked();
				//float* preData = _skinnedData[c.subMeshIndex][c.batchIndex];
				char* pPreData = NULL;
				int npPreDataSize = 0;
				if (!extractJSAB(ab, pPreData, npPreDataSize))
				{
					LOGE("computeSubSkinnedDataForNative preData error");
					return;
				}

				int srcIndex = c.batchBoneIndex * 16;
				int dstIndex = k * 16;
				for (int d = 0; d < 16; d++)
					data[dstIndex + d] = ((float*)pPreData)[srcIndex + d];
			}
			else
			{
				v8::Local<Array> _bones = bonesTransform.As<Array>();
				JsValue bone = _bones->Get(context, index).ToLocalChecked();
				JSTransform* pBone = __TransferToCpp<JSTransform*> ::ToCpp(bone);
				if (pBone)
				{
					pBone->_getWorldMatrix();
					_mulMatrixArray((float*)(&pBone->m_pTransform3D->getWorldMatrix().elements[0]), (float*)pInverseBindPosesBuffer + index * 16, 0, data, k * 16);
				}
				_skinnedDataLoopMarks[index] = RenderInfo::loopCount;
			}
		}
	}*/
    JsValue JSRuntime::createArrayBufferRef(JSValueAsParam pArrayBuffer, int nType, bool bSyncToRender, int nRefType)
    {
        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBuffer, pBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            JSArrayBufferRef* pArrayBufferRef = new JSArrayBufferRef();
            pArrayBufferRef->m_bSyncToRender = bSyncToRender;
            {
                pArrayBufferRef->m_nID = JCConch::s_pScriptRuntime->m_pArrayBufferManager->createArrayBuffer(pBuffer, nABLen, (JCArrayBufferManager::ARRAY_BUFFER_TYPE)nType, (JCArrayBufferManager::ARRAY_BUFFER_REF_TYPE)nRefType);
            }
            return JSP_TO_JS(JSArrayBufferRef*, pArrayBufferRef);
        }
        LOGE("JSRuntime::createArrayBufferRef type error");
        return JSP_TO_JS(JSArrayBufferRef*, NULL);
    }
    bool JSRuntime::registerFont(JSValueAsParam jsFamily, JSValueAsParam pathOrArrayBuffer)
    {
        Local value(pathOrArrayBuffer);
        if (value.isString())
        {
            std::string path = value.as<std::string>();
            std::string family = Converter<std::string>::ToCpp(jsFamily);
            return FontManager::getInstance()->registerFont(family, path);
        }
        else if (value.isArrayBuffer())
        {
            char* ab = NULL;
            int byte = 0;
            bool isab = extractJSAB(pathOrArrayBuffer, ab, byte);
            if (isab)
            {
                std::string family = Converter<std::string>::ToCpp(jsFamily);
                return FontManager::getInstance()->registerFont(family, (uint8_t*)ab, byte);
            }
        }
        LOGI("registerFont failed");
        return false;
    }
    void JSRuntime::exportJS(Context& context)
    {
        class_<JSRuntime> class_binding;
		//class_binding.class_function("computeSubSkinnedDataForNative", &JSRuntime::computeSubSkinnedDataForNative)TODO;
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
        //class_property必须在下面，否则导不出class_function
        class_binding.class_property("onunhandledrejection", &JSRuntime::getOnUnhandledRejection, &JSRuntime::setOnUnhandledRejection);
		class_binding.class_property("safeInsetTop", &JSRuntime::getSafeInsetTop);
		class_binding.class_property("safeInsetLeft", &JSRuntime::GetSafeInsetLeft);
		class_binding.class_property("safeInsetBottom", &JSRuntime::GetSafeInsetBottom);
		class_binding.class_property("safeInsetRight", &JSRuntime::GetSafeInsetRight);
        class_binding.class_property("presetUrl", &JSRuntime::getPresetUrl);
        context.class_("conch", class_binding);
    }
}

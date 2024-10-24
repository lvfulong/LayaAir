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
//#include "LayaAir/3D/JSTransform.h"
#include "JSArrayBufferRef.h"
#include "2D/FontManager.h"
#include "../downloadCache/DCC2/DirectDownloader.h"
#include "../downloadCache/DCC2/JSDownloader.h"

laya::JCZip *g_ZipPackage = NULL;
//------------------------------------------------------------------------------
namespace laya
{
    std::string JSRuntime::m_strReturn;
    void JSRuntime::setOnFrameFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnFrameFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setZipPackage(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSZipPackage = jsbind::Persistent(p_pFunction);
        JSZip* zip = (JSZip*)Converter<JSZip*>::ToCpp(JCConch::s_pScriptRuntime->m_pJSZipPackage.toLocal());
		if (zip)
		{
			g_ZipPackage = &zip->m_zip;
		}
    }
    void JSRuntime::setOnDrawFunction(JSValueAsParam p_pFunction) 
    {
        JCConch::s_pScriptRuntime->m_pJSOnDrawFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setOnResizeFunction(JSValueAsParam p_onresize)
    {
        JCConch::s_pScriptRuntime->m_pJSOnResizeFunction = jsbind::Persistent(p_onresize);
    }
    void JSRuntime::setOnBlurFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnBlurFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setOnFocusFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnFocusFunction = jsbind::Persistent(p_pFunction);
    }
    void  JSRuntime::setGetWorldTransformFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_bJSBulletGetWorldTransformHandle = jsbind::Persistent(p_pFunction);
    }
    void  JSRuntime::setSetWorldTransformFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_bJSBulletSetWorldTransformHandle = jsbind::Persistent(p_pFunction);
    }
	void  JSRuntime::setBulletDrawLineFunction(JSValueAsParam p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_bJSBulletDrawLineHandle = jsbind::Persistent(p_pFunction);
	}
	void  JSRuntime::setBulletClearLineFunction(JSValueAsParam p_pFunction)
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
    void JSRuntime::setHref(JSValueAsParam p_sHref)
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

                std::string cookiefile = JSConchConfig::getLocalStoragePath() + ss + "_curlcookie.txt";
                pdm->setCookieFile(cookiefile.c_str());
            }
        }
    }
    void JSRuntime::setMouseEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSMouseEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setTouchEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSTouchEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setDeviceMotionEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSDeviceMotionEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setKeyEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSKeyEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::setNetworkEvtFunction(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSNetworkEvtFunction = jsbind::Persistent(p_pFunction);
    }
    void JSRuntime::captureScreen(JSValueAsParam p_pFunction)
    {
        JCConch::s_pScriptRuntime->m_pJSOnceOtherEvtFuction = jsbind::Persistent(p_pFunction);
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
    void JSRuntime::setScreenWakeLock(bool bWakeLock)
    {
        JCConch::s_pConch->getOS()->setScreenWakeLock(bWakeLock);
    }
    void JSRuntime::setSensorAble(bool bSensorAble)
    {
        JCConch::s_pConch->getOS()->setSensorAble(bSensorAble);
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
    JsValue JSRuntime::convertBitmapToPng(jsbind::ArrayBuffer arrayBuffer, int w, int h)
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
        return JSP_TO_JS_NULL;
    }
    JsValue JSRuntime::convertBitmapToJpeg(jsbind::ArrayBuffer arrayBuffer, int w, int h)
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
        return JSP_TO_JS_NULL;
    }
	void JSRuntime::exit()
    {
        JCConch::s_pConch->getOS()->exit();
    }
	int JSRuntime::getSafeInsetTop()
	{
        return JCConch::s_pConch->getOS()->getSafeInsetTop();
	}
	int JSRuntime::GetSafeInsetLeft()
	{
        return JCConch::s_pConch->getOS()->getSafeInsetLeft();
	}
	int JSRuntime::GetSafeInsetBottom()
	{
        return JCConch::s_pConch->getOS()->getSafeInsetBottom();
	}
	int JSRuntime::GetSafeInsetRight()
	{
        return JCConch::s_pConch->getOS()->getSafeInsetRight();
	}

	JsValue JSRuntime::getLaunchOptionsSync()
	{
		return JSP_TO_JS(JSLaunchOptions*, new JSLaunchOptions());
	}
    void JSRuntime::setOnUnhandledRejection(JSValueAsParam p_pFunction)
    {
	    JCConch::s_pScriptRuntime->m_pJSOnUnhandledRejectionFunction = jsbind::Persistent(p_pFunction);
    }
    JsValue JSRuntime::getOnUnhandledRejection()
    {
        return JCConch::s_pScriptRuntime->m_pJSOnUnhandledRejectionFunction.getHandle();
    }
	void JSRuntime::setScreenOrientation(const char* p_strOrientation, JSValueAsParam p_pFunction)
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
		JCConch::s_pScriptRuntime->m_pJSOnScreenOrientationChanged.call<void>(jsbind::global());
	}
	void JSRuntime::setGlobalRepaint(JSValueAsParam p_pFunction)
	{
		JCConch::s_pScriptRuntime->m_pJSSetGlobalRepaintFunction = jsbind::Persistent(p_pFunction);
	}
	void JSRuntime::setGlobalRepaintCall()
	{
		if (JCConch::s_pScriptRuntime->m_pJSSetGlobalRepaintFunction.isValid())
		{
			JCConch::s_pScriptRuntime->m_pJSSetGlobalRepaintFunction.call<void>(jsbind::global());
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
    JsValue JSRuntime::createArrayBufferRef(jsbind::ArrayBuffer arrayBuffer, int nType, bool bSyncToRender, int nRefType)
    {
       
        bool bIsArrayBuffer = arrayBuffer.isValid();
        if (bIsArrayBuffer)
        { 
            char* pBuffer = reinterpret_cast<char*>(arrayBuffer.getData());
            int nABLen = arrayBuffer.getLength();
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
            return FontManager::registerFont(family, arrayBuffer.getData(), arrayBuffer.getLength());
        }
        LOGI("registerFont failed");
        return false;
    }

    void onProgJS(unsigned int now, unsigned int total, float speed, std::shared_ptr<v8::Persistent<v8::Value>>& jsOnProg) {
        auto isolate = v8::Isolate::GetCurrent();
        v8::HandleScope handleScope(isolate); // 创建 HandleScope
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        // 将下载的数据传递给 JS 回调
        //把持久句柄转成本地句柄            
        v8::Local func(jsOnProg->Get(isolate));
        // 确认value是函数
        if ( !func.IsEmpty() && func->IsFunction()) {
            auto v8function = v8::Local<v8::Function>::Cast(func);
            const unsigned argc = 3;
            v8::Local<v8::Value> argv[argc] = {
                v8::Number::New(isolate, now),
                v8::Number::New(isolate, total),
                v8::Number::New(isolate, speed),
            };

            // 调用函数
            v8::Local<v8::Value> result;
            if (v8function->Call(context, context->Global(), argc, argv).ToLocal(&result)) {
                // 函数调用成功，result 包含返回值
                // 在此处理 result （如果需要）
            }
            else {
                // 处理错误
            }

            //释放持久句柄
            jsOnProg->Reset();
        }
        else {
            // 抛出错误或处理非函数情况
        }
    }
    //std::vector<std::shared_ptr<DirectDownloader>> downloaders;
    int onprog(unsigned int now, unsigned int total, float speed, std::shared_ptr<v8::Persistent<v8::Value>>& jsOnProg) {
        postToJS(std::bind(onProgJS, now, total, speed, jsOnProg));
        return 0;
    }

    void onDownloaded_JS(JCBuffer & p_Buff,
            const std::string & pLocalAddr,
            const std::string & pSvAddr,
            int pnCurlRet,
            int pnHttpRet,
            const std::string & pstrHeader,
            std::shared_ptr<v8::Persistent<v8::Value>>& jsOnComp,
            std::shared_ptr<v8::Persistent<v8::Value>>& jsOnProg
    ) {
        auto isolate = v8::Isolate::GetCurrent();
        v8::HandleScope handleScope(isolate); // 创建 HandleScope
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        // 将下载的数据传递给 JS 回调
        //把持久句柄转成本地句柄            
        v8::Local func(jsOnComp->Get(isolate));
        // 确认value是函数
        if (func->IsFunction()) {
            auto v8function = v8::Local<v8::Function>::Cast(func);
            const unsigned argc = 3;
            v8::Local<v8::Value> argv[argc] = {
                createJSAB(p_Buff.m_pPtr, p_Buff.m_nLen),
                v8::String::NewFromUtf8(isolate, pLocalAddr.c_str(), v8::NewStringType::kNormal).ToLocalChecked(),
                v8::String::NewFromUtf8(isolate, pSvAddr.c_str(), v8::NewStringType::kNormal).ToLocalChecked()
            };

            // 调用函数
            v8::Local<v8::Value> result;
            if (v8function->Call(context, context->Global(), argc, argv).ToLocal(&result)) {
                // 函数调用成功，result 包含返回值
                // 在此处理 result （如果需要）
            }
            else {
                // 处理错误
            }

            //释放持久句柄
            jsOnComp->Reset();
            jsOnProg->Reset();
            //释放buffer
            p_Buff.m_bNeedDel = true;
            p_Buff.free();
        }
        else {
            // 抛出错误或处理非函数情况
        }
    }

    void onDownloaded(
        JCBuffer& p_Buff,
        const std::string& pLocalAddr,
        const std::string& pSvAddr,
        int pnCurlRet,
        int pnHttpRet,
        const std::string& pstrHeader,
        std::shared_ptr<v8::Persistent<v8::Value>>& jsOnComp,
        std::shared_ptr<v8::Persistent<v8::Value>>& jsOnProg) {

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
        postToJS(std::bind(onDownloaded_JS, p_Buff, std::ref(pLocalAddr), std::ref(pSvAddr), pnCurlRet, pnHttpRet, pstrHeader, jsOnComp, jsOnProg));
    }

    /**
    *   不带dcc和缓存的下载
    */
    void JSRuntime::downloadFile(const std::string& url, JSValueAsParam onProgress, JSValueAsParam onComplete, JSValueAsParam onError) {
        if (url.empty()) 
            return;

        auto isolate = v8::Isolate::GetCurrent();
        //v8::Persistent<v8::Value> onCompleteP(isolate, onComplete);  
        //转成持久句柄。由于lambda不允许拷贝，所以用shareptr
        auto onCompleteP = std::make_shared<v8::Persistent<v8::Value>>(isolate, onComplete);
        auto onProgP = std::make_shared<v8::Persistent<v8::Value>>(isolate, onProgress);

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


    void JSRuntime::setDownloader(JSValueAsParam obj){
        std::shared_ptr<JSDownloader> jsdownloader = std::make_shared<JSDownloader>();
        jsdownloader->setJSDownloader(obj);
        JCFileResManager* pfsMgr = JCConch::s_pScriptRuntime->m_pFileResMgr;
        pfsMgr->m_pDownloader = jsdownloader;
    }
    //todo 刷新生命周期
    std::shared_ptr<int> callbackRef(new int(1));
    JsValue JSRuntime::postAsyncMessage(const std::string &eventName, const std::string &data)
    {
        return JCConch::s_pConch->getOS()->postAsyncMessage(callbackRef, eventName, data);
    }
    std::string JSRuntime::postSyncMessage(const std::string &eventName, const std::string &data)
    {
        return JCConch::s_pConch->getOS()->postSyncMessage(eventName, data);
    }
    void JSRuntime::exportJS(jsbind::Object& context)
    {
        jsbind::class_<JSRuntime> class_binding;
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
        class_binding.class_function("downloadNoCache", &JSRuntime::downloadFile);
        class_binding.class_function("setDownloader", &JSRuntime::setDownloader);
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

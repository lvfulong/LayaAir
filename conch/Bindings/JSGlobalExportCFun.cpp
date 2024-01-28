/**
@file			JSGlobalExportCFun.cpp
@brief			
@author			James
@version		1.0
@date			2013_11_12
*/

#include "JSGlobalExportCFun.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include <utils/JCFileSystem.h>
#include <utils/JCMemorySurvey.h>
#include <utils/JCLayaUrl.h>
#include <utils/JCCrypto.h>
#include <utils/Log.h>
#include <utils/Base64.h>
#include "JSConsole.h"
#include "XMLHttpRequest.h"
#include "JSConchConfig.h"
#include "JSXmlNode.h"
#include "JSXmlAttr.h"
#include "JSDOMParser.h"
#include "JSAudio.h"
#include "JSAppCache.h"
#include "JSWebSocket.h"
#include "JSFileSystem.h"
#include "JSZip.h"
#include "JSNotify.h"

#include "JSCanvasRenderingContext2D.h"
#ifdef ANDROID
    #include "JSAndroidEditBox.h"
	#include "CToJavaBridge.h"
#elif WIN32
	#include <Windows.h>
    #include "JSWindowEditBox.h"
#elif LINUX
    #include "JSLinuxEditBox.h"
#elif __APPLE__
    #include "JSIOSEditBox.h"
    #include "CToObjectC.h"
#endif
#include "JSRuntime.h"
#include "../../JCConch.h"
#include <utils/JCFileSource.h>
#include "JSImage.h"
#include "JSHistory.h"
//#include "JSTextMemoryCanvas.h"
#include <downloadMgr/JCDownloadMgr.h> 
#include "JSLayaGL.h"
#include "JSShaderActiveInfo.h"
//#include "JSTextBitmapInfo.h"
#include "JSShaderPrecisionFormat.h"
#include "Video/JSVideo.h"
#include <LayaGL/JCLayaGLDispatch.h>
#include "Bullet/LayaBulletExport.h"
#if !defined(LINUX) && !defined(WIN32)
#include "PhysX/LayaPhysXExport.h"
#endif
#include "JSFileSystemManager.h"
#include "JSFileReaderWX.h"
#include "JSLayaNative.h"
#include "JSArrayBufferRef.h"
#include "JSLaunchOptions.h"
#include "JSStat.h"
#include "JSPromiseRejectionEvent.h"
#include "JSFile.h"
#include "JSFileReader.h"
#include "JSImageBitmap.h"
#include "LayaAir/2D/JSContext2D.h"
#include "LayaAir/2D/Shader2D.h"
#include "LayaAir/2D/JSWebGLCacheAsNormalCanvas.h"
#include "LayaAir/2D/JSWordText.h"
#include "LayaAir/2D/JSRenderTexture2D.h"
#include "LayaAir/3D/JSCommandUniformMap.h"
#include "LayaAir/3D/JSTransform.h"
#include "LayaAir/3D/JSSubmitScene3D.h"
#include "LayaAir/3D/JSBounds.h"
#include "LayaAir/3D/JSUniformBufferObject.h"
#include "2D/FontDescriptionParser.h"
#include "2D/ColorParser.h"
#include "LayaAir/RenderEngine/WebGLEngine/JSGL2TextureContext.h"
#include "LayaAir/RenderEngine/WebGLEngine/JSGLTextureContext.h"
#include "LayaAir/RenderEngine/WebGLEngine/JSWebGLEngine.h"
#include "LayaAir/RenderEngine/WebGLEngine/JSWebGLInternalTex.h"
#include "LayaAir/RenderEngine/WebGLEngine/JSWebGLInternalRT.h"
#include "LayaAir/RenderEngine/WebGLEngine/JSGLRenderDrawContext.h"
#include "2D/FontManager.h"
#include <stdarg.h>
#include <render/3D/RenderBindings.h>

extern int g_nInnerWidth ;
extern int g_nInnerHeight ;
extern bool g_bGLCanvasSizeChanged;
#ifdef WIN32
	int g_bEnableTouch = false;
#elif ANDROID
	int g_bEnableTouch = true;
#elif __APPLE__
	int g_bEnableTouch = true;
#elif LINUX
	int g_bEnableTouch = false;
#endif
 std::string g_sExePath = "";


/** @brief 这个函数是为了实现comman库中的alert函数
 * 不定长的函数
*/
void alert(const char* fmt, ...)
{
    char buf[1024];
    char* pBuf = NULL;
    va_list args;
    va_start(args, fmt);
    int len = vsprintf(buf, fmt, args);
    if (len < 0) {
        pBuf = new char[4096];
        len = vsprintf(pBuf, fmt, args);
    }
    va_end(args);
    laya::LayaAlert(pBuf ? pBuf : buf);
    if (pBuf) 
    {
        delete[] pBuf;
    }
}
namespace laya 
{
    //下载大文件，zip用的
    struct JSFuncWrapper
    {
        Persistent funcOnProg;
        Persistent funcOnComp;
        bool stop;
        JSFuncWrapper(JSValueAsParam onprog, JSValueAsParam onComp)
        {

            funcOnProg.reset(onprog);
            funcOnComp.reset(onComp);
            stop = false;
        }
		~JSFuncWrapper()
		{
			funcOnProg.reset();
			funcOnComp.reset();
		}
    };

    void downloadBig_onProg_js(JSFuncWrapper* pWrapper, unsigned int total, unsigned int now, float speed)
    {
        if (pWrapper->funcOnProg.isEmpty())return;
        pWrapper->stop = pWrapper->funcOnProg.call<bool>(getCurrentContext().global(), total, now, speed);
    }
    int downloadBig_onProg(unsigned int total, unsigned int now, float speed, JSFuncWrapper* pWrapper)
    {
        if (pWrapper && pWrapper->stop)return 1;
        postToJS(std::bind(downloadBig_onProg_js, pWrapper, total, now, speed));
        return 0;
    }
    void downloadBig_onComp_js(int curlret, int httpret, JSFuncWrapper* pWrapper)
    {
        /*if (!pWrapper->IsMyJsEnv()){
            delete pWrapper;
            return;
        }*/
        if (!pWrapper->funcOnComp.isEmpty())
        {
            pWrapper->funcOnComp.call<void>(getCurrentContext().global(), curlret,httpret);
        }
        delete pWrapper;
    }
    void downloadBig_onComp(JCBuffer& buff, const std::string& localip,const std::string& svip, int curlret, int httpret,const std::string& httpresheader, JSFuncWrapper* pWrapper)
    {
        postToJS(std::bind(downloadBig_onComp_js, curlret,httpret, pWrapper));
    }
	void _downloadBigFile(const char* p_pszUrl, const char* p_pszLocal, JSValueAsParam p_ProgCb,JSValueAsParam p_CompleteCb, int p_nTryNum, int p_nOptTimeout)
    {
        /*
        if (!canWrite(pCurProcess->getFSPermission(p_pszLocal))) {
        JSP_THROW("downloadBigFile to this localfile is forbidden!");
        LOGE("本用户不允许在%s目录下写文件", p_pszLocal);
        return 0;
        }
        */
        JCDownloadMgr* dmgr = JCDownloadMgr::getInstance();
        JSFuncWrapper* pJSObj = new JSFuncWrapper(p_ProgCb, p_CompleteCb);
        dmgr->downloadBigFile(p_pszUrl, p_pszLocal,
            std::bind(downloadBig_onProg, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, pJSObj),
            std::bind(downloadBig_onComp, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, pJSObj), 
            p_nTryNum, p_nOptTimeout);;
    }
    void downloadHeader_onComp_js(char* pBuff, int curlret, int httpret, JSFuncWrapper* pWrapper) 
    {
        /*if (!pWrapper->IsMyJsEnv()) 
        {
            delete pWrapper;
            return;
        }*/
        if (!pWrapper->funcOnComp.isEmpty())
        {
            if (pBuff) 
            {
                pWrapper->funcOnComp.call<void>(getCurrentContext().global(), curlret, httpret, (const char*)pBuff);
                delete [] pBuff;
            }
            else 
            {
                pWrapper->funcOnComp.call<void>(getCurrentContext().global(), curlret, httpret);
            }
        }
        delete pWrapper;
    }
    void downloadHeader_onComp(JCBuffer& buff, const std::string& localip,
        const std::string& svip, int curlret, int httpret,
        const std::string& httpresheader, JSFuncWrapper* pWrapper)
    {
        char* pBuff = nullptr;
        if (buff.m_pPtr && buff.m_nLen) 
        {
            //这个肯定是字符串
            pBuff = new char[buff.m_nLen+1];
            memcpy(pBuff, buff.m_pPtr, buff.m_nLen);
            pBuff[buff.m_nLen] = 0;
        }
        postToJS(std::bind(downloadHeader_onComp_js, pBuff, curlret, httpret, pWrapper));
    }
	void _downloadGetHeader(const char* p_pszUrl, JSValueAsParam p_CompleteCb, int p_nTryNum, int p_nOptTimeout)
    {
        JCDownloadMgr* dmgr = JCDownloadMgr::getInstance();
        JSFuncWrapper* pJSObj = new JSFuncWrapper(p_CompleteCb, p_CompleteCb);//第一个没有用
        dmgr->getHeader(p_pszUrl,
            std::bind(downloadHeader_onComp, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, pJSObj),
            p_nTryNum, p_nOptTimeout);
    }
    void setTouchEvtFunc(JSValueAsParam pObj) 
    {
    }
    void setKeyEvtFunc(JSValueAsParam pObj) 
    {
    }
    void setJoystickEvtFunc(JSValueAsParam pObj) 
    {
    }
	void copy(const char* data)
	{
#ifdef WIN32
#elif ANDROID
		std::string strBuffer = data;
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "copy", strBuffer.c_str(), kRet);
#elif __APPLE__
		CToObjectCopy(data);
#endif
	}

    void evalJS(const char* p_sSource)
    {
        JSP_RUN_SCRIPT(p_sSource);
    }
    void JSPrint(const char* p_sBuffer)
    {
        int nLen = strlen(p_sBuffer) + 3;
        unsigned short* ucStr = new unsigned short[nLen];
        int nlen = UTF8StrToUnicodeStr((unsigned char*)p_sBuffer, ucStr, nLen);
        LOGI("%ws\n", (wchar_t*)ucStr);
        delete[] ucStr;
        ucStr = NULL;
    }
    void LayaAlert(const char* p_sBuffer)
    {
#ifdef WIN32
        int nLen = strlen(p_sBuffer) + 3;
        unsigned short* ucStr = new unsigned short[nLen];
        int nlen = UTF8StrToUnicodeStr((unsigned char*)p_sBuffer, ucStr, nLen);
        delete[] ucStr;
        ucStr = NULL;
        std::wstring wsBuffer = (wchar_t*)utf8_unicode(p_sBuffer).c_str();
        MessageBoxW(NULL, wsBuffer.c_str(), L"alert", MB_OK);
#elif ANDROID
        std::string strBuffer = p_sBuffer;
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "alert", strBuffer.c_str(), kRet);
#elif __APPLE__
        CToObjectCAlert(p_sBuffer);
#endif
    }
    void JSAlert(const char* p_sBuffer)
    {
        LayaAlert(p_sBuffer);
#ifndef WIN32
        LOGI("alert=%s", p_sBuffer);
#endif
    }
    int getInnerWidth()
    {
        return g_nInnerWidth;
    }
    int getInnerHeight()
    {
        return g_nInnerHeight;
    }
    int getDevicePixelRatio()
    {
#ifdef WIN32
        return 1.0;
#elif ANDROID
        return 1.0;
#elif __APPLE__
		return 1.0;// CToObjectCGetDevicePixelRatio();
#elif LINUX
        return 1.0;
#endif
    }
	JsValue getExePath()
    {
#ifdef WIN32
        TCHAR szPath[MAX_PATH];
        ::GetModuleFileName(NULL, szPath, MAX_PATH);
        ::GetFullPathName(szPath, MAX_PATH, szPath, NULL);
        g_sExePath = szPath;
        return JSP_TO_JS(const char*, g_sExePath.c_str());
#elif __APPLE__
        return JSP_TO_JS_NULL;
#else
        return JSP_TO_JS_NULL;
#endif
    }
	void writeStrFileSync(const char* p_pszFile, const char* p_pString )
    {
        JCBuffer buf((char*)p_pString, strlen(p_pString), false, false);
        writeFileSync(p_pszFile, buf, JCBuffer::utf8);
	}
    void reloadJSThread()
    {
        //jsc todos
        //CToObjectCRunStopJSLoop();
        JCConch::s_pConch->reload();
    }
    std::string readTextAsset(const char* p_pszFile)
    {
        char* sBuffer = NULL;
        int nSize = 0;
        if (JCConch::s_pAssetsFiles->loadFileContent(p_pszFile, sBuffer, nSize))
        {
            std::string rsBuffer = sBuffer;
            delete[] sBuffer;
            return rsBuffer;
        }
        return "";
    }
	void open(const char* p_pszUrl)
	{
#ifdef WIN32
#elif ANDROID
		std::string strBuffer = p_pszUrl;
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "open", strBuffer.c_str(), kRet);
#elif __APPLE__
        CToObjectCOpenUrl(p_pszUrl);
#endif
	}
    bool gbAlertException = true;
    void showAlertOnJsException(bool show)
    {
        gbAlertException = show;
    }
    std::string calcMD5(unsigned char* buf, int len)
    {
        JCMD5 imd5;
        imd5.GenerateMD5(buf, len);
        return imd5.ToString();
    }
    std::string calcMD5_JSAB(JSValueAsParam pjs)
    {
        char* pABPtr = NULL;
        int nABLen = 0;
        if (!extractJSAB(pjs, pABPtr, nABLen))return "";
        std::string ret = calcMD5((unsigned char*)pABPtr, nABLen);
        return ret;
    }
    static std::string toBase64(const char* type, float encoderOptions, JSValueAsParam ab, int w, int h, bool flipY)
    {
        char* pPixels = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(ab, pPixels, nABLen);
        int size = sizeof(GLubyte) * w * h * 4;
        if (!bIsArrayBuffer || w == 0 || h == 0 || size != nABLen)
        {
            const char* pstrHeader = "data:";
            int length = strlen(pstrHeader);
            std::unique_ptr<char[]> pDest(new char[length + 1]);
            memcpy(pDest.get(), pstrHeader, length);
            pDest.get()[length] = '\0';
            return std::string(pDest.get());
        }

        if (flipY)
        {
            laya::flipPixelsY((uint8_t*)pPixels, w * 4, h);
        }

        std::string strType(type);
        int length = (size + 2) / 3 * 4;
        std::unique_ptr<char[]> pDest(new char[length]);
        memset(pDest.get(), 0, length);
        char *pCurrent = pDest.get();
        std::pair<unsigned char*, unsigned long> result;
        if (strType == "image/jpeg")
        {
            const char* pstrHeader = "data:image/jpeg;base64,";
            int length = strlen(pstrHeader);
            for (int i = 0; i < length; i++)
            {
                *pCurrent = pstrHeader[i];
                pCurrent++;
            }
            result = convertBitmapToJpeg((const char*)pPixels, w, h, 32);
        }
        else
        {
            const char* pstrHeader = "data:image/png;base64,";
            int length = strlen(pstrHeader);
            for (int i = 0; i < length; i++)
            {
                *pCurrent = pstrHeader[i];
                pCurrent++;
            }
            result = laya::convertBitmapToPng((const char*)pPixels, w, h, 8);
        }

        base64Encode(pCurrent, (const char*)result.first, result.second);
        delete[] result.first;
        return std::string(pDest.get());
    }
    std::string conchToBase64FlipY(const char* type, float encoderOptions, JSValueAsParam ab, int w, int h)
    {
        return toBase64(type, encoderOptions, ab, w, h, true);
    }
    std::string conchToBase64(const char* type, float encoderOptions, JSValueAsParam ab, int w, int h)
    {
        return toBase64(type, encoderOptions, ab, w, h, false);
    }
    std::string btoa(JSValueAsParam val)
    {
        std::string temp;
        v8::Local<v8::String> str = v8::Local<v8::String>::Cast(val);
        int length = str->Length();
        temp.resize(length);
        //str->WriteUtf8(Isolate::GetCurrent(), &temp[0], length, NULL, String::NO_NULL_TERMINATION);
        str->WriteOneByte(v8::Isolate::GetCurrent(), (uint8_t* )&temp[0], 0, length, v8::String::NO_OPTIONS);
        
        if (temp.length() == 0)
            return std::string();
        return base64Encode(temp.data(), temp.length());
    }
    JsValue atob(const char* encodedString)
    {
        std::vector<char> out;
        if (!base64Decode(std::string(encodedString), out, isHTMLSpace<uint16_t>, Base64ValidatePadding)) {
            return v8::String::NewFromOneByte(v8::Isolate::GetCurrent(), (const uint8_t*)out.data(), v8::NewStringType::kNormal, 0).ToLocalChecked();
        }
        return v8::String::NewFromOneByte(v8::Isolate::GetCurrent(), (const uint8_t*)out.data(), v8::NewStringType::kNormal, out.size()).ToLocalChecked();
    }
    bool getEnableTouch()
    {
        return g_bEnableTouch;
    }
    JsValue createImageBitmap(JsValue image, JsValue options)
    {
        if (Converter<JSImage>::is(image))
        {
            JSImage* jsImage = Converter<JSImage*>::ToCpp(image);
            assert(jsImage != nullptr);
            JSImageBitmap* jsImageBitmap = new JSImageBitmap();
            jsImageBitmap->m_ImageBitmap.m_image = jsImage->m_pImage;
            //return laya::Promise::resolve(Converter<JSImageBitmap*>::ToJs(jsImageBitmap)).getV8Promise();
            return Converter<JSImageBitmap*>::ToJs(jsImageBitmap);
        }
        //return JSP_TO_JS_PROMISE;
        return JSP_TO_JS_NULL;
    }
	void JSGlobalExportC()	
    {

        v8::Isolate* isolate = v8::Isolate::GetCurrent();
	    v8::HandleScope scope(isolate);
        Context context;
        ///Module global(context.isolate());
        JSCanvasRenderingContext2D::exportJS(context);
        JsFile::exportJS(context);
        JsFileReader::exportJS(context);
        JsFileReaderWX::exportJS(context);
		JSRuntime::exportJS(context);
		JSHistory::exportJS(context);
        JSConsole::exportJS(context);
        JSImage::exportJS(context);
        XMLHttpRequest::exportJS(context);
        JSConchConfig::exportJS(context);
        JSXmlNode::exportJS(context);
        JSXmlDocument::exportJS(context);
        JSXmlAttr::exportJS(context);
        JSDOMParser::exportJS(context);
        JSAudio::exportJS(context);
        JsAppCache::exportJS(context);
        JSWebSocket::exportJS(context);
        JSZip::exportJS(context);
        JSNotify::exportJS(context);
        JSLayaGL::getInstance()->exportJS(context);
        JSShaderActiveInfo::exportJS(context);
        JSShaderPrecisionFormat::exportJS(context);
		JSLaunchOptions::exportJS(context);
        JSPromiseRejectionEvent::exportJS(context);
        JSImageBitmap::exportJS(context);
#ifdef WIN32

        JSWindowEditBox::exportJS(context);
#elif LINUX
        JSLinuxEditBox::exportJS(context);
#elif ANDROID
        JSAndroidEditBox::exportJS(context);
#elif __APPLE__

        JSIOSEditBox::exportJS(context);
#endif
        //JSTextCanvas
;
        //JSTextBitmapInfo::exportJS(context);
		JSStat::exportJS(context);
        //JSTextMemoryCanvas::getInstance()->exportJS(context);
        JSArrayBufferRef::exportJS(context);
        /*if (!g_kSystemConfig.m_bConchWebGL) {
            JSTransform::exportJS(context);
            JSRenderStateCommand::exportJS(context);
            JSRenderState::exportJS(context);
            JSGLRenderDrawContext::exportJS(context);
            JSContext2D::exportJS(context);
            JSWebGLCacheAsNormalCanvas::exportJS(context);
            JSGLTextureContext::exportJS(context);
            JSGL2TextureContext::exportJS(context);
            JSWebGLInternalTex::exportJS(context);
            JSWebGLEngine::exportJS(context);
            JSRenderTexture2D::exportJS(context);
            JSGLVertexState::exportJS(context);
            JSVertexBuffer3D::exportJS(context);
            JSIndexBuffer3D::exportJS(context);
            JSShaderInstance::exportJS(context);
            JSAttributeMap::exportJS(context);
            JSShaderData::exportJS(context);
            JSCommandEncoder::exportJS(context);
            JSWebGLInternalRT::exportJS(context);
            JSGLBuffer::exportJS(context);
            JSRenderGeometryElement::exportJS(context);
            JSRenderElement::exportJS(context);
            JSRenderQueue::exportJS(context);
            JSCommandUniformMap::exportJS(context);
            JSRenderContext3D::exportJS(context);
            JSRenderNode::exportJS(context);
            JSSceneCullManger::exportJS(context);
            JSCullPass::exportJS(context);
            JSUploadMemoryManager::exportJS(context);
            JSQuickSort::exportJS(context);
            JSWordText::exportJS(context);
            JSCameraCullInfo::exportJS(context);
            JSShadowCullInfo::exportJS(context);
            JSSubmitScene3D::exportJS(context);
            JSBounds::exportJS(context);
            JSUniformBufferObject::exportJS(context);
        }*/
        Module layaNative(context.isolate());
		JSLayaNative::getInstance()->exportJS(layaNative);


	
        Module env(context.isolate());
        env.constant("USER_DATA_PATH", std::string("wxfile://usr"));
        layaNative.submodule("env", env);
        
                     
        JSFileSystemManager::getInstance()->exportJS(layaNative);
        
        JsDownloadTask::exportJS(context);


        context.module("LayaNative", layaNative);

		JSVideo::exportJS(context);   

        RenderBindings::exportJS(context);    
        //以下是全局函数
        //------------------------------------------------------------------------------
        context.function("setTouchEvtFunction", &setTouchEvtFunc);
        context.function("setKeyEvtFunction", &setKeyEvtFunc);
        context.function("setJoystickEvtFunction", &setJoystickEvtFunc);
        context.function("tmGetCurms", &tmGetCurms);
        context.function("reloadJS", &reloadJSThread);
        context.function("getExePath", &getExePath);
        context.function("getInnerHeight", &getInnerHeight);
        context.function("getInnerWidth", &getInnerWidth);
        context.function("getDevicePixelRatio", &getDevicePixelRatio);
        //global.property("enableTouch", &getEnableTouch);TODO
        context.function("alert", &JSAlert);
        context.function("print", &JSPrint);
        context.function("evalJS", &evalJS);
        //JSP_ADD_GLOBAL_FUNCTION(PerfShow, PerfShow, int);
        //JSP_ADD_GLOBAL_FUNCTION(PerfAddData, PerfAddData, int,int,float,float);
        //JSP_ADD_GLOBAL_FUNCTION(PerfUpdateDt, PerfUpdateDt, int,float);

        context.function("readFileSync", &readFileSync1);
        context.function("writeStrFileSync", &writeStrFileSync);
        context.function("readTextAsset", &readTextAsset);
        context.function("fs_exists", &JSFileSystem::exists);
        context.function("fs_mkdir", JSFileSystem::mkdir);
        context.function("fs_rm", &JSFileSystem::rm);
        context.function("fs_rmDir", &JSFileSystem::rmDir);
        context.function("fs_rmDirSync", &JSFileSystem::rmDirSync);
        context.function("fs_readdirSync", &JSFileSystem::readdirSync);
        context.function("fs_lstatSync", &JSFileSystem::lstatSync);
        context.function("fs_writeFileSync", &JSFileSystem::JSWriteFileSync);
        context.function("decodeTemp", &UrlDecode);//以后实现各和JS一样的这个就可以删了
        context.function("showAlertOnJsException", &showAlertOnJsException);
        context.function("fs_readFileSync", &JSFileSystem::readBinFileSync);//这个返回的是ArrayBuffer接口
        context.function("downloadBigFile", &_downloadBigFile);
        context.function("downloadGetHeader", &_downloadGetHeader);
        context.function("calcmd5", &calcMD5_JSAB);
        context.function("conchToBase64", &conchToBase64);
        context.function("conchToBase64FlipY", &conchToBase64FlipY);
        context.function("copy", &copy);
        context.function("preCompile2D", &Shader2D::preprocess2D);
        context.function("open", &open);
        context.function("set2DRenderConfig", &Context2D::set2DRenderConfig);
        context.function("btoa", &btoa);
        context.function("atob", &atob);
        context.function("_createImageBitmap", &createImageBitmap);
        JSLayaConchBullet::exportJS(context);
 #if !defined(LINUX) && !defined(WIN32)
        JSLayaConchPhysX::exportJS(context);
#endif
	}
    void JSGlobalDisExportC() {
        FontManager::deleteInstance();
        delete JSLayaGL::getInstance();

        //delete JSTextMemoryCanvas::getInstance();
        delete JSLayaNative::getInstance();
        delete JSFileSystemManager::getInstance();
    }
}

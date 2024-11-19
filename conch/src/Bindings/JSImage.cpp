#include "JSImage.h"
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include <jsbind/JSBind.h>
#include "../../JCScriptRuntime.h"
#include <resource/JCFileResManager.h> 
#include "JSRuntime.h"
#include <utils/JCFileSystem.h>
#include "../../JCConch.h"
#include "../../JCSystemConfig.h"
#include <LayaGL/JCLayaGLDispatch.h>
#include <utils/JCCrypto.h>

namespace laya
{
    JSImage::JSImage()
    {
        m_pImage = std::make_shared<JCImage>();
        m_nID = JCConch::s_pConchRender->m_pImageManager->getImageID();
        m_pImage->setManager(JCConch::s_pConchRender->m_pFileResManager, JCConch::s_pConchRender->m_pImageManager);
        m_CallbackRef.reset(new int(1));
        m_bComplete = false;
        m_nDownloadState = 0;
    }
    JSImage::~JSImage()
    {
	    m_pOnLoad.reset();
	    m_pOnError.reset();
        m_pObj.reset();
	    JCMemorySurvey::GetInstance()->releaseClass( "image",this );
        destroy();
    }
    void JSImage::destroy()
    {
        if (m_pImage)
        {
            //通知渲染线程
            deleteImageOnRenderThread(m_nID);
	    }
    }
    void JSImage::releaseTexture()
    {
        if (m_pImage)
        {
            releaseImageOnRenderThread(m_nID);
        }
    }

    void JSImage::onLoaded(std::weak_ptr<int> callbackref)
    {
	    std::function<void(void)> pFunction = std::bind(&JSImage::onLoadedCallJSFunction,this, callbackref);
        postToJS(pFunction);
    }
    void JSImage::onError( int p_nError,std::weak_ptr<int> callbackref )
    {
	    std::function<void(void)> pFunction = std::bind(&JSImage::onErrorCallJSFunction,this,p_nError, callbackref);
        postToJS( pFunction );
    }
    void JSImage::onLoadedCallJSFunction(std::weak_ptr<int> callbackref)
    {
	    if (!callbackref.lock()) return;
        //if (JCScriptRuntime::s_JSRT->m_bIsExit == true)return;
        //if (!IsMyJsEnv()) return;

        if (GetWidth() <= 0 || GetHeight() <= 0|| m_pImage->m_kBitmapData.m_pImageData==NULL) {
            m_pOnError.call<void>(jsbind::toLocal(this), 500);
        }
        else {
            int nMemSize = GetWidth() * GetHeight() * 4 + 272;
            jsbind::AdjustAmountOfExternalAllocatedMemory(nMemSize);
            JCMemorySurvey::GetInstance()->newClass("image", 1024, this);
            m_pImage->m_sUrl = m_sUrl;
            //通知渲染线程
            createImageOnRenderThread(m_nID, m_pImage);
            m_bComplete = true;
            m_pOnLoad.call<void>(jsbind::toLocal(this));
        }
        jsbind::makeWeak(this);
    }
    void JSImage::onErrorCallJSFunction( int p_nError,std::weak_ptr<int> callbackref )
    {
	    if (!callbackref.lock())return;
        //if (JCScriptRuntime::s_JSRT->m_bIsExit == true)return;
	    //if (!IsMyJsEnv())return;
        LOGW("download image file error! %s\n", m_sUrl.c_str());
        m_pOnError.call<void>(jsbind::toLocal(this), p_nError);
        jsbind::makeWeak(this);
    }
    bool JSImage::getComplete()
    {
        return m_bComplete;
    }
    void JSImage::SetOnload(jsvm::Value p_pFunction )
    {
	    m_pOnLoad = jsbind::Persistent(p_pFunction);
    }
    jsvm::Value JSImage::GetOnload()
    {
	    return m_pOnLoad.getHandle();
    }
    void JSImage::SetOnError(jsvm::Value p_pFunction )
    {
	    m_pOnError = jsbind::Persistent(p_pFunction);
    }
    jsvm::Value JSImage::GetOnError()
    {
        return m_pOnError.getHandle();
    }
    jsvm::Value JSImage::getObj()
    {
        return m_pObj.getHandle();
    }
    void JSImage::setObj(jsvm::Value obj)
    {
        m_pObj = jsbind::Persistent(obj);
    }
    const char* JSImage::getSrc()
    {
        return m_sUrl.c_str();
    }
    bool JSImage::syncRestoreResource()
    {
        //if (!m_pImage || !m_pImage->m_pImageFile ) return false;
        //return m_pImage->m_pImageFile->downloadImage(true);
        return false;
    }
    void JSImage::setSrc( const char* p_sSrc )
    {
	    if (!p_sSrc) return;
        m_sUrl = p_sSrc;
	    std::weak_ptr<int> cbref(m_CallbackRef);
        downloadImage( false );
    }
    void JSImage::onDecodeEnd(BitmapData& p_bmp, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock())
        {
            if (p_bmp.m_pImageData)
            {
                delete[] p_bmp.m_pImageData;
            }
            return;
        }
        if (m_pImage && m_pImage->m_kBitmapData.m_pImageData)
        {
            m_pImage->m_kBitmapData.releaseData();
            m_pImage->m_kBitmapData.m_pImageData = NULL;
        }
        m_pImage->m_kBitmapData = p_bmp;
        onLoaded(callbackref);
    }
    void JSImage::onDecodeEndDecThread(BitmapData p_bmp, std::weak_ptr<int>& callbackref)
    {
        postToJS(std::bind(&JSImage::onDecodeEnd, this, p_bmp, callbackref));
    }
    void JSImage::onDownloadOK(JCResStateDispatcher* p_pRes, bool p_bDecodeSync, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock())
        {
            return;
        }
        m_nDownloadState = 0;
        JCFileRes* pFileRes = (JCFileRes*)p_pRes;
        if (pFileRes->m_pBuffer.get())
        {
		
            //同步加载
            if (p_bDecodeSync)
            {
            }
            else
            {
                imgDecodeCB cb = std::bind(&JSImage::onDecodeEndDecThread, this, std::placeholders::_1, callbackref);
                loadImageMemASync(pFileRes->m_pBuffer, pFileRes->m_nLength, cb);
            }
        }
        else
        {
            onDownloadError(p_pRes, 1, callbackref);
        }
    }
    void JSImage::onDownloadError(JCResStateDispatcher*, int e, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock())return;
        m_nDownloadState = 0;
        onError(e,callbackref);
    }
    bool JSImage::downloadImage(bool p_bSyncDecode)
    {
        if (m_nDownloadState == 1)return false;
        m_nDownloadState = 1;
        //TODO 要处理本地文件的情况
        std::weak_ptr<int> cbref(m_CallbackRef);
        JCFileRes* pRes = JCConch::s_pScriptRuntime->m_pFileResMgr->getRes(m_sUrl);
        pRes->setOnReadyCB(std::bind(&JSImage::onDownloadOK, this, std::placeholders::_1, false, cbref));
        pRes->setOnErrorCB(std::bind(&JSImage::onDownloadError, this, std::placeholders::_1, std::placeholders::_2, cbref));
        jsbind::makeStrong(this);
        return true;
    }
    int JSImage::GetWidth()
    {
	    return m_pImage->getWidth();
    }
    int JSImage::GetHeight()
    {
	    return m_pImage->getHeight();
    }
    void JSImage::putBitmapDataJS(jsbind::ArrayBuffer arrayBuffer, int width, int height)
    {
        DEBUG_CHECK(arrayBuffer.isValid());
        char* pArrayBufferPtr = reinterpret_cast<char*>(arrayBuffer.getData());
        int nABLen = arrayBuffer.getByteLength();
        if (nABLen >= width * height * 4)
        {
            putBitmapData(pArrayBufferPtr,width, height);
        }
        else
        {
            LOGE("JSImage::pushBitmapData array buffer size < width * height * 4");
        }
    }

    static void deleter(char* p)
    {
        //不删除 JS保证在onDecodeEndDecThread前pArrayBuffer不垃圾回收
    }

    void JSImage::putDataJS(jsbind::ArrayBuffer arrayBuffer)
    {
        DEBUG_CHECK(arrayBuffer.isValid());
        char* pArrayBufferPtr = reinterpret_cast<char*>(arrayBuffer.getData());
        int nABLen = arrayBuffer.getByteLength();

        if (nABLen <= 0)
            return;
        //设置url的名字
        char sCachePath[1024];
        memset(sCachePath, 0, 1024);
        sprintf(sCachePath, "%s/%d.LayaBoxImg", JCConch::s_pConch->m_sCachePath.c_str(), m_nID);
        m_sUrl = sCachePath;
        std::weak_ptr<int> cbref(m_CallbackRef);
        imgDecodeCB cb = std::bind(&JSImage::onDecodeEndDecThread, this, std::placeholders::_1, cbref);
        std::shared_ptr<char> pBuffer(pArrayBufferPtr, deleter);
        loadImageMemASync(pBuffer, nABLen, cb);

    }
    void JSImage::setBase64(const char* base64)
    {
        if (!base64)
            return;
        //设置url的名字
        char sCachePath[1024];
        memset(sCachePath, 0, 1024);
        sprintf(sCachePath, "%s/%d.LayaBoxImg", JCConch::s_pConch->m_sCachePath.c_str(), m_nID);
        m_sUrl = sCachePath;
        std::weak_ptr<int> cbref(m_CallbackRef);
        imgDecodeCB cb = std::bind(&JSImage::onDecodeEndDecThread, this, std::placeholders::_1, cbref);
        int length = 0;
        std::shared_ptr<char> pBuffer(base64_decode((const unsigned char*)base64, strlen(base64), &length));
        loadImageMemASync(pBuffer, length, cb);
    }
    void JSImage::putBitmapData(char* pData, int width, int height )
    {
        if (m_pImage && m_pImage->m_kBitmapData.m_pImageData) 
        {
            m_pImage->m_kBitmapData.releaseData();
            m_pImage->m_kBitmapData.m_pImageData = NULL;
        }
        m_pImage->m_kBitmapData.m_nWidth = width;
        m_pImage->m_kBitmapData.m_nHeight = height;
        m_pImage->m_kBitmapData.m_pImageData = new char[width * height * 4];
        memcpy(m_pImage->m_kBitmapData.m_pImageData, pData, width*height * 4);
        //设置url的名字
        char sCachePath[1024];
        memset(sCachePath, 0, 1024);
        sprintf(sCachePath,"%s/%d.LayaBoxImg", JCConch::s_pConch->m_sCachePath.c_str(),m_nID);
        //写入文件
        writeFileSync1(sCachePath, pData, width*height * 4);
        m_sUrl = sCachePath;
        m_pImage->m_bPushBitmapData = true;
        onLoaded(m_CallbackRef);
    }
    jsvm::Value JSImage::getImageData( int p_nX,int p_nY,int p_nW,int p_nH )
    {
	    if( m_bComplete == false ) return jsbind::MakeNull();
	    if( m_pImage == NULL ) return jsbind::MakeNull();
	    BitmapData* pImg = &(m_pImage->m_kBitmapData);
	    if( pImg  )
	    {
		    if( p_nX < 0 || p_nY < 0 || p_nX >= pImg->m_nWidth || p_nY >= pImg->m_nHeight )return jsbind::MakeNull();
		    if( ( p_nX + p_nW ) > pImg->m_nWidth || ( p_nY + p_nH ) > pImg->m_nHeight  )return jsbind::MakeNull();

            if (pImg->m_pImageData != NULL || (pImg->m_pImageData == NULL && m_pImage->enableImage()))
            {
		        if( p_nX == 0 && p_nY == 0 && p_nW == pImg->m_nWidth && p_nH == pImg->m_nHeight )
		        {
			        return jsbind::ArrayBuffer::MakeArrayBuffer((uint8_t*)(pImg->m_pImageData),pImg->m_nWidth * pImg->m_nHeight * 4 ).getHandle();
		        }
		        else
		        {
			        unsigned char* pTemp = (unsigned char *)(pImg->m_pImageData);
			        int nSize = p_nH * p_nW * 4;
			        int nDstLine = p_nW*4;
			        int nSrcLine = pImg->m_nWidth*4;
			        unsigned char* pBuffer = new unsigned char[nSize];
			        for( int i = 0; i < p_nH; i++ )
			        {
				        memcpy(&pBuffer[nDstLine*i],&pTemp[nSrcLine*(i+p_nY)+p_nX*4],nDstLine);
			        }
			        return jsbind::ArrayBuffer::MakeArrayBuffer((uint8_t*)pBuffer, nSize).getHandle();
		        }
            }
	    }
	    return jsbind::MakeNull();
    }
    int JSImage::getImageID()
    {
	    return m_nID;
    }

    void JSImage::exportJS(jsbind::Object& context) 
    {
        jsbind::class_<JSImage> class_binding;
        class_binding.constructor<>();
        class_binding.property("conchImgId", &JSImage::getImageID);
        class_binding.property("width", &JSImage::GetWidth);
        class_binding.property("height", &JSImage::GetHeight);
        class_binding.function("setBase64", &JSImage::setBase64);
        class_binding.property("src", &JSImage::getSrc, &JSImage::setSrc);
        class_binding.property("_onload", &JSImage::GetOnload, &JSImage::SetOnload);
        class_binding.property("onload", &JSImage::GetOnload, &JSImage::SetOnload);
        class_binding.property("onerror", &JSImage::GetOnError, &JSImage::SetOnError);
        class_binding.property("obj", &JSImage::getObj, &JSImage::setObj);
        class_binding.property("complete", &JSImage::getComplete);
        class_binding.function("getImageID", &JSImage::getImageID);
        class_binding.function("setSrc", &JSImage::setSrc);
        class_binding.function("getImageData", &JSImage::getImageData);
        class_binding.function("putBitmapData", &JSImage::putBitmapDataJS);
        class_binding.function("putData", &JSImage::putDataJS);
        class_binding.function("syncRestoreResource", &JSImage::syncRestoreResource);
        class_binding.function("destroy", &JSImage::destroy);
        context.class_("conchImage", class_binding);
    }
    void JSImage::createImageOnRenderThread(int nID, std::shared_ptr<JCImage> pImage)
    {
        JCConch::s_pConchRender->postTaskFromJSToRenderAsync([nID, pImage]() {
            JCConch::s_pConchRender->m_pImageManager->setImage(nID, pImage);
        });
    }
    void JSImage::deleteImageOnRenderThread(int nID)
    {
        JCConch::s_pConchRender->postTaskFromJSToRenderAsync([nID]() {
            JCConch::s_pConchRender->m_pImageManager->deleteImage(nID);
         });
    }
    void JSImage::releaseImageOnRenderThread(int nID)
    {
        JCConch::s_pConchRender->postTaskFromJSToRenderAsync([nID]() {
            auto pImage = JCConch::s_pConchRender->m_pImageManager->getImage(nID);
            if (pImage){
                pImage->releaseBitmapData();
            }
        });
    }
}
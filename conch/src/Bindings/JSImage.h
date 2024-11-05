#ifndef __JSImage_H__
#define __JSImage_H__

//包含头文件
#include <stdio.h>
#include <binder/JSBind.h>
#include <Image/JCImage.h>
#include <resource/JCResource.h>

/** 
 * @brief 
*/
namespace laya
{
    class IConchThreadCmdMgr;
	class JSImage
	{
    public:

        JSImage();

        ~JSImage();

	public:
		enum { onloadid, onerrorid, thisid, objid };
        static void exportJS(jsbind::Object& context);

        void onLoaded(std::weak_ptr<int> callbackref);

		void onError( int p_nError,std::weak_ptr<int> callbackref );

		void onLoadedCallJSFunction(std::weak_ptr<int> callbackref);

		void onErrorCallJSFunction( int p_nError,std::weak_ptr<int> callbackref);
        
        void setObj(jsvm::Value p_pFunction);
        
        jsvm::Value getObj();

		void SetOnload(jsvm::Value p_pFunction );

		jsvm::Value GetOnload();

		void SetOnError(jsvm::Value p_pFunction );

		jsvm::Value GetOnError();

		int GetWidth();

		int GetHeight();
		
		const char* getSrc();

		void setSrc( const char* p_sSrc );

        bool getComplete();

		jsvm::Value getImageData( int p_nX,int p_nY,int p_nW,int p_nH );

        bool syncRestoreResource();

        void putBitmapData( char* pData,int width, int height);

        void putBitmapDataJS(jsbind::ArrayBuffer arrayBufferr, int width, int height );

        void putDataJS(jsbind::ArrayBuffer arrayBuffer);

        void setBase64(const char* base64);

    private:

        void onDecodeEnd(BitmapData& p_bmp, std::weak_ptr<int>& callbackref);

        void onDecodeEndDecThread(BitmapData p_bmp, std::weak_ptr<int>& callbackref);

        void onDownloadOK(JCResStateDispatcher* p_pRes, bool p_bDecodeSync, std::weak_ptr<int>& callbackref);

        void onDownloadError(JCResStateDispatcher* p_pRes, int e, std::weak_ptr<int>& callbackref);

        bool downloadImage(bool p_bSyncDecode);


    public:

        int getImageID();

        void destroy();

        void releaseTexture();

        void createImageOnRenderThread(int nID,std::shared_ptr<JCImage> pImage);

        void deleteImageOnRenderThread(int nID);

        void releaseImageOnRenderThread(int nID);
	public:
		jsbind::Persistent		                m_pOnLoad;
		jsbind::Persistent		                m_pOnError;
		jsbind::Persistent                     m_pObj;
        bool			                m_bComplete;
        std::shared_ptr<int>	        m_CallbackRef;
        std::string                     m_sUrl;
        int                             m_nDownloadState;
    public:
        std::shared_ptr<JCImage>	    m_pImage;
        int                             m_nID;

	};
}

#endif 
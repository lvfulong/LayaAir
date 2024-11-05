#ifndef __JSRuntime_H__
#define __JSRuntime_H__

#include <stdio.h>
#include <binder/JSBind.h>
#include "../../JCScriptRuntime.h"


namespace laya 
{
    class JSRuntime 
    {
    public:
        enum 
        {
            onframeid, ondrawid, onresizeid, ontouchevtid,ondevicemotionevtid, onkeyevtid, onmouseevtid, oninvalidglid,onotherevtid,onnetworkevt,onblurid,onfocusid, bulletsetid, bulletgetid, zippackageid,onunhandledrejection, screenorientationchangeid, setglobalrepaintid, bulletdrawlineid, bulletclearlineid
        };
    
	    static void exportJS(jsbind::Object& context);
	    
    public:

        static void setOnFrameFunction( jsvm::Value p_pFunction );
        static void setOnDrawFunction(jsvm::Value p_pFunction);

        static void setOnResizeFunction(jsvm::Value p_onresize);

        static void setOnBlurFunction(jsvm::Value p_pFunction);
        
        static void setOnFocusFunction(jsvm::Value p_pFunction);
        
        static void setHref(jsvm::Value p_sHref);

        static void setMouseEvtFunction(jsvm::Value p_pFunction);

        static void setTouchEvtFunction(jsvm::Value p_pFunction);

        static void setDeviceMotionEvtFunction(jsvm::Value p_pFunction);

        static void captureScreen(jsvm::Value p_pFunction);

        static void setKeyEvtFunction(jsvm::Value p_pFunction);

        static void setNetworkEvtFunction(jsvm::Value p_pFunction);

        static void setScreenWakeLock(bool bWakeLock);

        static void setSensorAble(bool bSensorAble);

        static void setBuffer(jsbind::ArrayBuffer arrayBuffer);

        static bool saveAsPng(jsbind::ArrayBuffer arrayBuffer, int w, int h, const char* p_pszFile);

        static bool saveAsJpeg(jsbind::ArrayBuffer arrayBuffer, int w, int h, const char* p_pszFile);
        
        static jsvm::Value convertBitmapToPng(jsbind::ArrayBuffer arrayBuffer, int w, int h);
        
        static jsvm::Value convertBitmapToJpeg(jsbind::ArrayBuffer arrayBuffer, int w, int h);

        static void setGetWorldTransformFunction(jsvm::Value p_pFunction);

        static void setSetWorldTransformFunction(jsvm::Value p_pFunction);

		static void setBulletDrawLineFunction(jsvm::Value p_pFunction);

		static void setBulletClearLineFunction(jsvm::Value p_pFunction);
		
        static void setZipPackage(jsvm::Value p_pFunction);

		static int getSafeInsetTop();

		static int GetSafeInsetLeft();

		static int GetSafeInsetBottom();

		static int GetSafeInsetRight();

		static jsvm::Value getLaunchOptionsSync();

		static void setOnUnhandledRejection(jsvm::Value p_pFunction);

		static jsvm::Value getOnUnhandledRejection();

		static void setScreenOrientation(const char* p_strOrientation, jsvm::Value p_pFunction);

		static void setGlobalRepaint(jsvm::Value p_pFunction);

		static void setGlobalRepaintCall();

		static void computeSubSkinnedDataForNative(jsvm::Value inverseBindPosesBuffer, jsvm::Value boneIndices, jsvm::Value subData, jsvm::Value skinnedMatrixCaches, jsvm::Value bonesTransform, jsvm::Value skinnedDataLoopMarks, jsvm::Value skinnedData);

        static bool registerFont(const std::string& family, jsbind::Local pathOrArrayBuffer);

        static void downloadFile(const std::string& url, jsvm::Value onProgress, jsvm::Value onComplete, jsvm::Value onError);
        static void setDownloader(jsvm::Value obj);

        static jsvm::Value postAsyncMessage(const std::string &eventName, const std::string &data);
        static std::string postSyncMessage(const std::string &eventName, const std::string &data);
    public:

        static jsvm::Value readFileFromAsset(const char* file, const char* encode);

        static jsvm::Value strTobufer(const char* s);

        static const char* callMethod(int objid,bool isSyn,const char*clsName, const char* methodName, const char* paramStr);

        static const char* getCachePath();

        static const char* getPresetUrl();

        static void printCorpseImages();

	    static void exit();

        static jsvm::Value createArrayBufferRef(jsbind::ArrayBuffer arrayBuffer, int nType, bool bSyncToRender, int nRefType);
        

        static std::string m_strReturn;
			
    };
}


#endif 

#ifndef __JSRuntime_H__
#define __JSRuntime_H__

#include <stdio.h>
#include <jsbind/JSBind.h>
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

        static void setOnFrameFunction( jsvm_value p_pFunction );
        static void setOnDrawFunction(jsvm_value p_pFunction);

        static void setOnResizeFunction(jsvm_value p_onresize);

        static void setOnBlurFunction(jsvm_value p_pFunction);
        
        static void setOnFocusFunction(jsvm_value p_pFunction);
        
        static void setHref(jsvm_value p_sHref);

        static void setMouseEvtFunction(jsvm_value p_pFunction);

        static void setTouchEvtFunction(jsvm_value p_pFunction);

        static void setDeviceMotionEvtFunction(jsvm_value p_pFunction);

        static void captureScreen(jsvm_value p_pFunction);

        static void setKeyEvtFunction(jsvm_value p_pFunction);

        static void setNetworkEvtFunction(jsvm_value p_pFunction);

        static void setScreenWakeLock(bool bWakeLock);

        static void setSensorAble(bool bSensorAble);

        static void setBuffer(jsbind::ArrayBuffer arrayBuffer);

        static bool saveAsPng(jsbind::ArrayBuffer arrayBuffer, int w, int h, const char* p_pszFile);

        static bool saveAsJpeg(jsbind::ArrayBuffer arrayBuffer, int w, int h, const char* p_pszFile);
        
        static jsvm_value convertBitmapToPng(jsbind::ArrayBuffer arrayBuffer, int w, int h);
        
        static jsvm_value convertBitmapToJpeg(jsbind::ArrayBuffer arrayBuffer, int w, int h);

        static void setGetWorldTransformFunction(jsvm_value p_pFunction);

        static void setSetWorldTransformFunction(jsvm_value p_pFunction);

		static void setBulletDrawLineFunction(jsvm_value p_pFunction);

		static void setBulletClearLineFunction(jsvm_value p_pFunction);
		
        static void setZipPackage(jsvm_value p_pFunction);

		static int getSafeInsetTop();

		static int GetSafeInsetLeft();

		static int GetSafeInsetBottom();

		static int GetSafeInsetRight();

		static jsvm_value getLaunchOptionsSync();

		static void setOnUnhandledRejection(jsvm_value p_pFunction);

		static jsvm_value getOnUnhandledRejection();

		static void setScreenOrientation(const char* p_strOrientation, jsvm_value p_pFunction);

		static void setGlobalRepaint(jsvm_value p_pFunction);

		static void setGlobalRepaintCall();

        static void setOnError(jsvm_value p_pFunction);

		static void computeSubSkinnedDataForNative(jsvm_value inverseBindPosesBuffer, jsvm_value boneIndices, jsvm_value subData, jsvm_value skinnedMatrixCaches, jsvm_value bonesTransform, jsvm_value skinnedDataLoopMarks, jsvm_value skinnedData);

        static bool registerFont(const std::string& family, jsbind::Local pathOrArrayBuffer);

        static void downloadFile(const std::string& url, jsvm_value onProgress, jsvm_value onComplete, jsvm_value onError);
        static void setDownloader(jsvm_value obj);

        static jsvm_value postAsyncMessage(const std::string &eventName, const std::string &data);
        static std::string postSyncMessage(const std::string &eventName, const std::string &data);
        static void setPreferredFramesPerSecond(uint32_t fps);
    public:

        static jsvm_value readFileFromAsset(const char* file, const char* encode);

        static jsvm_value strTobufer(const char* s);

        static const char* callMethod(int objid,bool isSyn,const char*clsName, const char* methodName, const char* paramStr);

        static const char* getCachePath();

        static const char* getPresetUrl();

        static void printCorpseImages();

	    static void exit();

        static jsvm_value createArrayBufferRef(jsbind::ArrayBuffer arrayBuffer, int nType, bool bSyncToRender, int nRefType);
        

        static std::string m_strReturn;
			
    };
}


#endif 

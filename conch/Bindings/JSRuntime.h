#ifndef __JSRuntime_H__
#define __JSRuntime_H__

#include <stdio.h>
#include <binder/JSInterface.h>
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

	    
	    static void exportJS(Context& context);
	    
    public:

        static void setOnFrameFunction( JSValueAsParam p_pFunction );
        static void setOnDrawFunction(JSValueAsParam p_pFunction);

        static void setOnResizeFunction(JSValueAsParam p_onresize);

        static void setOnBlurFunction(JSValueAsParam p_pFunction);
        
        static void setOnFocusFunction(JSValueAsParam p_pFunction);
        
        static void setHref(JSValueAsParam p_sHref);

        static void setMouseEvtFunction(JSValueAsParam p_pFunction);

        static void setTouchEvtFunction(JSValueAsParam p_pFunction);

        static void setDeviceMotionEvtFunction(JSValueAsParam p_pFunction);

        static void captureScreen(JSValueAsParam p_pFunction);

        static void setKeyEvtFunction(JSValueAsParam p_pFunction);

        static void setNetworkEvtFunction(JSValueAsParam p_pFunction);

        static void setScreenWakeLock(bool p_bWakeLock);

        static void setSensorAble(bool p_bSensorAble);

        static void setBuffer(JSValueAsParam pArrayBuffer);

        static bool saveAsPng(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile);

        static bool saveAsJpeg(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile);
        
        static JsValue convertBitmapToPng(JSValueAsParam pArrayBufferArgs, int w, int h);
        
        static JsValue convertBitmapToJpeg(JSValueAsParam pArrayBufferArgs, int w, int h);

        static void setGetWorldTransformFunction(JSValueAsParam p_pFunction);

        static void setSetWorldTransformFunction(JSValueAsParam p_pFunction);

		static void setBulletDrawLineFunction(JSValueAsParam p_pFunction);

		static void setBulletClearLineFunction(JSValueAsParam p_pFunction);
		
        static void setZipPackage(JSValueAsParam p_pFunction);

		static int getSafeInsetTop();

		static int GetSafeInsetLeft();

		static int GetSafeInsetBottom();

		static int GetSafeInsetRight();

		static JsValue getLaunchOptionsSync();

		static void setOnUnhandledRejection(JSValueAsParam p_pFunction);

		static JsValue getOnUnhandledRejection();

		static void setScreenOrientation(const char* p_strOrientation, JSValueAsParam p_pFunction);

		static void setGlobalRepaint(JSValueAsParam p_pFunction);

		static void setGlobalRepaintCall();

		static void computeSubSkinnedDataForNative(JSValueAsParam inverseBindPosesBuffer, JSValueAsParam boneIndices, JSValueAsParam subData, JSValueAsParam skinnedMatrixCaches, JSValueAsParam bonesTransform, JSValueAsParam skinnedDataLoopMarks, JSValueAsParam skinnedData);

        static bool registerFont(JSValueAsParam jsFamily, JSValueAsParam pathOrArrayBuffer);
    public:

        static JsValue readFileFromAsset(const char* file, const char* encode);

        static JsValue strTobufer(const char* s);

        static const char* callMethod(int objid,bool isSyn,const char*clsName, const char* methodName, const char* paramStr);

        static const char* getCachePath();

        static const char* getPresetUrl();

        static void printCorpseImages();

        static void setExternalLink(const char* sUrl);

        static void setExternalLinkEx( const char* sUrl,int x,int y,int w,int h,bool bCloseWebview );

        static void closeExternalLink();

        static void callWebviewJS( const char* sFunctionName,const char* sJsonParam,const char* sCallbackFunction );

        static void hideWebview();

        static void showWebView();
		
	    static void exit();

        static JsValue createArrayBufferRef(JSValueAsParam pArrayBuffer, int nType, bool bSyncToRender, int nRefType);
        

        static std::string m_strReturn;
			
    };
}


#endif 

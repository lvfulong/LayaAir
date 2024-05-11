#ifndef __JSInput_H__
#define __JSInput_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include "../../JCScriptRuntime.h"
#include <mutex>
#include <utils/InputTypes.h>

namespace laya
{
    struct TouchEventInfo
    {
        int nType;
        int nID;
        int x;
        int y;
    };
    class IConchThreadCmdMgr;
	class JSInput
	{
	public:
		static JSInput* getInstance();
		static void Reset();
	public:
		JSInput();
		~JSInput();
		void onInputCallJSFunction(inputEvent e);
        void onDeviceOrientationCallJSFunction(DeviceOrientationEvent e);
        void onDeviceMotionCallJSFunction(DeviceMotionEvent e);
		bool activeCall( inputEvent param );
        bool activeCall(DeviceOrientationEvent param);
        bool activeCall(DeviceMotionEvent param);
        void captureScreenCallBack(char *p_pBuffer,int p_nLen, int p_nW, int p_nH);
        void onCaptureScreenCallJSFunction(char *p_pBuffer, int p_nLen, int p_nW, int p_nH);
        void swapCurrentTouchEvent();
        void setTouchMode( bool bMode );
        bool getTouchMode();

	public:
		static JSInput*                     m_pInstance;
        bool                                m_bTouchMode;   ///<trueΪ��ѯģʽ  falseΪ�ص�ģʽ
        std::vector<TouchEventInfo>         m_vInputEvents;
        std::vector<TouchEventInfo>         m_vInputEventsJS;
        std::recursive_mutex                m_Lock;
        int                                 m_nTouchFrame;
	};
}
#endif
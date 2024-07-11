
#ifndef _LAYA_CONFIG_H__
#define _LAYA_CONFIG_H__

#include <string>
#include <utils/Types.h>

namespace laya
{

    enum FRAME_TYPE
    {
        FT_SLOW = 0,
        FT_FAST,
        FT_MOUSE,
    };
	class JCSystemConfig 
	{
	public:
		JCSystemConfig();
		void reset();
	public:
		std::string     m_strStartURL;
		bool	        m_bPerfStat;			        //����Ч��ͳ��
        FRAME_TYPE      m_nFrameType;
        double          m_nFrameThreshold;
        int             m_nSleepTime;
		int		        m_nPerf_UpdateNum;
		std::string	    m_strPerfOut;
        bool            m_bShowInternalPerBar;          //�Ƿ���ʾjs ondraw���ֺ�gl����״����ͼ��
        static bool     s_bIsPlug;                      //��Ϊ��ʼ��̫���ˣ�����static
        static bool     s_bLocalizable;                 //�����Ƿ��Ǳ��ذ�
		GraphicsAPI     m_graphicsAPI = GraphicsAPI::OpenGLES;
		int 			m_nOrientationType = 24;
		void 			loadConfigIniFile();
        int 			m_nJSDebugMode = 0;
    	int 			m_nJSDebugPort = 5959;
	};
	extern JCSystemConfig g_kSystemConfig;
}

#endif

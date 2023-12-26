#include "JCSystemConfig.h"
#include <utils/JCCommonMethod.h>

namespace laya
{
	JCSystemConfig g_kSystemConfig;
	JCSystemConfig::JCSystemConfig()
    {
		reset();
	}
    bool JCSystemConfig::s_bIsPlug = true;	//�����������ͬһ����Ҳ����ı䡣
    bool JCSystemConfig::s_bLocalizable = false;
	void JCSystemConfig::reset()
    {
		m_bPerfStat=false;
        m_nFrameType = FT_FAST;
        m_nFrameThreshold = 2000;
        m_nSleepTime = 0;
		m_strStartURL = "";
		m_nPerf_UpdateNum=500;
		m_jsonparamExt="";
        s_bLocalizable = false;
        m_bShowInternalPerBar = false;
        m_nThreadMODE = THREAD_MODE_DOUBLE;
        m_bUseDcc = true;
		m_nTmpCacheSpaceThreshold = 512;
		m_nTmpCacheTimeThreshold = 500;
		m_bConchWebGL = true;
	}
}

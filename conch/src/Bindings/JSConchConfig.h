#ifndef __JSConchConfig_H__
#define __JSConchConfig_H__


#include <string>
#include <jsbind/JSBind.h>
#include "JCSystemConfig.h"

namespace laya
{
    class JSConchConfig
    {
    public:
	    static void exportJS(jsbind::Object& context);
	    static bool getUrlIgnoreCase();
	    static void setUrlIgnoreCase(bool b);
	    static int getNetworkType();
		static const char* getIPAddress();
	    static void setDownloadTryNum(int,int);
	    static void setDownloadPathReplace(const char* p_pstrPath, const char* p_pszReplace);
	    static void setDownloadTail(int type, const char* p_strTail);
	    //下载的时候修改文件的扩展名，以防止中间被修改。
	    //扩展名没有.
	    static void setDownloadReplaceExt(const char* p_pszOrigin, const char* p_pszNew );
	    //是否转换文本文件的回车换行
	    static void setDownloadIgnoreCRLR(bool b);
	    static void resetDownloadIgnoreCRLR();

	    //设置不需要严格校验的文件。对于这些文件，如果校验失败了，则直接算作成功，并且保存下来
	    //扩展名有.
	    static void addChkIgnoreChksumExt( const char* p_pszExt );
	    static void clearChkIgnoreChksumExt();

	    //凡是扩展名为p_pszExt的文件,用key来进行异或操作，操作长度为p_nLen。如果p_nKey=0则去掉
	    //扩展名有.
	    static void setDownloadUnmask(const char* p_pszExt, unsigned int p_nKey, int p_nLen );
	    static void resetDownloadUnmask();

	    //如果传送过程中没有反应了，就在超时后重试，如果为0则不使用这一规则
	    static void setDownloadNoResponseTimeout(int p_nDuration);
	    static void resetDownloadNoResponseTimeout();

        /** @brief 设置外部版本控制字符串，例如 index.htm?ver=10 则 p_pszVersion就是 ver
         *  @param p_pszVersion NULL或者 "" 表示不进行外部版本管理
         */
        static void setDownloadVersionString(const char* p_pszVersion);

        static void setScreenScale(float fScaleW, float fScaleH, float ftx, float fty);
        static float getScreenScaleW();
        static float getScreenScaleH();

        static void setCurlProxy(const char* pProxy);
        static void setWebsocketProxy(const char* pProxy);

        static void setJSDebugMode(int nMode);
        static int getJSDebugMode();

        static void setJSDebugPort(int nPort);
        static int getJSDebugPort();

    public:
	    static const char* getLocalStoragePath();
	    static float getTotalMem();
	    static int getUsedMem();
	    static int getAvalidMem();
	    static float getScreenInch();
	    static void setTouchMoveRange( float p_fMM );
	    static void setScreenOrientation( int p_nOrientation );
        /** 
         *  @brief  如果需要自己管理文件更新的話，就通過url后加版本號的方法，這裡可以設置版本號字符串
         *          只有當searchPart只有版本號字符串的時候，才會緩存到同一個文件。這樣是為了避免一些錯誤
         *          例如獲取頭像使用同一個請求地址，但是不同的searchPart
         *          例如，設置的字符串為 v
         *          則 http://host/get?v=1, http://host/get?v=2 佔用同一個緩存文件
         *          http://host/get?v=1&bb=1 就不會佔用相同的緩存文件，
         *          http://host/get?id=1
         *  @param[in] p_strVersion
         */
        //void setUrlVersionStr(const char* p_strVersion);
        //const char* getUrlVersionStr();
        static void setDownloadConnTimeout(int tm);
        static void setDownloadOptTimeout(int tm);
    public:

	    static const char* getOS();
	    static const char* getBrowserInfo();
	    static const char* getGuid();
	    //得到一个版本描述字符串
	    static const char* getRuntimeVersion();
		static const char* getLanguage();
	    static const char* getDeviceInfo();
        static const char* getAppVersion();
        static const char* getAppLocalVersion();

		static void setLanguage(const char* pStrLanguage);
        static void setTouchMode(bool bMode);
        static bool getTouchMode();
        static void setLogLevel(int nLevel);
        //设置图片释放的间隔时间
        static void setImageReleaseSpaceTime(int nSpaceTime);
    
        //打开内存检测
        static void enableMemorySurvey(bool bEnable);

        static void setImageMisoperationWarningTime( int nSpaceTime );

        static float getCurrentDeviceSystemVersion();

        static void printAllMemorySurvey(const char* sPath);

        static void enableEncodeURI(bool b);

        static void setSoundGarbageCollectionTime(int nTime);

		static bool getConchWebGL();

        static GraphicsAPI getGraphicsAPI();

		//机型
		static const char* getModel();

		//国家&地区
		static const char* getCountryCode();

        //tel国家&地区码
        static const char* getTelCountryCode();

        static int getMemoryUsageInByte();

    public:
	    static std::string				m_sGUID;
	    static std::string				m_sDeviceModel;
	    static std::string				m_sDeviceInfo;
        static std::string             m_sAppVersion;
        static std::string             m_sAppLocalVersion;
		static std::string             m_sLanguage;
		static std::string             m_sIPAddress;
        //std::string             m_sVersionStr;
		static std::string             m_sCountryCode;
		static std::string             m_sModel;
        static float                   m_fScreenScaleW;
        static float                   m_fScreenScaleH;
        static float                   m_fScreenTx;
        static float                   m_fScreenTy;
        static bool                     s_useDCC2;
    };
}
#endif

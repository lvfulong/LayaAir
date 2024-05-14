
#ifndef __JCFileResDCC_H__
#define __JCFileResDCC_H__

#include "../../resource/JCFileResManager.h"

namespace laya{
	class JCFileResDCC : public JCFileRes
    {
    public:

        JCFileResDCC(JCDownloadMgr* pNetLoader, JCFileResManager* pMgr);	//只能由JCFileResManager创建
        ~JCFileResDCC();
        bool restoreRes() override;;
        //直接读取缓存的。如果不在缓存中，则返回false
        bool loadFromCache(JCBuffer& buff, bool bDoCheckSum) override;
        

        //设置不需要严格校验的文件。对于这些文件，如果校验失败了，则直接算作成功，并且保存下来
        static void addChkIgnoreChksumExt(const char* p_pszExt);
        static void clearChkIgnoreChksumExt();
        static bool checkIgnoreChksum(char* p_pszExt);

        /** @brief 加载资源。注意只允许调试或者manager调用。否则可能会同一个资源加载多次。
         *  @param[in] 
         *  @return 
        */
        void load(const char* p_pszURL, JCSharedBuffer* pSyncResult) override;
	protected:
		void onResDownloadOK_JSThread(std::weak_ptr<int> p_cbref);
        void onResDownloadOKDataEmpty_JSThread(std::weak_ptr<int> p_cbref);
		void onResDownloadErr_JSThread(std::weak_ptr<int> p_cbref,int p_nError, int p_nHttpResponse);

        void notifyErrorHandler(int p_nError, int p_nHttpResponse);

        /** @brief 下载完成的回调。
         *  @param[in] p_Buff 下载结果。
         *  @param[in] pLocalAddr 下载线程获得的本地地址。
         *  @param[in] pSvAddr 下载线程获得的服务器地址。
         *  @param[in] p_nChkSum 本次下载请求希望的校验值
         *  @param[in] p_nDownloadNum 这是本请求的第几次下载。因为校验错误的话会尝试多次下载。
         *  @param[in] p_cbref 保护用。
         *  @return void
        */
		void onDownloaded( JCBuffer& p_Buff , 
                const std::string& pLocalAddr, const std::string& pSvAddr, 
                int pnCurlRet, int pnHttpRet,
                const std::string& pstrHeader,
                unsigned int p_nChkSum, int p_nDownloadNum, std::weak_ptr<int> p_cbref );
        void onDownloadedOld(JCBuffer& p_Buff,
            const std::string& pLocalAddr, const std::string& pSvAddr,
            int pnCurlRet, int pnHttpRet,
            const std::string& pstrHeader,
            unsigned int p_nChkSum, int p_nDownloadNum, std::weak_ptr<int> p_cbref);

		void onDownloadError(int p_nError, int p_nHttpResponse,std::weak_ptr<int> p_cbref );
		int onProgress(unsigned int, unsigned int,float,std::weak_ptr<int> p_cbref );

		//带校验的下载。p_nChkSum可以为0，表示不检查
		void verifyDownload(const char* p_pszURL, unsigned int p_nChkSum );
		bool checkIsEncrypted(char *buf,int len);
        
        void normalizeUrl();
	public:
        Action                          m_nLastAction;  //测试用
		std::string						m_strURL;
  
		unsigned int					m_nLocalFileID;
        unsigned int                    m_nExtVersionID = 0;      //外部版本号。是一个hash后的值。
        bool                            m_bExtVersion = false;

		std::shared_ptr<int>			m_CallbackRef;
		static std::string	            s_strUploadChkErrUrl;	//上传校验错误的url，不需要则为空
        static std::string              s_strExtVersion;        //外部版本控制字符串。例如 ?v=1, 则为 v= 加上=是为了避免 v=2和vxx=2分不开
    
    
	protected:
        JCDownloadMgr*                  m_pNetLoader;
        JCFileResManager*	            m_pMgr;
		bool                            m_bDownloading;
		static std::vector<std::string>	s_vIgnoreChksumError;
		static bool						s_bHasIgnoreChksum;
		static std::mutex				s_ignorechklock;
        std::mutex                      m_CallbackLock;
		bool							m_bSendToJS_complete;	//完成事件已经post给js队列等待处理了。避免同一个对象多次post。必须都在js线程处理这个变量
        JCUrl                           m_Url;
	};
}

#endif
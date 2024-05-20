/**
 * 新的DCC管理
 * 可以被接管
 * 
*/

#ifndef __JCFILERESDCC2_H__
#define __JCFILERESDCC2_H__

#include "../../resource/JCFileResManager.h"
#include "DCCInterface.h"
#include "IDownloader.h"

namespace laya{

	class JCFileResDCC2 : public JCFileRes
    {
    private:
        std::string m_strURL;
        IDownloader* m_pDownloader;
        bool	m_bSendToJS_complete;	//完成事件已经post给js队列等待处理了。避免同一个对象多次post。必须都在js线程处理这个变量
        std::shared_ptr<int>	m_CallbackRef;

        
    private:
        int onProgress(unsigned int now, unsigned int total, float speed, std::weak_ptr<int> p_cbref);
        void onDownloadError(int p_nError, int p_nHttpResponse, std::weak_ptr<int> p_cbref);
        void onDownloaded(JCBuffer& p_Buff,
            const std::string& pLocalAddr, const std::string& pSvAddr,
            int pnCurlRet, int pnHttpRet,
            const std::string& pstrHeader,
            int p_nDownloadNum,
            const char* pszLocalPach,
            std::weak_ptr<int> p_cbref);
        void onResDownloadOK_JSThread(std::weak_ptr<int> p_cbref);
        void onResDownloadErr_JSThread(std::weak_ptr<int> p_cbref, int p_nError, int p_nHttpResponse);

    public:
        JCFileResDCC2();
        ~JCFileResDCC2();

        void setDownloader( IDownloader* downloader);
        bool restoreRes() override;
        bool loadFromCache(JCBuffer& buff, bool bDoCheckSum) override;

        /** @brief 加载资源。注意只允许调试或者manager调用。否则可能会同一个资源加载多次。
         *  @param[in] 
         *  @return 
        */
        void load(const char* p_pszURL, JCSharedBuffer* pSyncResult) override;

	};

}

#endif
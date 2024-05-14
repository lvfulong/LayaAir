
#ifndef __DIRECTDOWNLOADER_H__
#define __DIRECTDOWNLOADER_H__
#include <utils/JCBuffer.h>
#include "DCCInterface.h"
#include <string>

namespace laya{
    /**
     * 直接下载的接口，具体下载由更底层的实现
    */
    class DirectDownloader :public IDCCInterface{
    private:
        std::shared_ptr<int>			m_CallbackRef;
        int m_nOptTimeout;
        int m_nConnTimeout;
        std::string m_strURL;
    public:
        DirectDownloader();
        
        int onProgress(unsigned int now, unsigned int total, float speed, std::weak_ptr<int> p_cbref);
        void download(const char* pszUrl);

        void onDownloadError(int p_nError, int p_nHttpResponse, std::weak_ptr<int> p_cbref);
        void onDownloaded(
                JCBuffer& p_Buff,
                const std::string& pLocalAddr, 
                const std::string& pSvAddr,
                int pnCurlRet, 
                int pnHttpRet,
                const std::string& pstrHeader,
                int p_nDownloadNum, 
                std::weak_ptr<int> p_cbref);
    };
}
#endif
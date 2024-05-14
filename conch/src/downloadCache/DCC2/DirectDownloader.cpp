
#include "DirectDownloader.h"
#include <utils/Log.h>
#include <string>
#include <downloadMgr/JCDownloadMgr.h>
#include <functional>

namespace laya{

    DirectDownloader::DirectDownloader(){
        m_CallbackRef.reset(new int(1));
        m_nOptTimeout=0;
        m_nConnTimeout=0;
    }

    int DirectDownloader::onProgress(unsigned int now, unsigned int total, float speed, std::weak_ptr<int> p_cbref) {
        return 0;
    }    

    void DirectDownloader::download(const char* pszUrl){
        m_strURL = pszUrl;
        JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
        std::weak_ptr<int> wptr(m_CallbackRef);
        pNetLoader->download(pszUrl, 0,
            std::bind(&DirectDownloader::onProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, wptr),
            std::bind(&DirectDownloader::onDownloaded, this,
                std::placeholders::_1, 
                std::placeholders::_2, 
                std::placeholders::_3, 
                std::placeholders::_4,
                std::placeholders::_5,
                std::placeholders::_6,
                1, wptr), m_nOptTimeout, m_nConnTimeout);

    }

    void DirectDownloader::onDownloadError(int p_nError, int p_nHttpResponse, std::weak_ptr<int> p_cbref) {
    }

    void DirectDownloader::onDownloaded(
            JCBuffer& p_Buff,
            const std::string& pLocalAddr, 
            const std::string& pSvAddr,
            int pnCurlRet, 
            int pnHttpRet,
            const std::string& pstrHeader,
            int p_nDownloadNum, 
            std::weak_ptr<int> p_cbref) 
    {
                
        //用这种方式并不可靠，当多线程的时候，可能在这时候还有效，执行完这句就被删除了。
        if (!p_cbref.lock())
            return;
        LOGI("Downloaded %s@%s s=%x l=%d", m_strURL.c_str(), pSvAddr.c_str(), p_Buff.m_nLen);
        bool chkErr = false;
        if (pnCurlRet == 0/*CURLE_OK*/ && pnHttpRet >= 200 && pnHttpRet < 300) {
            // m_pBuffer = std::shared_ptr<char>(new char[p_Buff.m_nLen], std::default_delete<char[]>());
            // memcpy(m_pBuffer.get(), p_Buff.m_pPtr, p_Buff.m_nLen);
            // m_nLength = p_Buff.m_nLen;
            // std::weak_ptr<int> wptr(m_CallbackRef);
            // std::function<void()> cb = std::bind(&JCFileResDCC::onResDownloadOK_JSThread, this, wptr);
            // postToJS(cb);
            // goto end;
        }
        else {
            //error
            std::weak_ptr<int> wptr(m_CallbackRef);
            onDownloadError(pnCurlRet, pnHttpRet, wptr);
        }

        //转到js线程执行setState。
        // std::function<void()> cb = std::bind(&JCFileResDCC::onResDownloadOK_JSThread, this, wptr);
        // postToJS(cb);
    }    
}
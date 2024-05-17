
#include "JCFileResDCC2.h"
#include "../../downloadMgr/JCDownloadMgr.h"
#include "JCConch.h"
#include <utils/JCFileSystem.h>

namespace laya{
    JCFileResDCC2::JCFileResDCC2(){
        m_pDownloader=nullptr;
        m_bSendToJS_complete = false;
        m_CallbackRef.reset(new int(1));
    }
    JCFileResDCC2::~JCFileResDCC2(){
        m_pBuffer.reset((char*)0);
        m_CallbackRef.reset();        
    }

    int JCFileResDCC2::onProgress(unsigned int now, unsigned int total, float speed, std::weak_ptr<int> p_cbref) {
        return 0;
    }

    void JCFileResDCC2::onDownloadError(int p_nError, int p_nHttpResponse, std::weak_ptr<int> p_cbref) {
        *(int*)0=1;
    }

    void JCFileResDCC2::onDownloaded(JCBuffer& p_Buff,
            const std::string& pLocalAddr, const std::string& pSvAddr,
            int pnCurlRet, int pnHttpRet,
            const std::string& pstrHeader,
            int p_nDownloadNum, 
            const char* pszLocalPach, 
            std::weak_ptr<int> p_cbref) {

        m_pBuffer = std::shared_ptr<char>(new char[p_Buff.m_nLen], std::default_delete<char[]>());
        memcpy(m_pBuffer.get(), p_Buff.m_pPtr, p_Buff.m_nLen);
        m_nLength = p_Buff.m_nLen;
        m_strLocalPath = pszLocalPach?pszLocalPach:"";
        if (!m_bSendToJS_complete) {
            std::weak_ptr<int> wptr(m_CallbackRef);
            m_bSendToJS_complete = true;	//这里肯定是js线程，可以处理这个标志
            if (isInJSThread()) {
                //如果本身就在js线程，则立即做，这样可以节省一帧
                onResDownloadOK_JSThread(wptr);
            }
            else {
                std::function<void()> cb = std::bind(&JCFileResDCC2::onResDownloadOK_JSThread, this, wptr);
                postToJS(cb);
            }
        }
    }

    void JCFileResDCC2::onResDownloadOK_JSThread(std::weak_ptr<int> p_cbref) {
        if (!p_cbref.lock())
            return;
        if (m_nLength == 0)	//如果已经为0了，则表示已经处理的，状态改变已经通知给需要的人了，直接返回。
            return;
        //hugao add
        //checkIsEncrypted(m_pBuffer.get(), m_nLength);
        //if (gHandleDataFunc) {
        //    int nNewlen = m_nLength;
        //    char* pNewData = gHandleDataFunc(m_pBuffer.get(), nNewlen);
        //    if (pNewData) {
        //        m_nLength = nNewlen;
        //        m_pBuffer.reset(pNewData);
        //    }
        //}
        setState(ready);
        //立即失效。如果再有相同请求，需要重新加载
        m_pBuffer.reset((char*)0);	//TODO 测试：这个不一定会导致释放
        m_nLength = 0;
        setState(freed);
        m_bSendToJS_complete = false;	//处理完了，可以继续post了。
    }

    void JCFileResDCC2::setDownloader( IDownloader* downloader){
        m_pDownloader = downloader;
    }
    
    void JCFileResDCC2::load(const char* p_pszURL, JCSharedBuffer* pSyncResult){
        m_strURL = p_pszURL;
        std::weak_ptr<int> wptr(m_CallbackRef);
        if(m_pDownloader){
            //有人接管
            m_pDownloader->download(p_pszURL, 
                std::bind(&JCFileResDCC2::onDownloaded, this, std::placeholders::_1, "", "", 0, 0, "", 1, std::placeholders::_2, wptr)
            );
        }else{
            //直接下载
            JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
            pNetLoader->download(p_pszURL, 0, 
                std::bind(&JCFileResDCC2::onProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, wptr),
                std::bind(&JCFileResDCC2::onDownloaded, this,
                    std::placeholders::_1, 
                    std::placeholders::_2, 
                    std::placeholders::_3, 
                    std::placeholders::_4,
                    std::placeholders::_5,
                    std::placeholders::_6,
                    1, nullptr, wptr), m_nOptTimeout, m_nConnTimeout);
        }
    }

    bool JCFileResDCC2::loadFromCache(JCBuffer& buff, bool bDoCheckSum) {
        if(m_strLocalPath.length()>0){
            return readFileSync(m_strLocalPath.c_str(), buff);
        }
        return false;
    }

    bool JCFileResDCC2::restoreRes() {
        load(m_strURL.c_str(), nullptr);
        return true;
    }
}
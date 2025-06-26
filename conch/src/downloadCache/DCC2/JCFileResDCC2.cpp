
#include "JCFileResDCC2.h"
#include "../../downloadMgr/JCDownloadMgr.h"
#include "JCConch.h"
#include <resource/JCFileResManager.h>
#include <utils/JCFileSystem.h>
#include <core/Thread.h>
namespace laya
{
JCFileResDCC2::JCFileResDCC2(JCFileResManager *manager) : m_manager(manager)
{
    m_pDownloader = nullptr;
    m_bSendToJS_complete = false;
    m_CallbackRef.reset(new int(1));
}
JCFileResDCC2::~JCFileResDCC2()
{
    m_data.reset();
    m_CallbackRef.reset();
}

int JCFileResDCC2::onProgress(unsigned int now, unsigned int total, float speed, std::weak_ptr<int> p_cbref)
{
    return 0;
}

void JCFileResDCC2::onDownloadError(int p_nError, int p_nHttpResponse, std::weak_ptr<int> p_cbref)
{
    if (!p_cbref.lock())
        return;
    m_data.reset();

    if (!m_bIgnoreError)
    {
        LOGE("JCFileRes::onDownloadError file error[%d]:%s", p_nError, m_strURL.c_str());
        // errorflog("下载文件错误[%d]：%s",p_nError, m_strURL.c_str());
    }
    // m_pMgr->delRes(m_strURL.c_str());	到资源管理器中统一做
    // auto it = m_ResMap.find(m_strURL.c_str());
    std::weak_ptr<int> wptr(m_CallbackRef);
    if (isScriptThread())
    {
        onResDownloadErr_JSThread(wptr, p_nError, p_nHttpResponse);
    }
    else
    {
        // 转到js线程执行setState。
        std::function<void()> cb =
            std::bind(&JCFileResDCC2::onResDownloadErr_JSThread, this, wptr, p_nError, p_nHttpResponse);
        postToJS(cb);
    }
}

void JCFileResDCC2::onDownloaded(const std::shared_ptr<Data> &data, const std::string &pLocalAddr, const std::string &pSvAddr,
                                 int pnCurlRet, int pnHttpRet, const std::string &pstrHeader, int p_nDownloadNum,
                                 const char *pszLocalPach, std::weak_ptr<int> p_cbref)
{
#if 0
    if (p_Buff.m_nLen <= 0)
    {
        return onDownloadError(0, 0, p_cbref); // 不知道错误码
    }
#endif
    m_data = data;
    m_strLocalPath = pszLocalPach ? pszLocalPach : "";
    if (!m_bSendToJS_complete)
    {
        std::weak_ptr<int> wptr(m_CallbackRef);
        m_bSendToJS_complete = true; // 这里肯定是js线程，可以处理这个标志
        if (isScriptThread())
        {
            // 如果本身就在js线程，则立即做，这样可以节省一帧
            onResDownloadOK_JSThread(wptr);
        }
        else
        {
            std::function<void()> cb = std::bind(&JCFileResDCC2::onResDownloadOK_JSThread, this, wptr);
            postToJS(cb);
        }
    }
}

void JCFileResDCC2::onResDownloadOK_JSThread(std::weak_ptr<int> p_cbref)
{
    if (!p_cbref.lock())
        return;
#if 0
    if (m_nLength == 0) // 如果已经为0了，则表示已经处理的，状态改变已经通知给需要的人了，直接返回。
        return;
#endif
    // hugao add
    // checkIsEncrypted(m_pBuffer.get(), m_nLength);
    // if (gHandleDataFunc) {
    //     int nNewlen = m_nLength;
    //     char* pNewData = gHandleDataFunc(m_pBuffer.get(), nNewlen);
    //     if (pNewData) {
    //         m_nLength = nNewlen;
    //         m_pBuffer.reset(pNewData);
    //     }
    // }
    setState(ready);
    // 立即失效。如果再有相同请求，需要重新加载
    m_data.reset();
    setState(freed);
    m_bSendToJS_complete = false; // 处理完了，可以继续post了。
}

void JCFileResDCC2::onResDownloadErr_JSThread(std::weak_ptr<int> p_cbref, int p_nError, int p_nHttpResponse)
{
    if (!p_cbref.lock())
        return;
    // mnErrNo = downloadError;
    m_nErrNo = p_nError;
    m_nLastHttpResponse = p_nHttpResponse;
    setState(error);
    // 立即失效。如果再有相同请求，需要重新加载
    m_data.reset();
}

void JCFileResDCC2::setDownloader(IDownloader *downloader)
{
    m_pDownloader = downloader;
}

void JCFileResDCC2::load(const char *p_pszURL, JCSharedBuffer *pSyncResult)
{
    m_strURL = p_pszURL;
    std::weak_ptr<int> wptr(m_CallbackRef);
    if (m_url.m_nProto == JCUrl::wxblob)
    {
        int bytes;
        if (std::shared_ptr<Data> data = m_manager->searchBufferURL(m_strURL))
        {

			LOGI("found file local blob %s", m_strURL.c_str());
            onDownloaded(data, "", "", 0, 0, "", 0, "", wptr);
        }
        else
        {
            onDownloadError(0, 404, wptr); // lvtodo 错误码
        }
    }
    else
    {
        if (m_pDownloader)
        {
            // 有人接管
            m_pDownloader->download(p_pszURL, std::bind(&JCFileResDCC2::onDownloaded, this, std::placeholders::_1, "",
                                                        "", 0, 0, "", 1, std::placeholders::_2, wptr));
        }
        else
        {
            // 直接下载
            JCDownloadMgr *pNetLoader = JCDownloadMgr::getInstance();
            pNetLoader->download(p_pszURL, 0,
                                 std::bind(&JCFileResDCC2::onProgress, this, std::placeholders::_1,
                                           std::placeholders::_2, std::placeholders::_3, wptr),
                                 std::bind(&JCFileResDCC2::onDownloaded, this, std::placeholders::_1,
                                           std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
                                           std::placeholders::_5, std::placeholders::_6, 1, nullptr, wptr),
                                 m_nOptTimeout, m_nConnTimeout);
        }
    }
}

bool JCFileResDCC2::restoreRes()
{
    load(m_strURL.c_str(), nullptr);
    return true;
}
} // namespace laya

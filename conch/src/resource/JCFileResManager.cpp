/**
@file			JCFileResManager.cpp
@brief
@author			James
@version		1.0
@date			2016_5_11
*/

#include "JCFileResManager.h"
#include "../downloadCache/JCServerFileCache.h"
#include "../downloadCache/JCFileTable.h"
#include <utils/thread/JCWorkerThread.h>
#include <utils/JCCommonMethod.h>
#include <utils/Log.h>
#include <utils/JCLayaUrl.h>
#include <utils/JCFileSystem.h>
#include "JCScriptRuntime.h"
#include "JCConch.h"
#include <algorithm>
#include "../downloadMgr/JCDownloadMgr.h"
#include <utils/JCCrypto.h>
#include "../downloadMgr/JCHttpHeader.h"
#include "../JCSystemConfig.h"
namespace laya
{
    HandleFileData gHandleDataFunc = nullptr;

    char* _conch_mallocData(int len) {
        return new char[len];
    }

    void setFileDataHandler(HandleFileData h) {
        gHandleDataFunc = h;
    }
    char* tttt(const char* data, int& len) {
        //test setFileDataHandler
        char* pd = _conch_mallocData(len);
        memcpy(pd, data, len);
        return pd;
    }

    JCFileResDCC::JCFileResDCC(JCDownloadMgr* pNetLoader, JCFileResManager* pMgr)
    {
        m_nLastAction = INIT;
        m_nLength = 0;
        m_bDownloading = false;
        m_CallbackRef.reset(new int(1));
        m_bIgnoreError = false;
        m_bSendToJS_complete = false;
        m_pNetLoader = pNetLoader;
        m_pMgr = pMgr;
    }

    JCFileResDCC::~JCFileResDCC() {
        m_pBuffer.reset((char*)0);
        m_CallbackRef.reset();
    }

    void JCFileResDCC::normalizeUrl() {
        bool bToLower = m_pMgr ? m_pMgr->m_bUrlToLowerCase : false;
        if (m_Url.m_nProto == JCUrl::file) {
            std::string ret = m_Url.m_strPath + "/" + m_Url.m_vPath[m_Url.m_vPath.size() - 1];
            if (bToLower)
                UTF8ToLowercase((char*)ret.c_str());
            m_strURL = ret;
            return ;
        }
        std::string file = m_Url.m_vPath.size() > 0 ? m_Url.m_vPath[m_Url.m_vPath.size() - 1] : "";
        std::string attFile = file.length() > 0 ? ("/" + file) : "/";
        std::string& query = m_Url.m_Query;
        std::string attQuery = query.length() > 0 ? (query) : "";
        std::string ret = m_Url.m_strPath + attFile;
        if (bToLower)
            UTF8ToLowercase((char*)ret.c_str());
        ret += attQuery;
        m_strURL = ret;
    }

    void JCFileResDCC::load(const char* p_pszURL, JCSharedBuffer* pSyncResult) {
        if (!p_pszURL)
            return;
        std::weak_ptr<int> wptr(m_CallbackRef);
        m_Url.parse(p_pszURL);
        normalizeUrl();
        //bool bToLower = m_pMgr ? m_pMgr->m_bUrlToLowerCase : false;
        //m_strURL = normalizePath(p_pszURL, bToLower, nProto);
        //判断是否是file://
        if (m_Url.m_nProto == JCUrl::file) {
            std::weak_ptr<int> wptr(m_CallbackRef);
            const char* pPath = m_strURL.c_str() + strlen("file://");
            if (pPath[2] == ':') {
                pPath += 1;
            }
            JCBuffer buf;
            bool bret = readFileSync(pPath, buf);
            if (bret && buf.m_pPtr) {
                //如果本地打开成功，则可以立刻设置回调，因为是在一个线程
                //这么写可以做到立即回调，相当于同步函数。这样即使没有缓存也可以应付 JsAppCache::loadCachedURL 的要求。
                m_pBuffer.reset(new char[buf.m_nLen]);
                memcpy(m_pBuffer.get(), buf.m_pPtr, buf.m_nLen);
                m_nLength = buf.m_nLen;
                //hugao add
                checkIsEncrypted(m_pBuffer.get(), m_nLength);
                if (gHandleDataFunc) {
                    int nNewlen = m_nLength;
                    char* pNewData = gHandleDataFunc(m_pBuffer.get(), nNewlen);
                    if (pNewData ) {
                        m_nLength = nNewlen;
                        m_pBuffer.reset(pNewData);
                    }
                }
                setState(ready);
                if (pSyncResult) {
                    pSyncResult->m_pBuffer = m_pBuffer;
                    pSyncResult->m_nLen = m_nLength;
                }
                //立即失效。如果再有相同请求，需要重新加载
                m_pBuffer.reset((char*)0);	//TODO 测试：这个不一定会导致释放
                m_nLength = 0;
                setState(freed);
            }
            else {
                onDownloadError(1, 0, wptr);
            }
            return;
        }

        //再尝试从缓存中取
        JCSharedBuffer buff;
        JCServerFileCache* pSvFileCache = m_pMgr->m_pFileCache;
        //if (pSvFileCache) {
        //    std::string strQuery = m_Url.m_Query;
        //    m_nLocalFileID = pSvFileCache->getFileID(m_strURL.c_str());// hashURLFull(m_strURL.c_str());
        //    pSvFileCache->getFileInfo(m_nLocalFileID, chksum);
        //    needDownload = !pSvFileCache->load(m_nLocalFileID, chksum, buff,false,true);
        //}
        //如果有dcc接口，加载dcc
        if (false) {

        }
        else {
            if (m_bDownloading)
                return;

            //注意不能是p_pszURL。因为不可靠。如果是p_pszURL就是m_strURL获得的，则在 m_strURL = normalizePath
            //的时候，会导致p_pszURL被释放。
            m_bDownloading = true;
            verifyDownload(m_strURL.c_str());
        }

    //loadok:
            //m_nLastAction = LOADFROMCACHE;
            ////如果本地打开成功，则可以立刻设置回调，因为是在一个线程
            //m_pBuffer = buff.m_pBuffer;
            //m_nLength = buff.m_nLen;
            //if (pSyncResult) {
            //    pSyncResult->m_pBuffer = buff.m_pBuffer;
            //    pSyncResult->m_nLen = m_nLength;
            //}
            ////转到js线程执行setState。不要同步了，保持与浏览器一致。
            //if (!m_bSendToJS_complete) {
            //    m_bSendToJS_complete = true;	//这里肯定是js线程，可以处理这个标志
            //    std::function<void()> cb = std::bind(&JCFileResDCC::onResDownloadOK_JSThread, this, wptr);
            //    postToJS(cb);
            //}
    }

    bool JCFileResDCC::loadFromCache(JCBuffer& buff, bool bDoCheckSum) {
        //TODO
        *((int*)0) = 1; //制造异常
        return false;

        //JCUrl url;
        //url.parse(m_strURL.c_str());
        //if (url.m_nProto == JCUrl::file) {
        //    char* pFile = (char*)m_strURL.c_str();
        //    if (strstr(m_strURL.c_str(), "file:///") == pFile) {
        //        if (pFile[9] == ':') {
        //            pFile += 8;
        //        }
        //        else {
        //            pFile += 7;
        //        }
        //    }
        //    return readFileSync(pFile, buff);
        //}

        //if (m_bDownloading)
        //    return false;
        //if (!m_pMgr || !m_pMgr->m_pFileCache)
        //    return false;
        //if (bDoCheckSum && m_bExtVersion) {
        //    JCSharedBuffer _buff;
        //    JCCachedFileSys::typeChkSum tcs = (JCCachedFileSys::typeChkSum)m_nExtVersionID;
        //    if (m_pMgr->m_pFileCache->load(m_nLocalFileID, tcs, _buff, true, true)) {
        //        if (_buff.m_pBuffer.get()) {
        //            buff.create(_buff.m_nLen);
        //            memcpy(buff.m_pPtr, _buff.m_pBuffer.get(), _buff.m_nLen);
        //            return true;
        //        }
        //    }
        //    return false;
        //}
        //return m_pMgr->m_pFileCache->load(m_nLocalFileID, buff, bDoCheckSum);
    }

    bool JCFileResDCC::checkIsEncrypted(char *buf, int len){
        bool temp = JCEncrypt::decrypt(buf, len);
        if (temp)m_nLength = len - JCEncrypt::s_nPreLen;
        return temp;
    }

    void JCFileResDCC::verifyDownload(const char* p_pszURL) {
        m_nLastAction = DOWNLOADING;
        JCDownloadMgr* pNetLoader = m_pNetLoader;
        std::weak_ptr<int> wptr(m_CallbackRef);
        pNetLoader->download(p_pszURL, 0,
            std::bind(&JCFileResDCC::onProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, wptr),
            std::bind(&JCFileResDCC::onDownloaded, this,
                std::placeholders::_1, 
                std::placeholders::_2, 
                std::placeholders::_3, 
                std::placeholders::_4,
                std::placeholders::_5,
                std::placeholders::_6,
                1, wptr), m_nOptTimeout, m_nConnTimeout);
    }

    void JCFileResDCC::onResDownloadOK_JSThread(std::weak_ptr<int> p_cbref) {
        if (!p_cbref.lock())
            return;
        if (m_nLength == 0)	//如果已经为0了，则表示已经处理的，状态改变已经通知给需要的人了，直接返回。
            return;
        //hugao add
        checkIsEncrypted(m_pBuffer.get(), m_nLength);
        if (gHandleDataFunc) {
            int nNewlen = m_nLength;
            char* pNewData = gHandleDataFunc(m_pBuffer.get(), nNewlen);
            if (pNewData) {
                m_nLength = nNewlen;
                m_pBuffer.reset(pNewData);
            }
        }
        setState(ready);
        //立即失效。如果再有相同请求，需要重新加载
        m_pBuffer.reset((char*)0);	//TODO 测试：这个不一定会导致释放
        m_nLength = 0;
        setState(freed);
        m_bSendToJS_complete = false;	//处理完了，可以继续post了。
    }
    void JCFileResDCC::onResDownloadOKDataEmpty_JSThread(std::weak_ptr<int> p_cbref) {
        if (!p_cbref.lock())
            return;
        //if (m_nLength == 0)	//如果已经为0了，则表示已经处理的，状态改变已经通知给需要的人了，直接返回。
        //    return;
        //hugao add
        /*checkIsEncrypted(m_pBuffer.get(), m_nLength);
        if (gHandleDataFunc) {
            int nNewlen = m_nLength;
            char* pNewData = gHandleDataFunc(m_pBuffer.get(), nNewlen);
            if (pNewData) {
                m_nLength = nNewlen;
                m_pBuffer.reset(pNewData);
            }
        }*/
        setState(ready);
        //立即失效。如果再有相同请求，需要重新加载
        m_pBuffer.reset((char*)0);	//TODO 测试：这个不一定会导致释放
        m_nLength = 0;
        setState(freed);
        m_bSendToJS_complete = false;	//处理完了，可以继续post了。
    }
    void JCFileResDCC::notifyErrorHandler(int p_nError, int p_nHttpResponse) {
        if (!m_bIgnoreError) {
        }
    }

    void JCFileResDCC::onResDownloadErr_JSThread(std::weak_ptr<int> p_cbref, int p_nError, int p_nHttpResponse) {
        if (!p_cbref.lock())
            return;
        //mnErrNo = downloadError;
        m_bDownloading = false;
        m_nErrNo = p_nError;
        m_nLastHttpResponse = p_nHttpResponse;
        setState(error);
        //立即失效。如果再有相同请求，需要重新加载
        m_pBuffer.reset((char*)0);	//TODO 测试：这个不一定会导致释放
        m_nLength = 0;
        //setState(freed);	不应该再设置成freed，应该保留error，否则再次设置 setOnErrorCB的话
    }

    inline bool _MaybeAnImg(const char* pBuff, int nLen) {
        if (pBuff && nLen > 4) {
            static int jpegID = 0x00ffd8ff;	//最高位00那个位置不一定是什么
            static int gifID = 0x38464947;
            static int pngID = 0x474e5089;

            int idval = *(int*)pBuff;
            return (idval == pngID || idval == gifID || (idval & 0xffffff) == jpegID);
        }
        return false;
    }

    inline bool _StrInVec(const char* pStr, std::vector<std::string>& vec) {
        for (int ei = 0, sz = vec.size(); ei < sz; ei++) {
            if (vec[ei] == pStr) {
                return true;
            }
        }
        return false;
    }

    void JCFileResDCC::onDownloaded(JCBuffer& p_Buff,
        const std::string& pLocalAddr, const std::string& pSvAddr,
        int pnCurlRet, int pnHttpRet,
        const std::string& pstrHeader,
        int p_nDownloadNum, std::weak_ptr<int> p_cbref) {
        //用这种方式并不可靠，当多线程的时候，可能在这时候还有效，执行完这句就被删除了。
        if (!p_cbref.lock())
            return;
        m_nLastAction = DOWNLOADED;
        m_strSvIP = pSvAddr;
        //LOGI("Downloaded %s@%s s=%x l=%d", m_strURL.c_str(), pSvAddr.c_str(), p_nChkSum, p_Buff.m_nLen);
        bool chkErr = false;
        JCServerFileCache* pSvFileCache = nullptr;
        if (pnCurlRet == 0/*CURLE_OK*/ && pnHttpRet >= 200 && pnHttpRet < 300) {
            //如果什么都没有返回，则不用继续处理了
            if (p_Buff.m_pPtr == NULL || p_Buff.m_nLen == 0)
            {
                //goto end;
                //fix bug 数据空 JS没回调
                std::weak_ptr<int> wptr(m_CallbackRef);
                std::function<void()> cb = std::bind(&JCFileResDCC::onResDownloadOKDataEmpty_JSThread, this, wptr);
                postToJS(cb);
            }
            //转到js线程执行setState。
            m_pBuffer = std::shared_ptr<char>(new char[p_Buff.m_nLen], std::default_delete<char[]>());
            memcpy(m_pBuffer.get(), p_Buff.m_pPtr, p_Buff.m_nLen);
            m_nLength = p_Buff.m_nLen;
            std::weak_ptr<int> wptr(m_CallbackRef);
            std::function<void()> cb = std::bind(&JCFileResDCC::onResDownloadOK_JSThread, this, wptr);
            postToJS(cb);
        }
        else {
            //error
            std::weak_ptr<int> wptr(m_CallbackRef);
            onDownloadError(pnCurlRet, pnHttpRet, wptr);

        }
        m_bDownloading = false;
    }


    bool JCFileResDCC::restoreRes() {
        load(m_strURL.c_str(),nullptr);
        return true;
    }

    void JCFileResDCC::onDownloadError(int p_nError, int p_nHttpResponse, std::weak_ptr<int> p_cbref) {
        //std::lock_guard<std::mutex> lock(m_pMgr->m_maplock);//要删除map，可能会与请求的时候冲突
        if (!p_cbref.lock())
            return;
        m_pBuffer.reset((char*)0);
        m_nLength = 0;

        if (!m_bIgnoreError) {
            LOGE("JCFileRes::onDownloadError file error[%d]:%s", p_nError, m_strURL.c_str());
            //errorflog("下载文件错误[%d]：%s",p_nError, m_strURL.c_str());
        }
        //m_pMgr->delRes(m_strURL.c_str());	到资源管理器中统一做
        //auto it = m_ResMap.find(m_strURL.c_str());
        notifyErrorHandler(p_nError, p_nHttpResponse);
        std::weak_ptr<int> wptr(m_CallbackRef);
        //转到js线程执行setState。
        //if (m_pMgr->m_pCmdPoster) {
            std::function<void()> cb = std::bind(&JCFileResDCC::onResDownloadErr_JSThread, this, wptr, p_nError, p_nHttpResponse);
            postToJS(cb);
        //}
        //else {
        //    onResDownloadErr_JSThread(wptr, p_nError, p_nHttpResponse);
        //}
    }

    int JCFileResDCC::onProgress(unsigned int now, unsigned int total, float speed, std::weak_ptr<int> p_cbref) {
        return 0;
    }

    JCFileResManager::JCFileResManager(JCDownloadMgr* pDownloadMgr) {
        m_pFileCache = nullptr;
        m_bUrlToLowerCase = false;

		m_vExtNeedSave = { ".png",".jpg",".wav",".ogg" };
	
        m_pDownloadMgr = pDownloadMgr;
    }

    JCFileResManager::~JCFileResManager() {
        clear();
    }

    void JCFileResManager::clear() {
        std::lock_guard<std::mutex> lock(m_maplock);
        FileResMap::iterator it = m_ResMap.begin();
        FileResMap::iterator ed = m_ResMap.end();
        while (it != ed) {
            delete (*it).second;
            it++;
        }
        m_ResMap.clear();
    }

    //如果已经下载完了，应该同步去打开文件，不要再走下载了（在load中做了）
    JCFileRes* JCFileResManager::getRes(const std::string& url, int p_nConnTimeout , int p_nOptTimeout) {
        std::lock_guard<std::mutex> lock(m_maplock);//下面要添加资源，需要锁。因为失败的话会删除
        JCFileRes* pRes = NULL;
        FileResMap::iterator it = m_ResMap.find(url);
        if (it == m_ResMap.end()) {

			pRes = new JCFileResDCC(m_pDownloadMgr,this);

            if (p_nConnTimeout>0) {
                pRes->m_nConnTimeout = p_nConnTimeout;
            }
            if (p_nOptTimeout > 0) {
                pRes->m_nOptTimeout = p_nOptTimeout;
            }
            m_ResMap[url] = pRes;
            //问题：load和下载回调不在一个线程会有问题么？
            // 如果load不修改表和文件内容，下载回调在最后再设置ready，应该没事
            pRes->load(url.c_str(),nullptr);
            //m_pThread = workerThread::getCurThread();
            return pRes;
        }
        pRes = (*it).second;
        if (p_nConnTimeout>0) {
            pRes->m_nConnTimeout = p_nConnTimeout;
        }
        if (p_nOptTimeout > 0) {
            pRes->m_nOptTimeout = p_nOptTimeout;
        }

        return pRes;
    }



    bool JCFileResManager::delRes(const char* p_pszURL) {
        std::lock_guard<std::mutex> lock(m_maplock);
        auto it = m_ResMap.find(p_pszURL);
        if (it != m_ResMap.end()) {
            m_ResMap.erase(it);
            return true;
        }
        return false;
    }
	void JCFileResManager::createBufferURL(const std::string& url, const char* data, int bytes)
	{
		char* buffer = new char[bytes];
		memcpy(buffer, data, bytes);
		m_BufferURLMap.insert(std::make_pair(url, new JCBuffer((void*)buffer, bytes, true, true)));
	}
	void JCFileResManager::revokeBufferURL(const std::string& url)
	{
		std::map<std::string, JCBuffer*>::iterator it = m_BufferURLMap.find(url);
		if (it != m_BufferURLMap.end())
		{
			delete it->second;
			m_BufferURLMap.erase(it);
		}
	}
	bool JCFileResManager::searchBufferURL(const std::string& url, char** data, int& bytes)
	{
		std::map<std::string, JCBuffer*>::iterator it = m_BufferURLMap.find(url);
		if (it != m_BufferURLMap.end())
		{
			*data = it->second->m_pPtr;
			bytes = it->second->m_nLen;
			return true;
		}
		return false;
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------.

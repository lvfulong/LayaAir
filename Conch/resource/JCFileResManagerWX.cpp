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
#include <Utils/Thread/JCWorkerThread.h>
#include <Utils/JCCommonMethod.h>
#include <Utils/Log.h>
#include <Utils/JCLayaUrl.h>
#include <Utils/JCFileSystem.h>
#include "JCScriptRuntime.h"
#include "JCConch.h"
#include <algorithm>
#include "../downloadMgr/JCDownloadMgr.h"
#include <Utils/JCCrypto.h>
#include "../downloadMgr/JCHttpHeader.h"
#include "../zip/JCZip.h"
extern std::string LAYA_NATIVE_FILE_CACHE_USR_PATH;
extern std::string LAYA_NATIVE_FILE_CACHE_ROOT_PATH;
extern std::string LAYA_NATIVE_FILE_CACHE_TMP_PATH;
extern laya::JCZip *g_ZipPackage;
namespace laya
{
    //------------------------------------------------------------------------------
    JCFileResWX::JCFileResWX(JCDownloadMgr* pNetLoader, JCFileResManager* pMgr)
    {
        m_nLastAction = INIT;
        m_nLength = 0;
        m_nLocalFileID = 0;
        m_bDownloading = false;
        m_CallbackRef.reset(new int(1));
        m_bIgnoreError = false;
        m_bSendToJS_complete = false;
        m_pNetLoader = pNetLoader;
        m_pMgr = pMgr;
    }

    JCFileResWX::~JCFileResWX() 
    {
        m_pBuffer.reset((char*)0);
        m_CallbackRef.reset();
    }

    void JCFileResWX::load(const char* p_pszURL, JCSharedBuffer* pSyncResult)
    {
        if (!p_pszURL)
            return;
        std::weak_ptr<int> wptr(m_CallbackRef);
        m_Url.parse(p_pszURL);
        m_strURL = p_pszURL;

        // fileconfig.json-->JCUrl::file  flow/inPlatform.json-->m_Url.m_nProto == JCUrl::unk TODO
        if (m_Url.m_nProto == JCUrl::wxfile || m_Url.m_nProto == JCUrl::unk || m_Url.m_nProto == JCUrl::file || m_Url.m_nProto == JCUrl::wxblob)
        {  
			JCBuffer buf;
			bool bret = true;
            std::weak_ptr<int> wptr(m_CallbackRef);

			if (m_Url.m_nProto == JCUrl::wxfile)
			{
				const char* pPath = m_strURL.c_str() + strlen("wxfile://");
				if (pPath[2] == ':')
				{
					pPath += 1;
				}

				std::string realPath = LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + pPath;
				bret = readFileSync(realPath.c_str(), buf);
			}
			else if (m_Url.m_nProto == JCUrl::wxblob)
			{
				int bytes;
				bret = m_pMgr->searchBufferURL(m_strURL, &buf.m_pPtr, bytes);
				buf.m_nLen = bytes;
				if (bret)
				{
					LOGI("found file local blob %s", m_strURL.c_str());
				}
				else
				{
					LOGI("not found file local blob %s", m_strURL.c_str());
				}
			}
			else
			{

				if (!g_ZipPackage)
				{
					bret = false;
				}
				else
				{
					char* strTrimedURL = (char*)m_strURL.c_str();
					while (*strTrimedURL == '/')
					{
						strTrimedURL++;
					}
					bret = g_ZipPackage->readFileByName(strTrimedURL, buf);
					if (bret)
					{
						LOGI("found file local package %s", strTrimedURL);
					}
					else
					{
						LOGI("not found file local package %s", strTrimedURL);
					}
				}
			}
            if (bret && buf.m_pPtr) 
            {
                LOGI("found file local %s", m_strURL.c_str());

                m_nLastAction = LOADFROMCACHE;

                m_pBuffer.reset(new char[buf.m_nLen]);
                memcpy(m_pBuffer.get(), buf.m_pPtr, buf.m_nLen);
                m_nLength = buf.m_nLen;

                //转到js线程执行setState。不要同步了，保持与浏览器一致。
                //if (m_pMgr->m_pCmdPoster)
                {
                    if (!m_bSendToJS_complete)
                    {
                        m_bSendToJS_complete = true;	//这里肯定是js线程，可以处理这个标志
                        std::function<void()> cb = std::bind(&JCFileResWX::onResDownloadOK_JSThread, this, wptr);
                        postToJS(cb);
                    }
                }
                //else
                {
                    //onResDownloadOK_JSThread(wptr);
                }
            }
            else 
            {
                onResDownloadOK_JSThread(wptr);
            }
            return;
        }
        else
        {
			JCMD5 md5;
			md5.GenerateMD5((unsigned char *)m_strURL.c_str(), m_strURL.length());
			m_strLocalTempCachePath = LAYA_NATIVE_FILE_CACHE_TMP_PATH + "/" + md5.ToString() + getLowercaseExtOfUrl(m_strURL.c_str());
            if (m_bDownloading)
                return;
            m_bDownloading = true;
            verifyDownload(m_strURL.c_str(), 0);//TODO
        }
    }

    bool JCFileResWX::loadFromCache(JCBuffer& buff, bool bDoCheckSum) {
        JCUrl url;
        url.parse(m_strURL.c_str());
        if (url.m_nProto == JCUrl::wxfile)
        {
            char* pFile = (char*)m_strURL.c_str();
            if (strstr(m_strURL.c_str(), "wxfile://") == pFile)
            {
                pFile += 9;
            }
            return readFileSync((LAYA_NATIVE_FILE_CACHE_ROOT_PATH + "/" + pFile).c_str(), buff);
        }
		else if (url.m_nProto == JCUrl::wxblob)
		{
			int bytes;
			bool bret = m_pMgr->searchBufferURL(m_strURL, &buff.m_pPtr, bytes);
			buff.m_nLen = bytes;
			if (bret)
			{
				LOGI("found file local blob %s", m_strURL.c_str());
				return true;
			}
			else
			{
				LOGI("not found file local blob %s", m_strURL.c_str());
				return false;
			}
		}
		else if (m_Url.m_nProto == JCUrl::unk || m_Url.m_nProto == JCUrl::file)
		{
			if (!g_ZipPackage)
			{
				return false;
			}
			else
			{
				if (!g_ZipPackage->exists(m_strURL.c_str()))
				{
					return false;
				}
				return g_ZipPackage->readFileByName(m_strURL.c_str(), buff);
			}
		}
		

        if (m_bDownloading)
            return false;
        if (!m_pMgr)
            return false;
        return readFileSync(m_strLocalTempCachePath.c_str(), buff);
    }

    void JCFileResWX::verifyDownload(const char* p_pszURL, unsigned int p_nChkSum) {
        m_nLastAction = DOWNLOADING;
        JCDownloadMgr* pNetLoader = m_pNetLoader;
        std::weak_ptr<int> wptr(m_CallbackRef);
        pNetLoader->download(p_pszURL, 0,
            std::bind(&JCFileResWX::onProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, wptr),
            std::bind(&JCFileResWX::onDownloaded, this,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3,
                std::placeholders::_4,
                std::placeholders::_5,
                std::placeholders::_6,
                p_nChkSum, 1, wptr), m_nOptTimeout, m_nConnTimeout);
    }

    void JCFileResWX::onResDownloadOK_JSThread(std::weak_ptr<int> p_cbref) {
        if (!p_cbref.lock())
            return;
        //if (m_nLength == 0)	//如果已经为0了，则表示已经处理的，状态改变已经通知给需要的人了，直接返回。
        //    return;
        setState(ready);
        //立即失效。如果再有相同请求，需要重新加载
        m_pBuffer.reset((char*)0);	//TODO 测试：这个不一定会导致释放
        m_nLength = 0;
        setState(freed);
        m_bSendToJS_complete = false;	//处理完了，可以继续post了。
    }

    void JCFileResWX::notifyErrorHandler(int p_nError, int p_nHttpResponse) {
        if (!m_bIgnoreError) {
        }
    }

    void JCFileResWX::onResDownloadErr_JSThread(std::weak_ptr<int> p_cbref, int p_nError, int p_nHttpResponse) {
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
    void JCFileResWX::onDownloaded(JCBuffer& p_Buff,
        const std::string& pLocalAddr, const std::string& pSvAddr,
        int pnCurlRet, int pnHttpRet,
        const std::string& pstrHeader,
        unsigned int p_nChkSum,
        int p_nDownloadNum, std::weak_ptr<int> p_cbref) {
        //用这种方式并不可靠，当多线程的时候，可能在这时候还有效，执行完这句就被删除了。
        if (!p_cbref.lock())
            return;
        m_nLastAction = DOWNLOADED;
        m_strSvIP = pSvAddr;
        LOGI("Downloaded %s@%s l=%d", m_strURL.c_str(), pSvAddr.c_str(), p_Buff.m_nLen);
        if (pnCurlRet == 0/*CURLE_OK*/ && pnHttpRet >= 200 && pnHttpRet < 300)
        {
            //如果什么都没有返回，则不用继续处理了
            /*if (p_Buff.m_pPtr == NULL || p_Buff.m_nLen == 0)
            {
                m_bDownloading = false;
                return;
            }*/

            //update cache
            std::string ext = getLowercaseExtOfUrl(m_strURL.c_str());
            m_pBuffer = std::shared_ptr<char>(new char[p_Buff.m_nLen], std::default_delete<char[]>());
            memcpy(m_pBuffer.get(), p_Buff.m_pPtr, p_Buff.m_nLen);
            m_nLength = p_Buff.m_nLen;
            std::weak_ptr<int> wptr(m_CallbackRef);

            if (_MaybeAnImg(p_Buff.m_pPtr, p_Buff.m_nLen) || _StrInVec(ext.c_str(), m_pMgr->m_vExtNeedSave))
            {
                if (!writeFileSync1(m_strLocalTempCachePath.c_str(), m_pBuffer.get(), p_Buff.m_nLen, 0))
                {
                    LOGE("JCFileRes::JCFileRes write cache failed %s %s", m_strURL.c_str(), m_strLocalTempCachePath.c_str());
                }
            }

            m_nLastAction = UPDATECACHE;

            //转到js线程执行setState。
            //if (m_pMgr->m_pCmdPoster)
            {
                std::function<void()> cb = std::bind(&JCFileResWX::onResDownloadOK_JSThread, this, wptr);
                postToJS(cb);
            }
            //else
            {
               // onResDownloadOK_JSThread(wptr);
            } 
            m_bDownloading = false;
        }
        else 
        {
            //error
            std::weak_ptr<int> wptr(m_CallbackRef);
            onDownloadError(pnCurlRet, pnHttpRet, wptr);
            m_bDownloading = false;
        }
    }
    bool JCFileResWX::restoreRes() {
        load(m_strURL.c_str(), nullptr);
        return true;
    }
    void JCFileResWX::onDownloadError(int p_nError, int p_nHttpResponse, std::weak_ptr<int> p_cbref) {
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
            std::function<void()> cb = std::bind(&JCFileResWX::onResDownloadErr_JSThread, this, wptr, p_nError, p_nHttpResponse);
            postToJS(cb);
        //}
        //else {
        //    onResDownloadErr_JSThread(wptr, p_nError, p_nHttpResponse);
        //}
    }

    int JCFileResWX::onProgress(unsigned int now, unsigned int total, float speed, std::weak_ptr<int> p_cbref) {
        return 0;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------.

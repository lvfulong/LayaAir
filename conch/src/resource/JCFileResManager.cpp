/**
@file			JCFileResManager.cpp
@brief
@author			James
@version		1.0
@date			2016_5_11
*/

#include "JCFileResManager.h"
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
#include "../../downloadCache/DCC1/JCFileResDCC.h"
#include "../../downloadCache/DCC2/JCFileResDCC2.h"
#include <Bindings/JSConchConfig.h>

extern std::string gRedistPath;
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

    JCFileResManager::JCFileResManager(JCDownloadMgr* pDownloadMgr) {
        m_pFileCache = nullptr;
        m_bUrlToLowerCase = false;

		m_vExtNeedSave = { ".png",".jpg",".wav",".ogg" };
	
        m_pDownloadMgr = pDownloadMgr;
        m_pDownloader = nullptr;
    }

    JCFileResManager::~JCFileResManager() {
        clear();
        m_pDownloader = nullptr;
    }

    std::string JCFileResManager::getAppCachePath() {
        return gRedistPath + "/appCache";
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

            if(JSConchConfig::s_useDCC2){
                auto dcc2 = new JCFileResDCC2();
                dcc2->setDownloader(m_pDownloader.get());
                pRes = dcc2;
                m_ResMap[url] = pRes;
                pRes->load(url.c_str(), nullptr);
                return pRes;
            }else{
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

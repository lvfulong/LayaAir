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
#include "../../downloadCache/DCC2/JCFileResDCC2.h"
#include <Bindings/JSConchConfig.h>

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
            auto dcc2 = new JCFileResDCC2(this);
            dcc2->setDownloader(m_pDownloader.get());
            pRes = dcc2;
            m_ResMap[url] = pRes;
            pRes->load(url.c_str(), nullptr);
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
        std::shared_ptr<Data> d = Data::makeWithCopy((void*)data, bytes);
		m_BufferURLMap.insert(std::make_pair(url, d));
	}
	void JCFileResManager::revokeBufferURL(const std::string& url)
	{
		auto it = m_BufferURLMap.find(url);
		if (it != m_BufferURLMap.end())
		{
			m_BufferURLMap.erase(it);
		}
	}
	std::shared_ptr<Data> JCFileResManager::searchBufferURL(const std::string& url)
	{
		auto it = m_BufferURLMap.find(url);
		if (it != m_BufferURLMap.end())
		{
            return it->second;
		}
		return nullptr;
	}

}

#ifndef __JCFileResManager_H__
#define __JCFileResManager_H__


#include "JCResource.h"
#include <map>
#include <string>
#include <memory>
#include <utils/JCBuffer.h>
#include <mutex>
#include <utils/JCLayaUrl.h>
#include "../downloadCache/DCC2/IDownloader.h"

#define MAXDOWNLOADTRY 3

namespace laya{
    /**
     * @param [in] data {const char*} 原始文件内容。
     * @param [in out] len {int&} 原始文件内容的长度。
     * @return char* 如果改变了数据，就返回新的指针。
    */
    typedef char* (*HandleFileData)(const char* data, int& len);

    /**@brief 设置一个函数，可以用来修改下载的文件内容。一般用来解密。
    */
    void setFileDataHandler(HandleFileData h);

    /**
     * @brief 其实就是一个new，怕万一跨so的话有分配的问题。
    */
    char* _conch_mallocData(int len);

	class JCFileResManager;
	class JCServerFileCache;
    class JCDownloadMgr;
    class IConchThreadCmdMgr;
    //文件资源
	//例如避免下载的时候重复下载之类的
	//由于downloadManager已经缓存了，因此本身不缓存。
    class JCFileRes : public JCResStateDispatcher
    {		
        friend class JCFileResManager;
    public:
        //最后的行为。测试用
        enum Action {
            INIT,
            REQUEST,
            LOADFROMCACHE,
            DOWNLOADING,
            DOWNLOADED,
            UPDATECACHE,
        };
        virtual void load(const char* p_pszURL, JCSharedBuffer* pSyncResult) = 0;
        void setIgnoreError(bool b) { m_bIgnoreError = b; };
        int m_nConnTimeout = 0;
        int m_nOptTimeout = 0;
        std::shared_ptr<char> m_pBuffer;	            //注意必须指定deleter std::default_delete<char[]>
        int m_nLength;
        bool m_bIgnoreError = false;			//是否忽略下载错误
        std::string m_strSvIP;      //调试用
        int m_nLastHttpResponse = 0;
        //记录本地缓存路径，以便直接加载
        std::string m_strLocalPath;

    };

	class JCFileResManager{
	public:
		typedef std::map<std::string, JCFileRes*>	FileResMap;

		JCFileResManager(JCDownloadMgr* pDownloadMgr);

		~JCFileResManager();

		void setFileCache( JCServerFileCache* p_pFileCache){
			m_pFileCache = p_pFileCache;
		}
        JCFileRes* getRes(const std::string& url,int p_nConnTimeout=0,int p_nOptTimeout=0);
        bool delRes(const char* p_pszURL);
        void clear();
		void createBufferURL(const std::string& url, const char* data, int bytes);
		void revokeBufferURL(const std::string& url);
		bool searchBufferURL(const std::string& url, char** data, int& bytes);
	public:
		JCServerFileCache*		    m_pFileCache;
		bool			            m_bUrlToLowerCase;
		std::mutex		        m_maplock;
        std::vector<std::string>    m_vExtNeedSave;     //这种扩展名的需要保存到临时缓存中。
        std::shared_ptr<IDownloader> m_pDownloader;
    protected:
        JCDownloadMgr*              m_pDownloadMgr;
        FileResMap			        m_ResMap;
		std::map<std::string, JCBuffer*> m_BufferURLMap;
	};
}
//------------------------------------------------------------------------------


#endif //__JCFileResManager_H__

/**
@file			JCZip.h
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

#ifndef __JCZip_H__
#define __JCZip_H__


//包含头文件
//------------------------------------------------------------------------------
#include <Utils/JCBuffer.h>

namespace laya 
{
	class JCZip
    {
	public:
		JCZip();
		~JCZip();
		bool openZip(const char* p_pszFile);
		bool setSrc(const char* p_pSrc);
		bool readFile(int idx, JCBuffer& p_buf);
		bool readFileAsText(int idx, JCBuffer& p_buf);
		bool readFileByName(const char* pName, JCBuffer& p_buf);
		bool readFileAsTextByName(const char* pName, JCBuffer& p_buf);
        bool exists(const char* pName);
		void close();
	private:
		friend class JSZip;
		void*	m_pZip;
        void*   m_pZipSource;
	};
}
//------------------------------------------------------------------------------


#endif //__JCZip_H__

//-----------------------------END FILE--------------------------------
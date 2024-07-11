#ifndef __JCIosFileSource_H__
#define __JCIosFileSource_H__

#include <stdio.h>
#include <utils/JCFileSource.h>
#include <utils/JCCommonMethod.h>
#include <utils/Log.h>
#include <string.h>
#pragma warning (disable: 4996)

namespace laya
{
	class JCIosFileSource : public JCFileSource
	{
	public:
        JCIosFileSource()
		{
		}
		~JCIosFileSource()
		{
		}
		bool Init( const char* sRootPath )
		{
			if( sRootPath )
			{
                m_sRootPath = sRootPath;
				return true;
			}
			else
			{
				return false;
			}
		}
	public:
        virtual bool isFileExistInZipAPKExpansion(const char* sFileName) { return false; }
		virtual bool isFileExist( const char* sFileName )
		{
            std::string sFullName = m_sRootPath + "/"+sFileName;
        	#ifdef OS_WINDOWS
        	FILE* fp = _wfopen(utf8ToWide(sFullName).c_str(), L"rb");
        	#else
        	FILE* fp = fopen(sFullName.c_str(), "rb");
        	#endif
            if( !fp )
            {
                return false;
            }
            fclose(fp);
            return true;
		}
        virtual unsigned int getFileSize(const char* sFileName)
        {
            std::string sFullName = m_sRootPath + "/"+ sFileName;
            #ifdef OS_WINDOWS
        	FILE* fp = _wfopen(utf8ToWide(sFullName).c_str(), L"rb");
        	#else
        	FILE* fp = fopen(sFullName.c_str(), "rb");
       	 	#endif
            if (!fp) return 0;
            fseek(fp, 0, SEEK_END);
            int nBuffSize = (int)ftell(fp);
            fclose(fp);
            return nBuffSize;
        }
		virtual bool loadFileContent(const char* sFileName, ALLOCMEM pAllocFunc, void* pUserData, int& nBuffSize);

        virtual bool loadFileContent(const char* sFileName, char*& pBuffer, int& nBuffSize);

    private:
        std::string             m_sRootPath;
	};
}
#endif //__JCIosFileSource_H__


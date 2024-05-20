#include "JCIosFileSource.h"

namespace laya
{

	bool JCIosFileSource::loadFileContent(const char* sFileName, ALLOCMEM pAllocFunc, void* pUserData, int& nBuffSize)
    {
		bool bRet = false;
        std::string sFullName = m_sRootPath +"/"+ sFileName;
        FILE* fp = fopen(sFullName.c_str(), "rb");
        if (!fp) return false;
        fseek(fp, 0, SEEK_END);
        nBuffSize = (int)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        unsigned char* pBuff = pAllocFunc(nBuffSize, pUserData);
        if (!pBuff)
        {
            bRet = false;
        }
        else
        {
            int nReadSize = fread((char*)pBuff, 1, nBuffSize, fp);
            if (nReadSize == nBuffSize)
            {
                //LOGI("found the file in the package:%s\n", sFileName);
                bRet = true;
            }
        }
        fclose(fp);
        return bRet;
	}
    bool JCIosFileSource::loadFileContent(const char* sFileName, char*& pBuffer, int& nBuffSize)
    {
        bool bRet = false;
        std::string sFullName = m_sRootPath + "/"+sFileName;
        FILE* fp = fopen(sFullName.c_str(), "rb");
        if (!fp) return false;
        fseek(fp, 0, SEEK_END);
        nBuffSize = (unsigned int)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        pBuffer = new char[nBuffSize+1];
        int nReadSize = fread((char*)pBuffer, 1, nBuffSize, fp);
        if (nReadSize == nBuffSize)
        {
            //LOGI("found the file in the package:%s\n", sFileName);
            bRet = true;
        }
        pBuffer[nBuffSize] = 0;
        fclose(fp);
        return bRet;
    }
}


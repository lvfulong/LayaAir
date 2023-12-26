/**
 @file			JCFileSystem.cpp
 @brief
 @author			James
 @version		1.0
 @date			2016_5_11
 */

#include <utils/JCFileSystem.h>
#include <stdio.h>
#include <utils/JCCommonMethod.h>
#define ERROR_FILE_C_R_W (-6)
#pragma warning(disable:4996)


#ifdef WIN32
#else
#include <cstring>
#include <sys/stat.h>
#include <cstdlib>
#include <cerrno>
#include <dirent.h>
#include <unistd.h>
#endif



namespace laya
{
    //GLOBAL_ONCREATEFILEERROR global_onCreateFileError = NULL;
    bool readFileSync( const char* p_pszFile , JCBuffer& p_buf, int p_nEncode )
    {
        if(!p_pszFile)
            return false;
        FILE* pf = NULL;
        pf=fopen(p_pszFile,"rb");
        if( pf==NULL )
            return false;
        fseek(pf,0,SEEK_END);
        int len = ftell(pf);
        bool bAsText = p_nEncode!=JCBuffer::raw ;
        fseek(pf,0,SEEK_SET);
        p_buf.create(len+(bAsText?1:0));
        int readlen = fread(p_buf.m_pPtr, 1, len, pf);
        if( readlen!=len){
            fclose(pf);
            p_buf.free();
            return false;
        }
        fclose(pf);
        if( bAsText )
            p_buf.m_pPtr[len]=0;
        return true;
    }
    std::string readFileSync1(const char* p_pszFile, const char* p_pszEncode)
    {
        JCBuffer buf;
        if (readFileSync(p_pszFile, buf, JCBuffer::utf8))
        {
            return buf.m_pPtr;
        }
        return "";
    }
    bool writeFileSync1( const char* p_pszFile, char* p_pBuff, int p_nLen, int p_nEncode)
    {
        if (!p_pszFile || strlen(p_pszFile) <= 1)
            return false;
        FILE* pFile = NULL;
        //try{
            pFile = fopen( p_pszFile, "wb");
            if( pFile==NULL){
                //throw ERROR_FILE_C_R_W;
                return false;
            }
            int ret = fwrite( p_pBuff, 1, p_nLen, pFile );
            if(ret<p_nLen){
                fclose(pFile);
                //throw ERROR_FILE_C_R_W;
                return false;
            }
            fflush(pFile);
            fclose(pFile);
        //}
        //catch(int e)
        //{
        //    if( e==ERROR_FILE_C_R_W && global_onCreateFileError){
        //        global_onCreateFileError();
        //    }
        //    return false;
        //}
        return true;
    }
    bool writeFileSync( const char* p_pszFile, JCBuffer& p_buf, int p_nEncode )
    {
        return writeFileSync1(p_pszFile, p_buf.m_pPtr, p_buf.m_nLen, p_nEncode);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------

/**
 @file			JCFileSystem.h
 @brief
 @author			James
 @version		1.0
 @date			2016_5_11
 */
#ifndef __JCFileSystem_H__
#define __JCFileSystem_H__

#include <utils/JCBuffer.h>

//���ڸ���ƽ̨�������,��֧�����ٻ���C++17��׼��
//https://github.com/gulrak/filesystem.git
#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;


#ifdef WIN32
//#include <filesystem>
//namespace  fs = std::experimental::filesystem::v1;
#else
#include <string>
#include <ctime>
#include <cerrno>
#include <cassert>
#include <system_error>
#endif
namespace laya
{
    
    /** @brief ͬ����ȡ�����ļ�
     *  @param[in] �ļ�����
     *  @param[out] ���ص�buffer
     *  @param[in] Encode�Ǳ�ʾ�ļ����ݵ�ԭ���ı��뷽ʽ���������ֱ��뷽ʽȥ��������
     *  @return �Ƿ��ȡ�ɹ�
     */
    bool readFileSync(const char* p_pszFile, JCBuffer& p_buf, int p_nEncode = JCBuffer::raw);
    
    
    /** @brief ͬ����ȡ�����ļ�
     *  @param[in] �ļ�����
     *  @param[in] Encode�Ǳ�ʾ�ļ����ݵ�ԭ���ı��뷽ʽ���������ֱ��뷽ʽȥ��������
     *  @return �����ַ���
     */
    std::string readFileSync1(const char* p_pszFile, const char* p_pszEncode);
    
    /** @brief ͬ��д�����ļ�
     *  @param[in] �ļ�����
     *  @param[in] ���ص�buffer
     *  @param[in] Encode��Ҫ���ļ������ʲô�ı��뷽ʽ
     *  @return �Ƿ�д��ɹ�
     */
    bool writeFileSync(const char* p_pszFile, JCBuffer& p_buf, int p_nEncode = JCBuffer::raw);
    
    /** @brief д�����ļ�
     *  @param[in] �ļ���
     *  @param[in] д���buffer
     *  @param[in] ����
     *  @param[in] encodetype
     *  @return �Ƿ�д��ɹ�
     */
    bool writeFileSync1(const char* p_pszFile, char* p_pBuff, int p_nLen, int p_nEncode = JCBuffer::raw);
    
    //typedef void(*GLOBAL_ONCREATEFILEERROR)();
    
    //extern GLOBAL_ONCREATEFILEERROR global_onCreateFileError;
}
//------------------------------------------------------------------------------


#endif //__JCFileSystem_H__

//-----------------------------END FILE--------------------------------

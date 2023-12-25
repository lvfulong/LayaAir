/**
 @file			JCFileSystem.h
 @brief
 @author			James
 @version		1.0
 @date			2016_5_11
 */
#ifndef __JCFileSystem_H__
#define __JCFileSystem_H__

#include <Utils/JCBuffer.h>

//现在各个平台都用这个,都支持了再换成C++17标准库
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
    
    /** @brief 同步读取本地文件
     *  @param[in] 文件名字
     *  @param[out] 返回的buffer
     *  @param[in] Encode是表示文件内容的原本的编码方式。即用这种编码方式去解释内容
     *  @return 是否读取成功
     */
    bool readFileSync(const char* p_pszFile, JCBuffer& p_buf, int p_nEncode = JCBuffer::raw);
    
    
    /** @brief 同步读取本地文件
     *  @param[in] 文件名字
     *  @param[in] Encode是表示文件内容的原本的编码方式。即用这种编码方式去解释内容
     *  @return 返回字符串
     */
    std::string readFileSync1(const char* p_pszFile, const char* p_pszEncode);
    
    /** @brief 同步写本地文件
     *  @param[in] 文件名字
     *  @param[in] 返回的buffer
     *  @param[in] Encode是要把文件保存成什么的编码方式
     *  @return 是否写入成功
     */
    bool writeFileSync(const char* p_pszFile, JCBuffer& p_buf, int p_nEncode = JCBuffer::raw);
    
    /** @brief 写本地文件
     *  @param[in] 文件名
     *  @param[in] 写入的buffer
     *  @param[in] 长度
     *  @param[in] encodetype
     *  @return 是否写入成功
     */
    bool writeFileSync1(const char* p_pszFile, char* p_pBuff, int p_nLen, int p_nEncode = JCBuffer::raw);
    
    //typedef void(*GLOBAL_ONCREATEFILEERROR)();
    
    //extern GLOBAL_ONCREATEFILEERROR global_onCreateFileError;
}
//------------------------------------------------------------------------------


#endif //__JCFileSystem_H__

//-----------------------------END FILE--------------------------------

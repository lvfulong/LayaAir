#ifndef __JCFileSystem_H__
#define __JCFileSystem_H__

#include <utils/JCBuffer.h>

//#ifdef __APPLE__
// https://github.com/gulrak/filesystem.git
#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;
//#else
//#include <filesystem>
//namespace fs = std::filesystem;
//#endif

#ifdef WIN32
#else
#include <cassert>
#include <cerrno>
#include <ctime>
#include <string>
#include <system_error>
#endif
namespace laya
{

bool readFileSync(const char *p_pszFile, JCBuffer &p_buf, int p_nEncode = JCBuffer::raw);

std::string readFileSync1(const char *p_pszFile, const char *p_pszEncode);

bool writeFileSync(const char *p_pszFile, JCBuffer &p_buf, int p_nEncode = JCBuffer::raw);

bool writeFileSync1(const char *p_pszFile, char *p_pBuff, int p_nLen, int p_nEncode = JCBuffer::raw);

// typedef void(*GLOBAL_ONCREATEFILEERROR)();

// extern GLOBAL_ONCREATEFILEERROR global_onCreateFileError;

namespace FileSystem
{
bool exists(const std::string &path);
bool mkdir(const std::string &path);
bool rm(const std::string &path);
std::vector<std::string> readdirSync(const std::string& path);
bool is_directory(const std::string &path);
bool is_regular_file(const std::string &path);
uintmax_t file_size(const std::string &path);
std::time_t last_write_time(const std::string &path);
std::string filename(const std::string &path);
std::string remove_filename(const std::string& path);
} // namespace FileSystem
} // namespace laya

#endif //__JCFileSystem_H__

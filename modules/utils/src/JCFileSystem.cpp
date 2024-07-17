#include <stdio.h>
#include <utils/JCCommonMethod.h>
#include <utils/JCFileSystem.h>

#pragma warning(disable : 4996)


namespace laya
{
bool readFileSync(const char *p_pszFile, JCBuffer &p_buf, int p_nEncode)
{
    if (!p_pszFile)
        return false;
    #ifdef OS_WINDOWS
    FILE* pf = _wfopen(utf8ToWide(p_pszFile).c_str(), L"rb");
    #else
    FILE* pf = fopen(p_pszFile, "rb");
    #endif

    if (pf == NULL)
        return false;
    fseek(pf, 0, SEEK_END);
    int len = ftell(pf);
    bool bAsText = p_nEncode != JCBuffer::raw;
    fseek(pf, 0, SEEK_SET);
    p_buf.create(len + (bAsText ? 1 : 0));
    int readlen = fread(p_buf.m_pPtr, 1, len, pf);
    if (readlen != len)
    {
        fclose(pf);
        p_buf.free();
        return false;
    }
    fclose(pf);
    if (bAsText)
        p_buf.m_pPtr[len] = 0;
    return true;
}
std::string readFileSync1(const char *p_pszFile, const char *p_pszEncode)
{
    JCBuffer buf;
    if (readFileSync(p_pszFile, buf, JCBuffer::utf8))
    {
        return buf.m_pPtr;
    }
    return "";
}
bool writeFileSync1(const char *p_pszFile, char *p_pBuff, int p_nLen, int p_nEncode)
{
    if (!p_pszFile || strlen(p_pszFile) <= 1)
        return false;
    #ifdef OS_WINDOWS
    FILE* pFile = _wfopen(utf8ToWide(p_pszFile).c_str(), L"wb");
    #else
    FILE* pFile = fopen(p_pszFile, "wb");
    #endif

    if (pFile == NULL)
    {
        return false;
    }
    int ret = fwrite(p_pBuff, 1, p_nLen, pFile);
    if (ret < p_nLen)
    {
        fclose(pFile);
        return false;
    }
    fflush(pFile);
    fclose(pFile);
    return true;
}
bool writeFileSync(const char *p_pszFile, JCBuffer &p_buf, int p_nEncode)
{
    return writeFileSync1(p_pszFile, p_buf.m_pPtr, p_buf.m_nLen, p_nEncode);
}
namespace FileSystem
{
bool exists(const std::string &path)
{
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    std::error_code error;
    return fs::exists(wide_path, error);
#else
    std::error_code error;
    return fs::exists(path, error);
#endif
}
bool mkdir(const std::string &path)
{
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    std::error_code error;
    return fs::create_directories(wide_path, error);
#else
    std::error_code error;
    return fs::create_directories(path, error);
#endif
}
bool rm(const std::string &path)
{
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    std::error_code error;
    return fs::remove(wide_path, error);
#else
    std::error_code error;
    return fs::remove(path, error);
#endif
}
std::vector<std::string> readdirSync(const std::string &path)
{
    std::vector<std::string> paths;
    if (!FileSystem::exists(path))
        return paths;
    std::error_code error;
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    fs::directory_iterator item_begin(wide_path, error);
#else
    fs::directory_iterator item_begin(path, error);
#endif
    fs::directory_iterator item_end;
    for (; item_begin != item_end; item_begin.increment(error))
    {
        auto pp = (*item_begin).path().filename();
#if OS_WINDOWS
        paths.push_back(wideToUtf8(pp.wstring()));
#else
        paths.push_back(pp.generic_string());
#endif
    }
    return paths;
}
bool is_directory(const std::string &path)
{
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    std::error_code error;
    return fs::is_directory(wide_path, error);
#else
    std::error_code error;
    return fs::is_directory(path, error);
#endif
}
bool is_regular_file(const std::string &path)
{
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    std::error_code error;
    return fs::is_regular_file(wide_path, error);
#else
    std::error_code error;
    return fs::is_regular_file(path, error);
#endif
}
uintmax_t file_size(const std::string &path)
{
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    std::error_code error;
    return fs::is_regular_file(wide_path, error);
#else
    std::error_code error;
    return fs::is_regular_file(path, error);
#endif
}
std::time_t last_write_time(const std::string &path)
{
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    std::error_code error;
    return std::chrono::system_clock::to_time_t(fs::last_write_time(wide_path, error));
#else
    std::error_code error;
    return std::chrono::system_clock::to_time_t(fs::last_write_time(path, error));
#endif
}

std::string filename(const std::string &path)
{
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    fs::path p(wide_path);
    std::error_code error;
    return wideToUtf8(p.filename().wstring());
#else
    fs::path p(path);
    return p.filename().string();
#endif
}
std::string remove_filename(const std::string &path)
{
#if OS_WINDOWS
    std::wstring wide_path = utf8ToWide(path);
    fs::path p(wide_path);
    std::error_code error;
    return wideToUtf8(p.remove_filename().wstring());
#else
    fs::path p(path);
    return p.remove_filename().string();
#endif
}
} // namespace FileSystem
} // namespace laya
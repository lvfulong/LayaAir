#include <platform/OS.h>
#include "JCConch.h"
#include <utils/JCFileSystem.h>
#include <utils/JCFileSource.h>

namespace laya
{
#if defined(OS_ANDROID) || defined(OS_OHOS)
static std::string s_persistentDataPath = "";
static std::string s_temporaryCachePath = "";
const std::string& OS::getPersistentDataPath()
{
    DEBUG_CHECK(!s_persistentDataPath.empty() && "persistentDataPath is not initialized");
    return s_persistentDataPath;
}
const std::string& OS::getTemporaryCachePath()
{
    DEBUG_CHECK(!s_temporaryCachePath.empty() && "temporaryCachePath is not initialized");
    return s_temporaryCachePath;
}

void OS::setPersistentDataPath(const std::string &path)
{
    s_persistentDataPath = path;
}
void OS::setTemporaryCachePath(const std::string &path)
{
    s_temporaryCachePath = path;
}
static unsigned char* _readAssetAlloc(int sz, void* pUserData) 
{
    auto ret = new unsigned char[sz];
    *(unsigned char**)pUserData = ret;
    return ret;
}
std::string OS::getAssetFullPath(const std::string &assetRelativePath)
{
    static std::string androidExtractedAssetsPath = OS::getTemporaryCachePath() + std::string("/android_assets");
    if (!FileSystem::exists(androidExtractedAssetsPath))
    {
        if (!FileSystem::mkdir(androidExtractedAssetsPath))
        {
            LOGE("create androidExtractedAssetsPath failed");
            return "";
        }
    }
    std::string assetFullPath = androidExtractedAssetsPath + "/" + assetRelativePath;
    
    if (FileSystem::exists(assetFullPath))
    {
        return assetFullPath;
    }

    if (JCConch::s_pAssetsFiles->isFileExist(assetRelativePath.c_str()))
    { 
        std::string assetDir = FileSystem::parent_path(assetFullPath);
        if (!FileSystem::exists(assetDir))
        {
            if (!FileSystem::mkdir(assetDir))
            {
                LOGE("create assetDir failed");
                return "";
            }
        }
        int sz = 0;
        unsigned char* pBuff = NULL;
        if (JCConch::s_pAssetsFiles->loadFileContent(assetRelativePath.c_str(), _readAssetAlloc, &pBuff, sz))
        {
            writeFileSync1(assetFullPath.c_str(), (char *)pBuff, sz, JCBuffer::raw);
            delete[] pBuff;
            return assetFullPath;
        }
    }
    LOGE("%s not found in assets", assetRelativePath.c_str());
    return "";
}
std::string OS::getAssetRootPath()
{
    DEBUG_CHECK(false && "current platform can not call getAssetRootPath");
    return "";
}
#endif
} // namespace laya

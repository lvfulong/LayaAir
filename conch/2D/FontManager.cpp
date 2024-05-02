#include "FontManager.h"
#include <utils/Log.h>
#include <utils/JCFileSystem.h>
#ifdef __ANDROID__
#include "CanvasRenderingContext2DAndroid.h"
#elif __APPLE__
#include <CoreGraphics/CGDataProvider.h>
#include <CoreGraphics/CGFont.h>
#include <CoreText/CTFontManager.h>
#else
#endif


extern std::string gRedistPath;
namespace laya
{
#ifdef __ANDROID__
class NativeInfoImpl
{
    
};
#elif __APPLE__
struct NativeInfoImpl
{
    CGFontRef m_registerfont;
};
extern std::vector<std::string> getAllSystemFontsIOS();
static bool registerFontIOS(const std::string &family, CGDataProviderRef fontDataProvider)
{
    CGFontRef registerfont = CGFontCreateWithDataProvider(fontDataProvider);


    CFStringRef fontName = CGFontCopyFullName(registerfont);

    ssize_t maxSize = CFStringGetMaximumSizeForEncoding(CFStringGetLength(fontName), kCFStringEncodingUTF8);

    char *buffer = new char[maxSize];
    memset(buffer, 0, maxSize);

    CFStringGetCString(fontName, buffer, maxSize, kCFStringEncodingUTF8);
    std::string strRealFontName = buffer;

    CFRelease(fontName);

    if (strRealFontName.empty())
    {
        LOGE("registerFont error: %s", " invalid ttf file with no font name");
        return false;
    }

    CFErrorRef error = nullptr;
    auto it = FontManager::getInstance()->m_fontName2NativeInfoImpl.find(family);
    if (it != FontManager::getInstance()->m_fontName2NativeInfoImpl.end())
    {
        CTFontManagerUnregisterGraphicsFont(it->second->m_registerfont, &error);
        CGFontRelease(it->second->m_registerfont);
        delete it->second;
        FontManager::getInstance()->m_fontName2NativeInfoImpl.erase(it);
    }
    
    CTFontManagerRegisterGraphicsFont(registerfont, &error);

    if (error)
    {
        CFStringRef errorDescription = CFErrorCopyDescription(error);
        //kCTFontManagerErrorDuplicatedName = 305
        //An error that indicates the file can't register because of a duplicate font name.
        if (CFErrorGetCode(error) == 305) {
            return true;
        }
        else {
            LOGE("registerFont error: %s", errorDescription);
        }
        CFRelease(errorDescription);
        //CTFontManagerUnregisterGraphicsFont(registerfont, &error);
        //CTFontManagerRegisterGraphicsFont(registerfont, &error);
        return false;
    }

    
    FontManager::getInstance()->m_fontName2RealName.insert(std::make_pair(family, strRealFontName));
    NativeInfoImpl* info = new NativeInfoImpl;
    info->m_registerfont = registerfont;
    FontManager::getInstance()->m_fontName2NativeInfoImpl.insert(std::make_pair(family, info));
    LOGE("registerFont succeed: %s", family.c_str());
    return true;
}
#endif
FontManager *g_pFontManager = nullptr;
FontManager::FontManager(){
#ifdef __ANDROID__
#elif __APPLE__
#else
#endif
} FontManager::~FontManager()
{
    m_fontName2RealName.clear();
#ifdef __ANDROID__
#elif __APPLE__
    CFErrorRef error = nullptr;
    std::unordered_map<std::string, NativeInfoImpl*>::iterator it = m_fontName2NativeInfoImpl.begin();
    for (; it != m_fontName2NativeInfoImpl.end(); it++)
    {
        CTFontManagerUnregisterGraphicsFont(it->second->m_registerfont, &error);
        CGFontRelease(it->second->m_registerfont);
        delete it->second;
    }
#else
#endif
}
FontManager *FontManager::getInstance()
{
    if (g_pFontManager == nullptr)
        g_pFontManager = new FontManager();
    return g_pFontManager;
}
void FontManager::deleteInstance()
{
    if (g_pFontManager != nullptr)
        delete g_pFontManager;
    g_pFontManager = nullptr;
}
bool FontManager::registerFont(const std::string &family, const std::string &path)
{
#ifdef __ANDROID__
    return CanvasRenderingContext2DAndroid::registerFontFromPath(family, path);
#elif __APPLE__
    CGDataProviderRef fontDataProvider = CGDataProviderCreateWithFilename(path.c_str());
    if (fontDataProvider == nullptr)
    {
        LOGI("registerFont failes fontDataProvider == nullptr");
        return false;
    }
    return registerFontIOS(family, fontDataProvider);
#else
    return true;
#endif
}
bool FontManager::registerFont(const std::string &family, const uint8_t *data, int32_t byteLength)
{
#ifdef __ANDROID__
    JCBuffer buf((char *)data, byteLength, false, false);
    std::string tempFilePath = gRedistPath + "/appCache" +  std::string("/tmp_") + family;
    writeFileSync(tempFilePath.c_str(), buf);
    return CanvasRenderingContext2DAndroid::registerFontFromPath(family, tempFilePath);
#elif __APPLE__
    JCBuffer buf((char *)data, byteLength, false, false);
    std::string tempFilePath = gRedistPath + "/appCache" +  std::string("/tmp_") + family;
    writeFileSync(tempFilePath.c_str(), buf);
    CGDataProviderRef fontDataProvider = CGDataProviderCreateWithFilename(tempFilePath.c_str());
    //CGDataProviderRef fontDataProvider = CGDataProviderCreateWithData(nullptr, data, byteLength, nullptr);
    if (fontDataProvider == nullptr)
    {
        LOGI("registerFont failes fontDataProvider == nullptr");
        return false;
    }
    bool ret = registerFontIOS(family, fontDataProvider);
    CGDataProviderRelease(fontDataProvider);
    return ret;
#else
    return true;
#endif
}
std::vector<std::string> FontManager::getAllSystemFonts()
{
#ifdef __ANDROID__
    return std::vector<std::string>();
#elif __APPLE__
    return getAllSystemFontsIOS();
#else
    return std::vector<std::string>();
#endif
}
std::pair<bool, std::string> FontManager::getRealFontName(const std::string &family)
{
    auto it = FontManager::m_fontName2RealName.find(family);
    if (it != FontManager::m_fontName2RealName.end())
    {
        return std::make_pair(true, it->second);
    }
    return std::make_pair(false, "");
}

} // namespace laya

#include "FontManager.h"
#include <utils/Log.h>
#include <utils/JCFileSystem.h>
#if defined(OS_ANDROID)
#include "CanvasRenderingContext2DAndroid.h"
#elif defined(OS_IOS)
#include <CoreGraphics/CGDataProvider.h>
#include <CoreGraphics/CGFont.h>
#include <CoreText/CTFontManager.h>
#elif defined(OS_WINDOWS)
#include "CanvasRenderingContext2DWin.h"
#else
#endif


extern std::string gRedistPath;
namespace laya
{
#if defined(OS_ANDROID)
class NativeInfoImpl
{
    
};
#elif defined(OS_IOS)
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

    //LOGE("registerFont family: %s strRealFontName: %s", family.c_str(), strRealFontName.c_str());

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
        if (CFErrorGetCode(error) == 305) {
            LOGE("registerFont error: %s", "the file can't register because of a duplicate font name");
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
    LOGI("registerFont succeed: %s", family.c_str());
    return true;
}
#endif
FontManager *g_pFontManager = nullptr;
FontManager::FontManager(){
} FontManager::~FontManager()
{
    m_fontName2RealName.clear();
#if defined(OS_ANDROID)
#elif defined(OS_IOS)
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
#if defined(OS_ANDROID)
    return CanvasRenderingContext2DAndroid::registerFontFromPath(family, path);
#elif defined(OS_IOS)
    CGDataProviderRef fontDataProvider = CGDataProviderCreateWithFilename(path.c_str());
    if (fontDataProvider == nullptr)
    {
        LOGI("registerFont failes fontDataProvider == nullptr");
        return false;
    }
    return registerFontIOS(family, fontDataProvider);
#elif defined(OS_WINDOWS)
    return CanvasRenderingContext2DWin::registerFontFromPath(family, path);
#else
    return true;
#endif
}



bool FontManager::registerFont(const std::string &family, const uint8_t *data, int32_t byteLength)
{
#if defined(OS_ANDROID)
    JCBuffer buf((char *)data, byteLength, false, false);
    std::string tempFilePath = gRedistPath + "/appCache" +  std::string("/tmp_") + family;
    writeFileSync(tempFilePath.c_str(), buf);
    return CanvasRenderingContext2DAndroid::registerFontFromPath(family, tempFilePath);
#elif defined(OS_IOS)
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
#elif defined(OS_WINDOWS)
    return CanvasRenderingContext2DWin::registerFontFromBuffer(family,data, byteLength);
#else
    return true;
#endif
}
std::vector<std::string> FontManager::getAllSystemFonts()
{
#if defined(OS_ANDROID)
    return std::vector<std::string>();
#elif defined(OS_IOS)
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

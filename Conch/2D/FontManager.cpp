#include "FontManager.h"
#include <utils/Log.h>
#ifdef ANDROID
#include "CanvasRenderingContext2DAndroid.h"
#include <utils/JCFileSystem.h>
#elif __APPLE__
#include <CoreGraphics/CGDataProvider.h>
#include <CoreGraphics/CGFont.h>
#include <CoreText/CTFontManager.h>
#else
#endif


extern std::string gRedistPath;
namespace laya
{
#ifdef ANDROID
#elif __APPLE__
extern std::vector<std::string> getAllSystemFontsIOS();
static bool registerFontIOS(const std::string &family, CGDataProviderRef fontDataProvider)
{
    CGFontRef registerfont = CGFontCreateWithDataProvider(fontDataProvider);

    CGDataProviderRelease(fontDataProvider);

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

    CTFontManagerRegisterGraphicsFont(registerfont, &error);

    if (error)
    {
        CFStringRef errorDescription = CFErrorCopyDescription(error);
        LOGE("registerFont error: %s", errorDescription);
        CFRelease(errorDescription);

        CTFontManagerUnregisterGraphicsFont(registerfont, &error);

        CTFontManagerRegisterGraphicsFont(registerfont, &error);
        return false;
    }
    CGFontRelease(registerfont);
    FontManager::getInstance()->m_fontName2RealName.insert(std::make_pair(family, strRealFontName));
    return true;
}
#endif
FontManager *g_pFontManager = nullptr;
FontManager::FontManager(){
#ifdef ANDROID
#elif __APPLE__
#else
#endif
} FontManager::~FontManager()
{
    m_fontName2RealName.clear();
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
#ifdef ANDROID
    return CanvasRenderingContext2DAndroid::registerFontFromPath(family, path);
#elif __APPLE__
    CGDataProviderRef fontDataProvider = CGDataProviderCreateWithFilename(path.c_str());
    if (fontDataProvider == nullptr)
    {
        return false;
    }
    return registerFontIOS(family, fontDataProvider);
#else
    return true;
#endif
}
bool FontManager::registerFont(const std::string &family, const uint8_t *data, int32_t byteLength)
{
#ifdef ANDROID
    JCBuffer buf((char *)data, byteLength, false, false);
    std::string tempFilePath = gRedistPath + "/appCache" +  std::string("/tmp_") + family;
    writeFileSync(tempFilePath.c_str(), buf);
    return CanvasRenderingContext2DAndroid::registerFontFromPath(family, tempFilePath);
#elif __APPLE__
    CGDataProviderRef fontDataProvider = CGDataProviderCreateWithData(nullptr, data, byteLength, nullptr);
    if (fontDataProvider == nullptr)
    {
        return true;
    }
    return registerFontIOS(family, fontDataProvider);
#else
    return true;
#endif
}
std::vector<std::string> FontManager::getAllSystemFonts()
{
#ifdef ANDROID
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

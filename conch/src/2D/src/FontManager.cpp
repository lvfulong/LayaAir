#include "2D/FontManager.h"
#include <utils/JCFileSystem.h>
#include <utils/Log.h>
#if defined(OS_ANDROID)
#include "CanvasRenderingContext2DAndroid.h"
#elif defined(OS_IOS)
#include "CanvasRenderingContext2DCG.h"
#elif defined(OS_WINDOWS)
#include "CanvasRenderingContext2DWin.h"
#elif defined(OS_OHOS)
#include "CanvasRenderingContext2DOHOS.h"
#elif defined(OS_LINUX)
#include "CanvasRenderingContext2DLinux.h"
#endif

extern std::string gRedistPath;
namespace laya
{
#if defined(OS_IOS)
extern std::vector<std::string> getAllSystemFontsIOS();
#endif
void FontManager::init()
{
#if defined(OS_OHOS)
    CanvasRenderingContext2DOHOS::init();
#elif defined(OS_WINDOWS)
    CanvasRenderingContext2DWin::init();
#elif defined(OS_IOS)
    CanvasRenderingContext2DCG::init();
#endif
}
void FontManager::destroy()
{
#if defined(OS_OHOS)
    CanvasRenderingContext2DOHOS::destroy();
#elif defined(OS_WINDOWS)
    CanvasRenderingContext2DWin::clearAllBuffer();
#elif defined(OS_IOS)
    CanvasRenderingContext2DCG::destroy();
#elif defined(OS_LINUX)
    CanvasRenderingContext2DLinux::destroy();
#endif
}
bool FontManager::registerFont(const std::string &family, const std::string &path)
{
#if defined(OS_ANDROID)
    return CanvasRenderingContext2DAndroid::registerFontFromPath(family, path);
#elif defined(OS_IOS)
    return CanvasRenderingContext2DCG::registerFontFromPath(family, path);
#elif defined(OS_WINDOWS)
    return CanvasRenderingContext2DWin::registerFontFromPath(family, path);
#elif defined(OS_OHOS)
    return CanvasRenderingContext2DOHOS::registerFontFromPath(family, path);
#elif defined(OS_LINUX)
    return CanvasRenderingContext2DLinux::registerFontFromPath(family,path);
#else
    return true;
#endif
}

bool FontManager::registerFont(const std::string &family, uint8_t *data, int32_t byteLength)
{
#if defined(OS_ANDROID)
    return CanvasRenderingContext2DAndroid::registerFontFromBuffer(family, data, byteLength);
#elif defined(OS_IOS)
    return CanvasRenderingContext2DCG::registerFontFromBuffer(family, data, byteLength);
#elif defined(OS_WINDOWS)
    return CanvasRenderingContext2DWin::registerFontFromBuffer(family, data, byteLength);
#elif defined(OS_OHOS)
    return CanvasRenderingContext2DOHOS::registerFontFromBuffer(family, data, byteLength);
#elif defined(OS_LINUX)
    return CanvasRenderingContext2DLinux::registerFontFromBuffer(family,data,byteLength);
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

} // namespace laya

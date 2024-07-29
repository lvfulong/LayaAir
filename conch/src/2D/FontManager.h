#ifndef __FontManager_H__
#define __FontManager_H__
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace laya
{
class NativeInfoImpl;
class FontManager
{
  public:
    FontManager();
    ~FontManager();
    static void init();
    static void destroy();
    bool registerFont(const std::string &family, const std::string &path);
    bool registerFont(const std::string &family, uint8_t *data, int32_t byteLength);
    std::vector<std::string> getAllSystemFonts();
    std::pair<bool, std::string> getRealFontName(const std::string &family);

    static FontManager *getInstance();
    static void deleteInstance();

  public:
    std::unordered_map<std::string, std::string> m_fontName2RealName;
    std::unordered_map<std::string, NativeInfoImpl*> m_fontName2NativeInfoImpl;
};
} // namespace laya
#endif

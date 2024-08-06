#ifndef __FontManager_H__
#define __FontManager_H__
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace laya
{

class FontManager
{
  public:
    static void init();
    static void destroy();
    static bool registerFont(const std::string &family, const std::string &path);
    static bool registerFont(const std::string &family, uint8_t *data, int32_t byteLength);
    static std::vector<std::string> getAllSystemFonts();
};
} // namespace laya
#endif

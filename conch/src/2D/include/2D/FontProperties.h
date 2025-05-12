#ifndef __FontProperties_H__
#define __FontProperties_H__
#include <string>
#include <vector>
namespace laya
{
enum class FontStyle
{
    Normal,
    Italic,
    Oblique
};

enum class FontVariant
{
    Normal,
    SmallCaps
};

struct FontProperties
{
    double fontSize{16.0f};
    std::vector<std::string> fontFamily;
    uint16_t fontWeight{400};
    FontVariant fontVariant{FontVariant::Normal};
    FontStyle fontStyle{FontStyle::Normal};
    bool isBold() {
      return fontWeight >= 700;   
    }
    bool isItalic() {
      return fontStyle == FontStyle::Italic;
    }
};
} // namespace laya
#endif

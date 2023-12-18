#ifndef __FontDescription_H__
#define __FontDescription_H__
#include <string>
namespace laya
{
class FontDescription
{
  public:
    bool isBold()
    {
        return m_weight == "bold";
    }
    bool isItalic()
    {
        return m_style != "normal";
    }

  public:
    std::string m_weight;
    std::string m_style;
    double m_size;
    std::string m_unit;
    std::string m_family;
};
} // namespace laya
#endif

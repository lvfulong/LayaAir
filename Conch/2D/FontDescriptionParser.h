#ifndef __FontDescriptionParser_H__
#define __FontDescriptionParser_H__
#include "FontDescription.h"
#include <memory>
#include <string>
namespace laya
{
class FontDescriptionParser
{
  public:
    ~FontDescriptionParser();
    static void test();

  public:
    // dpi ??
    static void parse(const std::string &fontStr, FontDescription &out, int32_t dpi = 96);
};
} // namespace laya
#endif
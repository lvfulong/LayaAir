#include "FontDescriptionParser.h"
#include <math.h>

#include <utils/JCCommonMethod.h>
#include <utils/Log.h>

#include <regex>
#include <string>
#include <codecvt>
#include <locale>


namespace laya
{
FontDescriptionParser::~FontDescriptionParser()
{
}
// TODO default value check
void FontDescriptionParser::parse(const std::string &fontStr, FontDescription &out, int32_t dpi /*= 96*/)
{
    std::wregex fontRegex(
        L"^ *(?:(normal|bold|bolder|lighter|[1-9]00) *)?(?:(normal|italic|oblique) *)?([\\d\\.]+)(px|pt|pc|in|cm|mm|%) "
        "*((?:\'([^\']+)\'|\"([^\"]+)\"|[\\w\\s\\u4e00-\\u9fff-]+)( *, "
        "*(?:\'([^\']+)\'|\"([^\"]+)\"|[\\w\\s\\u4e00-\\u9fff-]+))*)");

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideFontStr = converter.from_bytes(fontStr);
    std::wsmatch results;
    if (std::regex_search(wideFontStr, results, fontRegex))
    {
        out.m_weight = !results[1].str().empty() ? converter.to_bytes(results[1].str()) : "normal";
        out.m_style = !results[2].str().empty() ? converter.to_bytes(results[2].str()) : "normal";
        out.m_size = !results[3].str().empty() ? atof(converter.to_bytes(results[3].str()).c_str()) : 30.0;
        out.m_unit = !results[4].str().empty() ? converter.to_bytes(results[4].str()) : "px";

        if (!results[5].str().empty())
        {
            std::vector<char *> temp;
            std::string tempString = converter.to_bytes(results[5].str());
            splitString(temp, (char *)tempString.c_str(), ',');
            // replace(/["']/g, '').trim()
            out.m_family = temp[0];
        }
        else
        {
            out.m_family = "Arial";
        }
    }
    else
    {
        LOGI("FontDescriptionParser::parse error %s", fontStr.c_str());
    }

    if (out.m_unit == "pt")
    {
        out.m_size /= .75;
    }
    else if (out.m_unit == "in")
    {
        out.m_size *= dpi;
    }
    else if (out.m_unit == "mm")
    {
        out.m_size *= dpi / 25.4;
    }
    else if (out.m_unit == "cm")
    {
        out.m_size *= dpi / 2.54;
    }
}
void FontDescriptionParser::test()
{
    {
        FontDescriptionParser parser;
        FontDescription out;
        parser.parse("20px Arial", out);
        LOGI("[weight %s] [style %s] [size %f] [unit %s] [family %s]", out.m_weight.c_str(), out.m_style.c_str(),
             out.m_size, out.m_unit.c_str(), out.m_family.c_str());
    }
    {
        FontDescriptionParser parser;
        FontDescription out;
        parser.parse("bold italic 50px Arial, sans-serif", out);
        LOGI("[weight %s] [style %s] [size %f] [unit %s] [family %s]", out.m_weight.c_str(), out.m_style.c_str(),
             out.m_size, out.m_unit.c_str(), out.m_family.c_str());
    }
    {
        FontDescriptionParser parser;
        FontDescription out;

        std::wstring testStr = L"20px 微软雅黑";
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;


        parser.parse(converter.to_bytes(testStr), out);
        LOGI("[weight %s] [style %s] [size %f] [unit %s] [family %s]", out.m_weight.c_str(), out.m_style.c_str(),
             out.m_size, out.m_unit.c_str(), out.m_family.c_str());
    }
}
} // namespace laya

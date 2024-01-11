#include "FontDescriptionParser.h"
#include <math.h>

#include <utils/JCCommonMethod.h>
#include <utils/Log.h>

#if WIN32
#include <boost/regex.hpp>
using boost::regex;
using boost::regex_search;
using boost::smatch;
#else
#include <regex>
using std::regex;
using std::regex_search;
using std::smatch;
#endif

namespace laya
{
FontDescriptionParser::~FontDescriptionParser()
{
}
// TODO default value check
void FontDescriptionParser::parse(const std::string &fontStr, FontDescription &out, int32_t dpi /*= 96*/)
{
    regex fontRegex(
        "^ *(?:(normal|bold|bolder|lighter|[1-9]00) *)?(?:(normal|italic|oblique) *)?([\\d\\.]+)(px|pt|pc|in|cm|mm|%) "
        "*((?:\'([^\']+)\'|\"([^\"]+)\"|[\\w\\s\\u4e00-\\u9fff-]+)( *, "
        "*(?:\'([^\']+)\'|\"([^\"]+)\"|[\\w\\s\\u4e00-\\u9fff-]+))*)");
    smatch results;
    if (regex_search(fontStr.begin(), fontStr.end(), results, fontRegex))
    {
        out.m_weight = !results[1].str().empty() ? results[1].str() : "normal";
        out.m_style = !results[2].str().empty() ? results[2].str() : "normal";
        out.m_size = !results[3].str().empty() ? atof(results[3].str().c_str()) : 30.0;
        out.m_unit = !results[4].str().empty() ? results[4].str() : "px";

        if (!results[4].str().empty())
        {
            std::vector<char *> temp;
            splitString(temp, (char *)results[5].str().c_str(), ',');
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
        parser.parse("20px 微软雅黑", out);
        LOGI("[weight %s] [style %s] [size %f] [unit %s] [family %s]", out.m_weight.c_str(), out.m_style.c_str(),
             out.m_size, out.m_unit.c_str(), out.m_family.c_str());
    }
}
} // namespace laya

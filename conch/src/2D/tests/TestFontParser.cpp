#include "../src/font_parser/FontParser.h"
#include "../src/FontDescriptionParser.h"
#include <gtest/gtest.h>
using namespace laya;

class FontParserTest : public testing::Test {};

TEST_F(FontParserTest, ParseTest) {


    {laya::FontDescriptionParser parser;
    laya::FontDescription out;
    parser.parse("20px Arial", out);

    ASSERT_EQ(out.m_weight, "normal");
    ASSERT_EQ(out.m_style, "normal");
    ASSERT_EQ(out.m_size, 20.0f);
    ASSERT_EQ(out.m_unit, "px");
    ASSERT_EQ(out.m_family, "Arial"); }

    bool success;
    FontProperties p = FontParser::parse("20px Arial", &success);
    ASSERT_EQ(success, true);
    ASSERT_EQ(p.fontFamily[0], "Arial");
    ASSERT_EQ(p.fontSize, 20);
    ASSERT_EQ(p.fontStyle, FontStyle::Normal);
    ASSERT_EQ(p.fontVariant, FontVariant::Normal);
    ASSERT_EQ(p.fontWeight, 400);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

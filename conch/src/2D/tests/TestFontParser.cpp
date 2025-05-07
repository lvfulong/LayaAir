#include "../src/FontDescriptionParser.h"
#include <gtest/gtest.h>


class FontParserTest : public testing::Test {};

TEST_F(FontParserTest, ParseTest) {


    laya::FontDescriptionParser parser;
    laya::FontDescription out;
    parser.parse("20px Arial", out);

    ASSERT_EQ(out.m_weight, "normal");
    ASSERT_EQ(out.m_style, "normal");
    ASSERT_EQ(out.m_size, 20.0f);   
    ASSERT_EQ(out.m_unit, "px");
    ASSERT_EQ(out.m_family, "Arial");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

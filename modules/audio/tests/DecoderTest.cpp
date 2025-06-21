#include <audio/Decoder.h>
#include <gtest/gtest.h>
#include <fstream>
#include <vector>
using namespace audio;
class DecoderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 测试前的设置
    }

    void TearDown() override {
        // 测试后的清理
    }
};

TEST_F(DecoderTest, LoadMp3Test) {
    // 创建一个简单的测试数据
    std::vector<uint8_t> data = {
        0xFF, 0xFB, 0x90, 0x44, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    // 创建解码器并测试
    Decoder decoder;
    bool result = decoder.loadMp3(data.data(), data.size());
    
    // 验证解码结果
    EXPECT_FALSE(result) << "无效的 MP3 数据应该返回 false";
}
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

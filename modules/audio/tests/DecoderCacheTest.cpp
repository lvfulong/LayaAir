#include <audio/AudioDecoderCache.h>
#include <gtest/gtest.h>
#include <fstream>
#include <vector>
using namespace audio;
class DecoderCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 测试前的设置
    }

    void TearDown() override {
        // 测试后的清理
    }
};

TEST_F(DecoderCacheTest, CreateStaticDecoder) {
    std::vector<uint8_t> data = {
        0xFF, 0xFB, 0x90, 0x44, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    auto decoder = AudioDecoderCache::createStaticDecoder("test.mp3", data);
    EXPECT_NE(decoder, nullptr);
}
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

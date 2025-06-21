#include <audio/StreamDecoder.h>
#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <memory>
#include <cstring>
#include <iostream>

using namespace audio;

// 读取真实Ogg文件的辅助函数
std::vector<uint8_t> loadOggFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    return data;
}

// 尝试多个可能的文件路径
std::string findOggFile() {
    std::vector<std::string> possiblePaths = {
        "test/sound/cccc.ogg",
        "../test/sound/cccc.ogg",
        "../../test/sound/cccc.ogg",
        "../../../test/sound/cccc.ogg",
        "../../../../test/sound/cccc.ogg",
        "modules/audio/test/sound/cccc.ogg",
        "build/test/sound/cccc.ogg"
    };
    
    for (const auto& path : possiblePaths) {
        if (std::filesystem::exists(path)) {
            std::cout << "Found Ogg file at: " << path << std::endl;
            return path;
        }
    }
    
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    std::cout << "Could not find test.ogg file in any of the expected locations" << std::endl;
    return "";
}

TEST(StreamDecoderOggRealTest, LoadRealOggFile) {
    std::string oggFile = findOggFile();
    
    if (oggFile.empty()) {
        GTEST_SKIP() << "Test Ogg file not found";
        return;
    }
    
    auto data = loadOggFile(oggFile);
    ASSERT_FALSE(data.empty()) << "Failed to load Ogg file";
    
    auto decoder = StreamDecoder::create(oggFile, data.data(), data.size());
    ASSERT_TRUE(decoder != nullptr) << "Failed to create Ogg decoder";
    
    // 验证基本属性
    EXPECT_GT(decoder->getSampleRate(), 0) << "Sample rate should be positive";
    EXPECT_GT(decoder->getChannelCount(), 0) << "Channel count should be positive";
    EXPECT_GT(decoder->getSampleCount(), 0) << "Sample count should be positive";
    EXPECT_GT(decoder->getSize(), 0) << "Size should be positive";
    EXPECT_GT(decoder->getDuration(), 0) << "Duration should be positive";
    EXPECT_EQ(decoder->getType(), Decoder::TYPE_STREAM) << "Should be stream type";
}

TEST(StreamDecoderOggRealTest, AudioDecoding) {
    std::string oggFile = findOggFile();
    
    if (oggFile.empty()) {
        GTEST_SKIP() << "Test Ogg file not found";
        return;
    }
    
    auto data = loadOggFile(oggFile);
    ASSERT_FALSE(data.empty()) << "Failed to load Ogg file";
    
    auto decoder = StreamDecoder::create(oggFile, data.data(), data.size());
    ASSERT_TRUE(decoder != nullptr) << "Failed to create Ogg decoder";
    
    // 测试音频解码
    uint8_t buffer[4096];
    size_t totalBytesRead = 0;
    int readCount = 0;
    
    while (readCount < 10) { // 读取前10个缓冲区
        size_t bytesRead = decoder->getAudio(buffer, sizeof(buffer));
        if (bytesRead == 0) {
            break; // 到达文件末尾
        }
        
        EXPECT_GT(bytesRead, 0) << "Should read some audio data";
        totalBytesRead += bytesRead;
        readCount++;
    }
    
    EXPECT_GT(totalBytesRead, 0) << "Should have read some audio data";
}

TEST(StreamDecoderOggRealTest, SeekAndTell) {
    std::string oggFile = findOggFile();
    
    if (oggFile.empty()) {
        GTEST_SKIP() << "Test Ogg file not found";
        return;
    }
    
    auto data = loadOggFile(oggFile);
    ASSERT_FALSE(data.empty()) << "Failed to load Ogg file";
    
    auto decoder = StreamDecoder::create(oggFile, data.data(), data.size());
    ASSERT_TRUE(decoder != nullptr) << "Failed to create Ogg decoder";
    
    double duration = decoder->getDuration();
    EXPECT_GT(duration, 0) << "Duration should be positive";
    
    // 测试tell方法
    double initialPosition = decoder->tell();
    EXPECT_EQ(initialPosition, 0.0) << "Initial position should be 0";
    
    // 测试seek到开始位置
    EXPECT_TRUE(decoder->seek(0)) << "Should be able to seek to start";
    EXPECT_EQ(decoder->tell(), 0.0) << "Position should be 0 after seeking to start";
    
    // 测试seek到中间位置
    double middlePosition = duration / 2.0;
    if (middlePosition > 0) {
        EXPECT_TRUE(decoder->seek(middlePosition)) << "Should be able to seek to middle";
        double currentPosition = decoder->tell();
        EXPECT_NEAR(currentPosition, middlePosition, 0.1) << "Position should be near middle";
    }
    
    // 测试rewind
    EXPECT_TRUE(decoder->rewind()) << "Should be able to rewind";
    EXPECT_EQ(decoder->tell(), 0.0) << "Position should be 0 after rewind";
}

TEST(StreamDecoderOggRealTest, LoopFunctionality) {
    std::string oggFile = findOggFile();
    
    if (oggFile.empty()) {
        GTEST_SKIP() << "Test Ogg file not found";
        return;
    }
    
    auto data = loadOggFile(oggFile);
    ASSERT_FALSE(data.empty()) << "Failed to load Ogg file";
    
    auto decoder = StreamDecoder::create(oggFile, data.data(), data.size());
    ASSERT_TRUE(decoder != nullptr) << "Failed to create Ogg decoder";
    
    // 测试循环功能
    decoder->setLoop(true);
    EXPECT_TRUE(decoder->getLoop()) << "Loop should be enabled";
    
    decoder->setLoop(false);
    EXPECT_FALSE(decoder->getLoop()) << "Loop should be disabled";
}

TEST(StreamDecoderOggRealTest, EndOfFile) {
    std::string oggFile = findOggFile();
    
    if (oggFile.empty()) {
        GTEST_SKIP() << "Test Ogg file not found";
        return;
    }
    
    auto data = loadOggFile(oggFile);
    ASSERT_FALSE(data.empty()) << "Failed to load Ogg file";
    
    auto decoder = StreamDecoder::create(oggFile, data.data(), data.size());
    ASSERT_TRUE(decoder != nullptr) << "Failed to create Ogg decoder";
    
    // 读取到文件末尾
    uint8_t buffer[4096];
    while (true) {
        size_t bytesRead = decoder->getAudio(buffer, sizeof(buffer));
        if (bytesRead == 0) {
            break;
        }
    }
    
    // 检查是否到达文件末尾
    EXPECT_TRUE(decoder->isFinished()) << "Should be finished after reading all data";
}

TEST(StreamDecoderOggRealTest, MultipleDecoders) {
    std::string oggFile = findOggFile();
    
    if (oggFile.empty()) {
        GTEST_SKIP() << "Test Ogg file not found";
        return;
    }
    
    auto data = loadOggFile(oggFile);
    ASSERT_FALSE(data.empty()) << "Failed to load Ogg file";
    
    // 创建多个解码器实例
    std::vector<std::shared_ptr<StreamDecoder>> decoders;
    for (int i = 0; i < 3; ++i) {
        auto decoder = StreamDecoder::create(oggFile, data.data(), data.size());
        ASSERT_TRUE(decoder != nullptr) << "Failed to create decoder " << i;
        decoders.push_back(decoder);
    }
    
    // 验证所有解码器都有相同的属性
    for (size_t i = 1; i < decoders.size(); ++i) {
        EXPECT_EQ(decoders[0]->getSampleRate(), decoders[i]->getSampleRate()) 
            << "All decoders should have same sample rate";
        EXPECT_EQ(decoders[0]->getChannelCount(), decoders[i]->getChannelCount()) 
            << "All decoders should have same channel count";
        EXPECT_EQ(decoders[0]->getDuration(), decoders[i]->getDuration()) 
            << "All decoders should have same duration";
    }
}

TEST(StreamDecoderOggRealTest, FormatDetection) {
    std::string oggFile = findOggFile();
    
    if (oggFile.empty()) {
        GTEST_SKIP() << "Test Ogg file not found";
        return;
    }
    
    auto data = loadOggFile(oggFile);
    ASSERT_FALSE(data.empty()) << "Failed to load Ogg file";
    
    // 测试不同的文件名
    auto decoder1 = StreamDecoder::create("test.ogg", data.data(), data.size());
    auto decoder2 = StreamDecoder::create("test.oga", data.data(), data.size());
    auto decoder3 = StreamDecoder::create("test.ogv", data.data(), data.size());
    auto decoder4 = StreamDecoder::create("test.mp3", data.data(), data.size()); // 错误扩展名
    
    // Ogg相关扩展名应该都能识别
    EXPECT_TRUE(decoder1 != nullptr) << "Should handle .ogg extension";
    EXPECT_TRUE(decoder2 != nullptr) << "Should handle .oga extension";
    EXPECT_TRUE(decoder3 != nullptr) << "Should handle .ogv extension";
    
    // 错误扩展名应该返回nullptr
    EXPECT_FALSE(decoder4 != nullptr) << "Should not handle .mp3 extension";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 
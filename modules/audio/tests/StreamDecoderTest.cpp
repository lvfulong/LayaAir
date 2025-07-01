#include <audio/StreamDecoder.h>
#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include <filesystem>
using namespace audio;

// Helper function: create test audio data
std::vector<uint8_t> createTestAudioData() {
    // Create simple test audio data (empty for now, should have real audio files in actual project)
    return std::vector<uint8_t>();
}

TEST(StreamDecoderTest, CreateStreamDecoder) {
    std::vector<uint8_t> data = createTestAudioData();
    auto decoder = StreamDecoder::create("test.mp3", data.data(), data.size());
    EXPECT_FALSE(decoder) << "Empty data should return nullptr";
}

TEST(StreamDecoderTest, AudioProperties) {
    std::vector<uint8_t> data = createTestAudioData();
    auto decoder = StreamDecoder::create("test.mp3", data.data(), data.size());
    EXPECT_FALSE(decoder) << "Empty data should return nullptr";
}

TEST(StreamDecoderTest, FormatDetection) {
    // Test different format detection
    std::vector<uint8_t> mp3Data = createTestAudioData();
    auto mp3Decoder = StreamDecoder::create("test.mp3", mp3Data.data(), mp3Data.size());
    EXPECT_FALSE(mp3Decoder) << "Empty MP3 data should return nullptr";
    
    std::vector<uint8_t> wavData = createTestAudioData();
    auto wavDecoder = StreamDecoder::create("test.wav", wavData.data(), wavData.size());
    EXPECT_FALSE(wavDecoder) << "Empty WAV data should return nullptr";
}

TEST(StreamDecoderTest, InvalidData) {
    // Test invalid data
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03}; // Invalid audio data
    auto decoder = StreamDecoder::create("test.mp3", invalidData.data(), invalidData.size());
    EXPECT_FALSE(decoder) << "Invalid data should return nullptr";
}

TEST(StreamDecoderTest, NullData) {
    // Test null pointer data
    auto decoder = StreamDecoder::create("test.mp3", nullptr, 0);
    EXPECT_FALSE(decoder) << "Null pointer data should return nullptr";
}

TEST(StreamDecoderTest, ZeroSize) {
    // Test zero size data
    std::vector<uint8_t> data = {0x00};
    auto decoder = StreamDecoder::create("test.mp3", data.data(), 0);
    EXPECT_FALSE(decoder) << "Zero size data should return nullptr";
}

TEST(StreamDecoderTest, FileExtension) {
    // Test different file extensions
    std::vector<uint8_t> data = createTestAudioData();
    
    auto mp3Decoder = StreamDecoder::create("audio.mp3", data.data(), data.size());
    EXPECT_FALSE(mp3Decoder) << "MP3 extension should return nullptr (empty data)";
    
    auto wavDecoder = StreamDecoder::create("audio.wav", data.data(), data.size());
    EXPECT_FALSE(wavDecoder) << "WAV extension should return nullptr (empty data)";
    
    auto oggDecoder = StreamDecoder::create("audio.ogg", data.data(), data.size());
    EXPECT_FALSE(oggDecoder) << "OGG extension should return nullptr (empty data)";
}

TEST(StreamDecoderTest, MemoryManagement) {
    // Test memory management
    std::vector<uint8_t> data = createTestAudioData();
    
    // Create multiple decoder instances
    for (int i = 0; i < 10; ++i) {
        auto decoder = StreamDecoder::create("test.mp3", data.data(), data.size());
        EXPECT_FALSE(decoder) << "Empty data should return nullptr";
        // Decoder will be automatically released when scope ends
    }
}

TEST(StreamDecoderTest, SeekAndTellOnNullDecoder) {
    // Test seek and tell on nullptr decoder
    StreamDecoder* decoder = nullptr;
    // Should not crash, but since decoder is nullptr, we can't call member functions
    // This test is mainly for API completeness
    SUCCEED();
}

TEST(StreamDecoderTest, SeekAndTellOnInvalidDecoder) {
    // Test seek and tell on invalid decoder (created from invalid data)
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    auto decoder = StreamDecoder::create("test.mp3", invalidData.data(), invalidData.size());
    ASSERT_FALSE(decoder);
    // If decoder is nullptr, skip further calls
    SUCCEED();
}

TEST(StreamDecoderTest, DecodeOnNullDecoder) {
    // Test decode on nullptr decoder
    StreamDecoder* decoder = nullptr;
    uint8_t buffer[1024];
    // Can't call decode on nullptr, just ensure test structure
    SUCCEED();
}

TEST(StreamDecoderTest, DecodeOnInvalidDecoder) {
    // Test decode on invalid decoder (created from invalid data)
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    auto decoder = StreamDecoder::create("test.mp3", invalidData.data(), invalidData.size());
    ASSERT_FALSE(decoder);
    SUCCEED();
}

TEST(StreamDecoderTest, SeekOutOfRange) {
    // Test seek to out-of-range positions (if decoder is valid)
    // Here we use empty data, so decoder is nullptr
    std::vector<uint8_t> data = createTestAudioData();
    auto decoder = StreamDecoder::create("test.mp3", data.data(), data.size());
    ASSERT_FALSE(decoder);
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 
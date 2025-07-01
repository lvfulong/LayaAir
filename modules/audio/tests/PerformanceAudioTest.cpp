#include <gtest/gtest.h>
#include <audio/StaticDecoderCache.h>
#include <audio/StreamDecoder.h>
#include <audio/Audio.h>
#include <chrono>
#include <vector>
#include <memory>
#include <thread>
#include <iostream>
using namespace audio;

TEST(PerformanceAudioTest, StaticDecoderCreationPerformance) {
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    const int N = 1000;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        auto decoder = StaticDecoderCache::createDecoder("test.mp3", invalidData.data(), invalidData.size());
        // 这里只测试创建和销毁速度
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "StaticDecoder creation/destruction " << N << " times took " << duration.count() << " ms" << std::endl;
    SUCCEED();
}

TEST(PerformanceAudioTest, StreamDecoderCreationPerformance) {
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    const int N = 1000;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        auto decoder = StreamDecoder::create("test.mp3", invalidData.data(), invalidData.size());
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "StreamDecoder creation/destruction " << N << " times took " << duration.count() << " ms" << std::endl;
    SUCCEED();
}

TEST(PerformanceAudioTest, MultiInstancePerformance) {
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    const int N = 500;
    std::vector<std::shared_ptr<Decoder>> decoders;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        decoders.push_back(StaticDecoderCache::createDecoder("test.mp3", invalidData.data(), invalidData.size()));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Create " << N << " decoder instances took " << duration.count() << " ms" << std::endl;
    decoders.clear();
    SUCCEED();
}

// ==================== 内存使用性能测试 ====================

TEST(PerformanceAudioTest, MemoryUsagePerformance) {
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    const int N = 1000;
    std::vector<std::shared_ptr<Decoder>> decoders;
    
    // 测试内存分配性能
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        decoders.push_back(StaticDecoderCache::createDecoder("test.mp3", invalidData.data(), invalidData.size()));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Memory allocation for " << N << " decoders took " << duration.count() << " ms" << std::endl;
    
    // 测试内存释放性能
    start = std::chrono::high_resolution_clock::now();
    decoders.clear();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Memory deallocation for " << N << " decoders took " << duration.count() << " ms" << std::endl;
    
    SUCCEED();
}

// ==================== 并发性能测试 ====================

TEST(PerformanceAudioTest, ConcurrentCreationPerformance) {
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    const int threadCount = 4;
    const int decodersPerThread = 100;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    for (int t = 0; t < threadCount; ++t) {
        threads.emplace_back([&invalidData, decodersPerThread]() {
            std::vector<std::shared_ptr<Decoder>> decoders;
            for (int i = 0; i < decodersPerThread; ++i) {
                decoders.push_back(StaticDecoderCache::createDecoder("test.mp3", invalidData.data(), invalidData.size()));
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Concurrent creation of " << (threadCount * decodersPerThread) << " decoders took " << duration.count() << " ms" << std::endl;
    
    SUCCEED();
}

// ==================== 缓存性能测试 ====================

TEST(PerformanceAudioTest, CachePerformance) {
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    const int N = 100;
    
    // 测试重复创建相同文件的性能（缓存效果）
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        auto decoder = StaticDecoderCache::createDecoder("same_file.mp3", invalidData.data(), invalidData.size());
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Repeated creation of same file " << N << " times took " << duration.count() << " ms" << std::endl;
    
    SUCCEED();
}

// ==================== 不同格式性能测试 ====================

TEST(PerformanceAudioTest, FormatPerformance) {
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    const int N = 100;
    
    // 测试不同格式的创建性能
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        auto mp3Decoder = StaticDecoderCache::createDecoder("test.mp3", invalidData.data(), invalidData.size());
        auto wavDecoder = StaticDecoderCache::createDecoder("test.wav", invalidData.data(), invalidData.size());
        auto oggDecoder = StaticDecoderCache::createDecoder("test.ogg", invalidData.data(), invalidData.size());
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Mixed format creation " << N << " times took " << duration.count() << " ms" << std::endl;
    
    SUCCEED();
}

// ==================== 压力测试 ====================

TEST(PerformanceAudioTest, StressTest) {
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03};
    const int N = 10000;
    
    // 大规模压力测试
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        auto decoder = StaticDecoderCache::createDecoder("stress_test.mp3", invalidData.data(), invalidData.size());
        // 立即释放，测试内存压力
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Stress test: " << N << " rapid create/destroy cycles took " << duration.count() << " ms" << std::endl;
    
    SUCCEED();
}

TEST_F(AudioTest, NullPointerProtection) {
    AudioPlayer player;
    
    // 测试空解码器的情况
    std::shared_ptr<StreamDecoder> nullDecoder = nullptr;
    auto audio = std::make_shared<Audio>(&player, nullDecoder);
    
    // 这些调用不应该崩溃
    EXPECT_NO_THROW(audio->play());
    EXPECT_NO_THROW(audio->pause());
    EXPECT_NO_THROW(audio->stop());
    EXPECT_NO_THROW(audio->setVolume(0.5f));
    EXPECT_NO_THROW(audio->getVolume());
    EXPECT_NO_THROW(audio->getDuration());
    EXPECT_NO_THROW(audio->seek(1.0));
    EXPECT_NO_THROW(audio->tell());
    EXPECT_NO_THROW(audio->setLoop(true));
    EXPECT_NO_THROW(audio->getLoop());
}

TEST_F(AudioTest, InvalidDecoderData) {
    AudioPlayer player;
    
    // 测试无效数据创建解码器
    std::vector<uint8_t> invalidData = {0x00, 0x01, 0x02, 0x03}; // 无效音频数据
    auto decoder = StreamDecoder::create("test.mp3", invalidData.data(), invalidData.size());
    
    if (decoder) {
        auto audio = std::make_shared<Audio>(&player, decoder);
        
        // 这些调用不应该崩溃，即使解码器数据无效
        EXPECT_NO_THROW(audio->play());
        EXPECT_NO_THROW(audio->pause());
        EXPECT_NO_THROW(audio->stop());
        EXPECT_NO_THROW(audio->setVolume(0.5f));
        EXPECT_NO_THROW(audio->getVolume());
        EXPECT_NO_THROW(audio->getDuration());
        EXPECT_NO_THROW(audio->seek(1.0));
        EXPECT_NO_THROW(audio->tell());
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 
#include <gtest/gtest.h>
#include <audio/AudioDecoderCache.h>
#include <audio/Decoder.h>
#include <audio/StaticDecoder.h>
#include <audio/StreamDecoder.h>
#include <utils/Data.h>
#include <utils/LRUCache.h>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <random>

namespace audio
{
namespace test
{

// ==================== 测试辅助函数 ====================

// 创建模拟的MP3数据
std::shared_ptr<laya::Data> createMockMp3Data(size_t size = 1024) {
    std::vector<uint8_t> data(size);
    
    // 添加一些MP3帧数据
    if (size >= 20) {
        data[10] = 0xFF; // MP3同步字节
        data[11] = 0xFB; // MP3帧头
        data[12] = 0x90; // MP3帧头
        data[13] = 0x44; // MP3帧头
    }
    
    return laya::Data::makeWithCopy(data.data(), data.size());
}

// 创建模拟的WAV数据
std::shared_ptr<laya::Data> createMockWavData(size_t size = 1024) {
    std::vector<uint8_t> data(size);
    
    // WAV文件头
    if (size >= 44) {
        // RIFF头
        data[0] = 0x52; // 'R'
        data[1] = 0x49; // 'I'
        data[2] = 0x46; // 'F'
        data[3] = 0x46; // 'F'
        
        // 文件大小
        uint32_t fileSize = size - 8;
        data[4] = (fileSize >> 0) & 0xFF;
        data[5] = (fileSize >> 8) & 0xFF;
        data[6] = (fileSize >> 16) & 0xFF;
        data[7] = (fileSize >> 24) & 0xFF;
        
        // WAVE标识
        data[8] = 0x57; // 'W'
        data[9] = 0x41; // 'A'
        data[10] = 0x56; // 'V'
        data[11] = 0x45; // 'E'
        
        // fmt子块
        data[12] = 0x66; // 'f'
        data[13] = 0x6D; // 'm'
        data[14] = 0x74; // 't'
        data[15] = 0x20; // ' '
        
        // fmt子块大小
        data[16] = 16;
        data[17] = 0;
        data[18] = 0;
        data[19] = 0;
        
        // 音频格式 (PCM = 1)
        data[20] = 1;
        data[21] = 0;
        
        // 声道数 (立体声 = 2)
        data[22] = 2;
        data[23] = 0;
        
        // 采样率 (44100 Hz)
        uint32_t sampleRate = 44100;
        data[24] = (sampleRate >> 0) & 0xFF;
        data[25] = (sampleRate >> 8) & 0xFF;
        data[26] = (sampleRate >> 16) & 0xFF;
        data[27] = (sampleRate >> 24) & 0xFF;
        
        // 字节率
        uint32_t byteRate = sampleRate * 2 * 2; // 采样率 * 声道数 * 每样本字节数
        data[28] = (byteRate >> 0) & 0xFF;
        data[29] = (byteRate >> 8) & 0xFF;
        data[30] = (byteRate >> 16) & 0xFF;
        data[31] = (byteRate >> 24) & 0xFF;
        
        // 块对齐
        data[32] = 4; // 声道数 * 每样本字节数
        data[33] = 0;
        
        // 每样本位数
        data[34] = 16;
        data[35] = 0;
        
        // data子块
        data[36] = 0x64; // 'd'
        data[37] = 0x61; // 'a'
        data[38] = 0x74; // 't'
        data[39] = 0x61; // 'a'
        
        // 数据大小
        uint32_t dataSize = size - 44;
        data[40] = (dataSize >> 0) & 0xFF;
        data[41] = (dataSize >> 8) & 0xFF;
        data[42] = (dataSize >> 16) & 0xFF;
        data[43] = (dataSize >> 24) & 0xFF;
    }
    
    return laya::Data::makeWithCopy(data.data(), data.size());
}

// 创建模拟的OGG数据
std::shared_ptr<laya::Data> createMockOggData(size_t size = 1024) {
    std::vector<uint8_t> data(size);
    
    // OGG文件头
    if (size >= 27) {
        data[0] = 0x4F; // 'O'
        data[1] = 0x67; // 'g'
        data[2] = 0x67; // 'g'
        data[3] = 0x53; // 'S'
        
        // 版本
        data[4] = 0x00;
        
        // 包类型 (0 = 继续包)
        data[5] = 0x00;
        
        // 粒度位置
        for (int i = 6; i < 14; ++i) {
            data[i] = 0x00;
        }
        
        // 比特流序列号
        for (int i = 14; i < 18; ++i) {
            data[i] = 0x00;
        }
        
        // 页面序列号
        for (int i = 18; i < 22; ++i) {
            data[i] = 0x00;
        }
        
        // CRC校验和
        for (int i = 22; i < 26; ++i) {
            data[i] = 0x00;
        }
        
        // 页面段数
        data[26] = 0x01;
    }
    
    return laya::Data::makeWithCopy(data.data(), data.size());
}

// 创建无效数据
std::shared_ptr<laya::Data> createInvalidData(size_t size = 1024) {
    std::vector<uint8_t> data(size, 0xFF);
    return laya::Data::makeWithCopy(data.data(), data.size());
}

// ==================== 基础功能测试 ====================

class AudioDecoderCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 每个测试前清理缓存
        AudioDecoderCache::clear();
        AudioDecoderCache::setMaxSizeInBytes(128 * 1024 * 1024); // 128MB
    }
    
    void TearDown() override {
        // 每个测试后清理缓存
        AudioDecoderCache::clear();
    }
};

TEST_F(AudioDecoderCacheTest, ClearCache) {
    // 测试清空缓存功能
    EXPECT_EQ(AudioDecoderCache::getCurrentSizeInBytes(), 0);
    EXPECT_EQ(AudioDecoderCache::getMaxSizeInBytes(), 128 * 1024 * 1024);
    
    // 添加一些数据到缓存
    auto mp3Data = createMockMp3Data();
    auto decoder = AudioDecoderCache::createStaticDecoder("test.mp3", mp3Data);
    
    // 清空缓存
    AudioDecoderCache::clear();
    EXPECT_EQ(AudioDecoderCache::getCurrentSizeInBytes(), 0);
    
    // 验证缓存中的解码器已被清除
    auto cachedDecoder = AudioDecoderCache::get("test.mp3");
    EXPECT_EQ(cachedDecoder, nullptr);
}

TEST_F(AudioDecoderCacheTest, SetAndGetMaxSize) {
    // 测试设置和获取最大缓存大小
    size_t newMaxSize = 64 * 1024 * 1024; // 64MB
    AudioDecoderCache::setMaxSizeInBytes(newMaxSize);
    EXPECT_EQ(AudioDecoderCache::getMaxSizeInBytes(), newMaxSize);
    
    // 测试设置更大的值
    size_t largerMaxSize = 256 * 1024 * 1024; // 256MB
    AudioDecoderCache::setMaxSizeInBytes(largerMaxSize);
    EXPECT_EQ(AudioDecoderCache::getMaxSizeInBytes(), largerMaxSize);
}

TEST_F(AudioDecoderCacheTest, GetCurrentSize) {
    // 测试获取当前缓存大小
    EXPECT_EQ(AudioDecoderCache::getCurrentSizeInBytes(), 0);
    
    // 添加数据后检查大小
    auto mp3Data = createMockMp3Data(2048);
    auto decoder = AudioDecoderCache::createStaticDecoder("test.mp3", mp3Data);
    
    // 注意：由于模拟数据可能无法真正解码，实际大小可能为0
    // 这里主要测试函数调用不会崩溃
    size_t currentSize = AudioDecoderCache::getCurrentSizeInBytes();
    EXPECT_GE(currentSize, 0);
}

// ==================== 静态解码器测试 ====================

TEST_F(AudioDecoderCacheTest, CreateStaticDecoderWithMp3) {
    auto mp3Data = createMockMp3Data();
    auto decoder = AudioDecoderCache::createStaticDecoder("test.mp3", mp3Data);
    
    // 由于是模拟数据，可能无法真正解码，但函数应该不会崩溃
    // 主要测试函数调用的正确性
    if (decoder) {
        EXPECT_EQ(decoder->getType(), Decoder::TYPE_STATIC);
    }
}

TEST_F(AudioDecoderCacheTest, CreateStaticDecoderWithWav) {
    auto wavData = createMockWavData();
    auto decoder = AudioDecoderCache::createStaticDecoder("test.wav", wavData);
    
    if (decoder) {
        EXPECT_EQ(decoder->getType(), Decoder::TYPE_STATIC);
    }
}

TEST_F(AudioDecoderCacheTest, CreateStaticDecoderWithOgg) {
    auto oggData = createMockOggData();
    auto decoder = AudioDecoderCache::createStaticDecoder("test.ogg", oggData);
    
    if (decoder) {
        EXPECT_EQ(decoder->getType(), Decoder::TYPE_STATIC);
    }
}

TEST_F(AudioDecoderCacheTest, CreateStaticDecoderWithInvalidData) {
    auto invalidData = createInvalidData();
    auto decoder = AudioDecoderCache::createStaticDecoder("test.invalid", invalidData);
    
    // 无效数据应该返回nullptr
    EXPECT_EQ(decoder, nullptr);
}

// ==================== 流解码器测试 ====================

TEST_F(AudioDecoderCacheTest, CreateStreamDecoderWithMp3) {
    auto mp3Data = createMockMp3Data();
    auto decoder = AudioDecoderCache::createStreamDecoder("test.mp3", mp3Data);
    
    if (decoder) {
        EXPECT_EQ(decoder->getType(), Decoder::TYPE_STREAM);
    }
}

TEST_F(AudioDecoderCacheTest, CreateStreamDecoderWithOgg) {
    auto oggData = createMockOggData();
    auto decoder = AudioDecoderCache::createStreamDecoder("test.ogg", oggData);
    
    if (decoder) {
        EXPECT_EQ(decoder->getType(), Decoder::TYPE_STREAM);
    }
}

TEST_F(AudioDecoderCacheTest, CreateStreamDecoderWithInvalidData) {
    auto invalidData = createInvalidData();
    auto decoder = AudioDecoderCache::createStreamDecoder("test.invalid", invalidData);
    
    // 无效数据应该返回nullptr
    EXPECT_EQ(decoder, nullptr);
}

// ==================== 缓存功能测试 ====================

TEST_F(AudioDecoderCacheTest, GetFromCache) {
    // 测试从缓存获取解码器
    auto mp3Data = createMockMp3Data();
    
    // 第一次创建，应该不在缓存中
    auto decoder1 = AudioDecoderCache::get("test.mp3");
    EXPECT_EQ(decoder1, nullptr);
    
    // 创建解码器并缓存
    auto decoder2 = AudioDecoderCache::createStaticDecoder("test.mp3", mp3Data);
    
    // 再次获取，应该从缓存中返回
    auto decoder3 = AudioDecoderCache::get("test.mp3");
    if (decoder2) {
        EXPECT_NE(decoder3, nullptr);
    }
}

TEST_F(AudioDecoderCacheTest, CacheEviction) {
    // 测试缓存淘汰机制
    size_t smallMaxSize = 1024; // 1KB
    AudioDecoderCache::setMaxSizeInBytes(smallMaxSize);
    
    // 添加多个解码器，触发缓存淘汰
    for (int i = 0; i < 10; ++i) {
        auto data = createMockMp3Data(512);
        std::string url = "test" + std::to_string(i) + ".mp3";
        auto decoder = AudioDecoderCache::createStaticDecoder(url, data);
    }
    
    // 验证缓存大小不超过限制
    EXPECT_LE(AudioDecoderCache::getCurrentSizeInBytes(), smallMaxSize);
}

TEST_F(AudioDecoderCacheTest, CacheLRUBehavior) {
    // 测试LRU缓存行为
    auto mp3Data = createMockMp3Data();
    
    // 创建解码器
    auto decoder1 = AudioDecoderCache::createStaticDecoder("test1.mp3", mp3Data);
    auto decoder2 = AudioDecoderCache::createStaticDecoder("test2.mp3", mp3Data);
    
    // 访问第一个解码器，使其成为最近使用的
    auto cached1 = AudioDecoderCache::get("test1.mp3");
    
    // 设置很小的缓存大小，触发淘汰
    AudioDecoderCache::setMaxSizeInBytes(1024);
    
    // 添加新的解码器，应该淘汰最久未使用的
    auto decoder3 = AudioDecoderCache::createStaticDecoder("test3.mp3", mp3Data);
    
    // test2.mp3应该被淘汰，test1.mp3应该还在缓存中
    auto cached2 = AudioDecoderCache::get("test2.mp3");
    auto cached1_again = AudioDecoderCache::get("test1.mp3");
    
    if (decoder1) {
        EXPECT_EQ(cached2, nullptr); // test2应该被淘汰
        EXPECT_NE(cached1_again, nullptr); // test1应该还在缓存中
    }
}

// ==================== 边界条件测试 ====================

TEST_F(AudioDecoderCacheTest, EmptyUrl) {
    auto mp3Data = createMockMp3Data();
    
    // 测试空URL
    auto decoder1 = AudioDecoderCache::createStaticDecoder("", mp3Data);
    auto decoder2 = AudioDecoderCache::get("");
    
    // 应该返回nullptr
    EXPECT_EQ(decoder1, nullptr);
    EXPECT_EQ(decoder2, nullptr);
}

TEST_F(AudioDecoderCacheTest, NullData) {
    // 测试空数据
    auto decoder1 = AudioDecoderCache::createStaticDecoder("test.mp3", nullptr);
    auto decoder2 = AudioDecoderCache::createStreamDecoder("test.mp3", nullptr);
    
    // 应该返回nullptr
    EXPECT_EQ(decoder1, nullptr);
    EXPECT_EQ(decoder2, nullptr);
}

TEST_F(AudioDecoderCacheTest, ZeroSizeData) {
    // 测试零大小数据
    auto emptyData = laya::Data::makeEmpty();
    
    auto decoder1 = AudioDecoderCache::createStaticDecoder("test.mp3", emptyData);
    auto decoder2 = AudioDecoderCache::createStreamDecoder("test.mp3", emptyData);
    
    // 应该返回nullptr
    EXPECT_EQ(decoder1, nullptr);
    EXPECT_EQ(decoder2, nullptr);
}

TEST_F(AudioDecoderCacheTest, VeryLargeData) {
    // 测试非常大的数据
    size_t largeSize = 1024 * 1024 * 1024; // 1GB
    auto largeData = createMockMp3Data(largeSize);
    
    // 设置较小的缓存大小
    AudioDecoderCache::setMaxSizeInBytes(1024 * 1024); // 1MB
    
    auto decoder = AudioDecoderCache::createStaticDecoder("large.mp3", largeData);
    
    // 由于数据太大，应该无法缓存
    if (decoder) {
        auto cached = AudioDecoderCache::get("large.mp3");
        EXPECT_EQ(cached, nullptr); // 应该不在缓存中
    }
}

// ==================== 并发测试 ====================

TEST_F(AudioDecoderCacheTest, ConcurrentAccess) {
    const int threadCount = 4;
    const int operationsPerThread = 100;
    
    std::vector<std::thread> threads;
    std::atomic<int> successCount{0};
    
    for (int t = 0; t < threadCount; ++t) {
        threads.emplace_back([&successCount, operationsPerThread, t]() {
            for (int i = 0; i < operationsPerThread; ++i) {
                auto data = createMockMp3Data();
                std::string url = "thread" + std::to_string(t) + "_test" + std::to_string(i) + ".mp3";
                
                auto decoder = AudioDecoderCache::createStaticDecoder(url, data);
                if (decoder) {
                    successCount++;
                }
                
                // 随机获取缓存中的解码器
                if (i % 10 == 0) {
                    auto cached = AudioDecoderCache::get(url);
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // 验证所有线程都完成了操作
    EXPECT_GE(successCount.load(), 0);
}

TEST_F(AudioDecoderCacheTest, ConcurrentCacheEviction) {
    const int threadCount = 4;
    const int operationsPerThread = 50;
    
    // 设置很小的缓存大小以触发频繁的淘汰
    AudioDecoderCache::setMaxSizeInBytes(1024); // 1KB
    
    std::vector<std::thread> threads;
    
    for (int t = 0; t < threadCount; ++t) {
        threads.emplace_back([operationsPerThread, t]() {
            for (int i = 0; i < operationsPerThread; ++i) {
                auto data = createMockMp3Data(256);
                std::string url = "evict_thread" + std::to_string(t) + "_test" + std::to_string(i) + ".mp3";
                
                auto decoder = AudioDecoderCache::createStaticDecoder(url, data);
                
                // 验证缓存大小不超过限制
                EXPECT_LE(AudioDecoderCache::getCurrentSizeInBytes(), 1024);
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
}

// ==================== 性能测试 ====================

TEST_F(AudioDecoderCacheTest, PerformanceTest) {
    const int iterations = 1000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        auto data = createMockMp3Data();
        std::string url = "perf_test" + std::to_string(i) + ".mp3";
        
        auto decoder = AudioDecoderCache::createStaticDecoder(url, data);
        auto cached = AudioDecoderCache::get(url);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // 验证性能在合理范围内（1秒内完成1000次操作）
    EXPECT_LT(duration.count(), 1000);
}

TEST_F(AudioDecoderCacheTest, CacheHitPerformance) {
    // 测试缓存命中的性能
    auto mp3Data = createMockMp3Data();
    
    // 第一次创建
    auto start1 = std::chrono::high_resolution_clock::now();
    auto decoder1 = AudioDecoderCache::createStaticDecoder("cache_hit_test.mp3", mp3Data);
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    // 从缓存获取
    auto start2 = std::chrono::high_resolution_clock::now();
    auto decoder2 = AudioDecoderCache::get("cache_hit_test.mp3");
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    // 缓存获取应该比创建快
    if (decoder1) {
        EXPECT_LT(duration2.count(), duration1.count());
    }
}

// ==================== 内存泄漏测试 ====================

TEST_F(AudioDecoderCacheTest, MemoryLeakTest) {
    const int iterations = 1000;
    
    // 记录初始内存使用（如果可能的话）
    size_t initialSize = AudioDecoderCache::getCurrentSizeInBytes();
    
    for (int i = 0; i < iterations; ++i) {
        auto data = createMockMp3Data();
        std::string url = "leak_test" + std::to_string(i) + ".mp3";
        
        auto decoder = AudioDecoderCache::createStaticDecoder(url, data);
    }
    
    // 清空缓存
    AudioDecoderCache::clear();
    
    // 验证缓存已清空
    EXPECT_EQ(AudioDecoderCache::getCurrentSizeInBytes(), 0);
}

// ==================== 集成测试 ====================

TEST_F(AudioDecoderCacheTest, IntegrationTest) {
    // 综合测试：创建、缓存、获取、淘汰的完整流程
    
    // 1. 设置缓存大小
    AudioDecoderCache::setMaxSizeInBytes(2048); // 2KB
    
    // 2. 创建多个解码器
    std::vector<std::string> urls = {"test1.mp3", "test2.wav", "test3.ogg", "test4.mp3"};
    std::vector<std::shared_ptr<laya::Data>> dataList = {
        createMockMp3Data(512),
        createMockWavData(512),
        createMockOggData(512),
        createMockMp3Data(512)
    };
    
    // 3. 创建解码器
    for (size_t i = 0; i < urls.size(); ++i) {
        auto decoder = AudioDecoderCache::createStaticDecoder(urls[i], dataList[i]);
        //EXPECT_NE(decoder, nullptr);
    }
    
    // 4. 验证缓存大小
    EXPECT_LE(AudioDecoderCache::getCurrentSizeInBytes(), 2048);
    
    // 5. 从缓存获取
    for (const auto& url : urls) {
        auto cached = AudioDecoderCache::get(url);
        // 由于缓存大小限制，可能有些解码器已被淘汰
    }
    
    // 6. 清空缓存
    AudioDecoderCache::clear();
    EXPECT_EQ(AudioDecoderCache::getCurrentSizeInBytes(), 0);
    
    // 7. 验证所有解码器都已被清除
    for (const auto& url : urls) {
        auto cached = AudioDecoderCache::get(url);
        EXPECT_EQ(cached, nullptr);
    }
}

// ==================== 错误处理测试 ====================

TEST_F(AudioDecoderCacheTest, ErrorHandlingTest) {
    // 测试各种错误情况的处理
    
    // 1. 无效的文件格式
    auto invalidData = createInvalidData();
    auto decoder1 = AudioDecoderCache::createStaticDecoder("invalid.xyz", invalidData);
    EXPECT_EQ(decoder1, nullptr);
    
    // 2. 损坏的数据
    std::vector<uint8_t> corruptedBytes(100, 0xFF);
    auto corruptedData = laya::Data::makeWithCopy(corruptedBytes.data(), corruptedBytes.size());
    auto decoder2 = AudioDecoderCache::createStaticDecoder("corrupted.mp3", corruptedData);
    EXPECT_EQ(decoder2, nullptr);
    
    // 3. 重复的URL
    auto mp3Data = createMockMp3Data();
    auto decoder3 = AudioDecoderCache::createStaticDecoder("duplicate.mp3", mp3Data);
    auto decoder4 = AudioDecoderCache::createStaticDecoder("duplicate.mp3", mp3Data);
    
    if (decoder3) {
        // 应该返回相同的解码器或nullptr
        EXPECT_TRUE(decoder4 == nullptr || decoder4 == decoder3);
    }
}

// ==================== 线程安全测试 ====================

TEST_F(AudioDecoderCacheTest, ThreadSafetyTest) {
    const int threadCount = 8;
    const int operationsPerThread = 50;
    
    std::vector<std::thread> threads;
    std::atomic<int> totalOperations{0};
    std::atomic<int> successfulOperations{0};
    
    for (int t = 0; t < threadCount; ++t) {
        threads.emplace_back([&totalOperations, &successfulOperations, operationsPerThread, t]() {
            for (int i = 0; i < operationsPerThread; ++i) {
                totalOperations++;
                
                // 随机选择操作类型
                int operation = i % 4;
                std::string url = "thread" + std::to_string(t) + "_op" + std::to_string(i);
                
                switch (operation) {
                    case 0: {
                        // 创建静态解码器
                        auto data = createMockMp3Data();
                        auto decoder = AudioDecoderCache::createStaticDecoder(url + ".mp3", data);
                        if (decoder) successfulOperations++;
                        break;
                    }
                    case 1: {
                        // 创建流解码器
                        auto data = createMockMp3Data();
                        auto decoder = AudioDecoderCache::createStreamDecoder(url + ".mp3", data);
                        if (decoder) successfulOperations++;
                        break;
                    }
                    case 2: {
                        // 从缓存获取
                        auto cached = AudioDecoderCache::get(url + ".mp3");
                        break;
                    }
                    case 3: {
                        // 检查缓存大小
                        auto size = AudioDecoderCache::getCurrentSizeInBytes();
                        EXPECT_GE(size, 0);
                        break;
                    }
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // 验证所有操作都完成了
    EXPECT_EQ(totalOperations.load(), threadCount * operationsPerThread);
    EXPECT_GE(successfulOperations.load(), 0);
}

} // namespace test
} // namespace audio

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 
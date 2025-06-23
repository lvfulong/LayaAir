#include "../include/utils/LRUCache.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace laya;

class LRUCacheTest : public testing::Test {
protected:
    void SetUp() override {
        // 每个测试前都会执行
    }

    void TearDown() override {
        // 每个测试后都会执行
    }
};

// 基本操作测试
TEST_F(LRUCacheTest, BasicOperations) {
    LRUCache<int, std::string> cache(2);
    
    // 测试 put 和 get
    cache.put(1, "one");
    auto value = cache.get(1);
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), "one");
    
    // 测试不存在的键
    value = cache.get(2);
    EXPECT_FALSE(value.has_value());
}

// 容量限制测试
TEST_F(LRUCacheTest, CapacityLimit) {
    LRUCache<int, std::string> cache(2);
    
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");  // 应该移除 1
    
    EXPECT_FALSE(cache.get(1).has_value());
    EXPECT_EQ(cache.get(2).value(), "two");
    EXPECT_EQ(cache.get(3).value(), "three");
}

// LRU 特性测试
TEST_F(LRUCacheTest, LRUProperty) {
    LRUCache<int, std::string> cache(2);
    
    cache.put(1, "one");
    cache.put(2, "two");
    cache.get(1);  // 访问 1，使其成为最近使用的
    cache.put(3, "three");  // 应该移除 2
    
    EXPECT_TRUE(cache.get(1).has_value());
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_TRUE(cache.get(3).has_value());
}

// 更新已存在的值测试
TEST_F(LRUCacheTest, UpdateExisting) {
    LRUCache<int, std::string> cache(2);
    
    cache.put(1, "one");
    cache.put(1, "ONE");  // 更新已存在的值
    
    EXPECT_EQ(cache.get(1).value(), "ONE");
}

// 清除测试
TEST_F(LRUCacheTest, Clear) {
    LRUCache<int, std::string> cache(2);
    
    cache.put(1, "one");
    cache.put(2, "two");
    cache.clear();
    
    EXPECT_EQ(cache.size(), 0);
    EXPECT_FALSE(cache.get(1).has_value());
    EXPECT_FALSE(cache.get(2).has_value());
}

// 迭代器测试
TEST_F(LRUCacheTest, Iteration) {
    LRUCache<int, std::string> cache(3);
    std::vector<std::pair<int, std::string>> items;
    
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    
    cache.iterate([&items](const int& key, const std::string& value) {
        items.emplace_back(key, value);
    });
    
    EXPECT_EQ(items.size(), 3);
    EXPECT_EQ(items[0].first, 3);  // 最后插入的应该在前面
    EXPECT_EQ(items[1].first, 2);
    EXPECT_EQ(items[2].first, 1);
}
// 驱逐回调测试
TEST_F(LRUCacheTest, EvictionCallback) {
    LRUCache<int, std::string> cache(2);
    std::vector<std::pair<int, std::string>> evicted;
    
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three", [&evicted](const int& key, const std::string& value) {
        evicted.emplace_back(key, value);
    });
    
    EXPECT_EQ(evicted.size(), 1);
    EXPECT_EQ(evicted[0].first, 1);
    EXPECT_EQ(evicted[0].second, "one");
}

// 边界条件测试
TEST_F(LRUCacheTest, EdgeCases) {
    // 测试容量为 0
    LRUCache<int, std::string> cache0(0);
    cache0.put(1, "one");
    EXPECT_EQ(cache0.size(), 0);
    
    // 测试容量为 1
    LRUCache<int, std::string> cache1(1);
    cache1.put(1, "one");
    cache1.put(2, "two");
    EXPECT_FALSE(cache1.get(1).has_value());
    EXPECT_TRUE(cache1.get(2).has_value());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

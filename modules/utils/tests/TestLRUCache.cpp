#include "../include/utils/LRUCache.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <set>

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
    LRUCache<int, std::string> cache(3);
    
    // 测试清除有数据的缓存
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    
    EXPECT_EQ(cache.size(), 3);
    EXPECT_TRUE(cache.get(1).has_value());
    EXPECT_TRUE(cache.get(2).has_value());
    EXPECT_TRUE(cache.get(3).has_value());
    
    cache.clear();
    
    EXPECT_EQ(cache.size(), 0);
    EXPECT_FALSE(cache.get(1).has_value());
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_FALSE(cache.get(3).has_value());
    
    // 测试清除后可以重新使用
    cache.put(4, "four");
    cache.put(5, "five");
    EXPECT_EQ(cache.size(), 2);
    EXPECT_TRUE(cache.get(4).has_value());
    EXPECT_TRUE(cache.get(5).has_value());
    
    // 测试清除空缓存
    cache.clear();
    EXPECT_EQ(cache.size(), 0);
    
    // 测试多次清除
    cache.put(6, "six");
    cache.clear();
    cache.clear(); // 再次清除空缓存
    EXPECT_EQ(cache.size(), 0);
    
    // 测试清除后peekOldest的行为
    cache.put(7, "seven");
    cache.put(8, "eight");
    cache.clear();
    EXPECT_FALSE(cache.peekOldest().has_value());
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
    std::vector<std::pair<int, std::string>> evicted;
    LRUCache<int, std::string> cache(2, [&evicted](const int& key, const std::string& value) {
        evicted.emplace_back(key, value);
    });
   
    
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    
    EXPECT_EQ(evicted.size(), 1);
    EXPECT_EQ(evicted[0].first, 1);
    EXPECT_EQ(evicted[0].second, "one");
}

// 清除回调测试
TEST_F(LRUCacheTest, ClearCallback) {
    std::vector<std::pair<int, std::string>> cleared;
    LRUCache<int, std::string> cache(3, [&cleared](const int& key, const std::string& value) {
        cleared.emplace_back(key, value);
    });
    
    // 添加一些数据
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    
    EXPECT_EQ(cleared.size(), 0); // 还没有清除任何数据
    
    // 清除缓存，应该触发回调
    cache.clear();
    
    EXPECT_EQ(cleared.size(), 3);
    // 检查所有元素都被清除回调处理了
    std::set<int> clearedKeys;
    for (const auto& item : cleared) {
        clearedKeys.insert(item.first);
    }
    EXPECT_EQ(clearedKeys.size(), 3);
    EXPECT_TRUE(clearedKeys.find(1) != clearedKeys.end());
    EXPECT_TRUE(clearedKeys.find(2) != clearedKeys.end());
    EXPECT_TRUE(clearedKeys.find(3) != clearedKeys.end());
    
    // 测试清除空缓存不会触发回调
    cleared.clear();
    cache.clear();
    EXPECT_EQ(cleared.size(), 0);
}

// 边界条件测试
TEST_F(LRUCacheTest, EdgeCases) {
    // 测试容量为 0 - 修复后应该正确工作
    LRUCache<int, std::string> cache0(0);
    cache0.put(1, "one");
    EXPECT_EQ(cache0.size(), 0);
    EXPECT_FALSE(cache0.get(1).has_value());
    
    // 测试容量为 1
    LRUCache<int, std::string> cache1(1);
    cache1.put(1, "one");
    cache1.put(2, "two");
    EXPECT_FALSE(cache1.get(1).has_value());
    EXPECT_TRUE(cache1.get(2).has_value());
    
    // 测试空缓存的操作
    LRUCache<int, std::string> emptyCache(5);
    EXPECT_EQ(emptyCache.size(), 0);
    EXPECT_FALSE(emptyCache.get(1).has_value());
    EXPECT_FALSE(emptyCache.peekOldest().has_value());
    
    // 测试重复插入相同键
    LRUCache<int, std::string> cache2(2);
    cache2.put(1, "first");
    cache2.put(1, "second");
    EXPECT_EQ(cache2.size(), 1);
    EXPECT_EQ(cache2.get(1).value(), "second");
    
    // 测试清除空缓存
    emptyCache.clear();
    EXPECT_EQ(emptyCache.size(), 0);
    
    // 测试容量为1时的访问顺序
    LRUCache<int, std::string> cache3(1);
    cache3.put(1, "one");
    cache3.get(1);  // 访问但不应该影响LRU顺序
    cache3.put(2, "two");
    EXPECT_FALSE(cache3.get(1).has_value());
    EXPECT_TRUE(cache3.get(2).has_value());
}

// 专门测试容量为0的边界情况
TEST_F(LRUCacheTest, ZeroCapacity) {
    LRUCache<int, std::string> cache(0);
    
    // 测试put操作
    cache.put(1, "one");
    EXPECT_EQ(cache.size(), 0);
    EXPECT_FALSE(cache.get(1).has_value());
    
    // 测试多个put操作
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    EXPECT_EQ(cache.size(), 0);
    EXPECT_FALSE(cache.get(1).has_value());
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_FALSE(cache.get(3).has_value());
}

// 测试清除操作的内存管理和迭代器
TEST_F(LRUCacheTest, ClearMemoryAndIteration) {
    LRUCache<int, std::string> cache(5);
    
    // 添加数据
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    
    // 测试迭代器在清除前
    std::vector<std::pair<int, std::string>> items;
    cache.iterate([&items](const int& key, const std::string& value) {
        items.emplace_back(key, value);
    });
    EXPECT_EQ(items.size(), 3);
    
    // 清除缓存
    cache.clear();
    
    // 测试迭代器在清除后
    items.clear();
    cache.iterate([&items](const int& key, const std::string& value) {
        items.emplace_back(key, value);
    });
    EXPECT_EQ(items.size(), 0);
    
    // 测试清除后重新添加数据
    cache.put(4, "four");
    cache.put(5, "five");
    
    items.clear();
    cache.iterate([&items](const int& key, const std::string& value) {
        items.emplace_back(key, value);
    });
    EXPECT_EQ(items.size(), 2);
    
    // 验证迭代顺序（新添加的应该在前面）
    EXPECT_EQ(items[0].first, 5);
    EXPECT_EQ(items[1].first, 4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

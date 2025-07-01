# AudioDecoderCache 测试说明

## 概述

AudioDecoderCache测试套件为LayaNative3.0项目的音频解码器缓存功能提供了全面的测试覆盖。该测试包含单元测试、集成测试、性能测试和并发测试等多个方面。

## 测试文件结构

```
modules/audio/tests/
├── AudioDecoderCacheTest.cpp          # 主要测试文件
├── run_audio_decoder_cache_test.sh    # Linux/macOS运行脚本
├── run_audio_decoder_cache_test.bat   # Windows运行脚本
└── README_AudioDecoderCache_Test.md   # 本文档
```

## 测试覆盖范围

### 1. 基础功能测试
- **缓存管理**: 清空缓存、设置/获取最大缓存大小、获取当前缓存大小
- **静态解码器**: MP3、WAV、OGG格式的静态解码器创建和缓存
- **流解码器**: MP3、OGG格式的流解码器创建和缓存
- **缓存操作**: 从缓存获取解码器、缓存命中/未命中测试

### 2. 缓存策略测试
- **LRU淘汰**: 测试最近最少使用淘汰策略
- **容量限制**: 测试缓存大小限制和自动淘汰
- **内存管理**: 测试缓存大小计算和内存使用

### 3. 边界条件测试
- **空数据**: 测试空URL、空数据、零大小数据
- **大文件**: 测试超大文件的处理
- **无效格式**: 测试不支持的文件格式
- **重复操作**: 测试重复创建相同URL的解码器

### 4. 并发测试
- **多线程访问**: 测试多线程同时访问缓存的线程安全性
- **并发淘汰**: 测试多线程同时触发缓存淘汰
- **竞争条件**: 测试各种竞争条件的处理

### 5. 性能测试
- **创建性能**: 测试解码器创建的耗时
- **缓存命中性能**: 测试缓存命中的性能优势
- **内存泄漏**: 测试长时间运行是否存在内存泄漏

### 6. 集成测试
- **完整流程**: 测试创建、缓存、获取、淘汰的完整流程
- **错误处理**: 测试各种错误情况的处理
- **线程安全**: 测试多线程环境下的安全性

## 构建和运行

### 前置条件

1. 确保已安装CMake (3.16或更高版本)
2. 确保已安装Google Test框架
3. 确保已构建LayaNative3.0项目

### 构建测试

```bash
# 在项目根目录
mkdir build
cd build
cmake .. -DBUILD_TESTING=ON
make
```

### 运行测试

#### Linux/macOS
```bash
# 运行AudioDecoderCache测试
./modules/audio/tests/run_audio_decoder_cache_test.sh

# 详细模式运行
./modules/audio/tests/run_audio_decoder_cache_test.sh -v

# 运行所有音频测试
./modules/audio/tests/run_audio_decoder_cache_test.sh -a

# 指定构建目录
./modules/audio/tests/run_audio_decoder_cache_test.sh -b ../build
```

#### Windows
```cmd
# 运行AudioDecoderCache测试
modules\audio\tests\run_audio_decoder_cache_test.bat

# 详细模式运行
modules\audio\tests\run_audio_decoder_cache_test.bat -v

# 运行所有音频测试
modules\audio\tests\run_audio_decoder_cache_test.bat -a

# 指定构建目录
modules\audio\tests\run_audio_decoder_cache_test.bat -b ..\build
```

#### 直接运行
```bash
# 在构建目录中
./test_audio_decoder_cache

# 生成XML报告
./test_audio_decoder_cache --gtest_output=xml:test_results.xml

# 运行特定测试
./test_audio_decoder_cache --gtest_filter=AudioDecoderCacheTest.CacheLRUBehavior
```

## 测试用例详解

### 基础功能测试

#### ClearCache
测试缓存清空功能，验证：
- 清空后缓存大小为0
- 清空后无法获取之前缓存的解码器

#### SetAndGetMaxSize
测试缓存大小设置功能，验证：
- 可以正确设置和获取最大缓存大小
- 设置更大的值不会影响现有缓存

#### CreateStaticDecoderWithMp3/Wav/Ogg
测试不同格式的静态解码器创建，验证：
- 能够正确识别和创建MP3、WAV、OGG格式的解码器
- 创建的解码器类型正确
- 无效数据返回nullptr

### 缓存策略测试

#### CacheLRUBehavior
测试LRU缓存行为，验证：
- 最近使用的项目不会被淘汰
- 最久未使用的项目会被优先淘汰
- 访问项目会更新其使用时间

#### CacheEviction
测试缓存淘汰机制，验证：
- 当缓存大小超过限制时，会自动淘汰旧项目
- 淘汰后缓存大小不超过限制
- 淘汰策略正确执行

### 并发测试

#### ConcurrentAccess
测试多线程并发访问，验证：
- 多线程同时创建解码器不会崩溃
- 缓存操作是线程安全的
- 没有数据竞争问题

#### ThreadSafetyTest
测试线程安全性，验证：
- 各种操作的组合在多线程环境下是安全的
- 没有死锁或竞态条件
- 缓存状态保持一致

### 性能测试

#### PerformanceTest
测试基本性能，验证：
- 1000次操作能在1秒内完成
- 性能在可接受范围内

#### CacheHitPerformance
测试缓存命中性能，验证：
- 缓存获取比重新创建快
- 缓存机制确实提供了性能优势

## 测试数据

测试使用模拟的音频数据：

### MP3数据
- 包含ID3v2文件头
- 包含MP3帧头信息
- 模拟真实的MP3文件结构

### WAV数据
- 包含完整的WAV文件头
- 包含RIFF、fmt、data子块
- 模拟44.1kHz立体声PCM数据

### OGG数据
- 包含OGG文件头
- 包含页面信息
- 模拟OGG容器格式

### 无效数据
- 全FF字节数据
- 用于测试错误处理

## 测试结果分析

### 成功标准
- 所有测试用例通过
- 没有内存泄漏
- 性能指标达标
- 线程安全验证通过

### 失败处理
- 查看详细错误信息
- 检查测试环境配置
- 验证依赖库是否正确安装
- 检查构建配置

### 性能基准
- 1000次操作 < 1秒
- 缓存命中比创建快 > 50%
- 内存使用稳定，无泄漏

## 扩展测试

### 添加新测试用例
1. 在`AudioDecoderCacheTest.cpp`中添加新的`TEST_F`宏
2. 遵循现有的命名规范
3. 确保测试是独立的和可重复的

### 添加新的音频格式
1. 创建相应的模拟数据生成函数
2. 添加格式特定的测试用例
3. 更新测试辅助函数

### 性能基准更新
1. 根据实际使用情况调整性能基准
2. 定期更新性能测试的阈值
3. 监控性能回归

## 常见问题

### Q: 测试失败，提示找不到解码器
A: 检查是否正确安装了音频解码库（dr_libs、vorbis等）

### Q: 并发测试偶尔失败
A: 这可能是正常的，因为并发测试涉及随机性。多次运行确认是否稳定失败。

### Q: 性能测试超时
A: 检查系统资源使用情况，可能需要调整性能基准或优化测试代码。

### Q: 内存泄漏测试失败
A: 检查是否有全局变量或静态变量没有正确清理。

## 维护说明

### 定期维护
- 每月运行完整测试套件
- 检查性能基准是否需要调整
- 更新测试数据以覆盖新的音频格式

### 版本兼容性
- 测试新版本的依赖库
- 验证API变更对测试的影响
- 更新测试以适应新的接口

### 持续集成
- 将测试集成到CI/CD流程
- 设置自动化的性能回归检测
- 配置测试覆盖率报告

## 联系信息

如有问题或建议，请联系：
- 项目维护者
- 音频模块负责人
- 测试团队

---

*最后更新: 2024年* 
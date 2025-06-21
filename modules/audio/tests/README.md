# LayaNative Audio 测试套件

本目录包含了LayaNative音频系统的全面测试用例，用于验证音频功能的正确性和稳定性。

## 测试文件说明

### 1. ComprehensiveAudioTest.cpp
**全面音频功能测试**
- 基本功能测试：音频创建、播放控制
- 播放控制测试：播放、暂停、停止、音量控制、循环控制
- 位置控制测试：seek和tell功能
- 音频信息测试：时长、采样率、声道数等
- 多格式支持测试：MP3、WAV、OGG格式
- 错误处理测试：无效数据、空数据、空指针
- 性能测试：多音频创建、播放性能
- 并发测试：多音频同时播放
- 内存管理测试：资源释放
- 边界条件测试：异常参数处理
- 回调函数测试：播放结束回调

### 2. StreamDecoderTest.cpp
**流式解码器专门测试**
- StreamDecoderMp3基本功能测试
- 音频属性测试：采样率、声道数、位深度等
- 音频数据读取测试：单次读取、多次读取
- Seek功能测试：开始位置、中间位置、结束位置、多次seek
- Tell功能测试：初始位置、读取后位置
- Rewind功能测试：回到开始位置
- 循环播放测试：循环模式下的行为
- 结束状态测试：到达结束时的状态
- 错误处理测试：无效seek、未初始化解码器
- 格式检测测试：MP3格式识别
- 性能测试：seek操作性能

### 3. AudioTest.cpp
**基础音频测试**
- 基本的音频播放功能测试
- 简单的播放流程验证

### 4. DecoderTest.cpp
**解码器基础测试**
- 解码器创建和加载测试
- 无效数据处理测试

## 测试环境要求

### 硬件要求
- 支持音频播放的设备
- 足够的系统内存（建议4GB以上）

### 软件要求
- Windows 10/11
- Visual Studio 2019或更高版本
- CMake 3.16或更高版本
- Google Test框架

### 测试文件
测试需要以下音频文件（位于test/sound目录）：
- `test.mp3` - MP3格式测试文件
- `test.wav` - WAV格式测试文件
- `test.ogg` - OGG格式测试文件

## 构建和运行测试

### 1. 构建项目
```bash
# 在项目根目录
mkdir build
cd build
cmake .. -DBUILD_TESTING=ON
cmake --build . --config Release
```

### 2. 运行所有测试
```bash
# 使用批处理脚本（Windows）
cd modules/audio/tests
run_tests.bat
```

### 3. 运行单个测试
```bash
# 在build/bin目录下
./test_comprehensive_audio.exe
./test_stream_decoder.exe
./test_audio.exe
./test_decoder.exe
```

### 4. 使用Google Test选项
```bash
# 运行特定测试
./test_comprehensive_audio.exe --gtest_filter=ComprehensiveAudioTest.PlayPauseStop

# 详细输出
./test_comprehensive_audio.exe --gtest_verbose

# 生成XML报告
./test_comprehensive_audio.exe --gtest_output=xml:test_report.xml
```

## 测试覆盖范围

### 功能覆盖
- ✅ 音频创建和销毁
- ✅ 播放控制（播放、暂停、停止）
- ✅ 音量控制
- ✅ 循环播放
- ✅ 位置控制（seek、tell）
- ✅ 音频信息获取
- ✅ 多格式支持
- ✅ 错误处理
- ✅ 内存管理
- ✅ 并发处理
- ✅ 回调函数

### 格式支持
- ✅ MP3格式（静态和流式）
- ✅ WAV格式
- ✅ OGG格式

### 边界条件
- ✅ 无效数据
- ✅ 空数据
- ✅ 空指针
- ✅ 超出范围参数
- ✅ 负数参数

## 测试结果解读

### 成功标准
- 所有测试用例通过
- 无内存泄漏
- 性能指标在预期范围内
- 错误处理正确

### 常见问题
1. **测试文件不存在**
   - 确保test/sound目录下有相应的音频文件
   - 检查文件路径是否正确

2. **音频设备问题**
   - 确保系统有可用的音频设备
   - 检查音频驱动是否正常

3. **构建失败**
   - 检查CMake配置
   - 确保所有依赖库已正确链接

4. **测试超时**
   - 某些测试可能需要较长时间
   - 可以调整测试超时时间

## 性能基准

### 预期性能指标
- 音频创建时间：< 100ms
- Seek操作时间：< 50ms
- 内存使用：< 100MB（单个音频）
- 并发播放：支持至少8个音频同时播放

### 性能测试
```bash
# 运行性能测试
./test_comprehensive_audio.exe --gtest_filter=*Performance*
```

## 持续集成

### 自动化测试
测试套件支持CI/CD集成：
- 可以生成XML格式的测试报告
- 支持并行测试执行
- 提供详细的失败信息

### 测试报告
```bash
# 生成详细报告
./test_comprehensive_audio.exe --gtest_output=xml:audio_test_report.xml
```

## 维护和扩展

### 添加新测试
1. 在相应的测试文件中添加新的TEST_F函数
2. 更新CMakeLists.txt（如果需要新的测试可执行文件）
3. 更新此文档

### 测试数据
- 测试音频文件应保持较小体积（< 1MB）
- 音频时长建议在10-30秒之间
- 使用标准格式，避免特殊编码

### 问题报告
如发现测试问题，请提供：
- 测试环境信息
- 错误日志
- 复现步骤
- 期望行为

## 联系信息

如有问题或建议，请联系音频模块开发团队。 
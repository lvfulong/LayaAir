#!/bin/bash

# AudioDecoderCache测试运行脚本
# 使用方法: ./run_audio_decoder_cache_test.sh [选项]

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 默认参数
BUILD_DIR="build"
TEST_TARGET="test_audio_decoder_cache"
VERBOSE=false
RUN_ALL=false

# 帮助信息
show_help() {
    echo "AudioDecoderCache测试运行脚本"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  -h, --help          显示此帮助信息"
    echo "  -b, --build-dir DIR 指定构建目录 (默认: build)"
    echo "  -t, --test TARGET   指定测试目标 (默认: test_audio_decoder_cache)"
    echo "  -v, --verbose       详细输出"
    echo "  -a, --all           运行所有音频测试"
    echo ""
    echo "示例:"
    echo "  $0                    # 运行默认测试"
    echo "  $0 -v                 # 详细模式运行"
    echo "  $0 -a                 # 运行所有音频测试"
    echo "  $0 -b ../build        # 指定构建目录"
}

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -b|--build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        -t|--test)
            TEST_TARGET="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -a|--all)
            RUN_ALL=true
            shift
            ;;
        *)
            echo -e "${RED}错误: 未知选项 $1${NC}"
            show_help
            exit 1
            ;;
    esac
done

# 检查构建目录是否存在
if [[ ! -d "$BUILD_DIR" ]]; then
    echo -e "${RED}错误: 构建目录 '$BUILD_DIR' 不存在${NC}"
    echo "请先运行 cmake 和 make 构建项目"
    exit 1
fi

# 进入构建目录
cd "$BUILD_DIR"

echo -e "${BLUE}=== AudioDecoderCache测试开始 ===${NC}"
echo "构建目录: $(pwd)"
echo "测试目标: $TEST_TARGET"
echo ""

# 检查测试可执行文件是否存在
if [[ ! -f "$TEST_TARGET" ]]; then
    echo -e "${RED}错误: 测试可执行文件 '$TEST_TARGET' 不存在${NC}"
    echo "请确保已经构建了测试目标"
    exit 1
fi

# 运行测试
echo -e "${YELLOW}运行 AudioDecoderCache 测试...${NC}"

if [[ "$VERBOSE" == true ]]; then
    # 详细模式
    ./$TEST_TARGET --gtest_color=yes --gtest_output=xml:test_results.xml
else
    # 标准模式
    ./$TEST_TARGET --gtest_color=yes --gtest_output=xml:test_results.xml 2>/dev/null
fi

# 检查测试结果
if [[ $? -eq 0 ]]; then
    echo -e "${GREEN}✓ AudioDecoderCache 测试通过${NC}"
else
    echo -e "${RED}✗ AudioDecoderCache 测试失败${NC}"
    exit 1
fi

# 如果指定运行所有测试
if [[ "$RUN_ALL" == true ]]; then
    echo ""
    echo -e "${BLUE}=== 运行所有音频测试 ===${NC}"
    
    # 定义所有音频测试
    declare -a audio_tests=(
        "test_decoder"
        "test_audio"
        "test_comprehensive_audio"
        "test_stream_decoder"
        "test_performance_audio"
        "test_stream_decoder_ogg"
        "test_stream_decoder_ogg_real"
        "test_audio_decoder_cache"
    )
    
    failed_tests=()
    
    for test in "${audio_tests[@]}"; do
        if [[ -f "$test" ]]; then
            echo -e "${YELLOW}运行 $test...${NC}"
            if ./$test --gtest_color=yes >/dev/null 2>&1; then
                echo -e "${GREEN}✓ $test 通过${NC}"
            else
                echo -e "${RED}✗ $test 失败${NC}"
                failed_tests+=("$test")
            fi
        else
            echo -e "${YELLOW}跳过 $test (文件不存在)${NC}"
        fi
    done
    
    # 显示总结
    echo ""
    echo -e "${BLUE}=== 测试总结 ===${NC}"
    if [[ ${#failed_tests[@]} -eq 0 ]]; then
        echo -e "${GREEN}所有测试都通过了！${NC}"
    else
        echo -e "${RED}以下测试失败:${NC}"
        for test in "${failed_tests[@]}"; do
            echo -e "${RED}  - $test${NC}"
        done
        exit 1
    fi
fi

echo ""
echo -e "${GREEN}测试完成！${NC}"

# 显示测试结果文件位置
if [[ -f "test_results.xml" ]]; then
    echo "测试结果已保存到: $(pwd)/test_results.xml"
fi 
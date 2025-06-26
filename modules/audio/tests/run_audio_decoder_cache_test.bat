@echo off
REM AudioDecoderCache测试运行脚本 (Windows版本)
REM 使用方法: run_audio_decoder_cache_test.bat [选项]

setlocal enabledelayedexpansion

REM 默认参数
set BUILD_DIR=build
set TEST_TARGET=test_audio_decoder_cache.exe
set VERBOSE=false
set RUN_ALL=false

REM 解析命令行参数
:parse_args
if "%~1"=="" goto :end_parse
if "%~1"=="-h" goto :show_help
if "%~1"=="--help" goto :show_help
if "%~1"=="-b" (
    set BUILD_DIR=%~2
    shift
    shift
    goto :parse_args
)
if "%~1"=="--build-dir" (
    set BUILD_DIR=%~2
    shift
    shift
    goto :parse_args
)
if "%~1"=="-t" (
    set TEST_TARGET=%~2
    shift
    shift
    goto :parse_args
)
if "%~1"=="--test" (
    set TEST_TARGET=%~2
    shift
    shift
    goto :parse_args
)
if "%~1"=="-v" (
    set VERBOSE=true
    shift
    goto :parse_args
)
if "%~1"=="--verbose" (
    set VERBOSE=true
    shift
    goto :parse_args
)
if "%~1"=="-a" (
    set RUN_ALL=true
    shift
    goto :parse_args
)
if "%~1"=="--all" (
    set RUN_ALL=true
    shift
    goto :parse_args
)
echo 错误: 未知选项 %~1
goto :show_help

:show_help
echo AudioDecoderCache测试运行脚本 (Windows版本)
echo.
echo 用法: %0 [选项]
echo.
echo 选项:
echo   -h, --help          显示此帮助信息
echo   -b, --build-dir DIR 指定构建目录 (默认: build)
echo   -t, --test TARGET   指定测试目标 (默认: test_audio_decoder_cache.exe)
echo   -v, --verbose       详细输出
echo   -a, --all           运行所有音频测试
echo.
echo 示例:
echo   %0                    # 运行默认测试
echo   %0 -v                 # 详细模式运行
echo   %0 -a                 # 运行所有音频测试
echo   %0 -b ..\build        # 指定构建目录
exit /b 1

:end_parse

REM 检查构建目录是否存在
if not exist "%BUILD_DIR%" (
    echo 错误: 构建目录 '%BUILD_DIR%' 不存在
    echo 请先运行 cmake 和 make 构建项目
    exit /b 1
)

REM 进入构建目录
cd /d "%BUILD_DIR%"

echo === AudioDecoderCache测试开始 ===
echo 构建目录: %CD%
echo 测试目标: %TEST_TARGET%
echo.

REM 检查测试可执行文件是否存在
if not exist "%TEST_TARGET%" (
    echo 错误: 测试可执行文件 '%TEST_TARGET%' 不存在
    echo 请确保已经构建了测试目标
    exit /b 1
)

REM 运行测试
echo 运行 AudioDecoderCache 测试...

if "%VERBOSE%"=="true" (
    REM 详细模式
    %TEST_TARGET% --gtest_color=yes --gtest_output=xml:test_results.xml
) else (
    REM 标准模式
    %TEST_TARGET% --gtest_color=yes --gtest_output=xml:test_results.xml >nul 2>&1
)

REM 检查测试结果
if %ERRORLEVEL% EQU 0 (
    echo ✓ AudioDecoderCache 测试通过
) else (
    echo ✗ AudioDecoderCache 测试失败
    exit /b 1
)

REM 如果指定运行所有测试
if "%RUN_ALL%"=="true" (
    echo.
    echo === 运行所有音频测试 ===
    
    REM 定义所有音频测试
    set audio_tests=test_decoder.exe test_audio.exe test_comprehensive_audio.exe test_stream_decoder.exe test_performance_audio.exe test_stream_decoder_ogg.exe test_stream_decoder_ogg_real.exe test_audio_decoder_cache.exe
    
    set failed_tests=
    
    for %%t in (%audio_tests%) do (
        if exist "%%t" (
            echo 运行 %%t...
            %%t --gtest_color=yes >nul 2>&1
            if !ERRORLEVEL! EQU 0 (
                echo ✓ %%t 通过
            ) else (
                echo ✗ %%t 失败
                set failed_tests=!failed_tests! %%t
            )
        ) else (
            echo 跳过 %%t (文件不存在)
        )
    )
    
    REM 显示总结
    echo.
    echo === 测试总结 ===
    if "%failed_tests%"=="" (
        echo 所有测试都通过了！
    ) else (
        echo 以下测试失败:
        for %%t in (%failed_tests%) do echo   - %%t
        exit /b 1
    )
)

echo.
echo 测试完成！

REM 显示测试结果文件位置
if exist "test_results.xml" (
    echo 测试结果已保存到: %CD%\test_results.xml
)

endlocal 
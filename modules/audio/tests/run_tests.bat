@echo off
echo ========================================
echo 运行 LayaNative Audio 测试套件
echo ========================================

REM 设置测试文件路径
set TEST_DIR=%~dp0
set BUILD_DIR=%TEST_DIR%..\..\..\build\bin

REM 检查构建目录是否存在
if not exist "%BUILD_DIR%" (
    echo 错误: 构建目录不存在: %BUILD_DIR%
    echo 请先运行 CMake 构建项目
    pause
    exit /b 1
)

echo 构建目录: %BUILD_DIR%
echo.

REM 运行各个测试
echo 1. 运行 Decoder 测试...
if exist "%BUILD_DIR%\test_decoder.exe" (
    "%BUILD_DIR%\test_decoder.exe"
    if errorlevel 1 (
        echo Decoder 测试失败!
        set FAILED=1
    ) else (
        echo Decoder 测试通过!
    )
) else (
    echo 警告: test_decoder.exe 不存在
    set FAILED=1
)
echo.

echo 2. 运行 Audio 测试...
if exist "%BUILD_DIR%\test_audio.exe" (
    "%BUILD_DIR%\test_audio.exe"
    if errorlevel 1 (
        echo Audio 测试失败!
        set FAILED=1
    ) else (
        echo Audio 测试通过!
    )
) else (
    echo 警告: test_audio.exe 不存在
    set FAILED=1
)
echo.

echo 3. 运行 StreamDecoder 测试...
if exist "%BUILD_DIR%\test_stream_decoder.exe" (
    "%BUILD_DIR%\test_stream_decoder.exe"
    if errorlevel 1 (
        echo StreamDecoder 测试失败!
        set FAILED=1
    ) else (
        echo StreamDecoder 测试通过!
    )
) else (
    echo 警告: test_stream_decoder.exe 不存在
    set FAILED=1
)
echo.

echo 4. 运行 Comprehensive Audio 测试...
if exist "%BUILD_DIR%\test_comprehensive_audio.exe" (
    "%BUILD_DIR%\test_comprehensive_audio.exe"
    if errorlevel 1 (
        echo Comprehensive Audio 测试失败!
        set FAILED=1
    ) else (
        echo Comprehensive Audio 测试通过!
    )
) else (
    echo 警告: test_comprehensive_audio.exe 不存在
    set FAILED=1
)
echo.

echo ========================================
if defined FAILED (
    echo 测试结果: 部分测试失败
    exit /b 1
) else (
    echo 测试结果: 所有测试通过!
    exit /b 0
)
echo ========================================

pause 
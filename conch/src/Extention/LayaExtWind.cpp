
#include "LayaExtWin.h"
#include <windows.h>
#include <string>
#include <filesystem>
#include <vector>
#include <iostream>
#include <codecvt>
#include <locale>

namespace fs = std::filesystem;

namespace laya{

    typedef void (*LayaInitFunc)(void *);

    void importAllDynaLib(std::string exePath, void *ctx)
    {
        // 将 std::string 转换为 std::wstring
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wExePath = converter.from_bytes(exePath);        

        //当前目录
        fs::path currentPath(std::move(wExePath));

        // 组合扩展目录路径
        fs::path extensionPath = currentPath / fs::path( L"extensions");

        // 检查扩展目录是否存在
        if (!fs::exists(extensionPath))
        {
            return;
        }

        // 遍历扩展目录中的所有文件
        for (const auto &entry : fs::directory_iterator(extensionPath))
        {
            if (entry.path().extension() == std::wstring(L".dll"))
            {
                // 加载 DLL
                HMODULE hModule = LoadLibraryW(entry.path().c_str());
                if (hModule != NULL)
                {
                    // 获取 layainit 函数地址
                    LayaInitFunc layainit = (LayaInitFunc)GetProcAddress(hModule, "layainit_js");
                    if (layainit != NULL)
                    {
                        // 调用 layainit 函数
                        layainit(ctx);
                    }
                    else
                    {
                    }
                }
                else
                {
                }
            }
        }
}    
}
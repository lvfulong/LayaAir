
#include "LayaExtWin.h"
#include <windows.h>
#include <string>
#include <filesystem>
#include <vector>
#include <iostream>
#include <codecvt>
#include <locale>
namespace fs = std::filesystem;

#include "jsvm/JSEnv.h"
extern std::string gRedistPath;

namespace laya
{

    typedef void (*LayaInitFunc)(jsvm_env, jsvm_value);

#if defined(OS_WINDOWS)
    jsvm_value importNative_win(std::string dll){
        std::string exePath = gRedistPath;
        GET_ENV
        fs::path dllPath;
        // 检查是否是绝对路径
        if (fs::path(dll).is_absolute()){
            dllPath = dll;
        }else{
            // 相对路径，构建完整路径
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wExePath = converter.from_bytes(exePath);
            fs::path exeDir = fs::path(wExePath).parent_path();
            dllPath = exeDir / "extensions" / dll;
        }

        // 确保文件存在并且扩展名为 .dll
        if (!fs::exists(dllPath) || dllPath.extension() != ".dll"){
            LOGE("DLL file not found or invalid: %s", dllPath.string().c_str());
            return jsvm_value();
        }

        // 加载 DLL
        HMODULE hModule = LoadLibraryW(dllPath.c_str());
        if (hModule == NULL){
            DWORD error = GetLastError();
            wchar_t msgBuf[512];
            FormatMessageW(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                error,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                msgBuf,
                sizeof(msgBuf) / sizeof(wchar_t),
                NULL);
            LOGE("Failed to load DLL: %s. Error: %d - %ls", dllPath.string().c_str(), error, msgBuf);
            return jsvm_value();
        }

        // 获取 LayaExtInit 函数地址
        LayaInitFunc layainit = (LayaInitFunc)GetProcAddress(hModule, "LayaExtInit");
        if (layainit == NULL){
            LOGE("LayaExtInit function not found in DLL: %s", dllPath.string().c_str());
            FreeLibrary(hModule);
            return jsvm_value();
        }

        // 调用 LayaExtInit 函数
        jsvm_value export_obj;
        jsvm_create_object(env, &export_obj);
        layainit(env, export_obj);

        return export_obj;
    }
#endif

    jsvm_value importNative(std::string dll){
#if defined(OS_WINDOWS)
        return importNative_win(dll);
#else
        LOGE("只支持windows平台");
        GET_ENV;
        jsvm_value nul;
        jsvm_get_null(env,&nul);
        return nul;
#endif
    }
}
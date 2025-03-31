#if defined(OS_WINDOWS)
#include "LayaExtWin.h"
#include <windows.h>
#include <string>
#include <filesystem>
#include <vector>
#include <iostream>
#include <codecvt>
#include <locale>
#include <utils/JCCommonMethod.h>
#include "platform/OS.h"

namespace fs = std::filesystem;

#include "jsvm/JSEnv.h"

namespace laya
{

    typedef void (*LayaInitFunc)(jsvm_env, jsvm_value);

    jsvm_value importNative_win(std::string dll){
        std::string exePath = fs::path(OS::getExecutablePath()).parent_path().string();
        GET_ENV
        fs::path dllPath;
	    fs::path dllDir;
        // 检查是否是绝对路径
        if (fs::path(dll).is_absolute()){
            dllPath = dll;
			dllDir = dllPath.parent_path();
        }else{
            // 相对路径，构建完整路径
            //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            //std::wstring wExePath = converter.from_bytes(exePath);
            std::wstring wExePath = utf8ToWide(exePath);
            fs::path exeDir = fs::path(wExePath);
        	dllDir = exeDir / "extensions";
      		dllPath = dllDir / dll;
        }

        // 确保文件存在并且扩展名为 .dll
        if (!fs::exists(dllPath) || dllPath.extension() != ".dll"){
            LOGE("DLL file not found or invalid: %s", dllPath.string().c_str());
            return jsvm_value();
        }

        // 保存当前 DLL 搜索目录
        WCHAR oldDllDirectory[MAX_PATH];
        DWORD oldDllDirectoryLength = GetDllDirectory(MAX_PATH, oldDllDirectory);

    	// 设置新的 DLL 搜索目录
    	SetDllDirectory(dllDir.c_str());
        // 加载 DLL
        HMODULE hModule = LoadLibraryW(dllPath.c_str());
	    // 恢复原来的 DLL 搜索目录
        if(oldDllDirectoryLength>0)
	        SetDllDirectory(oldDllDirectory);
        else
            SetDllDirectory(NULL);
		
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

    jsvm_value importNative(std::string dll){
        return importNative_win(dll);
        // LOGE("只支持windows平台");
        // GET_ENV;
        // jsvm_value nul;
        // jsvm_get_null(env,&nul);
        // return nul;
    }

void importAllDynaLib(std::string extPath)
    {
        GET_ENV
        // 将 std::string 转换为 std::wstring
        std::wstring wExtPath = utf8ToWide(extPath);

        //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        //std::wstring wExtPath = converter.from_bytes(extPath);        

        //当前目录
        fs::path currentPath(std::move(wExtPath));

        // 检查扩展目录是否存在
        if (!fs::exists(currentPath))
        {
            return;
        }

        // 保存原来的 DLL 搜索路径
        WCHAR oldDllDirectory[MAX_PATH];
        DWORD oldDllDirectoryLength = GetDllDirectory(MAX_PATH, oldDllDirectory);

        // 将当前路径添加到 DLL 搜索路径
        SetDllDirectoryW(currentPath.c_str());
        // 遍历扩展目录中的所有文件
        for (const auto &entry : fs::directory_iterator(currentPath))
        {
            if (entry.path().extension() == std::wstring(L".dll"))
            {
                // 加载 DLL
                HMODULE hModule = LoadLibraryW(entry.path().c_str());
                if (hModule != NULL){
                    // 获取 LayaExtInit 函数地址
                    LayaInitFunc layainit = (LayaInitFunc)GetProcAddress(hModule, "LayaExtInit");
                    if (layainit == NULL){
                        //LOGE("LayaExtInit function not found in DLL: %s", dllPath.string().c_str());
                        FreeLibrary(hModule);
                        continue;
                    }

                    // 调用 LayaExtInit 函数
                    jsvm_value export_obj;
                    jsvm_create_object(env, &export_obj);
                    layainit(env, export_obj);
                    //添加
                    std::string dllname = entry.path().stem().string();
                    jsvm_value global;
                    jsvm_get_global(env, &global);
                    jsvm_set_named_property(env, global, dllname.c_str(), export_obj);
                }
                else
                {
                    LOGE("Failed to load DLL: %s", entry.path().string().c_str());
                }
            }
        }
        // 恢复原来的 DLL 搜索路径
        if (oldDllDirectoryLength > 0)
        {
            SetDllDirectoryW(oldDllDirectory);
        }
        else
        {
            SetDllDirectoryW(NULL);
        }
    }
}

#endif
// LayaBox.cpp : 定义应用程序的入口点。
//

#include "LayaBox.h"
#include "ExportsShared.h"
#include "framework.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    conchSetHandleMessageCallback(
        [](const std::string &eventName, const std::string &data) -> std::string { return ""; },
        [](const std::string &eventName, const std::string &data, handleResultCallback resultCallback) -> void {
            resultCallback("");
        });
    return conchMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

#include "stdafx.h"
#if defined(BUILDING_CONCH_FROME_SOURCE)
#include "Exports.h"
#else
#include "ExportsShared.h"
#endif

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    conchSetHandleMessageCallback(
        [](const std::string &eventName, const std::string &data) -> std::string { return "sync result"; },
        [](const std::string &eventName, const std::string &data, handleResultCallback resultCallback) -> void {
            resultCallback("async result");
        });
    return conchMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
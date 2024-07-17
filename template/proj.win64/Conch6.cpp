#include "stdafx.h"
#if BUILDING_CONCH_FROME_SOURCE
#include "Exports.h"
#else
#include "ExportsShared.h"
#endif

int main(int argc, _TCHAR *argv[])
{
    conchSetHandleMessageCallback(
        [](const std::string &eventName, const std::string &data) -> std::string { return "sync result"; },
        [](const std::string &eventName, const std::string &data, handleResultCallback resultCallback) -> void {
            resultCallback("async result");
        });
    return conchMainConsole(argc, argv);
}

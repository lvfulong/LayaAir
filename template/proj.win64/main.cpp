#include "stdafx.h"
#if defined(BUILDING_CONCH_FROME_SOURCE)
#include "Exports.h"
#else
#include "ExportsShared.h"
#endif

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    conchSetHandleMessageCallback(
        [](const char *eventName, const char *data) -> void { conchSendHandleMessageResult(eventName, "sync result"); },
        [](const char *eventName, const char *data) -> void {
            conchSendHandleMessageResult(eventName, "async result");
        });
    return conchMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
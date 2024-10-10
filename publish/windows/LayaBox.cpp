// LayaBox.cpp : 定义应用程序的入口点。
//

#include "LayaBox.h"
#include "ExportsShared.h"
#include "framework.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    conchSetHandleMessageCallback(
        [](const char *eventName, const char *data) -> void { conchSendHandleMessageResult(eventName, "sync result"); },
        [](const char *eventName, const char *data) -> void {
            conchSendHandleMessageResult(eventName, "async result");
        });
    return conchMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

#include <core/Thread.h>
#include <utils/Log.h>
#include <utils/MessageLoop.h>

namespace laya
{
static MessageLoop *s_scriptMessageLoop = nullptr;
void initializeCurrentThreadAsScriptThread()
{
    MessageLoop::initializeMessageLoopForCurrentThread();
    DEBUG_CHECK(s_scriptMessageLoop == nullptr);
    s_scriptMessageLoop = &MessageLoop::getCurrent();
}
bool isScriptThread()
{
    return &MessageLoop::getCurrent() == s_scriptMessageLoop;
}
void postToJS(std::function<void(void)> task)
{
    if (isScriptThread())
    {
        task();
    }
    else
    {
        s_scriptMessageLoop->post(task);
    }
}
} // namespace laya

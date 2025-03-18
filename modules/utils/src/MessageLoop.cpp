#include <utils/Log.h>
#include <utils/MessageLoop.h>
#include <utils/MessageLoopImpl.h>

namespace laya
{
static thread_local std::unique_ptr<MessageLoop> tls_messageLoop;
MessageLoop::MessageLoop() : m_impl(MessageLoopImpl::create())
{
}
MessageLoop::~MessageLoop() = default;
MessageLoop &MessageLoop::getCurrent()
{
    auto *loop = tls_messageLoop.get();
    DEBUG_CHECK(loop != nullptr);
    return *loop;
}
void MessageLoop::initializeMessageLoopForCurrentThread()
{
    auto *loop = tls_messageLoop.get();
    DEBUG_CHECK(loop == nullptr);
    tls_messageLoop = std::make_unique<MessageLoop>();
}
} // namespace laya

#include <utils/MessageLoopGeneric.h>
#include <utils/MessageLoopImpl.h>

namespace laya
{
std::unique_ptr<MessageLoopImpl> MessageLoopImpl::create()
{
    return std::make_unique<MessageLoopGeneric>();
}
} // namespace laya

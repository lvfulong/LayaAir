#ifndef __MESSAGELOOPIMPL_H__
#define __MESSAGELOOPIMPL_H__

#include <memory>

namespace laya
{

class MessageLoopImpl
{
  public:
    virtual ~MessageLoopImpl() = default;
    static std::unique_ptr<MessageLoopImpl> create();
    virtual void enqueue(nano_seconds_t time, std::function<void()> &&task) = 0;
    virtual void processExpiredTasks() = 0;

  private:
};
} // namespace laya
#endif
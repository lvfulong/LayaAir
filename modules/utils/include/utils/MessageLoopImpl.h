#ifndef __MESSAGELOOPIMPL_H__
#define __MESSAGELOOPIMPL_H__

#include <memory>
#include <utils/Time.h>

namespace laya
{

class MessageLoopImpl
{
  public:
    virtual ~MessageLoopImpl() = default;
    static std::unique_ptr<MessageLoopImpl> create();
    virtual void enqueue(nano_seconds_t time, std::function<void()> &&task) = 0;
    virtual void iterate() = 0;
    virtual void stop() = 0;
  private:
};
} // namespace laya
#endif
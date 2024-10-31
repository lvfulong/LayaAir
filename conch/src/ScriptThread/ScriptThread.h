#ifndef __SCRIPT_THREAD_H__
#define __SCRIPT_THREAD_H__

#include <binder/JSVM.h>
#include <mutex>
#include <thread>
#include <utils/ListNode.h>
#include <utils/Log.h>
#include <utils/thread/JCWorkerThread.h>

namespace laya
{
class ScriptThread : public JCWorkerThread
{
  public:
    virtual ~ScriptThread()
    {
    }
    ScriptThread()
    {
    }
    virtual void _defRunLoop();

    virtual void _runLoop();

  private:
    jsvm::VM m_vm;
};
} // namespace laya
#endif
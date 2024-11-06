#ifndef __JSBIND_SCRIPT_THREAD_H__
#define __JSBIND_SCRIPT_THREAD_H__

#include <binder/JSVM.h>
#include <mutex>
#include <thread>
#include <utils/ListNode.h>
#include <utils/Log.h>
#include <utils/thread/JCWorkerThread.h>

namespace jsbind
{
class ScriptThread : public laya::JCWorkerThread
{
  public:
    virtual ~ScriptThread()
    {
    }
    ScriptThread()
    {
    }
    void _defRunLoop() override;

    void _runLoop() override;

  private:
    jsvm::VM m_vm;
};
} // namespace jsbind
#endif
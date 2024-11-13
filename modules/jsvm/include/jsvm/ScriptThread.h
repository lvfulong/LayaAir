#ifndef __JSVM_SCRIPT_THREAD_H__
#define __JSVM_SCRIPT_THREAD_H__

#include <jsvm/JSVM.h>
#include <mutex>
#include <thread>
#include <utils/ListNode.h>
#include <utils/Log.h>
#include <utils/thread/JCWorkerThread.h>

namespace jsvm
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
    void runLoop(jsvm::Env env);

    void _runLoop() override;

  private:
    jsvm::VM m_vm;
};
} // namespace jsbind
#endif
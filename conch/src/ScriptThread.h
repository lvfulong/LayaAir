#ifndef __SCRIPT_THREAD_H__
#define __SCRIPT_THREAD_H__

#include <jsvm/JSVM.h>
#include <mutex>
#include <thread>
#include <utils/ListNode.h>
#include <utils/Log.h>
#include <utils/thread/JCWorkerThread.h>

namespace laya
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
    void runLoop(jsvm_env env);

    void _runLoop() override;
  private:
    jsvm_vm m_vm;
    
};
} // namespace jsvm
#endif
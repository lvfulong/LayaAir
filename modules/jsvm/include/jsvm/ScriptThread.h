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

    void pushDbgFunc(std::function<void(void)> task)
    {
        m_DbgFuncLock.lock();
        m_DbgFunction.push_back(task);
        m_DbgFuncLock.unlock();
    }
    void runDbgFuncs()
    {
        m_DbgFuncLock.lock();
        for (std::function<void(void)> &task : m_DbgFunction)
        {
            task();
        }
        m_DbgFunction.clear();
        m_DbgFuncLock.unlock();
    }
    void waitAndRunDbgFuncs()
    {
        m_DbgFuncLock.lock();
        m_DbgFuncLock.unlock();
    }
    bool hasDbgFuncs()
    {
        bool bRet = false;
        m_DbgFuncLock.lock();
        bRet = m_DbgFunction.size() > 0;
        m_DbgFuncLock.unlock();
        return bRet;
    }

  private:
    jsvm::VM m_vm;
    std::vector<std::function<void(void)>> m_DbgFunction; // 调试函数
    std::mutex m_DbgFuncLock;

};
} // namespace jsbind
#endif
#include "ScriptThread.h"

namespace laya
{

void ScriptThread::_defRunLoop()
{
#ifdef OS_WINDOWS
    {
        DWORD thid = GetCurrentThreadId();
        SetNameInternal(thid, m_strName.c_str());
        // threadInfoLog("start thread:%s,%d", m_strName.c_str(), thid);
    }
#elif defined(OS_ANDROID)
    {
        // threadInfoLog("start thread:%s,%ld", m_strName.c_str(), gettidv1());
    }
#endif
    // 开始事件
    JCEventEmitter::evtPtr startEvt(new JCEventBase);
    startEvt->m_nID = JCWorkerThread::Event_threadStart;
    emit(startEvt);
    JCWorkerThread::runObj task;
    auto isolate = v8::Isolate::GetCurrent();
    while (!m_bStop)
    {
        v8::HandleScope handle_scope(isolate);
        v8::TryCatch trycatch(isolate);
        if (!m_funcLoop)
        {
            // 现在的waitdata返回false不再表示要退出。事件唤醒流程
            if (m_ThreadTasks.WaitData(&task))
                task();
        }
        else
        {
            while (v8::platform::PumpMessageLoop(m_pJS->m_pPlatform, isolate,
                                                 v8::platform::MessageLoopBehavior::kDoNotWait))
            {
                continue;
            }
            isolate->PerformMicrotaskCheckpoint();
            // 固定循环流程
            runQueue();
            if (!m_funcLoop())
            {
                break;
            }
        }
        if (trycatch.HasCaught())
        {
            v8::Isolate *piso = v8::Isolate::GetCurrent();
            if (piso)
                __JSRun::ReportException(piso, &trycatch);
        }
    }
    // 退出事件
    JCEventEmitter::evtPtr stopEvt(new JCEventBase);
    stopEvt->m_nID = JCWorkerThread::Event_threadStop;
    emit(stopEvt);
}
static void call_JSThread__defRunLoop(void *pdata)
{
    ScriptThread *pthis = (ScriptThread *)pdata;
    pthis->_defRunLoop();
}
void ScriptThread::_runLoop()
{
    m_pJS->initJSEngine();
    m_pJS->run(call_JSThread__defRunLoop, this);
    m_pJS->uninitJSEngine();
}
} // namespace laya

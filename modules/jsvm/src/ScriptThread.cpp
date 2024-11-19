#include "jsvm/ScriptThread.h"

namespace jsvm
{

    void ScriptThread::_defRunLoop()
    {}
    void ScriptThread::runLoop(jsvm::Env env)
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
    JCEventEmitter::evtPtr startEvt(new laya::JCEventBase);
    startEvt->m_nID = JCWorkerThread::Event_threadStart;
    emit(startEvt);
    JCWorkerThread::runObj task;
    jsvm::Status status;
    while (!m_bStop)
    {
        // v8::HandleScope handle_scope(isolate);
        // v8::TryCatch trycatch(isolate);

        jsvm::HandleScope scope;
        status = jsvm::OpenHandleScope(env, &scope);
        DEBUG_CHECK(status == jsvm::Status::OK);
        if (!m_funcLoop)
        {
            // 现在的waitdata返回false不再表示要退出。事件唤醒流程
            if (m_ThreadTasks.WaitData(&task))
                task();
        }
        else
        {
            /*while (v8::platform::PumpMessageLoop(m_pJS->m_pPlatform, isolate,
                                                 v8::platform::MessageLoopBehavior::kDoNotWait))
            {
                continue;
            }
            isolate->PerformMicrotaskCheckpoint();*/
            bool result = false;
            do
            {
                status = jsvm::PumpMessageLoop(m_vm, &result);
                DEBUG_CHECK(status == jsvm::Status::OK);
            } while (result);

            status = jsvm::PerformMicrotaskCheckpoint(m_vm);
            DEBUG_CHECK(status == jsvm::Status::OK);
            // 固定循环流程
            runQueue();
            if (!m_funcLoop())
            {
                break;
            }
        }
        status = jsvm::CloseHandleScope(env, scope);
        DEBUG_CHECK(status == jsvm::Status::OK);
        /*if (trycatch.HasCaught())
        {
            v8::Isolate *piso = v8::Isolate::GetCurrent();
            if (piso)
                __JSRun::ReportException(piso, &trycatch);
        }*/
    }
    // 退出事件
    JCEventEmitter::evtPtr stopEvt(new laya::JCEventBase);
    stopEvt->m_nID = JCWorkerThread::Event_threadStop;
    emit(stopEvt);
}
/*static void call_JSThread__defRunLoop(void* pdata)
{
    ScriptThread *pthis = (ScriptThread *)pdata;
    pthis->_defRunLoop();
}*/
static bool s_bSVMInit = false;
void ScriptThread::_runLoop()
{
    // m_pJS->initJSEngine();
    // m_pJS->run(call_JSThread__defRunLoop, this);
    // m_pJS->uninitJSEngine();
   
    jsvm::InitOptions initOptions;
    memset(&initOptions, 0, sizeof(initOptions));
    static int argc = 2;
    initOptions.argc = &argc;
    //initOptions.argc = (int*)malloc(sizeof(int));
    //*initOptions.argc = 3;
    initOptions.argv = (char**)malloc(2 * sizeof(char*));
    initOptions.argv[0] = "";//--expose-gc-as=gc";
    initOptions.argv[1] = "--expose-gc";
    initOptions.removeFlags = true;
    // InitOptions外部引用赋值
    initOptions.externalReferences = nullptr;
    if (!s_bSVMInit)
    {
        s_bSVMInit = true;
        // 初始化JavaScript引擎实例
        jsvm::Init(&initOptions);
    }
    jsvm::CreateVMOptions options;
    memset(&options, 0, sizeof(options));
    // 创建JavaScript引擎实例
    jsvm::CreateVM(&options, &m_vm);
    jsvm::VMScope vmScope;
    // 打开一个新的VM scope，引擎实例只能在scope范围内使用，可以保证引擎实例不被销毁
    jsvm::OpenVMScope(m_vm, &vmScope);
    jsvm::Env env;
    // 创建一个新的JS执行上下文环境，并注册指定的Native函数
    // jsvm::Status res = jsvm::CreateEnv(vm, sizeof(descriptor) / sizeof(descriptor[0]), descriptor, &env);
    jsvm::Status res = jsvm::CreateEnv(m_vm, 0, nullptr, &env);
    // if (res != JSVM_OK) {
    //     OH_LOG_INFO(LOG_APP, "JSVM API OH_JSVM_CreateEnv failed res is %{public}d", static_cast<int>(res));
    // }
    jsvm::EnvScope envScope;
    // 打开一个新的Env scope，Env只能在scope范围内使用
    jsvm::OpenEnvScope(env, &envScope);
    jsvm::HandleScope handleScope;
    // 打开一个Handle scope
    jsvm::OpenHandleScope(env, &handleScope);


  


    // v8::TryCatch try_catch(m_pIsolate);
    runLoop(env);
    // if (try_catch.HasCaught())
    {
        // v8::String::Utf8Value exceptioninfo(m_pIsolate, try_catch.Exception());
        // printf("Exception info [%s]\n", *exceptioninfo);
    }

    jsvm::CloseHandleScope(env, handleScope); // 关闭Handle scope
    jsvm::CloseEnvScope(env, envScope);       // 关闭Env scope
    jsvm::CloseVMScope(m_vm, vmScope);        // 关闭VM scope
    jsvm::DestroyEnv(env);                    // 销毁一个JS执行上下文环境
    jsvm::DestroyVM(m_vm);                    // 销毁JavaScript引擎实例
}
} // namespace laya

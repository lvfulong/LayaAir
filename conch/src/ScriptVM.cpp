#include "ScriptVM.h"

namespace laya
{

void ScriptVM::runLoop(jsvm_env env, std::function<bool(jsvm_env)> funcLoop)
{
    jsvm_status status;
    {
        // v8::HandleScope handle_scope(isolate);
        // v8::TryCatch trycatch(isolate);

        jsvm_handle_scope scope;
        status = jsvm_open_handle_scope(env, &scope);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        // if (!m_funcLoop)
        //{
        //  现在的waitdata返回false不再表示要退出。事件唤醒流程
        // if (m_ThreadTasks.WaitData(&task))
        //     task();
        //}
        // else
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
                status = jsvm_pump_messageloop(m_vm, &result);
                DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            } while (result);

            status = jsvm_perform_microtask_checkpoint(m_vm);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            // 固定循环流程
            // runQueue();
            if (!funcLoop(env))
            {
                //break;
            }
        }
        status = jsvm_close_handle_scope(env, scope);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        /*if (trycatch.HasCaught())
        {
            v8::Isolate *piso = v8::Isolate::GetCurrent();
            if (piso)
                __JSRun::ReportException(piso, &trycatch);
        }*/
    }
}
static bool s_bSVMInit = false;
void ScriptVM::initialize()
{
    // m_pJS->initJSEngine();
    // m_pJS->run(call_JSThread__defRunLoop, this);
    // m_pJS->uninitJSEngine();

    jsvm_init_options initOptions;
    memset(&initOptions, 0, sizeof(initOptions));
    static int argc = 2;
    initOptions.argc = &argc;
    // initOptions.argc = (int*)malloc(sizeof(int));
    //*initOptions.argc = 3;
    initOptions.argv = (char **)malloc(2 * sizeof(char *));
    initOptions.argv[0] = ""; //--expose-gc-as=gc";
    initOptions.argv[1] = "--expose-gc";
    initOptions.removeFlags = true;
    // InitOptions外部引用赋值
    initOptions.externalReferences = nullptr;
    if (!s_bSVMInit)
    {
        s_bSVMInit = true;
        // 初始化JavaScript引擎实例
        jsvm_init(&initOptions);
    }
    jsvm_create_vm_options options;
    memset(&options, 0, sizeof(options));
    // 创建JavaScript引擎实例
    jsvm_create_vm(&options, &m_vm);
    // 打开一个新的VM scope，引擎实例只能在scope范围内使用，可以保证引擎实例不被销毁
    jsvm_open_vm_scope(m_vm, &m_vmScope);

    // 创建一个新的JS执行上下文环境，并注册指定的Native函数
    // jsvm_status res = jsvm::CreateEnv(vm, sizeof(descriptor) / sizeof(descriptor[0]), descriptor, &env);
    jsvm_status res = jsvm_create_env(m_vm, 0, nullptr, &m_env);
    // if (res != JSVM_OK) {
    //     OH_LOG_INFO(LOG_APP, "JSVM API OH_JSVM_CreateEnv failed res is %{public}d", static_cast<int>(res));
    // }

    // 打开一个新的Env scope，Env只能在scope范围内使用
    jsvm_open_env_scope(m_env, &m_envScope);
}

void ScriptVM::uninitialize()
{
    jsvm_close_env_scope(m_env, m_envScope); // 关闭Env scope
    jsvm_close_vm_scope(m_vm, m_vmScope);    // 关闭VM scope
    jsvm_destroy_env(m_env);                 // 销毁一个JS执行上下文环境
    jsvm_destroy_vm(m_vm);                   // 销毁JavaScript引擎实例
}
} // namespace laya

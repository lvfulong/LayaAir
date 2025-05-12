#include <jsvm/JSEnv.h>

namespace jsvm
{
#if defined(JS_V8)
IsolateData::IsolateData(v8::Isolate *isolate) : isolate_(isolate)
{
    createProperties();
}
void IsolateData::createProperties()
{
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handle_scope(isolate_);
    napi_wrapper_.Set(isolate_,
                      v8::Private::New(isolate_, v8::String::NewFromOneByte(
                                                     isolate_, reinterpret_cast<const uint8_t *>("laya:napi:wrapper"),
                                                     v8::NewStringType::kInternalized, sizeof("laya:napi:wrapper") - 1)
                                                     .ToLocalChecked()));
}
#endif
IsolateData::IsolateData()
{
}

static thread_local JSEnv *s_threadLocalEnv;
JSEnv *JSEnv::getCurrent()
{
    return s_threadLocalEnv;
}
void JSEnv::setCurrent(JSEnv *env)
{
    s_threadLocalEnv = env;
    if (env)
    {
        env->thread_id_ = std::this_thread::get_id();
    }
}
#if defined(JS_V8)
JSEnv::JSEnv(IsolateData *isolate_data, v8::Isolate *isolate, jsvm_env env)
    : isolate_data_(isolate_data), isolate_(isolate), env_(env)
{
}
void JSEnv::pushDbgFunc(std::function<void(void)> task)
{
    m_DbgFuncLock.lock();
    m_DbgFunction.push_back(task);
    m_DbgFuncLock.unlock();
}
void JSEnv::runDbgFuncs()
{
    m_DbgFuncLock.lock();
    for (std::function<void(void)>& task : m_DbgFunction)
    {
        task();
    }
    m_DbgFunction.clear();
    m_DbgFuncLock.unlock();
}
void JSEnv::waitAndRunDbgFuncs()
{
    m_DbgFuncLock.lock();
    m_DbgFuncLock.unlock();
}
bool JSEnv::hasDbgFuncs()
{
    bool bRet = false;
    m_DbgFuncLock.lock();
    bRet = m_DbgFunction.size() > 0;
    m_DbgFuncLock.unlock();
    return bRet;
}
#endif
#if defined(JS_OHOS_JSVM)
JSEnv::JSEnv(IsolateData *isolate_data, jsvm_env env) : env_(env)
{
}
#endif
} // namespace jsvm
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
}
#if defined(JS_V8)
JSEnv::JSEnv(IsolateData *isolate_data, v8::Isolate *isolate, jsvm::Env env)
    : isolate_data_(isolate_data), isolate_(isolate), env_(env)
{
}
#endif
#if defined(JS_OHOS_JSVM)
JSEnv::JSEnv(IsolateData *isolate_data, jsvm::Env env) : env_(env)
{
}
#endif
} // namespace jsvm
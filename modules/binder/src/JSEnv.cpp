#include <binder/JSEnv.h>

namespace jsbind
{
#if defined(JS_V8)
    IsolateData::IsolateData(v8::Isolate* isolate):isolate_(isolate)
    {
        createProperties();
    }
    void IsolateData::createProperties()
    {
        v8::Isolate::Scope isolate_scope(isolate_);
        v8::HandleScope handle_scope(isolate_);
        napi_wrapper_.Set(
            isolate_,
            v8::Private::New(isolate_,
                v8::String::NewFromOneByte(
                    isolate_,
                    reinterpret_cast<const uint8_t*>("laya:napi:wrapper"),
                    v8::NewStringType::kInternalized,
                    sizeof("laya:napi:wrapper") - 1)
                .ToLocalChecked()));
    }
#endif


static thread_local JSEnv *s_threadLocalEnv;
JSEnv *JSEnv::getCurrent()
{
    return s_threadLocalEnv;
}
void JSEnv::setCurrent(JSEnv *env)
{
    s_threadLocalEnv = env;
}
JSEnv::JSEnv(IsolateData* isolate_data, v8::Isolate* isolate):isolate_data_(isolate_data), isolate_(isolate)
{

}
} // namespace jsbind
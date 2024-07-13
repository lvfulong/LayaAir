#include "assert.h"
#include <binder/v8/Promise.h>
#include <utils/Log.h>
namespace laya
{
namespace internal
{



inline void ConcludeDeferred(v8::Local<v8::Context> context, napi_deferred deferred, napi_value result,
                             bool is_resolved)
{
    v8::Global<v8::Value> *deferred_ref = reinterpret_cast<v8::Global<v8::Value> *>(deferred);
    v8::Local<v8::Value> v8_deferred = v8::Local<v8::Value>::New(context->GetIsolate(), *deferred_ref);

    auto v8_resolver = v8_deferred.As<v8::Promise::Resolver>();

    v8::Maybe<bool> success = is_resolved ? v8_resolver->Resolve(context, V8LocalValueFromJsValue(result))
                                          : v8_resolver->Reject(context, V8LocalValueFromJsValue(result));

    delete deferred_ref;
}
} // namespace internal
void napi_create_promise(v8::Local<v8::Context> context, napi_deferred *deferred, napi_value *promise)
{
    auto maybe = v8::Promise::Resolver::New(context);

    auto v8_resolver = maybe.ToLocalChecked();
    auto v8_deferred = new v8::Global<v8::Value>();
    v8_deferred->Reset(v8::Isolate::GetCurrent(), v8_resolver);

    *deferred = JsDeferredFromNodePersistent(v8_deferred);
    *promise = JsValueFromV8LocalValue(v8_resolver->GetPromise());
}

void napi_resolve_deferred(v8::Local<v8::Context> context, napi_deferred deferred, napi_value resolution)
{
    internal::ConcludeDeferred(context, deferred, resolution, true);
}

void napi_reject_deferred(v8::Local<v8::Context> context, napi_deferred deferred, napi_value resolution)
{
    internal::ConcludeDeferred(context, deferred, resolution, false);
}
} // namespace laya
#ifndef __V8_PROMISE_H__
#define __V8_PROMISE_H__

#include <map>
#include <v8.h>

namespace laya
{
typedef struct napi_value__ *napi_value;
typedef struct napi_deferred__ *napi_deferred;
inline v8::Local<v8::Value> V8LocalValueFromJsValue(napi_value v)
{
    v8::Local<v8::Value> local;
    memcpy(static_cast<void *>(&local), &v, sizeof(v));
    return local;
}
inline napi_deferred JsDeferredFromNodePersistent(v8::Global<v8::Value> *local)
{
    return reinterpret_cast<napi_deferred>(local);
}

inline napi_value JsValueFromV8LocalValue(v8::Local<v8::Value> local)
{
    return reinterpret_cast<napi_value>(*local);
}
void napi_create_promise(v8::Local<v8::Context> context, napi_deferred *deferred, napi_value *promise);
void napi_resolve_deferred(v8::Local<v8::Context> context, napi_deferred deferred, napi_value resolution);
void napi_reject_deferred(v8::Local<v8::Context> context, napi_deferred deferred, napi_value resolution);
} // namespace laya
#endif
#include <binder/v8/Promise.h>
#include "assert.h"
#include <Utils/Log.h>
namespace laya
{
Promise::Promise(v8::Local<v8::Value> value)
{
    if (value.IsEmpty())
        return;

    if (!value->IsPromise())
    {
        assert("the given value is not a Promise");
        return;
    }

    promise_ = value.As<v8::Promise>();
}

Promise Promise::reject(v8::Local<v8::Value> value)
{
    if (value.IsEmpty())
        return Promise();
    Resolver resolver;
    v8::Local<v8::Promise> v8promise = resolver.getV8Promise();
    resolver.reject(value);
    return Promise(v8promise);
}

Promise Promise::resolve(v8::Local<v8::Value> value)
{
    if (value.IsEmpty())
        return Promise();
    if (value->IsPromise())
    {
        return Promise(value); //
    }
    Resolver resolver;
    v8::Local<v8::Promise> v8promise = resolver.getV8Promise();
    resolver.resolve(value);
    return Promise(v8promise);
}

v8::Local<v8::Promise> Promise::rejectRaw(v8::Local<v8::Value> value)
{
    if (value.IsEmpty())
        return v8::Local<v8::Promise>();
    v8::Local<v8::Promise::Resolver> resolver;
    if (!v8::Promise::Resolver::New(v8::Isolate::GetCurrent()->GetCurrentContext()).ToLocal(&resolver))
        return v8::Local<v8::Promise>();
    v8::Local<v8::Promise> promise = resolver->GetPromise();
    resolver->Reject(v8::Isolate::GetCurrent()->GetCurrentContext(), value).ToChecked();
    return promise;
}
v8::Local<v8::Promise> Promise::resolveRaw(v8::Local<v8::Value> value)
{
    if (value.IsEmpty())
        return v8::Local<v8::Promise>();

    // v8::Local<v8::Promise::Resolver> resolver =
    //         v8::Promise::Resolver::New(v8::Isolate::GetCurrent()->GetCurrentContext()).ToLocalChecked();

    v8::Local<v8::Promise::Resolver> resolver;
    if (!v8::Promise::Resolver::New(v8::Isolate::GetCurrent()->GetCurrentContext()).ToLocal(&resolver))
        return v8::Local<v8::Promise>();
    v8::Local<v8::Promise> promise = resolver->GetPromise();
    resolver->Resolve(v8::Isolate::GetCurrent()->GetCurrentContext(), value).ToChecked();
    return resolver->GetPromise();
}
Promise::Resolver::Resolver()
    : resolver_(v8::Promise::Resolver::New(v8::Isolate::GetCurrent()->GetCurrentContext()).ToLocalChecked())
{
}

v8::Local<v8::Promise> Promise::Resolver::getV8Promise() const
{
    if (resolver_.IsEmpty())
        return v8::Local<v8::Promise>();
    return resolver_.As<v8::Promise::Resolver>()->GetPromise();
}

void Promise::Resolver::resolve(v8::Local<v8::Value> value)
{
    if (resolver_.IsEmpty())
        return;
    v8::Maybe<bool> result =
        resolver_.As<v8::Promise::Resolver>()->Resolve(v8::Isolate::GetCurrent()->GetCurrentContext(), value);
    // |result| can be empty when the thread is being terminated. We ignore such
    // errors.
    // ALLOW_UNUSED_LOCAL(result);
    clear();
}

void Promise::Resolver::reject(v8::Local<v8::Value> value)
{
    if (resolver_.IsEmpty())
        return;
    v8::Maybe<bool> result =
        resolver_.As<v8::Promise::Resolver>()->Reject(v8 ::Isolate::GetCurrent()->GetCurrentContext(), value);
    // |result| can be empty when the thread is being terminated. We ignore such
    // errors.
    // ALLOW_UNUSED_LOCAL(result);

    clear();
}

} // namespace laya
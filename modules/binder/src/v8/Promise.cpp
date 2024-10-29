
#include "binder/v8/internal/Value.h"
#include <binder/v8/Promise.h>
namespace jsbind
{

Promise::Promise()
{
    GET_ENV
    jsvm::Status status;
    status = jsvm::CreatePromise(env, &deferred_, &promise_);
    DEBUG_CHECK(status == jsvm::Status::OK);
}
template <typename T> void Promise::resolve(T &&t) const
{
    GET_ENV
    jsvm::ResolveDeferred(env, deferred_, Converter<T>::ToJs(std::forward<T>(t), true));
}

template <typename T> void Promise::reject(T &&t) const
{
    GET_ENV
    jsvm::RejectDeferred(env, deferred_, Converter<T>::ToJs(std::forward<T>(t), true));
}
} // namespace jsbind

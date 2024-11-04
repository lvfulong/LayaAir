
#include "binder/v8/internal/Value.h"
#include <binder/v8/Promise.h>
namespace jsbind
{
    Promise Promise::Make()
    {
        return Promise();
    }
Promise::Promise()
{
    GET_ENV
    jsvm::Status status;
    status = jsvm::CreatePromise(env, &deferred_, &promise_);
    DEBUG_CHECK(status == jsvm::Status::OK);
}

} // namespace jsbind


#include <jsbind/internal/Value.h>
#include <jsbind/Promise.h>
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

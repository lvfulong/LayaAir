
#include <jsbind/Promise.h>
#include <jsbind/internal/Value.h>
namespace jsbind
{
Promise Promise::Make()
{
    return Promise();
}
Promise::Promise()
{
    GET_ENV
    jsvm_status status;
    status = jsvm_create_promise(env, &deferred_, &promise_);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
}

} // namespace jsbind

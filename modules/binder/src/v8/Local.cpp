#include <binder/v8/Local.h>

namespace jsbind
{
Local::Local()
{
    GET_ENV
    jsvm::Status status;
    jsvm::Value undefined;
    status = jsvm::GetUndefined(env, &undefined);
    DEBUG_CHECK(status == jsvm::Status::OK);
    handle_ = undefined;
}

Local::Local(jsvm::Value handle) : handle_(handle)
{
}

} // namespace jsbind

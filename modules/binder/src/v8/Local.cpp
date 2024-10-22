#include <binder/v8/Local.h>

namespace jsbind
{
Local::Local()
{
    auto JSEnv = JSEnv::getCurrent();
    DEBUG_CHECK(nullptr != JSEnv);
    jsvm::Env env = JSEnv->getEnv();
    DEBUG_CHECK(nullptr != env);

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

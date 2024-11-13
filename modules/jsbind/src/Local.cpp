#include <jsbind/Local.h>

namespace jsbind
{
Local::Local():handle_(nullptr)
{
#if 0
    GET_ENV
    jsvm::Status status;
    jsvm::Value undefined;
    status = jsvm::GetUndefined(env, &undefined);
    DEBUG_CHECK(status == jsvm::Status::OK);
    handle_ = undefined;
#endif
}

Local::Local(jsvm::Value handle) : handle_(handle)
{
}
Local Local::operator[](const std::string &key) const
{
    DEBUG_CHECK(isValid());
    GET_ENV
    jsvm::Status status;
    jsvm::Value result = nullptr;
    status = jsvm::GetNamedProperty(env, handle_, key.c_str(), &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return Local(result);
}
} // namespace jsbind

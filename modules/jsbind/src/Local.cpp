#include <jsbind/Local.h>

namespace jsbind
{
Local::Local():handle_(nullptr)
{
#if 0
    GET_ENV
    jsvm_status status;
    jsvm_value undefined;
    status = jsvm::GetUndefined(env, &undefined);
    DEBUG_CHECK(status == jsvm_status::ok);
    handle_ = undefined;
#endif
}

Local::Local(jsvm_value handle) : handle_(handle)
{
}
Local Local::operator[](const std::string &key) const
{
    DEBUG_CHECK(isValid());
    GET_ENV
    jsvm_status status;
    jsvm_value result = nullptr;
    status = jsvm_get_named_property(env, handle_, key.c_str(), &result);
    DEBUG_CHECK(status == jsvm_status::ok);
    return Local(result);
}
} // namespace jsbind

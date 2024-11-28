#include <jsbind/Error.h>
#include <string>
#include <utils/Log.h>
namespace jsbind
{
static std::function<void(jsvm_env, jsvm_value)> s_callback;
void reportError(jsvm_env env)
{
    // GET_ENV
    // bool isExceptionPending;
    // auto status = jsvm_is_exception_pending(env, &isExceptionPending);
    // DEBUG_CHECK(status == jsvm_status::jsvm_ok);

    // if (isExceptionPending)
    {
        jsvm_value exceptionValue;
        jsvm_status status = jsvm_get_and_clear_last_exception(env, &exceptionValue);
        if (status == jsvm_status::jsvm_ok)
        {
            if (s_callback)
            {
                s_callback(env, exceptionValue);
            }
        }
        else
        {
            jsvm_throw_error(env, std::to_string(static_cast<int>(status)).c_str(), nullptr);
            LOGE("jsvm failed Error code:  %d", static_cast<int>(status));
        }
    }
}
void setOnError(std::function<void(jsvm_env, jsvm_value)> callback)
{
    s_callback = callback;
}
} // namespace jsbind

#include <jsbind/Error.h>
#include <string>
#include <utils/Log.h>
namespace jsbind
{
void reportError(jsvm_env env, jsvm_status res)
{
    jsvm_value exceptionValue;
    jsvm_status status = jsvm_get_and_clear_last_exception(env, &exceptionValue);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    if (status == jsvm_status::jsvm_ok)
    {
        jsvm_value message;
        status = jsvm_get_named_property(env, exceptionValue, "message", &message);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        size_t length;
        status = jsvm_get_value_string_utf8(env, message, nullptr, 0, &length);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        char *buffer = new char[length + 1];
        status = jsvm_get_value_string_utf8(env, message, buffer, length + 1, nullptr);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        status = jsvm_throw_error(env, std::to_string(static_cast<int>(status)).c_str(), buffer);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        LOGE("jsvm failed Error code:  %s", buffer);
        delete[] buffer;
    }
    else
    {
        jsvm_throw_error(env, std::to_string(static_cast<int>(status)).c_str(), nullptr);
        LOGE("jsvm failed Error code:  %d", static_cast<int>(res));
    }
}
} // namespace jsbind

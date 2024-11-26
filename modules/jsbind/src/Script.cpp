#include <jsbind/Error.h>
#include <jsbind/Script.h>
#include <jsbind/internal/Value.h>

namespace jsbind
{
jsvm_value runScript(const std::string &code)
{
    GET_ENV
    jsvm_value script;
    jsvm_status status;
    status = jsvm_create_string_utf8(env, code.c_str(), code.length(), &script);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);

    jsvm_value result;
    status = jsvm_run_script(env, script, &result);
    if (status != jsvm_status::jsvm_ok)
    {
        reportError(env, status);
    }

    return result;
}
} // namespace jsbind

#include <jsbind/Error.h>
#include <jsbind/Script.h>
#include <jsbind/Value.h>

namespace jsbind
{
jsvm_status runScript(const std::string &code, jsvm_value *result)
{
    GET_ENV
    jsvm_value script_content;
    jsvm_status status;
    status = jsvm_create_string_utf8(env, code.c_str(), code.length(), &script_content);
    if (status != jsvm_status::jsvm_ok)
    {
        reportError(env);
        return status;
    }

    jsvm_script script = nullptr;
    status = jsvm_compile_script(env, script_content, nullptr, 0, true, nullptr, &script);
    if (status != jsvm_status::jsvm_ok)
    {
        reportError(env);
        return status;
    }

    status = jsvm_run_script(env, script, result);
    if (status != jsvm_status::jsvm_ok)
    {
        reportError(env);
        return status;
    }
    return status;
}
} // namespace jsbind

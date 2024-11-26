#include <jsbind/Error.h>
#include <jsbind/Script.h>
#include <jsbind/internal/Value.h>

namespace jsbind
{
jsvm_status runScript(const std::string &code, jsvm_value* result)
{
    GET_ENV
    jsvm_value script;
    jsvm_status status;
    status = jsvm_create_string_utf8(env, code.c_str(), code.length(), &script);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return jsvm_run_script(env, script, result);
}
} // namespace jsbind

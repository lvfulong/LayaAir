
#include "binder/internal/Value.h"
#include <binder/Script.h>
namespace jsbind
{
jsvm::Value runScript(const std::string& code)
{
    GET_ENV
    jsvm::Value script;
    jsvm::Status status;
    status = jsvm::CreateStringUtf8(env, code.c_str(), code.length(), &script);
    DEBUG_CHECK(status == jsvm::Status::OK);

    jsvm::Value result;
    status = jsvm::RunScript(env, script, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    // todo report exception
    return result;
}
} // namespace jsbind

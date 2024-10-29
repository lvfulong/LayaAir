
#include <binder/v8/Script.h>
#include "binder/v8/internal/Value.h"
namespace jsbind
{
    jsvm::Value runScript(const char* code)
	{
        GET_ENV
        jsvm::Value script;
        jsvm::Status status;
        status = jsvm::CreateStringUtf8(env, value.c_str(), value.length(), &script);
        DEBUG_CHECK(status == jsvm::Status::OK);
        
        jsvm::Value result;
        status = jsvm::RunScript(env, script, result);
        DEBUG_CHECK(status == jsvm::Status::OK);
        //todo report exception
        return result;
	}
} // namespace jsbind


#include <jsbind/internal/Value.h>
#include <jsbind/Script.h>
namespace jsbind
{
jsvm::Value runScript(const std::string& code)
{
    GET_ENV
    jsvm::Value script;
    jsvm::Status status;


     //   jsvm::HandleScope scope;
    //status = jsvm::OpenHandleScope(env, &scope);


    
    status = jsvm::CreateStringUtf8(env, code.c_str(), code.length(), &script);
    DEBUG_CHECK(status == jsvm::Status::OK);

    jsvm::Value result;
    status = jsvm::RunScript(env, script, &result); 
    //todo
    status = jsvm::ReportException( env);
    DEBUG_CHECK(status == jsvm::Status::OK);
    DEBUG_CHECK(status == jsvm::Status::OK);



    
   

    //status = jsvm::CloseHandleScope(env, scope);
    // todo report exception
    return result;
}
} // namespace jsbind

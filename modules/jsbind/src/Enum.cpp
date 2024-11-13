#include <jsbind/Enum.h>
namespace jsbind
{
Enum_::Enum_(Object*owner, std::string_view name)
    : owner_(owner), name_(name)
{

    GET_ENV
    jsvm::Status status;
    jsvm::Value result;
    status = jsvm::CreateObject(env, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    object_ = std::make_unique<Object>(result);

    //lvtodo owner_->submodule(name_, *module_);
}
Enum_::~Enum_()
{
    /*if (module_)
    {
    owner_->submodule(name_, *module_);
    }*/
}

} // namespace laya

#include <jsbind/Enum.h>
namespace jsbind
{
Enum_::Enum_(Object *owner, std::string_view name) : owner_(owner), name_(name)
{

    GET_ENV
    jsvm_status status;
    jsvm_value result;
    status = jsvm_create_object(env, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    object_ = std::make_unique<Object>(result);

    // lvtodo owner_->submodule(name_, *module_);
}
Enum_::~Enum_()
{
    /*if (module_)
    {
    owner_->submodule(name_, *module_);
    }*/
}

} // namespace jsbind

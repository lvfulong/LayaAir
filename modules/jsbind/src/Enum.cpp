#include <jsbind/Enum.h>
namespace jsbind
{
Enum_::Enum_(Object *owner, const char * name) : owner_(owner), name_(name)
{

    GET_ENV
    jsvm_status status;
    jsvm_value result;
    status = jsvm_create_object(env, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    object_ = std::make_unique<Object>(result);

    owner_->constant(name_, object_->getHandle());
}
Enum_::~Enum_()
{
}

} // namespace jsbind

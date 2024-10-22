#include <binder/v8/Module.h>
#include <binder/v8/Context.h>

namespace jsbind
{
Enum_::Enum_(Module *owner, std::string_view name, v8::Isolate *isolate)
    : owner_(owner), name_(name), module_(new Module(isolate))
{
    owner_->submodule(name_, *module_);
}
Enum_::~Enum_()
{
    /*if (module_)
    {
    owner_->submodule(name_, *module_);
    }*/
}

} // namespace laya

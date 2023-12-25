#include <binder/V8/Module.h>
#include <binder/V8/Context.h>

namespace laya
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

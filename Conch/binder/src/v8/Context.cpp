#include <binder/V8/Context.h>
#include <binder/V8/Module.h>

namespace laya
{
Context &Context::module(std::string_view name, Module &m)
{
    v8::HandleScope scope(isolate());
    global()->Set(context(), Converter<const char *>::ToJs(name), m.newInstance());
    return *this;

    // return value(name, m.new_instance());
}

static Context s_context;
Context &getCurrentContext()
{
    return s_context; // todo 临时
}
} // namespace laya

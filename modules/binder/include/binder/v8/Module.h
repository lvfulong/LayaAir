#ifndef __V8_MODULE_H__
#define __V8_MODULE_H__

#include "Class.h"
#include "Converter.h"
#include "Invocation.h"
#include "Utility.h"
#include <string>
#include <v8.h>
#include <vector>

namespace binder
{
class Module;
class Enum_
{
  public:
    Enum_(Module *owner, std::string_view name, v8::Isolate *isolate);
    ~Enum_();
    template <typename EnumType> Enum_ &value(std::string_view name, EnumType value);

    Enum_(Enum_ const &) = delete;
    Enum_ &operator=(Enum_ const &) = delete;

    Enum_(Enum_ &&) = default;
    Enum_ &operator=(Enum_ &&) = default;

  private:
    friend class Module;
    std::string name_;
    Module *owner_;
    std::unique_ptr<Module> module_;
};
class Module
{
  public:
    explicit Module(v8::Isolate *isolate) : isolate_(isolate), obj_(isolate_, v8::ObjectTemplate::New(isolate))
    {
    }

    explicit Module(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> obj)
        : isolate_(isolate), obj_(isolate_, v8::ObjectTemplate::New(isolate))
    {
    }
    ~Module()
    {
        obj_.Reset();
    }
    Module(Module const &) = delete;
    Module &operator=(Module const &) = delete;

    Module(Module &&) = default;
    Module &operator=(Module &&) = default;

    template <typename Value> Module &constant(std::string_view name, Value const &value)
    {
        v8::HandleScope scope(isolate_);
        getLocal()->Set(Converter<const char *>::ToJs(name).As<v8::Name>(), Converter<Value>::ToJs(value),
                        v8::PropertyAttribute(v8::ReadOnly | v8::DontDelete));
        return *this;
    }
    v8::Local<v8::Object> newInstance()
    {
        return getLocal()->NewInstance(isolate_->GetCurrentContext()).ToLocalChecked();
    }
    template <typename ReturnType, typename... Args>
    Module &function(std::string_view name, ReturnType (*func)(Args...))
    {
        v8::HandleScope scope(isolate_);
        FuncInfo<decltype(func)> *info = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([info]() { delete info; });
#if 0
        info->name = name;
#endif
        v8::Local<v8::Value> data = v8::External::New(isolate_, info);

        auto ft = v8::FunctionTemplate::New(isolate_, laya::internal::InvokeFunction<ReturnType, Args...>, data);
        getLocal()->Set(isolate_, name.data(), ft);
        return *this;
    }
    template <typename EnumType> Enum_ enum_(std::string_view name)
    {
        return Enum_(this, name, isolate_);
    }
    Module &submodule(std::string_view name, Module &m)
    {
        v8::HandleScope scope(isolate_);
        getLocal()->Set(Converter<const char *>::ToJs(name).As<v8::Name>(), m.getLocal(),
                        v8::PropertyAttribute(v8::ReadOnly | v8::DontDelete));
        return *this;
    }
    template <typename T> Module &add_class_(const char* /*std::string_view*/ name, class_<T>& cl)
    {
        add_class_(name, cl.js_function_template(), cl.class_function_template());
        return *this;
    }
    Module & add_class_(std::string_view name, v8::Local<v8::FunctionTemplate> js_function_template,
                   v8::Local<v8::FunctionTemplate> class_function_template)
    {
        v8::HandleScope scope(isolate_);
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate_, name.data(), v8::NewStringType::kInternalized).ToLocalChecked();
        class_function_template->SetClassName(name_string);
        getLocal()->Set(Converter<const char *>::ToJs(name).As<v8::Name>(), js_function_template,
                        v8::PropertyAttribute(v8::ReadOnly | v8::DontDelete));
        return *this;
    }
    template <typename DataType> Module &value(std::string_view name, v8::Local<DataType> value)
    {
        getLocal()->Set(Converter<const char *>::ToJs(name).As<v8::Name>(), value);
        return *this;
    }
    v8::Local<v8::ObjectTemplate> getLocal()
    {
        return obj_.Get(isolate_);
    }

  private:
    v8::Isolate *isolate_;
    v8::Global<v8::ObjectTemplate> obj_;
};
template <typename EnumType> Enum_ &Enum_::value(std::string_view name, EnumType value)
{
    module_->constant(name, (int32_t)value);
    return *this;
}
} // namespace laya
#endif

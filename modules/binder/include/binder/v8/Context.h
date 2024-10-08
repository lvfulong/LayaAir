#ifndef __V8_CONTEXT_H__
#define __V8_CONTEXT_H__

#include <map>
#include <string>

#include "Class.h"
#include <v8.h>
#include  <binder/JSVM_Types.h>

namespace binder
{
class Module;
class Context
{
  public:
    explicit Context()
    {
    }

    Context(Context const &) = delete;
    Context &operator=(Context const &) = delete;

    Context(Context &&) = delete;
    Context &operator=(Context &&) = delete;

    ~Context()
    {
    }

    v8::Isolate *isolate() const
    {
        return v8::Isolate::GetCurrent();
    } // TODO
    v8::Local<v8::Context> context() const
    {
        return isolate()->GetCurrentContext();
    };
    v8::Local<v8::Object> global()
    {
        return context()->Global();
    }

    Context &module(std::string_view name, Module &m);

    template <typename T> Context &class_(std::string_view name, laya::class_<T> &cl)
    {
        v8::HandleScope scope(isolate());
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate(), name.data(), v8::NewStringType::kInternalized).ToLocalChecked();
        cl.class_function_template()->SetClassName(name_string);
        global()
            ->Set(isolate()->GetCurrentContext(), name_string,
                  cl.js_function_template()->GetFunction(isolate()->GetCurrentContext()).ToLocalChecked())
            .FromJust();
        return *this;
    }

    template <typename ReturnType, typename... Args>
    Context &function(std::string_view name, ReturnType (*func)(Args...))
    {

        FuncInfo<decltype(func)>* data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        m_propertyDescriptorVector.emplace_back(PropertyDescriptor{ (name), NULL, (internal::InvokeFunction<ReturnType, Args...>), NULL, NULL, NULL, napi_default, data });
        return *this;
    }
    template <typename ReturnType, typename... Args>
    Context &function_optional_override(std::string_view name, ReturnType (*func)(Args...))
    {
        FuncInfo<decltype(func)>* data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        m_propertyDescriptorVector.emplace_back(PropertyDescriptor{ (name), NULL, (internal::InvokeGlobalMethodOptionalOverride<ReturnType, Args...>), NULL, NULL, NULL, napi_default, data });

        return *this;
    }
    void export()
    {
        //todos
    }
private:
    std::vector<jsvm::PropertyDescriptor> m_propertyDescriptorVector;
};
Context &getCurrentContext();
} // namespace jsvm
#endif
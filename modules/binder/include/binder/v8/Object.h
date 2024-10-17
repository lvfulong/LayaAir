#ifndef __V8_OBJECT_H__
#define __V8_OBJECT_H__

#include <cstring>

#include <v8.h>

#include "Converter.h"
#include  <binder/JSVM_Types.h>

namespace laya
{

template <typename T>
bool get_option(v8::Isolate *isolate, v8::Local<v8::Object> options, std::string_view name, T &value)
{
    std::string_view::size_type const dot_pos = name.find('.');
    if (dot_pos != name.npos)
    {
        v8::Local<v8::Object> suboptions;
        return get_option(isolate, options, name.substr(0, dot_pos), suboptions) &&
               get_option(isolate, suboptions, name.substr(dot_pos + 1), value);
    }
    v8::Local<v8::Value> val;
    if (!options->Get(isolate->GetCurrentContext(), Converter<const char *>::ToJs(name.data())).ToLocal(&val) ||
        val->IsUndefined())
    {
        return false;
    }
    value = laya::Converter<T>::ToCpp(val);
    return true;
}

template <typename T>
bool set_option(v8::Isolate *isolate, v8::Local<v8::Object> options, std::string_view name, T const &value)
{
    std::string_view::size_type const dot_pos = name.find('.');
    if (dot_pos != name.npos)
    {
        v8::Local<v8::Object> suboptions;
        return get_option(isolate, options, name.substr(0, dot_pos), suboptions) &&
               set_option(isolate, suboptions, name.substr(dot_pos + 1), value);
    }
    return options
        ->Set(isolate->GetCurrentContext(), Converter<const char *>::ToJs(name.data()), Converter<T>::ToJs(value))
        .FromJust();
}

template <typename T>
void set_const(v8::Isolate *isolate, v8::Local<v8::Object> options, std::string_view name, T const &value)
{
    options
        ->DefineOwnProperty(isolate->GetCurrentContext(), Converter<const char *>::ToJs(name.data()),
                            Converter<T>::ToJs(value), v8::PropertyAttribute(v8::ReadOnly | v8::DontDelete))
        .FromJust();
}

namespace internal
{
struct value_object_field
{
    std::string field_name_str;
    v8::Global<v8::Value> field_name;
    void *pfield;
    void (*from_v8)(v8::Local<v8::Value> value, void *obj, void *pfield);
    v8::Local<v8::Value> (*to_v8)(const void *obj, void *pfield);

    v8::Local<v8::Value> toLocalFieldName()
    {

        return v8::Local<v8::Value>::New(v8::Isolate::GetCurrent(), field_name);
    }
};

template <class T> struct is_value_object : std::false_type
{
};
} // namespace internal

template <typename T> class value_object : public value_object_base<T>
{
  public:
    value_object(const char *)
    {
        internal::addDeinitializer([]() { value_object<T>::clear_private_data(); });
        assert(!is_bound && "Multiple exposes of value_object");
        is_bound = true;
    }

    template <typename Field> static void field_from_v8(v8::Local<v8::Value> value, void *obj, void *pfield)
    {
        auto t = reinterpret_cast<T *>(obj);
        Field field = internal::ptr_cast<Field>(pfield);
        // Field field = reinterpret_cast<Field>(pfield);
        using FieldType = typename internal::function_traits<Field>::return_type;
        // obj->*field = internal::from_v8<FieldType>(value);
        t->*field = laya::Converter<FieldType>::ToCpp(value);
    }

    template <typename Field> static v8::Local<v8::Value> field_to_v8(const void *obj, void *pfield)
    {
        auto t = reinterpret_cast<const T *>(obj);
        Field field = internal::ptr_cast<Field>(pfield);
        // Field field = reinterpret_cast<Field>(pfield);
        using FieldType = typename internal::function_traits<Field>::return_type;
        // return internal::to_v8(obj->*field);
        return Converter<FieldType>::ToJs(t->*field);
    }
    template <typename Field> value_object &field(const char *js_name, Field field)
    {
        internal::value_object_field f = {
            js_name, v8::Global<v8::Value>(v8::Isolate::GetCurrent(), laya::Converter<const char *>::ToJs(js_name)),
            internal::ptr_cast<Field>(field), value_object::field_from_v8<Field>, value_object::field_to_v8<Field>};
        fields.emplace_back(std::move(f));
        return *this;
    }

    static std::vector<internal::value_object_field> fields;
    static bool is_bound;

  private:
    static void clear_private_data()
    {
        fields.clear();
        is_bound = false;
    }
};

template <typename T> std::vector<internal::value_object_field> value_object<T>::fields;
template <typename T> bool value_object<T>::is_bound = false;

namespace internal
{

template <typename T> T convert_value_object_from_v8(v8::Local<v8::Value> value)
{
    assert(value_object<T>::is_bound && "casting to an unbound value_type");
    auto obj = v8::Object::Cast(*value);
    T ret{};
    for (auto &field : value_object<T>::fields)
    {
        v8::MaybeLocal<v8::Value> prop =
            obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), field.toLocalFieldName());
        if (!prop.IsEmpty())
        {
            field.from_v8(prop.ToLocalChecked(), (void *)&ret, field.pfield);
        }
        else
        {
            std::string error = std::string("Could not find property with name of ") + field.field_name_str;
            LOGE("%s", error.c_str());
            v8::Isolate::GetCurrent()->ThrowException(
                v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), error.c_str()).ToLocalChecked());
        }
    }
    return ret;
}

template <typename T> v8::Local<v8::Value> convert_value_object_to_v8(const T &value)
{
    assert(value_object<T>::is_bound && "casting from an unbound value_type");
    auto ret = v8::Object::New(v8::Isolate::GetCurrent());
    for (auto &field : value_object<T>::fields)
    {
        auto name = *reinterpret_cast<v8::Local<v8::String> *>(&field.field_name);
        ret->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), name, field.to_v8(&value, field.pfield));
    }
    return ret;
}
template <typename T> v8::Local<v8::Value> convert_value_object_to_v8(T *value)
{
    assert(value_object<T>::is_bound && "casting from an unbound value_type");
    if (value == nullptr)
    {
        return v8::Null(v8::Isolate::GetCurrent());
    }
    return convert_value_object_to_v8(*value);
}

} // namespace internal


class Object
{
public:
    explicit Object(jsvm::Value object): object_(object)
    {
    }

    Object(Object const&) = delete;
    Object& operator=(Object const&) = delete;

    Object(Object&&) = delete;
    Object& operator=(Object&&) = delete;

    ~Object()
    {
    }
    //Object& module(std::string_view name, Module& m);

    template <typename T> Object& class_(std::string_view name, laya::class_<T>& cl)
    {
        /*v8::HandleScope scope(isolate());
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate(), name.data(), v8::NewStringType::kInternalized).ToLocalChecked();
        cl.class_function_template()->SetClassName(name_string);
        global()
            ->Set(isolate()->GetCurrentContext(), name_string,
                cl.js_function_template()->GetFunction(isolate()->GetCurrentContext()).ToLocalChecked())
            .FromJust();*/
        NODE_API_CALL(env, SetNamedProperty(env, exports_, name.data(), cl.ctor_));
        return *this;
    }

    template <typename ReturnType, typename... Args>
    Object& function(std::string_view name, ReturnType(*func)(Args...))
    {

        /*v8::HandleScope scope(isolate());

        FuncInfo<decltype(func)>* info = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([info]() { delete info; });
        info->name = name;
        v8::Local<v8::Value> data = v8::External::New(isolate(), info);

        v8::Local<v8::FunctionTemplate> t =
            v8::FunctionTemplate::New(isolate(), internal::InvokeFunction<ReturnType, Args...>, data);
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate(), name.data(), v8::NewStringType::kInternalized).ToLocalChecked();

        global()
            ->Set(isolate()->GetCurrentContext(), name_string,
                t->GetFunction(isolate()->GetCurrentContext()).ToLocalChecked())
            .FromJust();*/
        FuncInfo<decltype(func)>* data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        propertyDescriptorVector_.emplace_back(PropertyDescriptor{ (name), NULL, (internal::InvokeFunction<ReturnType, Args...>), NULL, NULL, NULL, napi_default, data });
        return *this;
    }
    template <typename ReturnType, typename... Args>
    Object& function_optional_override(std::string_view name, ReturnType(*func)(Args...))
    {

        /*v8::HandleScope scope(isolate());

        FuncInfo<decltype(func)>* info = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([info]() { delete info; });
        info->name = name;
        v8::Local<v8::Value> data = v8::External::New(isolate(), info);

        v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(
            isolate(), internal::InvokeGlobalMethodOptionalOverride<ReturnType, Args...>, data);
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate(), name.data(), v8::NewStringType::kInternalized).ToLocalChecked();

        global()
            ->Set(isolate()->GetCurrentContext(), name_string,
                t->GetFunction(isolate()->GetCurrentContext()).ToLocalChecked())
            .FromJust();*/
        FuncInfo<decltype(func)>* data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        propertyDescriptorVector_.emplace_back(PropertyDescriptor{ (name), NULL, (internal::InvokeGlobalMethodOptionalOverride<ReturnType, Args...>), NULL, NULL, NULL, napi_default, data });

        return *this;
    }
    /*void export()
    {
        //todos
    }*/
private:
    std::vector<jsvm::PropertyDescriptor>  propertyDescriptorVector_;
    jsvm::Value object_ = nullptr;
};
} // namespace laya

#endif

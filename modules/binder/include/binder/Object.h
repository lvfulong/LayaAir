#ifndef __JSBIND_OBJECT_H__
#define __JSBIND_OBJECT_H__

#include <cstring>
#include "internal/Converter.h"
#include <binder/JSVM_Types.h>
#include "Utility.h"
#include "Class.h"
#include "Invoke.h"
namespace jsbind
{

template <typename T>
bool get_option(jsvm::Value options, std::string_view name, T &value)
{
    std::string_view::size_type const dot_pos = name.find('.');
    if (dot_pos != name.npos)
    {
        jsvm::Value suboptions;
        return get_option(options, name.substr(0, dot_pos), suboptions) &&
               get_option(suboptions, name.substr(dot_pos + 1), value);
    }
    
    GET_ENV
    jsvm::Status status;
    bool hasProperty;
    status = jsvm::HasNamedProperty(env, options, name.data(), &hasProperty);
    DEBUG_CHECK(status == jsvm::Status::OK);
    if (!hasProperty)
    {
        return false;
    }
    jsvm::Value val;
    status = jsvm::GetNamedProperty(env, options, name.data(), &val);
    DEBUG_CHECK(status == jsvm::Status::OK);
    value = Converter<T>::ToCpp(val);
    return true;
}

template <typename T>
bool set_option(jsvm::Value options, std::string_view name, T const &value)
{
    std::string_view::size_type const dot_pos = name.find('.');
    if (dot_pos != name.npos)
    {
        jsvm::Value suboptions;
        return get_option(options, name.substr(0, dot_pos), suboptions) &&
               set_option(suboptions, name.substr(dot_pos + 1), value);
    }
    GET_ENV
    jsvm::Status status;
    status = jsvm::SetNamedProperty(env, options, name.data(), Converter<T>::ToJs(value));
    DEBUG_CHECK(status == jsvm::Status::OK);
    return true;
}

/*template <typename T>
void set_const(jsvm::Value options, std::string_view name, T const &value)
{
    options
        ->DefineOwnProperty(isolate->GetCurrentContext(), Converter<const char *>::ToJs(name.data()),
                            Converter<T>::ToJs(value), v8::PropertyAttribute(v8::ReadOnly | v8::DontDelete))
        .FromJust();
}*/

namespace internal
{
struct value_object_field
{
    std::string field_name;
    //v8::Global<v8::Value> field_name;
    void *pfield;
    void (*from_v8)(jsvm::Value value, void *obj, void *pfield);
    jsvm::Value (*to_v8)(const void *obj, void *pfield);

    /*v8::Local<v8::Value> toLocalFieldName()
    {

        return v8::Local<v8::Value>::New(v8::Isolate::GetCurrent(), field_name);
    }*/
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

    template <typename Field> static void field_from_v8(jsvm::Value value, void *obj, void *pfield)
    {
        auto t = reinterpret_cast<T *>(obj);
        Field field = internal::ptr_cast<Field>(pfield);
        // Field field = reinterpret_cast<Field>(pfield);
        using FieldType = typename internal::function_traits<Field>::return_type;
        // obj->*field = internal::from_v8<FieldType>(value);
        t->*field = Converter<FieldType>::ToCpp(value);
    }

    template <typename Field> static jsvm::Value field_to_v8(const void *obj, void *pfield)
    {
        auto t = reinterpret_cast<const T *>(obj);
        Field field = internal::ptr_cast<Field>(pfield);
        // Field field = reinterpret_cast<Field>(pfield);
        using FieldType = typename internal::function_traits<Field>::return_type;
        // return internal::to_v8(obj->*field);
        return internal::ToJSValue<FieldType>(t->*field);
    }
    template <typename Field> value_object &field(const char *js_name, Field field)
    {
        internal::value_object_field f = {js_name, internal::ptr_cast<Field>(field), value_object::field_from_v8<Field>, value_object::field_to_v8<Field>};
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

template <typename T> T convert_value_object_from_js(jsvm::Value value)
{
    DEBUG_CHECK(value_object<T>::is_bound && "casting to an unbound value_type");
    T ret{};
    for (auto &field : value_object<T>::fields)
    {
        GET_ENV
        jsvm::Status status;
        jsvm::Value prop;
        status = jsvm::GetNamedProperty(env, value, field.field_name.c_str(), &prop);
        DEBUG_CHECK(status == jsvm::Status::OK);


        //v8::MaybeLocal<v8::Value> prop =
        //    obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), field.toLocalFieldName());
        if (value != nullptr)
        {
            field.from_v8(prop, (void *)&ret, field.pfield);
        }
        else
        {
            std::string error = std::string("Could not find property with name of ") + field.field_name;
            LOGE("%s", error.c_str());
            //lvtodo v8::Isolate::GetCurrent()->ThrowException(
            //    v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), error.c_str()).ToLocalChecked());
        }
    }
    return ret;
}

template <typename T> jsvm::Value convert_value_object_to_js(const T &value)
{
    DEBUG_CHECK(value_object<T>::is_bound && "casting from an unbound value_type");
    GET_ENV
    jsvm::Status status;
    jsvm::Value result;
    status = jsvm::CreateObject( env, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
   // auto ret = v8::Object::New(v8::Isolate::GetCurrent());
    for (auto &field : value_object<T>::fields)
    {

        status = jsvm::SetNamedProperty(env, result, field.field_name.c_str(), field.to_v8(&value, field.pfield));
        DEBUG_CHECK(status == jsvm::Status::OK);
        //auto name = *reinterpret_cast<v8::Local<v8::String> *>(&field.field_name);
        //ret->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), name, field.to_v8(&value, field.pfield));
    }
    return result;
}
template <typename T> jsvm::Value convert_value_object_to_js(T *value)
{
    DEBUG_CHECK(value_object<T>::is_bound && "casting from an unbound value_type");
    if (value == nullptr)
    {
        return internal::makeNull();
    }
    return convert_value_object_to_js(*value);
}

} // namespace internal
template <typename T> class Converter<T, std::enable_if_t<internal::is_value_object<T>::value>>
{
public:
    static jsvm::Value ToJs(T value, bool callDestructor = true)
    {
        return internal::convert_value_object_to_js(value);
    }
    static T ToCpp(jsvm::Value value)
    {
        return internal::convert_value_object_from_js<T>(value);
    }
};

template <typename T> class Converter<T*, std::enable_if_t<internal::is_value_object<T>::value>>
{
public:
    static jsvm::Value ToJs(T* value, bool callDestructor = true)
    {
        return internal::convert_value_object_to_js(value);
    }
    static T ToCpp(jsvm::Value value)
    {
        return internal::convert_value_object_from_js<T>(value);
    }
};
class Object
{
  public:
    explicit Object(jsvm::Value object) : object_(object)
    {
    }

    Object(Object const &) = delete;
    Object &operator=(Object const &) = delete;

    Object(Object &&) = delete;
    Object &operator=(Object &&) = delete;

    ~Object()
    {
    }
    // Object& module(std::string_view name, Module& m);
    template <typename Value> Object &constant(std::string_view name, Value const &value)
    {
        // todo
        // v8::HandleScope scope(isolate_);
        // getLocal()->Set(Converter<const char*>::ToJs(name).As<v8::Name>(), Converter<Value>::ToJs(value),
        //     v8::PropertyAttribute(v8::ReadOnly | v8::DontDelete));
        return *this;
    }
    template <typename T> Object &class_(std::string_view name, jsbind::class_<T> &cl)
    {
        /*v8::HandleScope scope(isolate());
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate(), name.data(), v8::NewStringType::kInternalized).ToLocalChecked();
        cl.class_function_template()->SetClassName(name_string);
        global()
            ->Set(isolate()->GetCurrentContext(), name_string,
                cl.js_function_template()->GetFunction(isolate()->GetCurrentContext()).ToLocalChecked())
            .FromJust();*/
        GET_ENV
        jsvm::SetNamedProperty(env, object_, name.data(), cl.ctor_);
        return *this;
    }

    template <typename ReturnType, typename... Args>
    Object &function(const char*  name, ReturnType (*func)(Args...))
    {

        /*v8::HandleScope scope(isolate());

        FuncInfo<decltype(func)>* info = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([info]() { delete info; });
        info->name = name;
        v8::Local<v8::Value> data = v8::External::New(isolate(), info);

        v8::Local<v8::FunctionTemplate> t =
            v8::FunctionTemplate::New(isolate(), internal::<ReturnType, Args...>, data);
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate(), name.data(), v8::NewStringType::kInternalized).ToLocalChecked();

        global()
            ->Set(isolate()->GetCurrentContext(), name_string,
                t->GetFunction(isolate()->GetCurrentContext()).ToLocalChecked())
            .FromJust();*/
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;

        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeMethodStatic<ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::DEFAULT;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;

    }
    template <typename ReturnType, typename... Args>
    Object &function_optional_override(const char* name, ReturnType (*func)(Args...))
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
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeGlobalMethodOptionalOverride<ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::DEFAULT;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    /*void export()
    {
        //todos
    }*/
  private:
    std::vector<jsvm::PropertyDescriptor> propertyDescriptorVector_;
    jsvm::Value object_ = nullptr;
};
} // namespace jsbind

#endif

#ifndef __JSBIND_OBJECT_H__
#define __JSBIND_OBJECT_H__

#include <cstring>
#include <jsbind/Utility.h>
#include <jsbind/internal/Invoke.h>
#include <jsbind/internal/ValueTraits.h>
#include <jsvm/JSVM_Types.h>
#include <jsbind/Enum.h>

namespace jsbind
{

template <typename T> bool get_option(jsvm_value options, std::string_view name, T &value)
{
    std::string_view::size_type const dot_pos = name.find('.');
    if (dot_pos != name.npos)
    {
        jsvm_value suboptions;
        return get_option(options, name.substr(0, dot_pos), suboptions) &&
               get_option(suboptions, name.substr(dot_pos + 1), value);
    }

    GET_ENV
    jsvm_status status;
    bool hasProperty;
    status = jsvm_has_named_property(env, options, name.data(), &hasProperty);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    if (!hasProperty)
    {
        return false;
    }
    jsvm_value val;
    status = jsvm_get_named_property(env, options, name.data(), &val);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    value = internal::ValueTraits<T>::ToCpp(val);
    return true;
}

template <typename T> bool set_option(jsvm_value options, std::string_view name, T const &value)
{
    std::string_view::size_type const dot_pos = name.find('.');
    if (dot_pos != name.npos)
    {
        jsvm_value suboptions;
        return get_option(options, name.substr(0, dot_pos), suboptions) &&
               set_option(suboptions, name.substr(dot_pos + 1), value);
    }
    GET_ENV
    jsvm_status status;
    status = jsvm_set_named_property(env, options, name.data(), internal::ValueTraits<T>::ToJs(value));
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return true;
}

/*template <typename T>
void set_const(jsvm_value options, std::string_view name, T const &value)
{
    options
        ->DefineOwnProperty(isolate->GetCurrentContext(), ValueTraits<const char *>::ToJs(name.data()),
                            ValueTraits<T>::ToJs(value), v8::PropertyAttribute(v8::ReadOnly | v8::DontDelete))
        .FromJust();
}*/

namespace internal
{
struct value_object_field
{
    std::string field_name;
    void *pfield;
    void (*from_v8)(jsvm_value value, void *obj, void *pfield);
    jsvm_value (*to_v8)(const void *obj, void *pfield);
};
template <class T> struct is_value_object : std::false_type
{
};

} // namespace internal
template <typename T> class value_object : public internal::value_object_base<T>
{
  public:
    value_object(const char *)
    {
        internal::addDeinitializer([]() { value_object<T>::clear_private_data(); });
        assert(!is_bound && "Multiple exposes of value_object");
        is_bound = true;
    }

    template <typename Field> static void field_from_v8(jsvm_value value, void *obj, void *pfield)
    {
        auto t = reinterpret_cast<T *>(obj);
        Field field = internal::ptr_cast<Field>(pfield);
        using FieldType = typename internal::function_traits<Field>::return_type;
        t->*field = internal::ValueTraits<FieldType>::ToCpp(value);
    }

    template <typename Field> static jsvm_value field_to_v8(const void *obj, void *pfield)
    {
        auto t = reinterpret_cast<const T *>(obj);
        Field field = internal::ptr_cast<Field>(pfield);
        using FieldType = typename internal::function_traits<Field>::return_type;
        return internal::ToJSValue<FieldType>(t->*field);
    }
    template <typename Field> value_object &field(const char *js_name, Field field)
    {
        internal::value_object_field f = {js_name, internal::ptr_cast<Field>(field), value_object::field_from_v8<Field>,
                                          value_object::field_to_v8<Field>};
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

template <typename T> T convert_value_object_from_js(jsvm_value value)
{
    DEBUG_CHECK(jsbind::value_object<T>::is_bound && "casting to an unbound value_type");
    T ret{};
    for (auto &field : jsbind::value_object<T>::fields)
    {
        GET_ENV
        jsvm_status status;
        jsvm_value prop;
        status = jsvm_get_named_property(env, value, field.field_name.c_str(), &prop);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        // v8::MaybeLocal<v8::Value> prop =
        //     obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), field.toLocalFieldName());
        if (value != nullptr)
        {
            field.from_v8(prop, (void *)&ret, field.pfield);
        }
        else
        {
            std::string error = std::string("Could not find property with name of ") + field.field_name;
            LOGE("%s", error.c_str());
            // lvtodo v8::Isolate::GetCurrent()->ThrowException(
            //     v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), error.c_str()).ToLocalChecked());
        }
    }
    return ret;
}

template <typename T> jsvm_value convert_value_object_to_js(const T &value)
{
    DEBUG_CHECK(jsbind::value_object<T>::is_bound && "casting from an unbound value_type");
    GET_ENV
    jsvm_status status;
    jsvm_value result;
    status = jsvm_create_object(env, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    for (auto &field : jsbind::value_object<T>::fields)
    {
        status = jsvm_set_named_property(env, result, field.field_name.c_str(), field.to_v8(&value, field.pfield));
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
    return result;
}
template <typename T> jsvm_value convert_value_object_to_js(T *value)
{
    DEBUG_CHECK(jsbind::value_object<T>::is_bound && "casting from an unbound value_type");
    if (value == nullptr)
    {
        return internal::makeNull();
    }
    return convert_value_object_to_js(*value);
}

template <typename T> class ValueTraits<T, std::enable_if_t<internal::is_value_object<T>::value>>
{
  public:
    static jsvm_value ToJs(T value, bool callDestructor = true)
    {
        return internal::convert_value_object_to_js(value);
    }
    static T ToCpp(jsvm_value value)
    {
        return internal::convert_value_object_from_js<T>(value);
    }
};

template <typename T> class ValueTraits<T *, std::enable_if_t<internal::is_value_object<T>::value>>
{
  public:
    static jsvm_value ToJs(T *value, bool callDestructor = true)
    {
        return internal::convert_value_object_to_js(value);
    }
    static T ToCpp(jsvm_value value)
    {
        return internal::convert_value_object_from_js<T>(value);
    }
};
} // namespace internal
class Object
{
  public:
    explicit Object(jsvm_value object) : object_(object)
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
    template <typename Value> Object &constant(const char * name, const Value& value)
    {
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = internal::ValueTraits<Value>::ToJs(value);
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = NULL;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;

    }
    Object &constant(const char * name, jsvm_value value)
    {
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = value;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = NULL;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;

    }
    template <typename EnumType> Enum_ enum_(const char * name)
    {
        return Enum_(this, name);
    }
    template <typename T, typename Traits> Object &class_(const char *name, jsbind::class_<T, Traits> &cl)
    {
        GET_ENV
        cl.Export(env, object_, name);
        return *this;
    }
    template <typename T, typename Traits> Object &global_class_(const char *name, jsbind::global_class_<T, Traits> &cl, T *instance = nullptr)
    {
        GET_ENV
        cl.Export(env, object_, name);
        return *this;
    }

    template <typename ReturnType, typename... Args> Object &function(const char *name, ReturnType (*func)(Args...))
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;

        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeMethodStatic<ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    Object &function_optional_override(const char *name, ReturnType (*func)(Args...))
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeGlobalMethodOptionalOverride<ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    jsvm_value Export(jsvm_env env, jsvm_value exports, const char *className)
    {
        jsvm_define_properties(env, object_, propertyDescriptorVector_.size(), propertyDescriptorVector_.data());

        if (exports != nullptr && className != nullptr)
        {
            jsvm_set_named_property(env, exports, className, object_);
        }
        return exports;
    }
    jsvm_value getHandle()
    {
        return object_;
    }
  private:
    std::vector<jsvm_property_descriptor> propertyDescriptorVector_;
    jsvm_value object_ = nullptr;
};
    template <typename EnumType> Enum_ &Enum_::value(const char *name, EnumType value)
    {
        object_->constant(name, (int32_t)value);
        return *this;
    }
} // namespace jsbind

#endif

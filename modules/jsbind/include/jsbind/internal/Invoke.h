#ifndef __V8_INVOKE_H__
#define __V8_INVOKE_H__

#include <jsbind/Error.h>
#include <jsbind/internal/ValueTraits.h>
#include <jsvm/JSVM_Types.h>
#include <map>
#include <string>
#include <type_traits>

namespace jsbind
{
namespace internal
{

template <typename T, typename Tuple, size_t... Seq>
T *tuple_call_class_constructor(jsvm_env env, jsvm_callback_info info, std::index_sequence<Seq...>)
{

    size_t argc = sizeof...(Seq);
    jsvm_value argv[sizeof...(Seq) + 1];
    jsvm_value _this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, argv, &_this, &data);

    return new T(ValueTraits<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(argv[Seq])...);
}

template <typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<!internal::is_void_return<Func>::value, jsvm_value>::type tuple_call(
    Func func, jsvm_value *args, std::index_sequence<Seq...>)
{
    return ValueTraits<typename function_traits<Func>::return_type>::ToJs(
        func(ValueTraits<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...));
}
template <typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<internal::is_void_return<Func>::value, jsvm_value>::type tuple_call(Func func, jsvm_value *args,
                                                                                            std::index_sequence<Seq...>)
{
    func(ValueTraits<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...);
    return nullptr;
}
template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<!internal::is_void_return<Func>::value, jsvm_value>::type tuple_call_with_this(
    ClassType *thisObject, Func func, jsvm_value *args, std::index_sequence<Seq...>)
{
    return ValueTraits<typename function_traits<Func>::return_type>::ToJs(
        (thisObject->*func)(ValueTraits<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...));
}
template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<internal::is_void_return<Func>::value, jsvm_value>::type tuple_call_with_this(
    ClassType *thisObject, Func func, jsvm_value *args, std::index_sequence<Seq...>)
{
    (thisObject->*func)(ValueTraits<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...);
    return nullptr;
}
template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<!internal::is_void_return<Func>::value, jsvm_value>::type tuple_call_optional_override(
    ClassType *thisObject, Func func, jsvm_value *args, std::index_sequence<Seq...>)
{
    return ValueTraits<typename function_traits<Func>::return_type>::ToJs(
        func(*thisObject, ValueTraits<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...));
}
template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<internal::is_void_return<Func>::value, jsvm_value>::type tuple_call_optional_override(
    ClassType *thisObject, Func func, jsvm_value *args, std::index_sequence<Seq...>)
{
    func(*thisObject, ValueTraits<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...);
    return nullptr;
}

template <typename ReturnType, typename... Args> jsvm_value InvokeMethodStatic(jsvm_env env, jsvm_callback_info info)
{

    size_t argc = sizeof...(Args);
    jsvm_value argv[sizeof...(Args) + 1];
    jsvm_value _this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, argv, &_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Not enough arguments, expected 1.");
    typedef ReturnType (*FunctorType)(Args...);
    FuncInfo<FunctorType> *funcInfo = (FuncInfo<FunctorType> *)data;
    /*if ((unsigned long)args.Length() < sizeof...(Args))
    {
        LOGE("Not enough arguments for function  %s", funcInfo->name.c_str());
        args.GetIsolate()->ThrowException(
            v8::String::NewFromUtf8(args.GetIsolate(), "Not enough arguments for function.").ToLocalChecked());
        return;
    }
*/
    return tuple_call<std::tuple<Args...>>(funcInfo->func, argv, std::make_index_sequence<sizeof...(Args)>());
}

template <typename ReturnType, typename... Args>
jsvm_value InvokeGlobalMethodOptionalOverride(jsvm_env env, jsvm_callback_info info)
{
    size_t argc = sizeof...(Args);
    jsvm_value argv[sizeof...(Args) + 1];
    jsvm_value _this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, argv, &_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Not enough arguments, expected 1.");

    typedef ReturnType (*FunctorType)(Args...);
    FuncInfo<FunctorType> *funcInfo = (FuncInfo<FunctorType> *)data;
    /*v8::Local<v8::Object> pthis = args.This();
    if ((unsigned long)args.Length() < sizeof...(Args))
    {
        LOGE("Not enough arguments for function  %s", funcInfo->name.c_str());
        args.GetIsolate()->ThrowException(
            v8::String::NewFromUtf8(args.GetIsolate(), "Not enough arguments for function.").ToLocalChecked());
        return;
    }*/

    return tuple_call<std::tuple<Args...>>(funcInfo->func, argv, std::make_index_sequence<sizeof...(Args)>());
}

template <typename ClassType, typename ReturnType, typename... Args>
jsvm_value InvokeClassMethod(jsvm_env env, jsvm_callback_info info)
{
    size_t argc = sizeof...(Args);
    jsvm_value argv[sizeof...(Args) + 1];
    jsvm_value _this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, argv, &_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm_unwrap(env, _this, reinterpret_cast<void **>(&pObj));
    // void *data = args.Data().As<v8::External>()->Value();
    typedef ReturnType (ClassType::*FunctorType)(Args...);
    FuncInfo<FunctorType> *funcInfo = (FuncInfo<FunctorType> *)data;
    // v8::Local<v8::Object> pthis = args.This();
    // ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    /*if ((unsigned long)args.Length() < sizeof...(Args))
    {
        LOGE("Not enough arguments for function  %s", funcInfo->name.c_str());
        args.GetIsolate()->ThrowException(
            v8::String::NewFromUtf8(args.GetIsolate(), "Not enough arguments for function.").ToLocalChecked());
        return nullptr;
    }*/
    return tuple_call_with_this<ClassType, std::tuple<Args...>>(pObj, funcInfo->func, argv,
                                                                std::make_index_sequence<sizeof...(Args)>());
}

template <typename ClassType, typename ReturnType, typename... Args>
jsvm_value InvokeClassMethodOptionalOverride(jsvm_env env, jsvm_callback_info info)
{
    size_t argc = sizeof...(Args);
    jsvm_value argv[sizeof...(Args) + 1];
    jsvm_value _this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, argv, &_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm_unwrap(env, _this, reinterpret_cast<void **>(&pObj));

    // void *data = args.Data().As<v8::External>()->Value();
    typedef ReturnType (*FunctorType)(ClassType &, Args...);
    FuncInfo<FunctorType> *funcInfo = (FuncInfo<FunctorType> *)data;

    // v8::Local<v8::Object> pthis = args.This();
    // ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    /*if ((unsigned long)args.Length() < sizeof...(Args))
    {
        LOGE("Not enough arguments for function  %s", funcInfo->name.c_str());
        args.GetIsolate()->ThrowException(
            v8::String::NewFromUtf8(args.GetIsolate(), "Not enough arguments for function.").ToLocalChecked());
        return nullptr;
    }*/

    return tuple_call_optional_override<ClassType, std::tuple<Args...>>(pObj, funcInfo->func, argv,
                                                                        std::make_index_sequence<sizeof...(Args)>());
}

template <typename ClassType, typename... Args> ClassType *InvokeClassConstructor(jsvm_env env, jsvm_callback_info info)
{
    return tuple_call_class_constructor<ClassType, std::tuple<Args...>>(env, info,
                                                                        std::make_index_sequence<sizeof...(Args)>());
}

template <typename ClassType, typename PropertyType> jsvm_value InvokeClassGetter(jsvm_env env, jsvm_callback_info info)
{
    // size_t argc = 1;
    // napi_value args[1];
    jsvm_value js_this;
    void *data;
    jsvm_get_cb_info(env, info, nullptr, nullptr, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm_unwrap(env, js_this, reinterpret_cast<void **>(&pObj));

    auto funcInfo = (PropFuncInfo<PropertyType (ClassType::*)(), void (ClassType::*)(PropertyType data)> *)data;

    // v8::Local<v8::Object> pthis = info.This();
    // ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    return ValueTraits<PropertyType>::ToJs((pObj->*funcInfo->fGet)());
}

template <typename ClassType, typename PropertyType>
jsvm_value InvokeClassGetterOptionalOverride(jsvm_env env, jsvm_callback_info info)
{

    // size_t argc = 1;
    // napi_value args[1];
    jsvm_value js_this;
    void *data;
    jsvm_get_cb_info(env, info, nullptr, nullptr, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm_unwrap(env, js_this, reinterpret_cast<void **>(&pObj));

    auto funcInfo = (PropFuncInfo<PropertyType (*)(ClassType &), void (*)(ClassType &, PropertyType data)> *)data;
    // v8::Local<v8::Object> pthis = info.This();
    // ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    return ValueTraits<PropertyType>::ToJs((*funcInfo->fGet)(*pObj));
}

template <typename ClassType, typename PropertyType> jsvm_value InvokeClassSetter(jsvm_env env, jsvm_callback_info info)
{

    size_t argc = 1;
    jsvm_value args[1];
    jsvm_value js_this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, args, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm_unwrap(env, js_this, reinterpret_cast<void **>(&pObj));

    auto funcInfo = (PropFuncInfo<PropertyType (ClassType::*)(), void (ClassType::*)(PropertyType data)> *)data;

    (pObj->*funcInfo->fSet)(ValueTraits<PropertyType>::ToCpp(args[0]));
    return nullptr;
}

template <typename ClassType, typename PropertyType>
jsvm_value InvokeClassSetterOptionalOverride(jsvm_env env, jsvm_callback_info info)
{
    size_t argc = 1;
    jsvm_value args[1];
    jsvm_value js_this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, args, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm_unwrap(env, js_this, reinterpret_cast<void **>(&pObj));

    auto funcInfo = (PropFuncInfo<PropertyType (*)(ClassType &), void (*)(ClassType &, PropertyType data)> *)data;

    (*funcInfo->fSet)(*pObj, ValueTraits<PropertyType>::ToCpp(args[0]));
    return nullptr;
}

template <typename PropertyType> jsvm_value InvokeClassGetterStatic(jsvm_env env, jsvm_callback_info info)
{

    // size_t argc = 1;
    // napi_value args[1];
    jsvm_value js_this;
    void *data;
    jsvm_get_cb_info(env, info, nullptr, nullptr, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");

    auto funcInfo = (PropFuncInfo<PropertyType (*)(), void (*)(PropertyType data)> *)data;
    return ValueTraits<PropertyType>::ToJs((funcInfo->fGet)());
}

template <typename PropertyType> jsvm_value InvokeClassSetterStatic(jsvm_env env, jsvm_callback_info info)
{
    size_t argc = 1;
    jsvm_value args[1];
    jsvm_value js_this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, args, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");

    auto funcInfo = (PropFuncInfo<PropertyType (*)(), void (*)(PropertyType data)> *)data;

    (funcInfo->fSet)(ValueTraits<PropertyType>::ToCpp(args[0]));
    return nullptr;
}

template <typename ClassType, typename PropertyType>
jsvm_value InvokeClassGetterField(jsvm_env env, jsvm_callback_info info)
{
    jsvm_value js_this;
    void *data;
    jsvm_get_cb_info(env, info, nullptr, nullptr, &js_this, &data);
    // NODE_API_ASSERT(env, argc == 0, "Wrong number of arguments");
    ClassType *pObj;
    jsvm_unwrap(env, js_this, reinterpret_cast<void **>(&pObj));
    auto funcInfo = (FuncInfo<PropertyType ClassType::*> *)data;

    return ValueTraits<PropertyType>::ToJs(pObj->*(funcInfo->func));
}

template <typename ClassType, typename PropertyType>
jsvm_value InvokeClassSetterField(jsvm_env env, jsvm_callback_info info)
{
    size_t argc = 1;
    jsvm_value args[1];
    jsvm_value js_this;
    void *data;
    jsvm_get_cb_info(env, info, &argc, args, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm_unwrap(env, js_this, reinterpret_cast<void **>(&pObj));
    auto funcInfo = (FuncInfo<PropertyType ClassType::*> *)data;
    (pObj->*(funcInfo->func)) = (ValueTraits<PropertyType>::ToCpp(args[0]));
    return NULL;
}

// template <typename ReturnType, typename... Args> struct V8Call;

template <typename... Args> jsvm_value v8_call(jsvm_env env, jsvm_value self, jsvm_value func, const Args &...args)
{
    /*v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());

    const int num_args = sizeof...(Args);

    v8::Local<v8::Value> v8_args[num_args + 1] = {ToJSValue(args)...};

    v8::TryCatch try_catch(v8::Isolate::GetCurrent());

    auto result = func->Call(v8::Isolate::GetCurrent()->GetCurrentContext(), self, num_args, v8_args);

    if (try_catch.HasCaught())
    {
        __JSRun::ReportException(v8::Isolate::GetCurrent(), &try_catch);
    }

    if (result.IsEmpty())
        return v8::Undefined(v8::Isolate::GetCurrent());

    return scope.Escape(result.ToLocalChecked());*/
    jsvm_status status;
    const size_t argc = sizeof...(Args);
    jsvm_value argv[argc + 1] = {internal::ToJSValue(args)...};

    jsvm_value result = nullptr;
    status = jsvm_call_function(env, self, func, argc, argv, &result);
    if (status != jsvm_status::jsvm_ok)
    {
        reportError(env, status);
    }
    return result;
}
} // namespace internal
} // namespace jsbind
#endif

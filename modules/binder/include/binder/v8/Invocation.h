#ifndef __V8_INVOCATION_H__
#define __V8_INVOCATION_H__

#include "internal/Converter.h"
#include <binder/JSVM_Types.h>
#include <map>
#include <string>
#include <type_traits>


namespace jsbind
{
namespace internal
{

template <typename T, typename Tuple, size_t... Seq>
T *tuple_call_class_constructor(jsvm::Env env, jsvm::CallbackInfo info, std::index_sequence<Seq...>)
{

    size_t argc = 0;
    jsvm::Value argv[128];
    jsvm::Value _this;
    void *data;
    jsvm::GetCbInfo(env, info, &argc, argv, &_this, &data);

    return new T(Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(argv[Seq])...);
}

template <typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<!internal::is_void_return<Func>::value, jsvm::Value>::type tuple_call(
    Func func, jsvm::Value *args, std::index_sequence<Seq...>)
{
    return Converter<typename function_traits<Func>::return_type>::ToJs(
        func( Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...));
}
template <typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<internal::is_void_return<Func>::value, jsvm::Value>::type tuple_call(
    Func func, jsvm::Value *args, std::index_sequence<Seq...>)
{
    func( Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...);
    return nullptr;
}
template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<!internal::is_void_return<Func>::value, jsvm::Value>::type tuple_call_with_this(
    ClassType *thisObject, Func func, jsvm::Value *args, std::index_sequence<Seq...>)
{
    return Converter<typename function_traits<Func>::return_type>::ToJs((thisObject->*func)(Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...));
}
template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<internal::is_void_return<Func>::value, jsvm::Value>::type tuple_call_with_this(
    ClassType *thisObject, Func func, jsvm::Value *args, std::index_sequence<Seq...>)
{
    (thisObject->*func)( Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...);
    return nullptr;
}
template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<!internal::is_void_return<Func>::value, jsvm::Value>::type tuple_call_optional_override(
    ClassType *thisObject, Func func, jsvm::Value *args, std::index_sequence<Seq...>)
{
    return Converter<typename function_traits<Func>::return_type>::ToJs(
        func(*thisObject, Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...));
}
template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<internal::is_void_return<Func>::value, jsvm::Value>::type tuple_call_optional_override(
    ClassType *thisObject, Func func, jsvm::Value *args, std::index_sequence<Seq...>)
{
    func(*thisObject, Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...);
    return nullptr;
}

template <typename ReturnType, typename... Args> jsvm::Value InvokeMethodStatic(jsvm::Env env, jsvm::CallbackInfo info)
{

    size_t argc;
    jsvm::Value argv[sizeof...(Args) + 1];
    jsvm::Value _this;
    void *data;
    jsvm::GetCbInfo(env, info, &argc, argv, &_this, &data);
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
#if 0
template <typename ReturnType, typename... Args>
void InvokeGlobalMethodOptionalOverride(jsvm::Env env, jsvm::CallbackInfo info)
{
    size_t argc = sizeof...(Args);
    jsvm::Value argv[sizeof...(Args)];
    jsvm::Value _this;
    void *data;
    GetCbInfo(env, info, &argc, argv, &_this, &data);
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

    tuple_call<std::tuple<Args...>>(funcInfo->func, args, std::make_index_sequence<sizeof...(Args)>());
}
#endif
template <typename ClassType, typename ReturnType, typename... Args>
jsvm::Value InvokeClassMethod(jsvm::Env env, jsvm::CallbackInfo info)
{
    size_t argc;
    jsvm::Value argv[sizeof...(Args) + 1];
    jsvm::Value _this;
    void *data;
    jsvm::GetCbInfo(env, info, &argc, argv, &_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm::Unwrap(env, _this, reinterpret_cast<void **>(&pObj));
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
jsvm::Value InvokeClassMethodOptionalOverride(jsvm::Env env, jsvm::CallbackInfo info)
{
    size_t argc;
    jsvm::Value argv[sizeof...(Args) + 1];
    jsvm::Value _this;
    void *data;
    jsvm::GetCbInfo(env, info, &argc, argv, &_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm::Unwrap(env, wrapped, reinterpret_cast<void **>(&pObj));

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

    return tuple_call_optional_override<ClassType, std::tuple<Args...>>(pObj, funcInfo->func, args,
                                                                        std::make_index_sequence<sizeof...(Args)>());
}

template <typename ClassType, typename... Args>
ClassType *InvokeClassConstructor(jsvm::Env env, jsvm::CallbackInfo info)
{
    return tuple_call_class_constructor<ClassType, std::tuple<Args...>>(env, info,
                                                                        std::make_index_sequence<sizeof...(Args)>());
}

template <typename ClassType, typename PropertyType> jsvm::Value InvokeClassGetter(jsvm::Env env, jsvm::CallbackInfo info)
{
    // size_t argc = 1;
    // napi_value args[1];
    jsvm::Value js_this;
    void *data;
    jsvm::GetCbInfo(env, info, nullptr, nullptr, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm::Unwrap(env, js_this, reinterpret_cast<void **>(&pObj));

    auto funcInfo = (PropFuncInfo<PropertyType (ClassType::*)(), void (ClassType::*)(PropertyType data)> *)data;

    // v8::Local<v8::Object> pthis = info.This();
    // ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    return Converter<PropertyType>::ToJs((pObj->*funcInfo->fGet)());
}

template <typename ClassType, typename PropertyType>
jsvm::Value InvokeClassGetterOptionalOverride(jsvm::Env env, jsvm::CallbackInfo info)
{

    // size_t argc = 1;
    // napi_value args[1];
    jsvm::Value js_this;
    void *data;
    jsvm::GetCbInfo(env, info, nullptr, nullptr, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm::Unwrap(env, js_this, reinterpret_cast<void **>(&pObj));

    auto funcInfo = (PropFuncInfo<PropertyType (*)(ClassType &), void (*)(ClassType &, PropertyType data)> *)data;
    // v8::Local<v8::Object> pthis = info.This();
    // ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    return Converter<PropertyType>::ToJs((*funcInfo->fGet)(*pObj));
}

template <typename ClassType, typename PropertyType> jsvm::Value InvokeClassSetter(jsvm::Env env, jsvm::CallbackInfo info)
{

    size_t argc;
    jsvm::Value args[1];
    jsvm::Value js_this;
    void *data;
    jsvm::GetCbInfo(env, info, &argc, args, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm::Unwrap(env, js_this, reinterpret_cast<void **>(&pObj));

    auto funcInfo = (PropFuncInfo<PropertyType (ClassType::*)(), void (ClassType::*)(PropertyType data)> *)data;

    (pObj->*funcInfo->fSet)(Converter<PropertyType>::ToCpp(args[0]));
    return nullptr;
}

template <typename ClassType, typename PropertyType>
jsvm::Value InvokeClassSetterOptionalOverride(jsvm::Env env, jsvm::CallbackInfo info)
{
    size_t argc;
    jsvm::Value args[1];
    jsvm::Value js_this;
    void *data;
    jsvm::GetCbInfo(env, info, &argc, args, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm::Unwrap(env, js_this, reinterpret_cast<void **>(&pObj));

    auto funcInfo = (PropFuncInfo<PropertyType (*)(ClassType &), void (*)(ClassType &, PropertyType data)> *)data;

    (*funcInfo->fSet)(*pObj, Converter<PropertyType>::ToCpp(args[0]));
    return nullptr;
}

template <typename PropertyType> jsvm::Value InvokeClassGetterStatic(jsvm::Env env, jsvm::CallbackInfo info)
{

    // size_t argc = 1;
    // napi_value args[1];
    jsvm::Value js_this;
    void *data;
    jsvm::GetCbInfo(env, info, nullptr, nullptr, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");

    auto funcInfo = (PropFuncInfo<PropertyType (*)(), void (*)(PropertyType data)> *)data;
    return Converter<PropertyType>::ToJs((funcInfo->fGet)());
}

template <typename PropertyType> jsvm::Value InvokeClassSetterStatic(jsvm::Env env, jsvm::CallbackInfo info)
{
    size_t argc;
    jsvm::Value args[1];
    jsvm::Value js_this;
    void *data;
    jsvm::GetCbInfo(env, info, &argc, args, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");

    auto funcInfo = (PropFuncInfo<PropertyType (*)(), void (*)(PropertyType data)> *)data;

    (funcInfo->fSet)(Converter<PropertyType>::ToCpp(args[0]));
    return nullptr;
}

template <typename ClassType, typename PropertyType>
jsvm::Value InvokeClassGetterField(jsvm::Env env, jsvm::CallbackInfo info)
{
    jsvm::Value js_this;
    void *data;
    jsvm::GetCbInfo(env, info, nullptr, nullptr, &js_this, &data);
    // NODE_API_ASSERT(env, argc == 0, "Wrong number of arguments");
    ClassType *pObj;
    jsvm::Unwrap(env, js_this, reinterpret_cast<void **>(&pObj));
    auto funcInfo = (FuncInfo<PropertyType ClassType::*> *)data;

    return Converter<PropertyType>::ToJs(pObj->*(funcInfo->func));
}

template <typename ClassType, typename PropertyType>
jsvm::Value InvokeClassSetterField(jsvm::Env env, jsvm::CallbackInfo info)
{
    size_t argc;
    jsvm::Value args[1];
    jsvm::Value js_this;
    void *data;
    jsvm::GetCbInfo(env, info, &argc, args, &js_this, &data);
    // NODE_API_ASSERT(env, argc >= 1, "Wrong number of arguments");
    ClassType *pObj;
    jsvm::Unwrap(env, js_this, reinterpret_cast<void **>(&pObj));
    auto funcInfo = (FuncInfo<PropertyType ClassType::*> *)data;
    (pObj->*(funcInfo->func)) = (Converter<PropertyType>::ToCpp(args[0]));
    return NULL;
}

// template <typename ReturnType, typename... Args> struct V8Call;

template <typename... Args> jsvm::Value v8_call(jsvm::Env env, jsvm::Value self, jsvm::Value func, const Args &...args)
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
    const size_t argc = sizeof...(Args);
    jsvm::Value argv[argc] = {ToJSValue(env, args)...};

    jsvm::Value result = nullptr;
    jsvm::CallFunction(env, self, func, argc, argv, &result);
    return result;
}
} // namespace internal
} // namespace binder
#endif

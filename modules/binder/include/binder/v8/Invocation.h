#ifndef __V8_INVOCATION_H__
#define __V8_INVOCATION_H__

#include "Converter.h"
#include <map>
#include <string>
#include <type_traits>
#include <utils/FunctionTraits.h>
#include <v8.h>
namespace binder
{
namespace internal
{

template <typename T, typename Tuple, size_t... Seq>
T *tuple_call_class_constructor(jsvm::Env env, jsvm::CallbackInfo info, std::index_sequence<Seq...>)
{

    size_t argc = 0;
    jsvm::Value argv[128];
    jsvm::Value _this;
    void* data;
    NODE_API_CALL(env, GetCbInfo(env, info, &argc, argv, &_this, &data));

    return new T(::laya::Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(env, argv[Seq])...);
}

template <typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<!laya::internal::is_void_return<Func>::value, void>::type tuple_call(
    Func func, const v8::FunctionCallbackInfo<v8::Value> &args, std::index_sequence<Seq...>)
{
    args.GetReturnValue().Set(laya::Converter<typename function_traits<Func>::return_type>::ToJs(
        func(laya::Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...)));
}
template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<!laya::internal::is_void_return<Func>::value, void>::type tuple_call_with_this(
    ClassType *thisObject, Func func, const v8::FunctionCallbackInfo<v8::Value> &args, std::index_sequence<Seq...>)
{
    args.GetReturnValue().Set(laya::Converter<typename function_traits<Func>::return_type>::ToJs(
        (thisObject->*func)(laya::Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...)));
}

template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<!laya::internal::is_void_return<Func>::value, void>::type tuple_call_optional_override(
    ClassType *thisObject, Func func, const v8::FunctionCallbackInfo<v8::Value> &args, std::index_sequence<Seq...>)
{
    args.GetReturnValue().Set(laya::Converter<typename function_traits<Func>::return_type>::ToJs(
        func(*thisObject, laya::Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...)));
}

template <typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<laya::internal::is_void_return<Func>::value, void>::type tuple_call(
    Func func, const v8::FunctionCallbackInfo<v8::Value> &args, std::index_sequence<Seq...>)
{
    func(laya::Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...);
}

template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<laya::internal::is_void_return<Func>::value, void>::type tuple_call_with_this(
    ClassType *thisObject, Func func, const v8::FunctionCallbackInfo<v8::Value> &args, std::index_sequence<Seq...>)
{
    (thisObject->*func)(laya::Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...);
}

template <typename ClassType, typename Tuple, typename Func, size_t... Seq>
typename std::enable_if<laya::internal::is_void_return<Func>::value, void>::type tuple_call_optional_override(
    ClassType *thisObject, Func func, const v8::FunctionCallbackInfo<v8::Value> &args, std::index_sequence<Seq...>)
{
    func(*thisObject, laya::Converter<typename std::tuple_element<Seq, Tuple>::type>::ToCpp(args[Seq])...);
}

template <typename ReturnType, typename... Args> void InvokeFunction(jsvm::Env env, jsvm::CallbackInfo info)
{

    size_t argc = 0;
    jsvm::Value argv[128];
    jsvm::Value _this;
    void* data;
    NODE_API_CALL(env, jsvm::GetCbInfo(env, info, &argc, argv, &_this, &data));
    //NODE_API_ASSERT(env, argc >= 1, "Not enough arguments, expected 1.");
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
    tuple_call<std::tuple<Args...>>(funcInfo->func, args, std::make_index_sequence<sizeof...(Args)>());
}

template <typename ClassType, typename ReturnType, typename... Args>
void InvokeClassMethod(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    void *data = args.Data().As<v8::External>()->Value();
    typedef ReturnType (ClassType::*FunctorType)(Args...);
    FuncInfo<FunctorType> *funcInfo = (FuncInfo<FunctorType> *)data;
    v8::Local<v8::Object> pthis = args.This();
    ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    if ((unsigned long)args.Length() < sizeof...(Args))
    {
        LOGE("Not enough arguments for function  %s", funcInfo->name.c_str());
        args.GetIsolate()->ThrowException(
            v8::String::NewFromUtf8(args.GetIsolate(), "Not enough arguments for function.").ToLocalChecked());
        return;
    }
    tuple_call_with_this<ClassType, std::tuple<Args...>>(pObj, funcInfo->func, args,
                                                         std::make_index_sequence<sizeof...(Args)>());
}

template <typename ClassType, typename ReturnType, typename... Args>
void InvokeClassMethodOptionalOverride(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    void *data = args.Data().As<v8::External>()->Value();
    typedef ReturnType (*FunctorType)(ClassType &, Args...);
    FuncInfo<FunctorType> *funcInfo = (FuncInfo<FunctorType> *)data;
    v8::Local<v8::Object> pthis = args.This();
    ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    if ((unsigned long)args.Length() < sizeof...(Args))
    {
        LOGE("Not enough arguments for function  %s", funcInfo->name.c_str());
        args.GetIsolate()->ThrowException(
            v8::String::NewFromUtf8(args.GetIsolate(), "Not enough arguments for function.").ToLocalChecked());
        return;
    }

    tuple_call_optional_override<ClassType, std::tuple<Args...>>(pObj, funcInfo->func, args,
                                                                 std::make_index_sequence<sizeof...(Args)>());
}
template <typename ReturnType, typename... Args>
void InvokeGlobalMethodOptionalOverride(jsvm::Env env, jsvm::CallbackInfo info)
{
    size_t argc = 0;
    jsvm::Value argv[128];
    jsvm::Value _this;
    void* data;
    NODE_API_CALL(env, GetCbInfo(env, info, &argc, argv, &_this, &data));
    //NODE_API_ASSERT(env, argc >= 1, "Not enough arguments, expected 1.");


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
template <typename ClassType, typename... Args>
ClassType *InvokeClassConstructor(jsvm::Env env, jsvm::CallbackInfo info)
{
    return tuple_call_class_constructor<ClassType, std::tuple<Args...>>(env, info,
                                                                        std::make_index_sequence<sizeof...(Args)>());
}

template <typename ClassType, typename PropertyType>
void InvokeClassGetProperty(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    auto funcInfo =
        (PropFuncInfo<PropertyType (ClassType::*)(), void (ClassType::*)(PropertyType data)> *)v8::External::Cast(
            *info.Data())
            ->Value();
    v8::Local<v8::Object> pthis = info.This();
    ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    info.GetReturnValue().Set(laya::Converter<PropertyType>::ToJs((pObj->*funcInfo->fGet)()));
}

template <typename ClassType, typename PropertyType>
void InvokeClassGetPropertyOptionalOverride(v8::Local<v8::String> property,
                                            const v8::PropertyCallbackInfo<v8::Value> &info)
{
    auto funcInfo =
        (PropFuncInfo<PropertyType (*)(ClassType &), void (*)(ClassType &, PropertyType data)> *)v8::External::Cast(
            *info.Data())
            ->Value();
    v8::Local<v8::Object> pthis = info.This();
    ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);
    info.GetReturnValue().Set(laya::Converter<PropertyType>::ToJs((*funcInfo->fGet)(*pObj)));
}

template <typename ClassType, typename PropertyType>
void InvokeClassSetProperty(v8::Local<v8::String> property, v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void> &info)
{
    auto funcInfo =
        (PropFuncInfo<PropertyType (ClassType::*)(), void (ClassType::*)(PropertyType data)> *)v8::External::Cast(
            *info.Data())
            ->Value();

    v8::Local<v8::Object> pthis = info.This();
    ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);

    (pObj->*funcInfo->fSet)(Converter<PropertyType>::ToCpp(value));
}

template <typename ClassType, typename PropertyType>
void InvokeClassSetPropertyOptionalOverride(v8::Local<v8::String> property, v8::Local<v8::Value> value,
                                            const v8::PropertyCallbackInfo<void> &info)
{
    auto funcInfo =
        (PropFuncInfo<PropertyType (*)(ClassType &), void (*)(ClassType &, PropertyType data)> *)v8::External::Cast(
            *info.Data())
            ->Value();

    v8::Local<v8::Object> pthis = info.This();
    ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);

    (*funcInfo->fSet)(*pObj, Converter<PropertyType>::ToCpp(value));
}

template <typename PropertyType>
void InvokeGetProperty(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    auto funcInfo =
        (PropFuncInfo<PropertyType (*)(), void (*)(PropertyType data)> *)v8::External::Cast(*info.Data())->Value();
    info.GetReturnValue().Set(laya::Converter<PropertyType>::ToJs((funcInfo->fGet)()));
}

template <typename PropertyType>
void InvokeSetProperty(v8::Local<v8::Name> property, v8::Local<v8::Value> value,
                       const v8::PropertyCallbackInfo<void> &info)
{
    auto funcInfo =
        (PropFuncInfo<PropertyType (*)(), void (*)(PropertyType data)> *)v8::External::Cast(*info.Data())->Value();

    (funcInfo->fSet)(Converter<PropertyType>::ToCpp(value));
}

template <typename ClassType, typename PropertyType>
void InvokeGetPropertyField(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> pthis = info.This();
    ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);

    auto funcInfo = (FuncInfo<PropertyType ClassType::*> *)v8::External::Cast(*info.Data())->Value();
    info.GetReturnValue().Set(laya::Converter<PropertyType>::ToJs(pObj->*(funcInfo->func)));
}

template <typename ClassType, typename PropertyType>
void InvokeSetPropertyField(v8::Local<v8::String> property, v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void> &info)
{
    v8::Local<v8::Object> pthis = info.This();
    ClassType *pObj = (ClassType *)pthis->GetAlignedPointerFromInternalField(0);

    auto funcInfo = (FuncInfo<PropertyType ClassType::*> *)v8::External::Cast(*info.Data())->Value();

    (pObj->*(funcInfo->func)) = (Converter<PropertyType>::ToCpp(value));
}

template <typename ReturnType, typename... Args> struct V8Call;

template <typename... Args>
v8::Local<v8::Value> v8_call(v8::Local<v8::Value> self, v8::Function *func, const Args &...args)
{
    v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());

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

    return scope.Escape(result.ToLocalChecked());
}
} // namespace internal
} // namespace laya
#endif

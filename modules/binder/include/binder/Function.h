#ifndef __JSBIND_FUNCTION_H__
#define __JSBIND_FUNCTION_H__

#include <binder/JSEnv.h>
#include <binder/JSVM.h>
#include <binder/JSVM_Types.h>
#include <binder/Invoke.h>

namespace jsbind
{
namespace internal
{
template <typename R, typename... P> inline void finalizer(jsvm::Env env, void *finalize_data, void *finalize_hint)
{
    std::function<R(P...)> *data = reinterpret_cast<std::function<R(P...)> *>(finalize_data);
    delete data;
    data = nullptr;
}
template <typename ReturnType, typename... Args>
inline jsvm::Value makeFunction(std::function<ReturnType(Args...)> value)
{
    GET_ENV
    FuncInfo<decltype(value)> *data = new FuncInfo<decltype(value)>(value);
    internal::addDeinitializer([data]() { delete data; }); // TODO ?????

    jsvm::Status status;
    jsvm::Value result = nullptr;
    auto invoke = std::make_unique<std::function<ReturnType(Args...)>>(std::move(value));
    std::function<ReturnType(Args...)> *func = invoke.release();
    status =
        jsvm::CreateFunction(env, "", NAPI_AUTO_LENGTH,
                             internal::InvokeGlobalMethodOptionalOverride<ReturnType, Args...>, func, data, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    status = jsvm::AddFinalizer(env, result, func, finalizer, nullptr, nullptr);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline jsvm::Value cnm(jsvm::Env env, jsvm::CallbackInfo info)
{
    // todo
    return makeNull();
}
inline jsvm::Value makeFunctionRaw(std::function<jsvm::Value(jsvm::Env env, jsvm::CallbackInfo info)> value)
{
    GET_ENV

    jsvm::Status status;
    jsvm::Value result = nullptr;
    auto invoke =
        std::make_unique<std::function<jsvm::Value(jsvm::Env env, jsvm::CallbackInfo info)>>(std::move(value));
    std::function<jsvm::Value(jsvm::Env env, jsvm::CallbackInfo info)> *func = invoke.release();
    status = jsvm::CreateFunction(env, "", NAPI_AUTO_LENGTH, cnm, func, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    /// status = jsvm::AddFinalizer(env, result, func, finalizer, nullptr, nullptr);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
} // namespace internal
template <typename R, typename... Args> inline jsvm::Value MakeFunction(std::function<R(Args...)> value)
{
    return internal::makeFunction(value);
}
inline jsvm::Value MakeFunctionRaw(std::function<jsvm::Value(jsvm::Env env, jsvm::CallbackInfo info)> value)
{
    return internal::makeFunctionRaw(value);
}


} // namespace jsbind
#endif

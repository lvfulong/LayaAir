#ifndef __JSBIND_FUNCTION_H__
#define __JSBIND_FUNCTION_H__

#include <jsbind/internal/Invoke.h>
#include <jsbind/internal/Value.h>
#include <jsvm/JSEnv.h>
#include <jsvm/JSVM.h>
#include <jsvm/JSVM_Types.h>

namespace jsbind
{
namespace internal
{
template <typename R, typename... P> inline void finalizer(jsvm_env env, void *finalize_data, void *finalize_hint)
{
    std::function<R(P...)> *data = reinterpret_cast<std::function<R(P...)> *>(finalize_data);
    delete data;
    data = nullptr;
}
template <typename ReturnType, typename... Args>
inline jsvm_value makeFunction(std::function<ReturnType(Args...)> value)
{
    GET_ENV
    FuncInfo<decltype(value)> *data = new FuncInfo<decltype(value)>(value);
    internal::addDeinitializer([data]() { delete data; }); // TODO ?????

    jsvm_status status;
    jsvm_value result = nullptr;
    auto invoke = std::make_unique<std::function<ReturnType(Args...)>>(std::move(value));
    std::function<ReturnType(Args...)> *func = invoke.release();
    status =
        jsvm_create_function(env, "", NAPI_AUTO_LENGTH,
                             internal::InvokeGlobalMethodOptionalOverride<ReturnType, Args...>, func, data, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    status = jsvm_add_finalizer(env, result, func, finalizer, nullptr, nullptr);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline jsvm_value cnm(jsvm_env env, jsvm_callback_info info)
{
    // todo
    return makeNull();
}
inline jsvm_value makeFunctionRaw(std::function<jsvm_value(jsvm_env env, jsvm_callback_info info)> value)
{
    GET_ENV

    jsvm_status status;
    jsvm_value result = nullptr;
    auto invoke = std::make_unique<std::function<jsvm_value(jsvm_env env, jsvm_callback_info info)>>(std::move(value));
    std::function<jsvm_value(jsvm_env env, jsvm_callback_info info)> *func = invoke.release();
    status = jsvm_create_function(env, "", NAPI_AUTO_LENGTH, cnm, func, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    /// status = jsvm::AddFinalizer(env, result, func, finalizer, nullptr, nullptr);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
} // namespace internal
template <typename R, typename... Args> inline jsvm_value MakeFunction(std::function<R(Args...)> value)
{
    return internal::makeFunction(value);
}
inline jsvm_value MakeFunctionRaw(std::function<jsvm_value(jsvm_env env, jsvm_callback_info info)> value)
{
    return internal::makeFunctionRaw(value);
}

} // namespace jsbind
#endif

#include <jsbind/Error.h>
#include <string>
#include <utils/Log.h>
namespace jsbind
{
void reportError(jsvm::Env env, jsvm::Status res)
{
    jsvm::Value exceptionValue;
    jsvm::Status status = jsvm::GetAndClearLastException(env, &exceptionValue);
    if (status == jsvm::Status::OK)
    {
        jsvm::Value message;
        jsvm::GetNamedProperty(env, exceptionValue, "message", &message);
        size_t length;
        jsvm::GetValueStringUtf8(env, message, nullptr, 0, &length);
        char *buffer = new char[length + 1];
        jsvm::GetValueStringUtf8(env, message, buffer, length + 1, nullptr);
        jsvm::ThrowError(env, std::to_string(static_cast<int>(status)).c_str(), buffer);
        LOGE("jsvm failed Error code:  %s", buffer);
        delete[] buffer;
    }
    else
    {
        jsvm::ThrowError(env, std::to_string(static_cast<int>(status)).c_str(), nullptr);
        LOGE("jsvm failed Error code:  %d", static_cast<int>(res));
    }

#if 0
        bool isExceptionPending;
    auto status = napi_is_exception_pending(env, &isExceptionPending);
    DEBUG_CHECK(status == napi_ok);

    if (isExceptionPending)
    {
        napi_value result = nullptr;
        status = napi_get_and_clear_last_exception(env, &result);
        DEBUG_CHECK(status == napi_ok);
        //v8::Local<v8::Value> val = v8impl::V8LocalValueFromJsValue(result);
        //ReportException(env->isolate, val);
    }
    #endif
}
} // namespace jsbind

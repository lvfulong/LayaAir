#include <jsbind/Error.h>
#include <string>
#include <utils/Log.h>
namespace jsbind
{
void reportError(jsvm::Env env, jsvm::Status res)
{
    jsvm::Value exceptionValue;
    jsvm::Status status = jsvm::GetAndClearLastException(env, &exceptionValue);
    DEBUG_CHECK(status == jsvm::Status::OK);
    if (status == jsvm::Status::OK)
    {
        jsvm::Value message;
        status = jsvm::GetNamedProperty(env, exceptionValue, "message", &message);
        DEBUG_CHECK(status == jsvm::Status::OK);
        size_t length;
        status = jsvm::GetValueStringUtf8(env, message, nullptr, 0, &length);
        DEBUG_CHECK(status == jsvm::Status::OK);
        char *buffer = new char[length + 1];
        status = jsvm::GetValueStringUtf8(env, message, buffer, length + 1, nullptr);
        DEBUG_CHECK(status == jsvm::Status::OK);
        status = jsvm::ThrowError(env, std::to_string(static_cast<int>(status)).c_str(), buffer);
        DEBUG_CHECK(status == jsvm::Status::OK);
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

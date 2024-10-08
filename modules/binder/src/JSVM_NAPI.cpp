#include "binder/JSVM.h"
#include "binder/napi/js_native_api.h"
namespace jsvm
{
inline napi_status ConvertTo_NapiStatus(Status value)
{
    switch (value)
    {
    case Status::OK:
        return napi_ok;
    case Status::INVALID_ARG:
        return napi_invalid_arg;
    case Status::OBJECT_EXPECTED:
        return napi_object_expected;
    case Status::STRING_EXPECTED:
        return napi_string_expected;
    case Status::NAME_EXPECTED:
        return napi_name_expected;
    case Status::FUNCTION_EXPECTED:
        return napi_function_expected;
    case Status::NUMBER_EXPECTED:
        return napi_number_expected;
    case Status::BOOLEAN_EXPECTED:
        return napi_boolean_expected;
    case Status::ARRAY_EXPECTED:
        return napi_array_expected;
    case Status::GENERIC_FAILURE:
        return napi_generic_failure;
    case Status::PENDING_EXCEPTION:
        return napi_pending_exception;
    case Status::CANCELLED:
        return napi_cancelled;
    case Status::ESCAPE_CALLED_TWICE:
        return napi_escape_called_twice;
    case Status::HANDLE_SCOPE_MISMATCH:
        return napi_handle_scope_mismatch;
    case Status::CALLBACK_SCOPE_MISMATCH:
        return napi_callback_scope_mismatch;
    case Status::QUEUE_FULL:
        return napi_queue_full;
    case Status::CLOSING:
        return napi_closing;
    case Status::BIGINT_EXPECTED:
        return napi_bigint_expected;
    case Status::DATE_EXPECTED:
        return napi_date_expected;
    case Status::ARRAYBUFFER_EXPECTED:
        return napi_arraybuffer_expected;
    case Status::DETACHABLE_ARRAYBUFFER_EXPECTED:
        return napi_detachable_arraybuffer_expected;
    case Status::WOULD_DEADLOCK:
        return napi_would_deadlock;
    case Status::NO_EXTERNAL_BUFFERS_ALLOWED:
        return napi_no_external_buffers_allowed;
    case Status::CANNOT_RUN_JS:
        return napi_cannot_run_js;
    default:
        DEBUG_CHECK(false);
        break;
    }
}

inline Status ConvertToStatus(napi_status value)
{
    switch (value)
    {
    case napi_ok:
        return Status::OK;
    case napi_invalid_arg:
        return Status::INVALID_ARG;
    case napi_object_expected:
        return Status::OBJECT_EXPECTED;
    case napi_string_expected:
        return Status::STRING_EXPECTED;
    case napi_name_expected:
        return Status::NAME_EXPECTED;
    case napi_function_expected:
        return Status::FUNCTION_EXPECTED;
    case napi_number_expected:
        return Status::NUMBER_EXPECTED;
    case napi_boolean_expected:
        return Status::BOOLEAN_EXPECTED;
    case napi_array_expected:
        return Status::ARRAY_EXPECTED;
    case napi_generic_failure:
        return Status::GENERIC_FAILURE;
    case napi_pending_exception:
        return Status::PENDING_EXCEPTION;
    case napi_cancelled:
        return Status::CANCELLED;
    case napi_escape_called_twice:
        return Status::ESCAPE_CALLED_TWICE;
    case napi_handle_scope_mismatch:
        return Status::HANDLE_SCOPE_MISMATCH;
    case napi_callback_scope_mismatch:
        return Status::CALLBACK_SCOPE_MISMATCH;
    case napi_queue_full:
        return Status::QUEUE_FULL;
    case napi_closing:
        return Status::CLOSING;
    case napi_bigint_expected:
        return Status::BIGINT_EXPECTED;
    case napi_date_expected:
        return Status::DATE_EXPECTED;
    case napi_arraybuffer_expected:
        return Status::ARRAYBUFFER_EXPECTED;
    case napi_detachable_arraybuffer_expected:
        return Status::DETACHABLE_ARRAYBUFFER_EXPECTED;
    case napi_would_deadlock:
        return Status::WOULD_DEADLOCK;
    case napi_no_external_buffers_allowed:
        return Status::NO_EXTERNAL_BUFFERS_ALLOWED;
    case napi_cannot_run_js:
        return Status::CANNOT_RUN_JS;
    default:
        DEBUG_CHECK(false);
        break;
    }
}
inline /*JSVM_EXTERN*/ Status GetArrayLength(Env env, Value value, uint32_t *result)
{
    return ConvertToStatus(napi_get_array_length(env, value, result));
}
inline /*JSVM_EXTERN*/ Status IsArray(Env env, Value value, bool *result)
{
    return ConvertToStatus(napi_is_array(env, value, result));
}
inline /*JSVM_EXTERN*/ Status CreatePromise(Env env, Deferred *deferred, Value *promise)
{
    return ConvertToStatus(napi_create_promise(env, deferred, promise));
}
inline /*JSVM_EXTERN*/ Status CreateArray(Env env, Value *result)
{
    return ConvertToStatus(napi_create_array(env, result));
}
inline /*JSVM_EXTERN*/ Status CreateArrayWithLength(Env env, size_t length, Value *result)
{
    return ConvertToStatus(napi_create_array_with_length(env, length, result));
}
inline /*JSVM_EXTERN*/ Status CreateDouble(Env env, double value, Value *result)
{
    return ConvertToStatus(napi_create_double(env, value, result));
}
inline /*JSVM_EXTERN*/ Status CreateInt32(Env env, int32_t value, Value *result)
{
    return ConvertToStatus(napi_create_int32(env, value, result));
}
inline /*JSVM_EXTERN*/ Status CreateUint32(Env env, uint32_t value, Value *result)
{
    return ConvertToStatus(napi_create_uint32(env, value, result));
}
inline /*JSVM_EXTERN*/ Status CreateInt64(Env env, int64_t value, Value *result)
{
    return ConvertToStatus(napi_create_int64(env, value, result));
}

inline /*JSVM_EXTERN*/ Status GetValueDouble(Env env, Value value, double *result)
{
    return ConvertToStatus(napi_get_value_double(env, value, result));
}

inline /*JSVM_EXTERN*/ Status GetValueInt32(Env env, Value value, int32_t *result)
{
    return ConvertToStatus(napi_get_value_int32(env, value, result));
}
inline /*JSVM_EXTERN*/ Status GetValueUint32(Env env, Value value, uint32_t *result)
{
    return ConvertToStatus(napi_get_value_uint32(env, value, result));
}
inline /*JSVM_EXTERN*/ Status GetValueInt64(Env env, Value value, int64_t *result)
{
    return ConvertToStatus(napi_get_value_int64(env, value, result));
}
inline /*JSVM_EXTERN*/ Status GetValueBool(Env env, Value value, bool *result)
{
    return ConvertToStatus(napi_get_value_bool(env, value, result));
}
inline /*JSVM_EXTERN*/ Status GetNull(Env env, Value *result)
{
    return ConvertToStatus(napi_get_null(env, result));
}
inline /*JSVM_EXTERN*/ Status GetUndefined(Env env, Value *result)
{
    return ConvertToStatus(napi_get_undefined(env, result));
}
inline /*JSVM_EXTERN*/ Status GetElement(Env env, Value object, uint32_t index, Value *result)
{
    return ConvertToStatus(napi_get_element(env, object, index, result));
}
inline /*JSVM_EXTERN*/ Status SetElement(Env env, Value object, uint32_t index, Value value)
{
    return ConvertToStatus(napi_set_element(env, object, index, value));
}
inline /*JSVM_EXTERN*/ Status CreateStringUtf8(Env env, const char *value, size_t length, Value *result)
{
    return ConvertToStatus(napi_create_string_utf8(env, value, result));
}
inline /*JSVM_EXTERN*/ Status GetValueStringUtf8(Env env, Value value, char *buf, size_t bufsize, size_t *result)
{
    return ConvertToStatus(napi_get_value_string_utf8(env, value, buf, bufsize, result));
}
inline /*JSVM_EXTERN*/ Status AdjustExternalMemory(Env env, int64_t changeInBytes, int64_t *adjustedValue)
{
    return ConvertToStatus(napi_adjust_external_memory(env, changeInBytes, adjustedValue));
}
inline /*JSVM_EXTERN*/ Status IsSet(Env env, Value value, bool *isSet)
{
    return ConvertToStatus(napi_adjust_external_memory(env, changeInBytes, adjustedValue));
}
inline /*JSVM_EXTERN*/ Status CreateSet(Env env, Value *result)
{
    return ConvertToStatus(napi_adjust_external_memory(env, changeInBytes, adjustedValue));
}

inline /*JSVM_EXTERN*/ Status IsSet(Env env, Value value, bool *isSet)
{
    // todo
}
inline /*JSVM_EXTERN*/ Status CreateSet(Env env, Value *result)
{
    // todo
}
inline /*JSVM_EXTERN*/ Status GetCbInfo(Env env, CallbackInfo cbinfo, size_t *argc, Value *argv, Value *thisArg, void **data)
{
    return ConvertToStatus(napi_get_cb_info(env, cbinfo, argc, argv, thisArg, data));
}
} // namespace jsvm
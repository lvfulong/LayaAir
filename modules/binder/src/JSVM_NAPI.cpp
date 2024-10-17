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

inline napi_valuetype ConvertTo_NapiValueType(ValueType value)
{

    switch (value)
    {
    case ValueType::UNDEFINED:
        return napi_undefined;
    case ValueType::Null:
        return napi_null;
    case ValueType::BOOLEAN:
        return napi_boolean;
    case ValueType::NUMBER:
        return napi_number;
    case ValueType::STRING:
        return napi_string;
    case ValueType::SYMBOL:
        return napi_symbol;
    case ValueType::OBJECT:
        return napi_object;
    case ValueType::FUNCTION:
        return napi_function;
    case ValueType::EXTERNAL:
        return napi_external;
    case ValueType::BIGINT:
        return napi_bigint;
    default:
        DEBUG_CHECK(false);
        break;
    }
}

inline ValueType ConvertToValueType(napi_valuetype value)
{
    switch (value)
    {
    case napi_undefined:
        return ValueType::UNDEFINED;
    case napi_null:
        return ValueType::Null;
    case napi_boolean:
        return ValueType::BOOLEAN;
    case napi_number:
        return ValueType::NUMBER;
    case napi_string:
        return ValueType::STRING;
    case napi_symbol:
        return ValueType::SYMBOL;
    case napi_object:
        return ValueType::OBJECT;
    case napi_function:
        return ValueType::FUNCTION;
    case napi_external:
        return ValueType::EXTERNAL;
    case napi_bigint:
        return ValueType::BIGINT;
    default:
        DEBUG_CHECK(false);
        break;
    }
}

inline napi_typedarray_type ConvertTo_NapiTypedArrayType(TypedArrayType value)
{
    switch (value)
    {
    case TypedArrayType::INT8_ARRAY:
        return napi_int8_array;
    case TypedArrayType::UINT8_ARRAY:
        return napi_uint8_array;
    case TypedArrayType::UINT8_CLAMPED_ARRAY:
        return napi_uint8_clamped_array;
    case TypedArrayType::INT16_ARRAY:
        return napi_int16_array;
    case TypedArrayType::UINT16_ARRAY:
        return napi_uint16_array;
    case TypedArrayType::INT32_ARRAY:
        return napi_int32_array;
    case TypedArrayType::UINT32_ARRAY:
        return napi_uint32_array;
    case TypedArrayType::FLOAT32_ARRAY:
        return napi_float32_array;
    case TypedArrayType::FLOAT64_ARRAY:
        return napi_float64_array;
    case TypedArrayType::BIGINT64_ARRAY:
        return napi_bigint64_array;
    case TypedArrayType::BIGUINT64_ARRAY:
        return napi_biguint64_array;
    default:
        DEBUG_CHECK(false);
        break;
    }
}
inline TypedArrayType ConvertToTypedArrayType(napi_typedarray_type value)
{
    switch (value)
    {
    case napi_int8_array:
        return TypedArrayType::INT8_ARRAY;
    case napi_uint8_array:
        return TypedArrayType::UINT8_ARRAY;
    case napi_uint8_clamped_array:
        return TypedArrayType::UINT8_CLAMPED_ARRAY;
    case napi_int16_array:
        return TypedArrayType::INT16_ARRAY;
    case napi_uint16_array:
        return TypedArrayType::UINT16_ARRAY;
    case napi_int32_array:
        return TypedArrayType::INT32_ARRAY;
    case napi_uint32_array:
        return TypedArrayType::UINT32_ARRAY;
    case napi_float32_array:
        return TypedArrayType::FLOAT32_ARRAY;
    case napi_float64_array:
        return TypedArrayType::FLOAT64_ARRAY;
    case napi_bigint64_array:
        return TypedArrayType::BIGINT64_ARRAY;
    case napi_biguint64_array:
        return TypedArrayType::BIGUINT64_ARRAY;
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
    return ConvertToStatus(napi_create_string_utf8(env, value, length, result));
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
    return Status::OK; // todo
}
inline /*JSVM_EXTERN*/ Status CreateSet(Env env, Value *result)
{
    return Status::OK; // todo
}
inline /*JSVM_EXTERN*/ Status GetCbInfo(Env env, CallbackInfo cbinfo, size_t *argc, Value *argv, Value *thisArg,
                                        void **data)
{
    return ConvertToStatus(napi_get_cb_info(env, cbinfo, argc, argv, thisArg, data));
}
inline /*JSVM_EXTERN*/ Status SetNamedProperty(Env env, Value object, const char *utf8name, Value value)
{
    return ConvertToStatus(napi_set_named_property(env, object, utf8name, value));
}
inline /*JSVM_EXTERN*/ Status GetNewTarget(Env env, CallbackInfo cbinfo, Value *result)
{
    return ConvertToStatus(napi_get_new_target(env, cbinfo, result));
}
inline /*JSVM_EXTERN*/ Status Wrap(Env env, Value jsObject, void *nativeObject, Finalize finalizeCb, void *finalizeHint,
                                   Ref *result)
{
    return ConvertToStatus(napi_wrap(env, jsObject, nativeObject, finalizeCb, finalizeHint, result));
}
inline /*JSVM_EXTERN*/ Status Unwrap(Env env, Value jsObject, void **result)
{
    return ConvertToStatus(napi_unwrap(env, jsObject, result));
}
inline /*JSVM_EXTERN*/ Status CreateReference(Env env, Value value, uint32_t initialRefcount, Ref *result)
{
    return ConvertToStatus(napi_create_reference(env, value, initialRefcount, result));
}
inline /*JSVM_EXTERN*/ Status DeleteReference(Env env, Ref ref)
{
    return ConvertToStatus(napi_delete_reference(env, ref));
}
inline /*JSVM_EXTERN*/ Status ReferenceRef(Env env, Ref ref, uint32_t *result)
{
    return ConvertToStatus(napi_reference_ref(env, ref, result));
}
inline /*JSVM_EXTERN*/ Status ReferenceUnref(Env env, Ref ref, uint32_t *result)
{
    return ConvertToStatus(napi_reference_unref(env, ref, result));
}
inline /*JSVM_EXTERN*/ Status GetReferenceValue(Env env, Ref ref, Value *result)
{
    return ConvertToStatus(napi_get_reference_value(env, ref, result));
}
inline /*JSVM_EXTERN*/ Status NewInstance(Env env, Value constructor, size_t argc, const Value *argv, Value *result)
{
    return ConvertToStatus(napi_new_instance(env, constructor, argc, argv, result));
}
inline /*JSVM_EXTERN*/ Status DefineClass(Env env, const char *utf8name, size_t length, Callback constructor,
                                          size_t propertyCount, const PropertyDescriptor *properties, Value *result)
{
    return ConvertToStatus(
        napi_define_class(env, utf8name, length, constructor, nullptr, propertyCount, properties, result));
}
inline /*JSVM_EXTERN*/ Status CallFunction(Env env, Value recv, Value func, size_t argc, const Value *argv,
                                           Value *result)
{
    return ConvertToStatus(napi_call_function(env, recv, func, argc, argv, result));
}
inline /*JSVM_EXTERN*/ Status CreateFunction(Env env, const char *utf8name, size_t length, Callback cb, Value *result)
{
    return ConvertToStatus(napi_create_function(env, utf8name, length, cb, result, nullptr));
}
inline /*JSVM_EXTERN*/ Status Typeof(Env env, Value value, ValueType *result)
{
    napi_valuetype napi_result;
    auto status = napi_typeof(env, value, &napi_result);
    *result = ConvertToValueType(napi_result);
    return ConvertToStatus(status);
}
} // namespace jsvm
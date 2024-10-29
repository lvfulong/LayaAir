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

inline napi_typedarray_type ConvertTo_NapiTypedarrayType(TypedarrayType value)
{
    switch (value)
    {
    case TypedarrayType::INT8_ARRAY:
        return napi_int8_array;
    case TypedarrayType::UINT8_ARRAY:
        return napi_uint8_array;
    case TypedarrayType::UINT8_CLAMPED_ARRAY:
        return napi_uint8_clamped_array;
    case TypedarrayType::INT16_ARRAY:
        return napi_int16_array;
    case TypedarrayType::UINT16_ARRAY:
        return napi_uint16_array;
    case TypedarrayType::INT32_ARRAY:
        return napi_int32_array;
    case TypedarrayType::UINT32_ARRAY:
        return napi_uint32_array;
    case TypedarrayType::FLOAT32_ARRAY:
        return napi_float32_array;
    case TypedarrayType::FLOAT64_ARRAY:
        return napi_float64_array;
    case TypedarrayType::BIGINT64_ARRAY:
        return napi_bigint64_array;
    case TypedarrayType::BIGUINT64_ARRAY:
        return napi_biguint64_array;
    default:
        DEBUG_CHECK(false);
        break;
    }
}
inline TypedarrayType ConvertToTypedarrayType(napi_typedarray_type value)
{
    switch (value)
    {
    case napi_int8_array:
        return TypedarrayType::INT8_ARRAY;
    case napi_uint8_array:
        return TypedarrayType::UINT8_ARRAY;
    case napi_uint8_clamped_array:
        return TypedarrayType::UINT8_CLAMPED_ARRAY;
    case napi_int16_array:
        return TypedarrayType::INT16_ARRAY;
    case napi_uint16_array:
        return TypedarrayType::UINT16_ARRAY;
    case napi_int32_array:
        return TypedarrayType::INT32_ARRAY;
    case napi_uint32_array:
        return TypedarrayType::UINT32_ARRAY;
    case napi_float32_array:
        return TypedarrayType::FLOAT32_ARRAY;
    case napi_float64_array:
        return TypedarrayType::FLOAT64_ARRAY;
    case napi_bigint64_array:
        return TypedarrayType::BIGINT64_ARRAY;
    case napi_biguint64_array:
        return TypedarrayType::BIGUINT64_ARRAY;
    default:
        DEBUG_CHECK(false);
        break;
    }
}

napi_property_attributes convertTo(PropertyAttributes value)
{
    switch (value)
    {
    case PropertyAttributes::DEFAULT:
        return napi_default;
    case PropertyAttributes::WRITABLE:
        return napi_writable;
    case PropertyAttributes::ENUMERABLE:
        return napi_enumerable;
    case PropertyAttributes::CONFIGURABLE:
        return napi_configurable;
    case PropertyAttributes::STATIC:
        return napi_static;
    case PropertyAttributes::DEFAULT_METHOD:
        return napi_default_method;
    case PropertyAttributes::DEFAULT_JSPROPERTY:
        return napi_default_jsproperty;
    default:
        DEBUG_CHECK(false);
        break;
    }
}
inline /*JSVM_EXTERN*/ Status CreateEnv(VM vm, size_t propertyCount, const PropertyDescriptor *properties, Env *result)
{
}
inline /*JSVM_EXTERN*/ Status DestroyEnv(Env env)
{
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
    napi_property_descriptor napi_properties;
    napi_properties.utf8name = properties->utf8name;
    napi_properties.name = properties->name;
    napi_properties.method = properties->method;
    napi_properties.getter = properties->getter;
    napi_properties.setter = properties->setter;
    napi_properties.value = properties->value;
    napi_properties.attributes = convertTo(properties->attributes);
    napi_properties.data = properties->data;
    return ConvertToStatus(
        napi_define_class(env, utf8name, length, constructor, nullptr, propertyCount, &napi_properties, result));
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
inline /*JSVM_EXTERN*/ Status CreateArraybuffer(Env env, size_t byteLength, void **data, Value *result)
{
    return ConvertToStatus(napi_create_arraybuffer(env, byteLength, data, result));
}
inline /*JSVM_EXTERN*/ Status IsArraybuffer(Env env, Value value, bool *result)
{
    return ConvertToStatus(napi_is_arraybuffer(env, value, result));
}
inline /*JSVM_EXTERN*/ Status IsTypedarray(Env env, Value value, bool *result)
{
    return ConvertToStatus(napi_is_typedarray(env, value, result));
}
inline /*JSVM_EXTERN*/ Status IsDataview(Env env, Value value, bool *result)
{
    return ConvertToStatus(napi_is_dataview(env, value, result));
}
inline /*JSVM_EXTERN*/ Status CreateTypedarray(Env env, TypedarrayType type, size_t length, Value arraybuffer,
                                               size_t byteOffset, Value *result)
{
    return ConvertToStatus(
        napi_create_typedarray(env, ConvertTo_NapiTypedarrayType(type), length, arraybuffer, byteOffset, result));
}
inline /*JSVM_EXTERN*/ Status CreateDataview(Env env, size_t length, Value arraybuffer, size_t byteOffset,
                                             Value *result)
{
    return ConvertToStatus(napi_create_dataview(env, length, arraybuffer, byteOffset, result));
}
inline /*JSVM_EXTERN*/ Status GetArraybufferInfo(Env env, Value arraybuffer, void **data, size_t *byteLength)
{
    return ConvertToStatus(napi_get_arraybuffer_info(env, arraybuffer, data, byteLength));
}
inline /*JSVM_EXTERN*/ Status GetTypedarrayInfo(Env env, Value typedarray, TypedarrayType *type, size_t *length,
                                                void **data, Value *arraybuffer, size_t *byteOffset)
{
    napi_typedarray_type napi_type;
    auto status =
        ConvertToStatus(napi_get_typedarray_info(env, typedarray, &napi_type, length, data, arraybuffer, byteOffset));
    *type = ConvertToTypedarrayType(napi_type);
    return status;
}
inline /*JSVM_EXTERN*/ Status GetDataviewInfo(Env env, Value dataview, size_t *bytelength, void **data,
                                              Value *arraybuffer, size_t *byteOffset)
{

    return ConvertToStatus(napi_get_dataview_info(env, dataview, bytelength, data, arraybuffer, byteOffset));
}
inline /*JSVM_EXTERN*/ Status GetGlobal(Env env, Value *result)
{
    return ConvertToStatus(napi_get_global(env, result));
}
inline /*JSVM_EXTERN*/ Status SetInstanceData(Env env, void *data, Finalize finalizeCb, void *finalizeHint)
{
    return ConvertToStatus(napi_set_instance_data(env, data, finalizeCb, finalizeHint));
}
inline /*JSVM_EXTERN*/ Status GetInstanceData(Env env, void **data)
{
    return ConvertToStatus(napi_get_instance_data(env, data));
}
inline /*JSVM_EXTERN*/ Status CreateDate(Env env, double time, Value *result)
{
    return ConvertToStatus(napi_create_date(env, time, result));
}
inline /*JSVM_EXTERN*/ Status IsDate(Env env, Value value, bool *isDate)
{
    return ConvertToStatus(napi_is_date(env, value, isDate));
}
inline /*JSVM_EXTERN*/ Status GetDateValue(Env env, Value value, double *result)
{
    return ConvertToStatus(napi_get_date_value(env, value, result));
}
inline /*JSVM_EXTERN*/ Status IsError(Env env, Value value, bool *result)
{
    return ConvertToStatus(napi_is_error(env, value, result));
}
inline /*JSVM_EXTERN*/ Status CreateStringUtf16(Env env, const char16_t* str, size_t length, Value* result)
{
    return ConvertToStatus(napi_create_string_utf16(env, str, length,  result));
}
inline /*JSVM_EXTERN*/ Status GetValueStringUtf16(Env env, Value value, char16_t* buf, size_t bufsize, size_t* result)
{
    return ConvertToStatus(napi_get_value_string_utf16(env,  value, buf,  bufsize,  result));
}
inline /*JSVM_EXTERN*/ Status CreateStringLatin1(Env env, const char* str, size_t length, Value* result)
{
    return ConvertToStatus(napi_create_string_latin1(env, str, length, result));
}
inline /*JSVM_EXTERN*/ Status GetValueStringLatin1(Env env, Value value, char* buf, size_t bufsize, size_t* result)
{
    return ConvertToStatus(napi_get_value_string_latin1(env, value, buf, bufsize, result));
}
inline /*JSVM_EXTERN*/ Status CreateBigintInt64(Env env, int64_t value, Value *result)
{
    return ConvertToStatus(napi_create_bigint_int64(env, value, result));
}
inline /*JSVM_EXTERN*/ Status GetValueBigintInt64(Env env, Value value, int64_t *result, bool *lossless)
{
    return ConvertToStatus(napi_get_value_bigint_int64(env, value, result, lossless));
}
inline /*JSVM_EXTERN*/ Status CreateBigintUint64(Env env, uint64_t value, Value *result)
{
    return ConvertToStatus(napi_create_bigint_uint64(env, value, result));
}
inline /*JSVM_EXTERN*/ Status GetValueBigintUint64(Env env, Value value, uint64_t *result, bool *lossless)
{
    return ConvertToStatus(napi_get_value_bigint_uint64(env, value, result, lossless));
}
inline /*JSVM_EXTERN*/ Status RunScript(Env env, Script script, Value *result)
{
return ConvertToStatus(napi_run_script(env, script, result));
}
} // namespace jsvm
#ifndef __JSBIND_INTERNAL_VALUE_H__
#define __JSBIND_INTERNAL_VALUE_H__

#include <jsvm/JSEnv.h>
#include <jsvm/JSVM.h>
#include <jsvm/JSVM_Types.h>
#include <string>

namespace jsbind
{
namespace internal
{
inline jsvm_value makeObject()
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_object(env, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline bool getBool(jsvm_value value)
{
    GET_ENV
    jsvm_status status;
    bool result;
    jsvm_value resultCoerce;
    status = jsvm_coerce_to_bool(env, value, &resultCoerce);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    status = jsvm_get_value_bool(env, resultCoerce, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline jsvm_value makeBool(bool value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_get_boolean(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline double getDouble(jsvm_value value)
{
    GET_ENV
    double result;
    jsvm_value resultCoerce;
    jsvm_status status;
    status = jsvm_coerce_to_number(env, value, &resultCoerce);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    status = jsvm_get_value_double(env, resultCoerce, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline jsvm_value makeDouble(double value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_double(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline int32_t getInt32(jsvm_value value)
{
    GET_ENV
    int32_t result;
    jsvm_value resultCoerce;
    jsvm_status status;
    status = jsvm_coerce_to_number(env, value, &resultCoerce);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    status = jsvm_get_value_int32(env, resultCoerce, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline jsvm_value makeInt32(int32_t value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_int32(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline uint32_t getUint32(jsvm_value value)
{
    GET_ENV
    uint32_t result;
    jsvm_status status;
    status = jsvm_get_value_uint32(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline jsvm_value makeUint32(uint32_t value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_uint32(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline int64_t getInt64Noloss(jsvm_value value)
{
    GET_ENV
    int64_t result;
    jsvm_status status;
    bool lossless;
    status = jsvm_get_value_bigint_int64(env, value, &result, &lossless);
    DEBUG_CHECK(lossless == true);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline jsvm_value makeInt64Noloss(int64_t value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_bigint_int64(env, value, &result);
    return result;
}
inline uint64_t getUint64Noloss(jsvm_value value)
{
    GET_ENV
    uint64_t result;
    jsvm_status status;
    bool lossless;
    status = jsvm_get_value_bigint_uint64(env, value, &result, &lossless);
    DEBUG_CHECK(lossless == true);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline jsvm_value makeUint64Noloss(uint64_t value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_bigint_uint64(env, value, &result);
    return result;
}
inline std::string getStringLatin1(jsvm_value value)
{
    GET_ENV
    size_t length = 0;
    jsvm_status status;
    status = jsvm_get_value_string_latin1(env, value, NULL, 0, &length);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    std::string latin1Str(length, '\0');
    status = jsvm_get_value_string_latin1(env, value, latin1Str.data(), length + 1, &length);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return latin1Str;
}
inline jsvm_value makeStringLatin1(const std::string &value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_string_latin1(env, value.c_str(), value.length(), &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline std::string getStringUtf8(jsvm_value value)
{
    GET_ENV
    size_t length = 0;
    jsvm_status status;
    jsvm_value result;
    status = jsvm_coerce_to_string(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    status = jsvm_get_value_string_utf8(env, result, NULL, 0, &length);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    std::string utf8str(length, '\0');
    status = jsvm_get_value_string_utf8(env, result, utf8str.data(), length + 1, &length);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return utf8str;
}
inline jsvm_value makeStringUtf8(const std::string &value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_string_utf8(env, value.c_str(), value.length(), &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline std::u16string getStringUtf16(jsvm_value value)
{
    GET_ENV
    size_t length = 0;
    jsvm_status status;
    status = jsvm_get_value_string_utf16(env, value, NULL, 0, &length);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    std::u16string utf16str(length, '\0');
    status = jsvm_get_value_string_utf16(env, value, utf16str.data(), length + 1, &length);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return utf16str;
}
inline jsvm_value makeStringUtf16(const std::u16string &value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_string_utf16(env, value.c_str(), value.length(), &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}

inline double getDate(jsvm_value value)
{
    GET_ENV
    double result;
    jsvm_status status;
    status = jsvm_get_date_value(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline jsvm_value makeDate(double value)
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_create_date(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline bool isDate(jsvm_value value)
{
    GET_ENV
    bool result;
    jsvm_status status;
    status = jsvm_is_date(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline jsvm_value makeNull()
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_get_null(env, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}

inline jsvm_value makeUndefined()
{
    GET_ENV
    jsvm_value result;
    jsvm_status status;
    status = jsvm_get_undefined(env, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline bool isUndefined(jsvm_value value)
{
    GET_ENV
    jsvm_valuetype valueType;
    jsvm_status status = jsvm_typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return valueType == jsvm_valuetype::jsvm_undefined;
}
inline bool isNull(jsvm_value value)
{
    GET_ENV
    jsvm_valuetype valueType;
    jsvm_status status = jsvm_typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return valueType == jsvm_valuetype::jsvm_null;
}
inline bool isBool(jsvm_value value)
{
    GET_ENV
    DEBUG_CHECK(nullptr != value);
    jsvm_valuetype valueType;
    jsvm_status status;
    status = jsvm_typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return valueType == jsvm_valuetype::jsvm_boolean;
}
inline bool isNumber(jsvm_value value)
{
    GET_ENV
    jsvm_valuetype valueType;
    jsvm_status status = jsvm_typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return valueType == jsvm_valuetype::jsvm_number;
}

inline bool isString(jsvm_value value)
{
    GET_ENV
    jsvm_valuetype valueType;
    jsvm_status status = jsvm_typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return valueType == jsvm_valuetype::jsvm_string;
}

inline bool isObject(jsvm_value value)
{
    GET_ENV
    jsvm_valuetype valueType;
    jsvm_status status = jsvm_typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return valueType == jsvm_valuetype::jsvm_object;
}

inline bool isArray(jsvm_value value)
{
    GET_ENV
    bool result;
    jsvm_status status = jsvm_is_array(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}

inline bool isFunction(jsvm_value value)
{
    GET_ENV
    jsvm_valuetype valueType;
    jsvm_status status = jsvm_typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return valueType == jsvm_valuetype::jsvm_function;
}

inline bool isError(jsvm_value value)
{
    GET_ENV
    bool result;
    jsvm_status status = jsvm_is_error(env, value, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}

inline bool isArrayBuffer(jsvm_value value)
{
    GET_ENV
    bool isArraybuffer;
    jsvm_is_arraybuffer(env, value, &isArraybuffer);
    return isArraybuffer;
}

inline bool isArrayBufferView(jsvm_value value)
{
    GET_ENV
    bool isTypedarray;
    jsvm_is_typedarray(env, value, &isTypedarray);
    bool isDataview;
    jsvm_is_dataview(env, value, &isDataview);
    return isTypedarray || isDataview;
}

inline bool isTypedArray(jsvm_value value)
{
    GET_ENV
    bool isTypedarray;
    jsvm_is_typedarray(env, value, &isTypedarray);
    return isTypedarray;
}

inline bool isDataView(jsvm_value value)
{
    GET_ENV
    bool isDataview;
    jsvm_is_dataview(env, value, &isDataview);
    return isDataview;
}
inline bool isBigInt(jsvm_value value)
{
    GET_ENV
    jsvm_valuetype valueType;
    jsvm_status status = jsvm_typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return valueType == jsvm_valuetype::jsvm_bigint;
}
} // namespace internal
} // namespace jsbind
#endif

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
inline jsvm::Value makeObject()
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateObject(env, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline bool getBool(jsvm::Value value)
{
    GET_ENV
    jsvm::Status status;
    bool result;
    jsvm::Value resultCoerce;
    status = jsvm::CoerceToBool(env, value, &resultCoerce);
    DEBUG_CHECK(status == jsvm::Status::OK);
    status = jsvm::GetValueBool(env, resultCoerce, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline jsvm::Value makeBool(bool value)
{
    GET_ENV
    jsvm::Value resultUint32;
    jsvm::Value resultBoolean;
    jsvm::Status status;
    status = jsvm::CreateUint32(env, static_cast<uint32_t>(value), &resultUint32); //  no create bool function
    DEBUG_CHECK(status == jsvm::Status::OK);
    status = jsvm::CoerceToBool(env, resultUint32, &resultBoolean);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return resultBoolean;
}
inline double getDouble(jsvm::Value value)
{
    GET_ENV
    double result;
    jsvm::Value resultCoerce;
    jsvm::Status status;
    status = jsvm::CoerceToNumber(env, value, &resultCoerce);
    DEBUG_CHECK(status == jsvm::Status::OK);
    status = jsvm::GetValueDouble(env, resultCoerce, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline jsvm::Value makeDouble(double value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateDouble(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline int32_t getInt32(jsvm::Value value)
{
    GET_ENV
    int32_t result;
    jsvm::Status status;
    status = jsvm::GetValueInt32(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline jsvm::Value makeInt32(int32_t value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateInt32(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline uint32_t getUint32(jsvm::Value value)
{
    GET_ENV
    uint32_t result;
    jsvm::Status status;
    status = jsvm::GetValueUint32(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline jsvm::Value makeUint32(uint32_t value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateUint32(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline int64_t getInt64Noloss(jsvm::Value value)
{
    GET_ENV
    int64_t result;
    jsvm::Status status;
    bool lossless;
    status = jsvm::GetValueBigintInt64(env, value, &result, &lossless);
    DEBUG_CHECK(lossless == true);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline jsvm::Value makeInt64Noloss(int64_t value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateBigintInt64(env, value, &result);
    return result;
}
inline uint64_t getUint64Noloss(jsvm::Value value)
{
    GET_ENV
    uint64_t result;
    jsvm::Status status;
    bool lossless;
    status = jsvm::GetValueBigintUint64(env, value, &result, &lossless);
    DEBUG_CHECK(lossless == true);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline jsvm::Value makeUint64Noloss(uint64_t value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateBigintUint64(env, value, &result);
    return result;
}
inline std::string getStringLatin1(jsvm::Value value)
{
    GET_ENV
    size_t length = 0;
    jsvm::Status status;
    status = jsvm::GetValueStringLatin1(env, value, NULL, 0, &length);
    DEBUG_CHECK(status == jsvm::Status::OK);
    std::string latin1Str(length, '\0');
    status = jsvm::GetValueStringLatin1(env, value, latin1Str.data(), length + 1, &length);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return latin1Str;
}
inline jsvm::Value makeStringLatin1(const std::string &value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateStringLatin1(env, value.c_str(), value.length(), &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline std::string getStringUtf8(jsvm::Value value)
{
    GET_ENV
    size_t length = 0;
    jsvm::Status status;
    jsvm::Value result;
    status = jsvm::CoerceToString(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    status = jsvm::GetValueStringUtf8(env, result, NULL, 0, &length);
    DEBUG_CHECK(status == jsvm::Status::OK);
    std::string utf8str(length, '\0');
    status = jsvm::GetValueStringUtf8(env, result, utf8str.data(), length + 1, &length);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return utf8str;
}
inline jsvm::Value makeStringUtf8(const std::string &value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateStringUtf8(env, value.c_str(), value.length(), &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline std::u16string getStringUtf16(jsvm::Value value)
{
    GET_ENV
    size_t length = 0;
    jsvm::Status status;
    status = jsvm::GetValueStringUtf16(env, value, NULL, 0, &length);
    DEBUG_CHECK(status == jsvm::Status::OK);
    std::u16string utf16str(length, '\0');
    status = jsvm::GetValueStringUtf16(env, value, utf16str.data(), length + 1, &length);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return utf16str;
}
inline jsvm::Value makeStringUtf16(const std::u16string &value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateStringUtf16(env, value.c_str(), value.length(), &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

inline double getDate(jsvm::Value value)
{
    GET_ENV
    double result;
    jsvm::Status status;
    status = jsvm::GetDateValue(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline jsvm::Value makeDate(double value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateDate(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline bool isDate(jsvm::Value value)
{
    GET_ENV
    bool result;
    jsvm::Status status;
    status = jsvm::IsDate(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline jsvm::Value makeNull()
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::GetNull(env, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

inline jsvm::Value makeUndefined()
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::GetUndefined(env, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
inline bool isUndefined(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::UNDEFINED;
}
inline bool isNull(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::Null;
}
inline bool isBool(jsvm::Value value)
{
    GET_ENV
    DEBUG_CHECK(nullptr != value);
    jsvm::ValueType valueType;
    jsvm::Status status;
    status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::BOOLEAN;
}
inline bool isNumber(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::NUMBER;
}

inline bool isString(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::STRING;
}

inline bool isObject(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::OBJECT;
}

inline bool isArray(jsvm::Value value)
{
    GET_ENV
    bool result;
    jsvm::Status status = jsvm::IsArray(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

inline bool isFunction(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::FUNCTION;
}

inline bool isError(jsvm::Value value)
{
    GET_ENV
    bool result;
    jsvm::Status status = jsvm::IsError(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

inline bool isArrayBuffer(jsvm::Value value)
{
    GET_ENV
    bool isArraybuffer;
    jsvm::IsArraybuffer(env, value, &isArraybuffer);
    return isArraybuffer;
}

inline bool isArrayBufferView(jsvm::Value value)
{
    GET_ENV
    bool isTypedarray;
    jsvm::IsTypedarray(env, value, &isTypedarray);
    bool isDataview;
    jsvm::IsDataview(env, value, &isDataview);
    return isTypedarray || isDataview;
}

inline bool isTypedArray(jsvm::Value value)
{
    GET_ENV
    bool isTypedarray;
    jsvm::IsTypedarray(env, value, &isTypedarray);
    return isTypedarray;
}

inline bool isDataView(jsvm::Value value)
{
    GET_ENV
    bool isDataview;
    jsvm::IsDataview(env, value, &isDataview);
    return isDataview;
}

} // namespace internal
} // namespace jsbind
#endif

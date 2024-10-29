#ifndef __JSBIND_INTERNAL_VALUE_H__
#define __JSBIND_INTERNAL_VALUE_H__

#include <assert.h>
#include <binder/JSVM.h>
#include <binder/JSVM_Types.h>

namespace jsbind
{
namespace internal
{
static inline bool getBool(jsvm::Value value)
{
    GET_ENV
    bool result;
    jsvm::Status status;
    status = jsvm::GetValueBool(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline jsvm::Value makeBool(bool value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateUint32(env, static_cast<uint32_t>(value), &result); // why not create bool ?????
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline double getDouble(jsvm::Value value)
{
    GET_ENV
    double result;
    jsvm::Status status;
    status = jsvm::GetValueDouble(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline jsvm::Value makeDouble(double value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateDouble(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline int32_t getInt32(jsvm::Value value)
{
    GET_ENV
    int32_t result;
    jsvm::Status status;
    status = jsvm::GetValueInt32(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline jsvm::Value makeInt32(int32_t value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateInt32(env, value, &result);
    return result;
}
static uint32_t getUint32(jsvm::Value value)
{
    GET_ENV
    uint32_t result;
    jsvm::Status status;
    status = jsvm::GetValueUint32(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static jsvm::Value makeUint32(uint32_t value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateUint32(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline int64_t getInt64Noloss(jsvm::Value value)
{
    GET_ENV
    int64_t result;
    jsvm::Status status;
    bool lossless;
    status = jsvm::GetValueBigintInt64(env, value, &result, &lossless);
    DEBUG_CHECK(lossless == false);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline jsvm::Value makeInt64Noloss(int64_t value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateBigintInt64(env, value, &result);
    return result;
}
static inline uint64_t getUint64Noloss(jsvm::Value value)
{
    GET_ENV
    uint64_t result;
    jsvm::Status status;
    bool lossless;
    status = jsvm::GetValueBigintUint64(env, value, &result, &lossless);
    DEBUG_CHECK(lossless == false);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline jsvm::Value makeUint64Noloss(uint64_t value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateBigintUint64(env, value, &result);
    return result;
}
static std::string getStringLatin1(jsvm::Value value)
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
static jsvm::Value makeStringLatin1(const std::string &value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateStringLatin1(env, value.c_str(), value.length(), &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static std::string getStringUtf8(jsvm::Value value)
{
    GET_ENV
    size_t length = 0;
    jsvm::Status status;
    status = jsvm::GetValueStringUtf8(env, value, NULL, 0, &length);
    DEBUG_CHECK(status == jsvm::Status::OK);
    std::string utf8str(length, '\0');
    status = jsvm::GetValueStringUtf8(env, value, utf8str.data(), length + 1, &length);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return utf8str;
}
static jsvm::Value makeStringUtf8(const std::string &value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateStringUtf8(env, value.c_str(), value.length(), &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static std::u16string getStringUtf16(jsvm::Value value)
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
static jsvm::Value makeStringUtf16(const std::u16string &value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateStringUtf16(env, value.c_str(), value.length(), &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

static inline double getDate(jsvm::Value value)
{
    GET_ENV
    double result;
    jsvm::Status status;
    status = jsvm::GetDateValue(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline jsvm::Value makeDate(double value)
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::CreateDate(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline bool isDate(jsvm::Value value)
{
    GET_ENV
    bool result;
    jsvm::Status status;
    status = jsvm::IsDate(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline jsvm::Value makeNull()
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::GetNull(env, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

static inline jsvm::Value makeUndefined()
{
    GET_ENV
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::GetUndefined(env, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
static inline bool isUndefined(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::UNDEFINED;
}
static inline bool isNull(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::Null;
}
static inline bool isBool(jsvm::Value value)
{
    GET_ENV
    DEBUG_CHECK(nullptr != value);
    jsvm::ValueType valueType;
    jsvm::Status status;
    status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::BOOLEAN;
}
static inline bool isNumber(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::NUMBER;
}

static inline bool isString(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::STRING;
}

static inline bool isObject(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::OBJECT;
}

static inline bool isArray(jsvm::Value value)
{
    GET_ENV
    bool result;
    jsvm::Status status = jsvm::IsArray(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

static inline bool isFunction(jsvm::Value value)
{
    GET_ENV
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::FUNCTION;
}

static inline bool isError(jsvm::Value value)
{
    GET_ENV
    bool result;
    jsvm::Status status = jsvm::IsError(env, value, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

static inline bool isArrayBuffer(jsvm::Value value)
{
    GET_ENV
    bool isArraybuffer;
    jsvm::IsArraybuffer(env, value, &isArraybuffer);
}

static inline bool isArrayBufferView(jsvm::Value value)
{
    GET_ENV
    bool isTypedarray;
    jsvm::IsTypedarray(env, value, &isTypedarray);
    bool isDataview;
    jsvm::IsDataview(env, value, &isDataview);
    return isTypedarray && isDataview;
}

static inline bool isTypedArray(jsvm::Value value)
{
    GET_ENV
    bool isTypedarray;
    jsvm::IsTypedarray(env, value, &isTypedarray);
    return isTypedarray;
}

static inline bool isDataView(jsvm::Value value)
{
    GET_ENV
    bool isDataview;
    jsvm::IsDataview(env, value, &isDataview);
    return isDataview;
}
} // namespace internal
} // namespace jsbind
#endif

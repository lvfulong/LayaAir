#ifndef __JSBIND_INTERNAL_VALUE_H__
#define __JSBIND_INTERNAL_VALUE_H__

#include <assert.h>
#include <binder/JSVM.h>
#include <binder/JSVM_Types.h>

namespace jsbind
{
namespace internal
{
// struct JSVMValue
//{
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
// };
} // namespace internal

/*
static v8::Local<v8::Value> ToJsDate(int32_t p_vl)
    {
        return v8::Date::New(v8::Isolate::GetCurrent()->GetCurrentContext(), (double)p_vl).ToLocalChecked();
    }
static v8::Local<v8::Value> ToJsDate(double p_vl)
{

    return v8::Date::New(v8::Isolate::GetCurrent()->GetCurrentContext(), (double)p_vl).ToLocalChecked();
}*/
} // namespace jsbind
#endif

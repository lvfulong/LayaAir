#include <binder/v8/Local.h>

namespace jsbind
{
Local::Local()
{
    GET_ENV
    jsvm::Status status;
    jsvm::Value undefined;
    status = jsvm::GetUndefined(env, &undefined);
    DEBUG_CHECK(status == jsvm::Status::OK);
    handle_ = undefined;
}

Local::Local(jsvm::Value handle) : handle_(handle)
{
}
bool Local::isUndefined() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::UNDEFINED;
}

bool Local::isNull() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::Null;
}

bool Local::isBool() const
{
    DEBUG_CHECK(isValid());
    return Converter<bool>::is(handle_);
}

bool Local::isNumber() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::NUMBER;
}

bool Local::isString() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::STRING;
}

bool Local::isObject() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::OBJECT;
}

bool Local::isArray() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    bool result;
    jsvm::Status status = jsvm::IsArray(env, handle_, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

bool Local::isFunction() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::FUNCTION;
}

bool Local::isError() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    bool result;
    jsvm::Status status = jsvm::IsError(env, handle_, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

bool Local::isArrayBuffer() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    bool isArraybuffer;
    jsvm::IsArraybuffer(env, handle_, &isArraybuffer);
}

bool Local::isArrayBufferView() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    bool isTypedarray;
    jsvm::IsTypedarray(env, handle_, &isTypedarray);
    bool isDataview;
    jsvm::IsDataview(env, handle_, &isDataview);
    return isTypedarray && isDataview;
}

bool Local::isTypedArray() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    bool isTypedarray;
    jsvm::IsTypedarray(env, handle_, &isTypedarray);
    return isTypedarray;
}

bool Local::isDataView() const
{
    GET_ENV
    DEBUG_CHECK(isValid());
    bool isDataview;
    jsvm::IsDataview(env, handle_, &isDataview);
    return isDataview;
}

} // namespace jsbind

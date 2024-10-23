#include <binder/v8/Local.h>

namespace jsbind
{
Local::Local()
{
    auto JSEnv = JSEnv::getCurrent();
    DEBUG_CHECK(nullptr != JSEnv);
    jsvm::Env env = JSEnv->getEnv();
    DEBUG_CHECK(nullptr != env);

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
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType) DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::UNDEFINED;
}

bool Local::isNull() const
{
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType) DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::Null;
}

bool Local::isBool() const
{
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType) DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::BOOLEAN;
}

bool Local::isNumber() const
{
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType) DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::NUMBER;
}

bool Local::isString() const
{
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType) DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::STRING;
}

bool Local::isObject() const
{
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType) DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::OBJECT;
}

bool Local::isArray() const
{
    DEBUG_CHECK(isValid());
    bool result;
    jsvm::Status status = jsvm::IsArray(env, handle_, &result) DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}

bool Local::isFunction() const
{
    DEBUG_CHECK(isValid());
    jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, handle_, &valueType) DEBUG_CHECK(status == jsvm::Status::OK);
    return valueType == jsvm::ValueType::FUNCTION;
}

bool Local::isError() const
{
    DEBUG_CHECK(isValid());
    bool result;
    jsvm::Status status = jsvm::IsError(env, handle_, &result) DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
} // namespace jsbind

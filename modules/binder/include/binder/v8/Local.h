#ifndef __JSBIND_LOCAL_H__
#define __JSBIND_LOCAL_H__

#include <assert.h>
#include <binder/JSVM_Types.h>
#include <binder/v8/Class.h>
#include <binder/v8/internal/Value.h>
#include <binder/v8/internal/Converter.h>

namespace jsbind
{
class Local
{
  public:
    Local();
    Local(jsvm::Value);
    Local(Local const &) = default;
    Local &operator=(Local const &) = default;

    Local(Local &&) = default;
    Local &operator=(Local &&) = default;
    template <typename ReturnType, typename... Args> ReturnType call(jsvm::Value recv, const Args &...args)
    {
        GET_ENV
        return call(env, recv, ... args)
    }
    template <typename ClassType, typename ReturnType, typename... Args>
    ReturnType call(ClassType *recv, const Args &...args)
    {
        GET_ENV

        ClassRegistry<ClassType> &classRegistry =
            ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());
        auto objectRegistry = classRegistry.getObjectRegistry(recv);
        DEBUG_CHECK(objectRegistry != nullptr);
        jsvm::Value result_recv;
        jsvm::Status status;
        status = jsvm::GetReferenceValue(env, objectRegistry, &result_recv);
        DEBUG_CHECK(status == jsvm::Status::OK);

        return call(env, result_recv, ... args)
    }
    Local operator[](const std::string &key) const;

    jsvm::Value getHandle() const
    {
        return handle_;
    }
    inline bool isValid() const
    {
        return handle_ != nullptr;
    }
    inline bool isUndefined() const
    {
        DEBUG_CHECK(isValid());
        return internal::isUndefined(this->handle_);
    }

    inline bool isNull() const
    {
        DEBUG_CHECK(isValid());
        return internal::isNull(this->handle_);
    }

    inline bool isBool() const
    {
        DEBUG_CHECK(isValid());
        return internal::isBool(this->handle_);
    }

    inline bool isNumber() const
    {
        DEBUG_CHECK(isValid());
        return internal::isNumber(this->handle_);
    }

    inline bool isString() const
    {
        DEBUG_CHECK(isValid());
        return internal::isString(this->handle_);
    }

    inline bool isObject() const
    {
        DEBUG_CHECK(isValid());
        return internal::isObject(this->handle_);
    }

    inline bool isArray() const
    {
        DEBUG_CHECK(isValid());
        return internal::isArray(this->handle_);
    }

    inline bool isFunction() const
    {
        DEBUG_CHECK(isValid());
        return internal::isFunction(this->handle_);
    }

    inline bool isError() const
    {
        DEBUG_CHECK(isValid());
        return internal::isError(this->handle_);
    }

    inline bool isArrayBuffer() const
    {
        DEBUG_CHECK(isValid());
        return internal::isArrayBuffer(this->handle_);
    }

    inline bool isArrayBufferView() const
    {
        DEBUG_CHECK(isValid());
        return internal::isArrayBufferView(this->handle_);
    }

    inline bool isTypedArray() const
    {
        DEBUG_CHECK(isValid());
        return internal::isTypedArray(this->handle_);
    }

    inline bool isDataView() const
    {
        DEBUG_CHECK(isValid());
        return internal::isDataView(this->handle_);
    }
    inline bool isDate() const
    {
        DEBUG_CHECK(isValid());
        return internal::isDate(this->handle_);
    }

    static inline bool isUndefined(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isUndefined(value);
    }
    static inline bool isNull(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isNull(value);
    }

    static inline bool isBool(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isBool(value);
    }

    static inline bool isNumber(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isNumber(value);
    }

    static inline bool isString(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isString(value);
    }

    static inline bool isObject(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isObject(value);
    }

    static inline bool isArray(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isArray(value);
    }

    static inline bool isFunction(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isFunction(value);
    }

    static inline bool isError(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isError(value);
    }

    static inline bool isArrayBuffer(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isArrayBuffer(value);
    }

    static inline bool isArrayBufferView(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isArrayBufferView(value);
    }

    static inline bool isTypedArray(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isTypedArray(value);
    }

    static inline bool isDataView(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isDataView(value);
    }
    static inline bool isDate(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        return internal::isDate(value);
    }

    template <typename T> T as() const
    {
        DEBUG_CHECK(isValid());
        GET_ENV
        return Converter<T>::ToCpp(this->handle_);
    }
    template <typename T> bool is()
    {
        DEBUG_CHECK(isValid());
        GET_ENV
        return Converter<T>::is();
    }
    template <class T> static jsvm::Value Make(T t, bool callDestructor = true)
    {
        return Converter<T>::ToJs(t, callDestructor);
    }
    static inline jsvm::Value MakeNull()
    {
        return internal::makeNull();
    }
    static inline jsvm::Value MakeUndefined()
    {
        return internal::makeUndefined();
    }

  private:
    template <typename ClassType, typename ReturnType, typename... Args>
    ReturnType call(jsvm::Env env, jsvm::Value recv, const Args &...args)
    {
        jsvm::ValueType valueType;
        jsvm::Status status = jsvm::Typeof(env, func, &valueType) DEBUG_CHECK(status == jsvm::Status::OK);
        if (handle_ != nullptr && valueType == jsvm::ValueType::FUNCTION)
        {
            auto result = internal::v8_call(recv, handle_, args...);
            return Converter<ReturnType>::ToCpp(result);
        }
        else
        {
            return ReturnType();
        }
    }

  private:
    jsvm::Value handle_ = nullptr;
};
template <> class Converter<Local>
{
public:
    static Local ToCpp(jsvm::Value value)
    {
        return Local(value);
    }
    static jsvm::Value ToJs(Local value, bool callDestructor = true)
    {
        return value.getHandle();
    }
    /*static bool is(jsvm::Value value)
    {
        return true;
    }*/
};
} // namespace jsbind
#endif

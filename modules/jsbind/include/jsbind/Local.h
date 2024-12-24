#ifndef __JSBIND_LOCAL_H__
#define __JSBIND_LOCAL_H__

#include <jsbind/internal/Invoke.h>
#include <jsbind/internal/Value.h>
#include <jsbind/internal/ValueTraits.h>
#include <jsvm/JSVM_Types.h>

namespace jsbind
{
class Local
{
  public:
    Local();
    Local(jsvm_value);
    Local(Local const &) = default;
    Local &operator=(Local const &) = default;

    Local(Local &&) = default;
    Local &operator=(Local &&) = default;
    template <typename ReturnType, typename... Args> ReturnType call(jsvm_value recv, const Args &...args)
    {

        if (isValid() && isFunction())
        {
            GET_ENV
            auto result = internal::v8_call(env, recv, handle_, args...);
            return internal::ValueTraits<ReturnType>::ToCpp(result);
        }
        else
        {
            return ReturnType();
        }
    }
    template <typename ReturnType, typename... Args> ReturnType call(jsvm_value recv, const Args &...args) const
    {
        if (isValid() && isFunction())
        {
            GET_ENV
            auto result = internal::v8_call(env, recv, handle_, args...);
            return internal::ValueTraits<ReturnType>::ToCpp(result);
        }
        else
        {
            return ReturnType();
        }
    }
    template <typename ReturnType, typename... Args> ReturnType call(const char* name, const Args &...args)
    {

        if (isValid() && isFunction())
        {
            GET_ENV
            auto result = internal::v8_call(env, handle_, this->operator[](name).getHandle(),  args...);
            return internal::ValueTraits<ReturnType>::ToCpp(result);
        }
        else
        {
            return ReturnType();
        }
    }
    template <typename ReturnType, typename... Args> ReturnType call(const char* name, const Args &...args) const
    {

        if (isValid() && isFunction())
        {
            GET_ENV
            auto result = internal::v8_call(env, handle_, this->operator[](name).getHandle(), args...);
            return internal::ValueTraits<ReturnType>::ToCpp(result);
        }
        else
        {
            return ReturnType();
        }
    }
    Local operator[](const std::string &key) const;

    jsvm_value getHandle() const
    {
        return handle_;
    }
    inline bool isValid() const
    {
        return handle_ != nullptr;
    }
    inline bool isUndefined() const
    {
        return isValid() && internal::isUndefined(this->handle_);
    }

    inline bool isNull() const
    {
        return isValid() && internal::isNull(this->handle_);
    }

    inline bool isBool() const
    {
        return isValid() && internal::isBool(this->handle_);
    }

    inline bool isNumber() const
    {
        return isValid() && internal::isNumber(this->handle_);
    }

    inline bool isString() const
    {
        return isValid() && internal::isString(this->handle_);
    }

    inline bool isObject() const
    {
        return isValid() && internal::isObject(this->handle_);
    }

    inline bool isArray() const
    {
        return isValid() && internal::isArray(this->handle_);
    }

    inline bool isFunction() const
    {
        return isValid() && internal::isFunction(this->handle_);
    }

    inline bool isError() const
    {
        return isValid() && internal::isError(this->handle_);
    }

    inline bool isArrayBuffer() const
    {
        return isValid() && internal::isArrayBuffer(this->handle_);
    }

    inline bool isArrayBufferView() const
    {
        return isValid() && internal::isArrayBufferView(this->handle_);
    }

    inline bool isTypedArray() const
    {
        return isValid() && internal::isTypedArray(this->handle_);
    }

    inline bool isDataView() const
    {
        return isValid() && internal::isDataView(this->handle_);
    }
    inline bool isDate() const
    {
        return isValid() && internal::isDate(this->handle_);
    }

    static inline bool isUndefined(jsvm_value value)
    {
        return value != nullptr && internal::isUndefined(value);
    }
    static inline bool isNull(jsvm_value value)
    {
        return value != nullptr && internal::isNull(value);
    }

    static inline bool isBool(jsvm_value value)
    {
        return value != nullptr && internal::isBool(value);
    }

    static inline bool isNumber(jsvm_value value)
    {
        return value != nullptr && internal::isNumber(value);
    }

    static inline bool isString(jsvm_value value)
    {
        return value != nullptr && internal::isString(value);
    }

    static inline bool isObject(jsvm_value value)
    {
        return value != nullptr && internal::isObject(value);
    }

    static inline bool isArray(jsvm_value value)
    {
        return value != nullptr && internal::isArray(value);
    }

    static inline bool isFunction(jsvm_value value)
    {
        return value != nullptr && internal::isFunction(value);
    }

    static inline bool isError(jsvm_value value)
    {
        return value != nullptr && internal::isError(value);
    }

    static inline bool isArrayBuffer(jsvm_value value)
    {
        return value != nullptr && internal::isArrayBuffer(value);
    }

    static inline bool isArrayBufferView(jsvm_value value)
    {
        return value != nullptr && internal::isArrayBufferView(value);
    }

    static inline bool isTypedArray(jsvm_value value)
    {
        return value != nullptr && internal::isTypedArray(value);
    }

    static inline bool isDataView(jsvm_value value)
    {
        return value != nullptr && internal::isDataView(value);
    }
    static inline bool isDate(jsvm_value value)
    {
        return value != nullptr && internal::isDate(value);
    }

    template <typename T> T as() const
    {
        DEBUG_CHECK(isValid());
        GET_ENV
        return internal::ValueTraits<T>::ToCpp(this->handle_);
    }
    template <typename T> bool is()
    {
        GET_ENV
        return isValid() && internal::ValueTraits<T>::is(handle_);
    }

  private:
    jsvm_value handle_ = nullptr;
};
namespace internal
{
template <> class ValueTraits<Local>
{
  public:
    static Local ToCpp(jsvm_value value)
    {
        return Local(value);
    }
    static jsvm_value ToJs(Local value, bool callDestructor = true)
    {
        return value.getHandle();
    }
    static bool is(jsvm_value value)
    {
        return true;
    }
};
} // namespace internal
} // namespace jsbind
#endif

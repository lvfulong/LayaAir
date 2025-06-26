#ifndef __JSBIND_LOCAL_H__
#define __JSBIND_LOCAL_H__

#include <jsbind/internal/Invoke.h>
#include <jsbind/Value.h>
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
        return isValid() && jsbind::isUndefined(this->handle_);
    }

    inline bool isNull() const
    {
        return isValid() && jsbind::isNull(this->handle_);
    }

    inline bool isBool() const
    {
        return isValid() && jsbind::isBool(this->handle_);
    }

    inline bool isNumber() const
    {
        return isValid() && jsbind::isNumber(this->handle_);
    }

    inline bool isString() const
    {
        return isValid() && jsbind::isString(this->handle_);
    }

    inline bool isObject() const
    {
        return isValid() && jsbind::isObject(this->handle_);
    }

    inline bool isArray() const
    {
        return isValid() && jsbind::isArray(this->handle_);
    }

    inline bool isFunction() const
    {
        return isValid() && jsbind::isFunction(this->handle_);
    }

    inline bool isError() const
    {
        return isValid() && jsbind::isError(this->handle_);
    }

    inline bool isArrayBuffer() const
    {
        return isValid() && jsbind::isArrayBuffer(this->handle_);
    }

    inline bool isArrayBufferView() const
    {
        return isValid() && jsbind::isArrayBufferView(this->handle_);
    }

    inline bool isTypedArray() const
    {
        return isValid() && jsbind::isTypedArray(this->handle_);
    }

    inline bool isDataView() const
    {
        return isValid() && jsbind::isDataView(this->handle_);
    }
    inline bool isDate() const
    {
        return isValid() && jsbind::isDate(this->handle_);
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

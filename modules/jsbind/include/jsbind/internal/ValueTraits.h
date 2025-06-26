#ifndef __JSBIND_VALUE_TRAITS__H__
#define __JSBIND_VALUE_TRAITS__H__

#include "jsbind/Value.h"
#include "jsvm/JSVM.h"
#include "jsvm/JSVM_Types.h"
#include "jsvm/napi/js_native_api.h"
#include <assert.h>
#include <jsbind/Utility.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <type_traits>

namespace jsbind
{
template <typename ClassType> bool isWrappedClassOf();
template <typename ClassType, typename Traits> jsvm_value wrapCppObject(ClassType *objectPointer, bool callDestructor);
template <typename ClassType, typename Traits> typename Traits::template object_pointer_type<ClassType> unwrapCppObject(jsvm_value value);

namespace internal
{
template <class T> struct is_value_object;

template <typename T>
struct is_wrapped_class : std::conjunction<std::is_class<T>, std::negation<internal::is_value_object<T>>,
                                           // std::negation<detail::is_string<T>>,
                                           std::negation<internal::is_mapping<T>>,
                                           std::negation<internal::is_sequence<T>>,
                                           std::negation<internal::is_shared_ptr<T>>>
// std::negation<detail::is_array<T>>,
// std::negation<detail::is_tuple<T>>
{
};

template <typename T, typename Enable = void> class ValueTraits;

// template <typename T> class value_object;

template <typename T> class ValueTraits<T, std::enable_if_t<std::is_enum<T>::value>>
{
  public:
    static T ToCpp(jsvm_value value)
    {
        return static_cast<T>(getInt32(value));
    }
    static jsvm_value ToJs(T value, bool callDestructor = true)
    {
        return makeInt32(static_cast<int32_t>(value));
    }
    static bool is(jsvm_value value)
    {
        return isNumber(value);
    }
};

template <typename T> class ValueTraits<T, std::enable_if_t<internal::is_wrapped_class<T>::value>>
{
  public:
    static jsvm_value ToJs(T value, bool callDestructor = true)
    {
        T *object = new T(value); // copy construct to avoid life cycle issues
        return wrapCppObject<T, raw_ptr_traits>(object, callDestructor);
    }

    static T &ToCpp(jsvm_value value)
    {
        DEBUG_CHECK(value != nullptr);
        GET_ENV
        jsvm_status status;
        jsvm_valuetype valueType;
        status = jsvm_typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        DEBUG_CHECK(valueType == jsvm_valuetype::jsvm_object)

        T *obj;
        status = jsvm_unwrap(env, value, reinterpret_cast<void **>(&obj));
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        return *obj;
    }
    static bool is(jsvm_value value)
    {
        return isWrappedClassOf<T>();
    }
};

template <typename T> class ValueTraits<T *, std::enable_if_t<internal::is_wrapped_class<T>::value>>
{
  public:
    static jsvm_value ToJs(T *value, bool callDestructor = true)
    {
        if (value == nullptr)
        {
            return makeNull();
        }
        return wrapCppObject<T, raw_ptr_traits>(value, callDestructor);
    }
    static T *ToCpp(jsvm_value value)
    {
        DEBUG_CHECK(value != nullptr);
        GET_ENV
        jsvm_status status;
        jsvm_valuetype valueType;
        status = jsvm_typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (valueType != jsvm_valuetype::jsvm_object)
        {
            return nullptr;
        }
        T *obj;
        status = jsvm_unwrap(env, value, (void **)(&obj));
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        return obj;
    }
    static bool is(jsvm_value value)
    {
        return isWrappedClassOf<T>();
    }
};


template <typename T> struct ValueTraits<T &> : ValueTraits<T>
{
};

template <typename T> struct ValueTraits<const T &> : ValueTraits<T>
{
};

template <typename T> class ValueTraits<std::shared_ptr<T>, std::enable_if_t<internal::is_wrapped_class<T>::value>>
{
  public:
    static jsvm_value ToJs(std::shared_ptr<T> value, bool callDestructor = true)
    {
        if (value == nullptr)
        {
            return makeNull();
        }
        return wrapCppObject<T, shared_ptr_traits>(value, callDestructor);
    }
    static std::shared_ptr<T> ToCpp(jsvm_value value)
    {
        DEBUG_CHECK(value != nullptr);
        return unwrapCppObject<T, shared_ptr_traits>(value);
    }
    static bool is(jsvm_value value)
    {
        return isWrappedClassOf<T>();
    }
};
template <> class ValueTraits<int32_t>
{
  public:
    static int32_t ToCpp(jsvm_value value)
    {
        return getInt32(value);
    }
    static jsvm_value ToJs(int32_t value, bool callDestructor = true)
    {
        return makeInt32(value);
    }
    static bool is(jsvm_value value)
    {
        return isNumber(value);
    }
};

template <> class ValueTraits<const int32_t &> : public ValueTraits<int32_t>
{
};
template <> class ValueTraits<uint32_t>
{
  public:
    static uint32_t ToCpp(jsvm_value value)
    {
        return getUint32(value);
    }
    static jsvm_value ToJs(uint32_t value, bool callDestructor = true)
    {
        return makeUint32(value);
    }
    static bool is(jsvm_value value)
    {
        return isNumber(value);
    }
};

#ifdef OS_IOS

template <> class ValueTraits<long>
{
  public:
    static jsvm_value ToJs(long value, bool callDestructor = true)
    {
        static_assert(sizeof(long) == 8);
        return makeInt64Noloss(value);
    }
    static long ToCpp(jsvm_value value)
    {
        GET_ENV
        jsvm_valuetype valueType;
        jsvm_status status = jsvm_typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (valueType == jsvm_valuetype::jsvm_null || valueType == jsvm_valuetype::jsvm_undefined)
        {
            return 0;
        }
        return getInt64Noloss(value);
    }
    static bool is(jsvm_value value)
    {
       return isBigInt(value);
    }
};
#endif
// 用bigint 保证精度不丢失,可以用于bullet对象指针
template <> class ValueTraits<int64_t>
{
  public:
    static int64_t ToCpp(jsvm_value value)
    {
        GET_ENV
        jsvm_valuetype valueType;
        jsvm_status status = jsvm_typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (valueType == jsvm_valuetype::jsvm_null || valueType == jsvm_valuetype::jsvm_undefined)
        {
            return 0;
        }
        return getInt64Noloss(value);
    }
    static jsvm_value ToJs(int64_t value, bool callDestructor = true)
    {
        return makeInt64Noloss(value);
    }
    static bool is(jsvm_value value)
    {
       return isBigInt(value);
    }
};
template <> class ValueTraits<uint64_t>
{
  public:
    static uint64_t ToCpp(jsvm_value value)
    {
        /*if (!value->IsNumber() || value->IsNullOrUndefined())
        {
            return 0;
        }*/
        return getUint64Noloss(value);
    }
    static jsvm_value ToJs(uint64_t value, bool callDestructor = true)
    {
        return makeUint64Noloss(value);
    }
    static bool is(jsvm_value value)
    {
       return isBigInt(value);
    }
};

template <> class ValueTraits<uint16_t>
{
  public:
    static uint16_t ToCpp(jsvm_value value)
    {
        return static_cast<uint16_t>(getUint32(value));
    }
    static jsvm_value ToJs(uint16_t value, bool callDestructor = true)
    {
        return makeUint32(static_cast<uint32_t>(value));
    }
    static bool is(jsvm_value value)
    {
        return isNumber(value);
    }
};
template <> class ValueTraits<uint8_t>
{
  public:
    static uint8_t ToCpp(jsvm_value value)
    {
        return static_cast<uint8_t>(getUint32(value));
    }
    static jsvm_value ToJs(uint8_t value, bool callDestructor = true)
    {
        return makeUint32(static_cast<uint8_t>(value));
    }
    static bool is(jsvm_value value)
    {
        return isNumber(value);
    }
};
template <> class ValueTraits<const uint8_t &> : public ValueTraits<uint8_t>
{
};

template <> class ValueTraits<bool>
{
  public:
    static bool ToCpp(jsvm_value value)
    {
        return getBool(value);
    }
    static jsvm_value ToJs(bool value, bool callDestructor = true)
    {
        return makeBool(value);
    }
    static bool is(jsvm_value value)
    {
        return isBool(value);
    }
};

template <> class ValueTraits<float>
{
  public:
    static float ToCpp(jsvm_value value)
    {
        return static_cast<float>(getDouble(value));
    }
    static jsvm_value ToJs(float value, bool callDestructor = true)
    {
        return makeDouble(static_cast<float>(value));
    }
   static bool is(jsvm_value value)
    {
        return isNumber(value);
    }
};


template <> class ValueTraits<double>
{
  public:
    static double ToCpp(jsvm_value value)
    {
        return getDouble(value);
    }
    static jsvm_value ToJs(double value, bool callDestructor = true)
    {
        return makeDouble(value);
    }

    static bool is(jsvm_value value)
    {
        return isNumber(value);
    }
};

// utf16 u16string
template <> class ValueTraits<std::u16string>
{
  public:
    static std::u16string ToCpp(jsvm_value value)
    {
        return getStringUtf16(value);
    }
    static jsvm_value ToJs(const std::u16string &value, bool callDestructor = true)
    {
        return makeStringUtf16(value.c_str(), value.length());
    }
    static bool is(jsvm_value value)
    {
        return isString(value);
    }
};
// utf8 string
template <> class ValueTraits<std::string>
{
  public:
    static std::string ToCpp(jsvm_value value)
    {
        GET_ENV
        /*jsvm_valuetype valueType;
        jsvm_status status = jsvm::Typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (valueType == jsvm_valuetype::Null || valueType == jsvm_valuetype::UNDEFINED ||
            valueType != jsvm_valuetype::STRING)
        {
            return "";
        }*/
        return getStringUtf8(value);
    }
    static jsvm_value ToJs(const std::string &value, bool callDestructor = true)
    {
        return makeStringUtf8(value.c_str(), value.length());
    }
    static bool is(jsvm_value value)
    {
        return isString(value);
    }
};

// const char* sColor = ValueTraits<const char*>::ToCpp(args);          so not save sColor for latter use   get right
// value address const std::string sColor = ValueTraits<std::string>::ToCpp(args);  better const std::string sColor =
// ValueTraits<const char*>::ToCpp(args);  better
template <> class ValueTraits<const char *>
{
  public:
    class convertible_string
    {
      public:
        convertible_string(const std::string &str) : realString(str)
        {
        }

        operator char const *() const
        {
            return this->realString.c_str();
        }

      private:
        std::string realString;
    };
    using from_type = convertible_string;
    static from_type ToCpp(jsvm_value value)
    {
        GET_ENV
        /*jsvm_valuetype valueType;
    jsvm_status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    if (valueType == jsvm_valuetype::Null || valueType == jsvm_valuetype::UNDEFINED ||
        valueType != jsvm_valuetype::STRING)
    {
        return "";
    }*/
        return from_type(getStringUtf8(value));
    }
    static jsvm_value ToJs(std::string_view value, bool callDestructor = true)
    {
        return makeStringUtf8(value.data(), value.length());
    }
    static bool is(jsvm_value value)
    {
        return isString(value);
    }
};

template <> class ValueTraits<jsvm_value>
{
  public:
    static jsvm_value ToCpp(jsvm_value value)
    {
        return value;
    }
    static jsvm_value ToJs(jsvm_value value, bool callDestructor = true)
    {
        return value;
    }
    static bool is(jsvm_value value)
    {
        return true;
    }
};
template <> class ValueTraits<void>
{
  public:
    static void ToCpp(jsvm_value value)
    {
        return;
    }
    static jsvm_value ToJs(int value, bool callDestructor = true)
    {
        if (0 == value)
        {
            return makeUndefined();
        }
        else
        {
            return makeNull();
        }
    }
    static bool is(jsvm_value value)
    {
        return isNull(value) || isUndefined(value);
    }
};

template <class T> jsvm_value ToJSValue(T t, bool callDestructor = true)
{
    return ValueTraits<T>::ToJs(t, callDestructor);
}
} // namespace internal
} // namespace jsbind

#endif

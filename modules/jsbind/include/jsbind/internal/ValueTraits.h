#ifndef __JSBIND_VALUE_TRAITS__H__
#define __JSBIND_VALUE_TRAITS__H__

#include "jsbind/internal/Value.h"
#include "jsvm/JSVM.h"
#include "jsvm/JSVM_Types.h"
#include "jsvm/napi/js_native_api.h"
#include <assert.h>
#include <jsbind/Utility.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace jsbind
{
template <typename ClassType> bool isWrappedClassOf();
template <typename ClassType> jsvm::Value wrapCppObject(ClassType *objectPointer, bool callDestructor);
namespace internal
{
template <class T> struct is_value_object;

template <typename T>
struct is_wrapped_class : std::conjunction<std::is_class<T>, std::negation<internal::is_value_object<T>>,
                                           // std::negation<detail::is_string<T>>,
                                           // std::negation<detail::is_mapping<T>>,
                                           std::negation<internal::is_sequence<T>>>
// std::negation<detail::is_array<T>>,
// std::negation<detail::is_tuple<T>>,
// std::negation<detail::is_shared_ptr<T>>>
{
};

template <typename T, typename Enable = void> class ValueTraits;

//template <typename T> class value_object;

template <typename T> class ValueTraits<T, std::enable_if_t<std::is_enum<T>::value>>
{
  public:
    static T ToCpp(jsvm::Value value)
    {
        return static_cast<T>(internal::getInt32(value));
    }
    static jsvm::Value ToJs(T value, bool callDestructor = true)
    {
        return internal::makeInt32(static_cast<int32_t>(value));
    }
    /*static bool is(jsvm::Value value)
    {
        return p_vl->IsInt32();
    }*/
};

template <typename T> class ValueTraits<T, std::enable_if_t<internal::is_wrapped_class<T>::value>>
{
  public:
    static jsvm::Value ToJs(T value, bool callDestructor = true)
    {
        T *object = new T(value); // copy construct to avoid life cycle issues
        return wrapCppObject<T>(object, callDestructor);
    }

    static T &ToCpp(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        GET_ENV
        jsvm::Status status;
        jsvm::ValueType valueType;
        status = jsvm::Typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm::Status::OK);
        DEBUG_CHECK(valueType == jsvm::ValueType::OBJECT)

        T *obj;
        status = jsvm::Unwrap(env, value, reinterpret_cast<void **>(&obj));
        DEBUG_CHECK(status == jsvm::Status::OK);
        return *obj;
    }
    static bool is(jsvm::Value value)
    {
        return isWrappedClassOf<T>();
    }
};

template <typename T> class ValueTraits<T *, std::enable_if_t<internal::is_wrapped_class<T>::value>>
{
  public:
    static jsvm::Value ToJs(T *value, bool callDestructor = true)
    {
        if (value == nullptr)
        {
            internal::makeNull();
        }
        return wrapCppObject<T>(value, callDestructor);
    }
    static T *ToCpp(jsvm::Value value)
    {
        DEBUG_CHECK(value != nullptr);
        GET_ENV
        jsvm::Status status;
        jsvm::ValueType valueType;
        status = jsvm::Typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm::Status::OK);
        if (valueType != jsvm::ValueType::OBJECT)
        {
            return nullptr;
        }
        T *obj;
        status = jsvm::Unwrap(env, value, reinterpret_cast<void **>(&obj));
        DEBUG_CHECK(status == jsvm::Status::OK);
        return obj;
    }
    static bool is(jsvm::Value value)
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

template <> class ValueTraits<int32_t>
{
  public:
    static int32_t ToCpp(jsvm::Value value)
    {
        return internal::getInt32(value);
    }
    static jsvm::Value ToJs(int32_t value, bool callDestructor = true)
    {
        return internal::makeInt32(value);
    }
    /*
    static bool is(Env env, Value value)
    {
        return p_vl->IsInt32();
    }*/
};
#if 0
template <> class ValueTraits<int32_t *>
{
  public:
    static int32_t ToCpp(jsvm::Value value)
    {
        return internal::getInt32(value);
    }
    static jsvm::Value ToJs(int32_t *value, bool callDestructor = true)
    {
        return internal::makeInt32(*value);
    }
    /*
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }*/
};
#endif
template <> class ValueTraits<const int32_t &> : public ValueTraits<int32_t>
{
};
template <> class ValueTraits<uint32_t>
{
  public:
    static uint32_t ToCpp(jsvm::Value value)
    {
        return internal::getUint32(value);
    }
    static jsvm::Value ToJs(uint32_t value, bool callDestructor = true)
    {
        return internal::makeUint32(value);
    }
    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsUint32();
    }*/
};

#ifdef OS_IOS

template <> class ValueTraits<long>
{
  public:
    static jsvm::Value ToJs(long value, bool callDestructor = true)
    {
        static_assert(sizeof(long) == 8);
        return internal::makeInt64Noloss(value);
    }
    static long ToCpp(jsvm::Value value)
    {
        /*if (!value->IsNumber() || value->IsNullOrUndefined())
        {
            return 0;
        }*/
        return internal::getInt64Noloss(value);
    }

    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsNumber();
    }*/
};
#endif
// 用bigint 保证精度不丢失,可以用于bullet对象指针
template <> class ValueTraits<int64_t>
{
  public:
    static int64_t ToCpp(jsvm::Value value)
    {
        GET_ENV
        jsvm::ValueType valueType;
        jsvm::Status status = jsvm::Typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm::Status::OK);
        if (valueType == jsvm::ValueType::Null || valueType == jsvm::ValueType::UNDEFINED)
        {
            return 0;
        }
        return internal::getInt64Noloss(value);
    }
    static jsvm::Value ToJs(int64_t value, bool callDestructor = true)
    {
        return internal::makeInt64Noloss(value);
    }
    /*static bool is(v8::Local<v8::Value> p_vl)
   {
       return p_vl->IsNumber();
   }*/
};
template <> class ValueTraits<uint64_t>
{
  public:
    static uint64_t ToCpp(jsvm::Value value)
    {
        /*if (!value->IsNumber() || value->IsNullOrUndefined())
        {
            return 0;
        }*/
        return internal::getUint64Noloss(value);
    }
    static jsvm::Value ToJs(uint64_t value, bool callDestructor = true)
    {
        return internal::makeUint64Noloss(value);
    }
    /*static bool is(jsvm::Value value)
    {
        return p_vl->IsNumber();
    }*/
};

/*template <> class ValueTraits<uint16_t>
{
  public:
    static uint16_t ToCpp(v8::Local<v8::Value> p_vl)
    {
        return p_vl.As<v8::Uint32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(uint16_t p_vl)
    {
        return v8::Uint32::NewFromUnsigned(v8::Isolate::GetCurrent(), p_vl);
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsUint32();
    }
};*/
template <> class ValueTraits<uint8_t>
{
  public:
    static uint8_t ToCpp(jsvm::Value value)
    {
        return static_cast<uint8_t>(internal::getUint32(value));
    }
    static jsvm::Value ToJs(uint8_t value, bool callDestructor = true)
    {
        return internal::makeUint32(static_cast<uint8_t>(value));
    }
    // static bool is(v8::Local<v8::Value> p_vl)
    //{
    //     return p_vl->IsUint32();
    // }
};
template <> class ValueTraits<const uint8_t &> : public ValueTraits<uint8_t>
{
};

template <> class ValueTraits<bool>
{
  public:
    static bool ToCpp(jsvm::Value value)
    {
        return internal::getBool(value);
    }
    static jsvm::Value ToJs(bool value, bool callDestructor = true)
    {
        return internal::makeBool(value);
    }
    static bool is(jsvm::Value value)
    {
        return internal::isBool(value);
    }
};
#if 0
template <> class ValueTraits<bool *>
{
  public:
    static bool ToCpp(jsvm::Value value)
    {
        return internal::getBool(value);
    }
    static jsvm::Value ToJs(bool *value, bool callDestructor = true)
    {
        if (value == nullptr)
        {
            internal::makeNull();
        }
        return internal::makeBool(*value);
    }
    static bool is(jsvm::Value value)
    {
        return internal::isBool(value);
    }
};
#endif
template <> class ValueTraits<float>
{
  public:
    static float ToCpp(jsvm::Value value)
    {
        return static_cast<float>(internal::getDouble(value));
    }
    static jsvm::Value ToJs(float value, bool callDestructor = true)
    {
        return internal::makeDouble(static_cast<float>(value));
    }
    /*static bool is(jsvm::Value value)
    {
        return p_vl->IsNumber();
    }*/
};
/*
template <> class ValueTraits<float *>
{
  public:
    static float ToCpp(v8::Local<v8::Value> p_vl)
    {
        return static_cast<float>(p_vl->NumberValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked());
        // return static_cast<float>(val.As<Number>()->Value());
    }
    static v8::Local<v8::Value> ToJs(float *p_vl, bool callDestructor = true)
    {
        if (p_vl == nullptr)
        {
            return v8::Null(v8::Isolate::GetCurrent());
        }
        return v8::Number::New(v8::Isolate::GetCurrent(), *p_vl);
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsNumber();
    }
};*/

template <> class ValueTraits<double>
{
  public:
    static double ToCpp(jsvm::Value value)
    {
        return internal::getDouble(value);
    }
    static jsvm::Value ToJs(double value, bool callDestructor = true)
    {
        return internal::makeDouble(value);
    }

    static bool is(jsvm::Value value)
    {
        return internal::isNumber(value);
    }
};

// utf16 u16string
template <> class ValueTraits<std::u16string>
{
  public:
    static std::u16string ToCpp(jsvm::Value value)
    {
        return internal::getStringUtf16(value);
    }
    static jsvm::Value ToJs(const std::u16string &value, bool callDestructor = true)
    {
        return internal::makeStringUtf16(value);
    }
    /*static bool is(jsvm::Value value)
    {
        return p_vl->IsString();
    }*/
};
// utf8 string
template <> class ValueTraits<std::string>
{
  public:
    static std::string ToCpp(jsvm::Value value)
    {
        GET_ENV
        /*jsvm::ValueType valueType;
        jsvm::Status status = jsvm::Typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm::Status::OK);
        if (valueType == jsvm::ValueType::Null || valueType == jsvm::ValueType::UNDEFINED ||
            valueType != jsvm::ValueType::STRING)
        {
            return "";
        }*/
        return internal::getStringUtf8(value);
    }
    static jsvm::Value ToJs(const std::string &value, bool callDestructor = true)
    {
        return internal::makeStringUtf8(value);
    }
    /*static bool is(jsvm::Value value)
    {
        return p_vl->IsString();
    }*/
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
    static from_type ToCpp(jsvm::Value value)
    {
        GET_ENV
        /*jsvm::ValueType valueType;
    jsvm::Status status = jsvm::Typeof(env, value, &valueType);
    DEBUG_CHECK(status == jsvm::Status::OK);
    if (valueType == jsvm::ValueType::Null || valueType == jsvm::ValueType::UNDEFINED ||
        valueType != jsvm::ValueType::STRING)
    {
        return "";
    }*/
        return from_type(internal::getStringUtf8(value));
    }
    static jsvm::Value ToJs(std::string_view value, bool callDestructor = true)
    {
        return internal::makeStringUtf8(value.data());
    }
    /*static bool is(jsvm::Value value)
    {
        return p_vl->IsString();
    }*/
};

template <> class ValueTraits<jsvm::Value>
{
  public:
    static jsvm::Value ToCpp(jsvm::Value value)
    {
        return value;
    }
    static jsvm::Value ToJs(jsvm::Value value, bool callDestructor = true)
    {
        return value;
    }
    /*static bool is(jsvm::Value value)
 {
     return true;
 }*/
};
template <> class ValueTraits<void>
{
  public:
    static void ToCpp(jsvm::Value value)
    {
        return;
    }
    static jsvm::Value ToJs(int value, bool callDestructor = true)
    {
        if (0 == value)
        {
            return internal::makeUndefined();
        }
        else
        {
            return internal::makeNull();
        }
    }
    static bool is(jsvm::Value value)
    {
        return internal::isNull(value) || internal::isUndefined(value);
    }
};

template <class T> jsvm::Value ToJSValue(T t, bool callDestructor = true)
{
    return ValueTraits<T>::ToJs(t, callDestructor);
}
} // namespace internal
} // namespace jsbind

#endif

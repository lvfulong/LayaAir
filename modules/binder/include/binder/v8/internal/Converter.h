#ifndef __JSBIND_CONVERTER__H__
#define __JSBIND_CONVERTER__H__

#include "binder/JSVM.h"
#include "binder/JSVM_Types.h"
#include "binder/napi/js_native_api.h"
#include "binder/v8/internal/Value.h"
#include <assert.h>
#include <binder/v8/Utility.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace jsbind
{

namespace internal
{
//template <typename T> T convert_value_object_from_v8(jsvm::Env env, jsvm::Value value);

//template <typename T> jsvm::Value convert_value_object_to_v8(jsvm::Env env, const T &t);
//template <typename T> jsvm::Value convert_value_object_to_v8(jsvm::Env env, T *t);
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
} // namespace internal

template <typename ClassType> bool isWrappedClassOf();

template <typename ClassType> jsvm::Value wrapCppObject(ClassType *objectPointer, bool callDestructor);

template <typename T, typename Enable = void> class Converter;

template <typename T> class value_object;



template <typename T> class Converter<T, std::enable_if_t<std::is_enum<T>::value>>
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

template <typename T> class Converter<T, std::enable_if_t<internal::is_wrapped_class<T>::value>>
{
  public:
    static jsvm::Value ToJs(T value, bool callDestructor = true)
    {
        T *object = new T(value); // copy construct to avoid life cycle issues
        return wrapCppObject<T>(object, callDestructor);
    }

    static T &ToCpp(jsvm::Value value)
    {
        // assert(!value.IsEmpty() && value->IsObject());
        GET_ENV
        T *obj;
        jsvm::Unwrap(env, value, reinterpret_cast<void **>(&obj));
        return *obj;
    }
    static bool is(jsvm::Value value)
    {
        return isWrappedClassOf<T>();
    }
};

template <typename T> class Converter<T *, std::enable_if_t<internal::is_wrapped_class<T>::value>>
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
        GET_ENV
        T *obj;
        jsvm::Unwrap(env, value, reinterpret_cast<void **>(&obj));
        return obj;
    }
    static bool is(jsvm::Value value)
    {
        return isWrappedClassOf<T>();
    }
};

template <typename T> struct Converter<T &> : Converter<T>
{
};

template <typename T> struct Converter<const T &> : Converter<T>
{
};

template <> class Converter<int32_t>
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
template <> class Converter<int32_t *>
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
template <> class Converter<const int32_t &> : public Converter<int32_t>
{
};
template <> class Converter<uint32_t>
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
#if 0
//#ifdef OS_IOS
template <> class Converter<long>
{
  public:
    static v8::Local<v8::Value> ToJs(long p_vl, bool callDestructor = true)
    {
        return v8::Number::New(v8::Isolate::GetCurrent(), *reinterpret_cast<double *>(&p_vl));
    }
    static long ToCpp(v8::Local<v8::Value> value)
    {
        if (!value->IsNumber() || value->IsNullOrUndefined())
        {
            return 0;
        }
        double v = value->NumberValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked();
        return *reinterpret_cast<long *>(&v);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsNumber();
    }
};
#endif
// 用bigint 保证精度不丢失,可以用于bullet对象指针
template <> class Converter<int64_t>
{
  public:
    static int64_t ToCpp(jsvm::Value value)
    {
        // if (!value->IsNumber() || value->IsNullOrUndefined())
        //{
        // return 0;
        //}
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
template <> class Converter<uint64_t>
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

/*template <> class Converter<uint16_t>
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
};
template <> class Converter<uint8_t>
{
  public:
    static uint8_t ToCpp(v8::Local<v8::Value> p_vl)
    {
        return p_vl.As<v8::Uint32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(uint8_t p_vl, bool callDestructor = true)
    {
        return v8::Uint32::NewFromUnsigned(v8::Isolate::GetCurrent(), p_vl);
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsUint32();
    }
};
template <> class Converter<const uint8_t &> : public Converter<uint8_t>
{
};*/

template <> class Converter<bool>
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
template <> class Converter<bool *>
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
template <> class Converter<float>
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
template <> class Converter<float *>
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

template <> class Converter<double>
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
/*template <> class Converter<void>
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
            return MakeUndefined();
        }
        else
        {
            return MakeNull();
        }
    }
    static bool is(jsvm::Value value)
    {
        return p_vl->IsNullOrUndefined();
    }
};*/
// utf16 u16string
template <> class Converter<std::u16string>
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
template <> class Converter<std::string>
{
  public:
    static std::string ToCpp(jsvm::Value value)
    {
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

// const char* sColor = Converter<const char*>::ToCpp(args);          so not save sColor for latter use   get right
// value address const std::string sColor = Converter<std::string>::ToCpp(args);  better const std::string sColor =
// Converter<const char*>::ToCpp(args);  better
template <> class Converter<const char *>
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

template <> class Converter<jsvm::Value>
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
template <> class Converter<void>
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
#if 0
template <typename T> class __JsSet
{
  public:
    static void FromJsSet(v8::Local<v8::Value> value, std::unordered_set<T> &p_v1)
    {
        if (value->IsSet())
        {

            v8::Isolate *isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            p_v1.clear();
            v8::Local<v8::Set> __set = value.As<v8::Set>();
            unsigned long size = __set->Size();
            p_v1.reserve(size);
            v8::Local<v8::Array> __array = __set->AsArray();
            for (unsigned long i = 0; i < size; i++)
            {
                T pValue = Converter<T>::ToCpp(__array->Get(context, i).ToLocalChecked());
                p_v1.insert(pValue);
            }
        }
    }
};
template <typename T, typename R> class __JsMap
{
  public:
    static void FromJsMap(v8::Local<v8::Value> value, std::unordered_map<T, R> &p_v1)
    {
        if (value->IsMap())
        {

            v8::Isolate *isolate = v8::Isolate::GetCurrent();
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            p_v1.clear();
            v8::Local<v8::Map> __map = value.As<v8::Map>();
            unsigned long size = __map->Size();
            p_v1.reserve(size);
            v8::Local<v8::Array> __array = __map->AsArray();
            for (unsigned long i = 0; i < size; i++)
            {
                T pKey = Converter<T>::ToCpp(__array->Get(context, i * 2).ToLocalChecked());
                R pValue = Converter<R>::ToCpp(__array->Get(context, i * 2 + 1).ToLocalChecked());
                p_v1[pKey] = pValue;
            }
        }
    }
};
#endif
/*class __JsByteArray
{
  public:
    static v8::Local<v8::Value> ToJsByteArray(const unsigned char *p_vl, int p_iSize)
    {
        if (0 == p_vl || p_iSize <= 0)
        {
            return Null(v8::Isolate::GetCurrent());
        }
        else
        {
            v8::Local<v8::Array> __array = v8::Array::New(v8::Isolate::GetCurrent(), p_iSize);
            for (int i = 0; i < p_iSize; ++i)
            {
                UNUSED(__array->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), i,
                                    v8::Int32::New(v8::Isolate::GetCurrent(), p_vl[i])));
            }
            return __array;
        }
    }
};*/




#if 0
template <typename T> class Converter<std::unordered_set<T>>
{
  public:
    static std::unordered_set<T> ToCpp(v8::Local<v8::Value> p_vl)
    {
        std::unordered_set<T> vec;
        __JsSet<T>::FromJsSet(p_vl, vec);
        return vec;
    }
};
template <typename T, typename R> class Converter<std::unordered_map<T, R>>
{
  public:
    static std::unordered_map<T, R> ToCpp(v8::Local<v8::Value> p_vl)
    {
        std::unordered_map<T, R> map;
        __JsMap<T, R>::FromJsMap(p_vl, map);
        return map;
    }
    static v8::Local<v8::Value> ToJs(const std::vector<T> &p_vl, bool callDestructor = true)
    {
        assert("to do");
        return Undefined(v8::Isolate::GetCurrent());
    }
};

template <typename T, typename R> class Converter<const std::unordered_map<T, R> &>
{
  public:
    static std::unordered_map<T, R> ToCpp(v8::Local<v8::Value> p_vl)
    {
        std::unordered_map<T, R> map;
        __JsMap<T, R>::FromJsMap(p_vl, map);
        return map;
    }
    static v8::Local<v8::Value> ToJs(const std::vector<T> &p_vl, bool callDestructor = true)
    {
        assert("to do");
        return Undefined(v8::Isolate::GetCurrent());
    }
};
#endif
namespace internal
{
    template <class T> jsvm::Value ToJSValue(T t, bool callDestructor = true)
    {
        return Converter<T>::ToJs(t, callDestructor);
    }
}
} // namespace jsbind

#endif

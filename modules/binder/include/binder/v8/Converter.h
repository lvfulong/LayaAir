#ifndef __CONVERTER__H__
#define __CONVERTER__H__

#include "JSArrayBuffer.h"
#include "Utility.h"
#include "binder/JSVM.h"
#include "binder/JSVM_Types.h"
#include "binder/napi/js_native_api.h"
#include <assert.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <v8.h>
#include <vector>

namespace jsbind
{

namespace internal
{
template <typename T> T convert_value_object_from_v8(jsvm::Env env, jsvm::Value value);

template <typename T> jsvm::Value convert_value_object_to_v8(jsvm::Env env, const T &t);
template <typename T> jsvm::Value convert_value_object_to_v8(jsvm::Env env, T *t);
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

template <typename ClassType> v8::Local<v8::Object> wrapCppObject(ClassType *objectPointer, bool callDestructor);

template <typename T, typename Enable = void> class Converter;

template <typename T> class value_object;

template <typename T> class Converter<T, std::enable_if_t<internal::is_value_object<T>::value>>
{
  public:
    static v8::Local<v8::Value> ToJs(T value, bool callDestructor = true)
    {
        return internal::convert_value_object_to_v8(value);
    }
    static T ToCpp(v8::Local<v8::Value> value)
    {
        return internal::convert_value_object_from_v8<T>(value);
    }
};

template <typename T> class Converter<T *, std::enable_if_t<internal::is_value_object<T>::value>>
{
  public:
    static v8::Local<v8::Value> ToJs(T *value, bool callDestructor = true)
    {
        return internal::convert_value_object_to_v8(value);
    }
    static T ToCpp(v8::Local<v8::Value> value)
    {
        return internal::convert_value_object_from_v8<T>(value);
    }
};

template <typename T> class Converter<T, std::enable_if_t<std::is_enum<T>::value>>
{
  public:
    static T ToCpp(v8::Local<v8::Value> p_vl)
    {
        return static_cast<T>(p_vl.As<v8::Int32>()->Value());
    }
    static v8::Local<v8::Value> ToJs(T p_vl, bool callDestructor = true)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), static_cast<int32_t>(p_vl));
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};

template <typename T> class Converter<T, std::enable_if_t<internal::is_wrapped_class<T>::value>>
{
  public:
    static v8::Local<v8::Value> ToJs(T value, bool callDestructor = true)
    {
        T *object = new T(value); // copy construct to avoid life cycle issues
        return wrapCppObject<T>(object, callDestructor);
    }

    static T &ToCpp(v8::Local<v8::Value> value)
    {
        assert(!value.IsEmpty() && value->IsObject());
        v8::Local<v8::Object> obj = value.As<v8::Object>();
        return *static_cast<T *>(obj->GetAlignedPointerFromInternalField(0));
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return isWrappedClassOf<T>();
    }
};

template <typename T> class Converter<T *, std::enable_if_t<internal::is_wrapped_class<T>::value>>
{
  public:
    static jsvm::Value ToJs(jsvm::Env env, T *value, bool callDestructor = true)
    {
        if (value == nullptr)
        {
            return v8::Null(v8::Isolate::GetCurrent());
        }
        return wrapCppObject<T>(value, callDestructor);
    }

    static T *ToCpp(jsvm::Env env, jsvm::Value value)
    {
        if (!value.IsEmpty() && value->IsObject())
        {
            v8::Local<v8::Object> obj = value.As<v8::Object>();
            return static_cast<T *>(obj->GetAlignedPointerFromInternalField(0));
        }
        else
        {
            return nullptr;
        }
    }
    static bool is(v8::Local<v8::Value> p_vl)
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
    static int32_t ToCpp(jsvm::Env env, jsvm::Value value)
    {
        int32_t result{0};
        jsvm::GetValueInt32(env, value, &result);
        // CHECK todo

        return result;
    }
    static jsvm::Value ToJs(jsvm::Env env, int32_t value, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::CreateInt32(env, value, &result);
        // CHECK todo
        return result;
    }
    /*static v8::Local<v8::Value> ToJsDate(int32_t p_vl)
    {
        return v8::Date::New(v8::Isolate::GetCurrent()->GetCurrentContext(), (double)p_vl).ToLocalChecked();
    }
    static bool is(Env env, Value value)
    {
        return p_vl->IsInt32();
    }*/
};
template <> class Converter<int32_t *>
{
  public:
    static int32_t ToCpp(jsvm::Env env, jsvm::Value value)
    {
        int32_t result{0};
        jsvm::GetValueInt32(env, value, &result);
        // CHECK todo

        return result;
    }
    static jsvm::Value ToJs(jsvm::Env env, int32_t *value, bool callDestructor = true)
    {
        /*if (p_vl == nullptr)
        {
            return v8::Null(v8::Isolate::GetCurrent());
        }*/
        jsvm::Value result;
        jsvm::CreateInt32(env, *value, &result);
        // CHECK todo
        return result;
    }
    /*static v8::Local<v8::Value> ToJsDate(int32_t* p_vl)
    {
        return v8::Date::New(v8::Isolate::GetCurrent()->GetCurrentContext(), (double)(*p_vl)).ToLocalChecked();
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }*/
};
template <> class Converter<const int32_t &> : public Converter<int32_t>
{
};

#ifdef OS_IOS
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
    static v8::Local<v8::Value> ToJsDate(long p_vl)
    {
        return v8::Date::New(v8::Isolate::GetCurrent()->GetCurrentContext(), (double)p_vl).ToLocalChecked();
    }
};
#endif
// lvtodo 目前只适用于裸指针reinterpret_cast
/*template <> class Converter<int64_t>
{
  public:
    static int64_t ToCpp(v8::Local<v8::Value> value)
    {
        if (!value->IsNumber() || value->IsNullOrUndefined())
        {
            return 0;
        }
        double v = value->NumberValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked();
        return *reinterpret_cast<int64_t *>(&v);
    }
    static v8::Local<v8::Value> ToJs(int64_t p_vl, bool callDestructor = true)
    {
        return v8::Number::New(v8::Isolate::GetCurrent(), *reinterpret_cast<double *>(&p_vl));
    }

    static v8::Local<v8::Value> ToJsDate(int64_t p_vl)
    {

        return v8::Date::New(v8::Isolate::GetCurrent()->GetCurrentContext(), (double)p_vl).ToLocalChecked();
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsNumber();
    }
};*/

template <> class Converter<int64_t>
{
  public:
    static int64_t ToCpp(jsvm::Env env, jsvm::Value value)
    {
        /*if (!value->IsNumber() || value->IsNullOrUndefined())
        {
            return 0;
        }*/
        int64_t result{0};
        jsvm::GetValueInt64(env, value, &result);
        // CHECK todo

        return result;
    }
    static jsvm::Value ToJs(jsvm::Env env, int64_t value, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::CreateInt64(env, value, &result);
        // CHECK todo
        return result;
    }

    /*static v8::Local<v8::Value> ToJsDate(int64_t p_vl)
    {

        return v8::Date::New(v8::Isolate::GetCurrent()->GetCurrentContext(), (double)p_vl).ToLocalChecked();
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsNumber();
    }*/
};

template <> class Converter<uint32_t>
{
  public:
    static uint32_t ToCpp(jsvm::Env env, jsvm::Value value)
    {
        uint32_t result{0};
        jsvm::GetValueUint32(env, value, &result);
        // CHECK todo

        return result;
    }
    static jsvm::Value ToJs(jsvm::Env env, uint32_t value, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::CreateUint32(env, value, &result);
        // CHECK todo
        return result;
    }
    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsUint32();
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
// 目前只适用于裸指针reinterpret_cast
/*template <> class Converter<uint64_t>
{
  public:
    static uint64_t ToCpp(v8::Local<v8::Value> value)
    {
        if (!value->IsNumber() || value->IsNullOrUndefined())
        {
            return 0;
        }
        double v = value->NumberValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked();
        return *reinterpret_cast<uint64_t *>(&v);
    }
    static v8::Local<v8::Value> ToJs(uint64_t p_vl, bool callDestructor = true)
    {
        return v8::Number::New(v8::Isolate::GetCurrent(), *reinterpret_cast<double *>(&p_vl));
    }

    static v8::Local<v8::Value> ToJsDate(uint64_t p_vl)
    {
        return v8::Date::New(v8::Isolate::GetCurrent()->GetCurrentContext(), (double)p_vl).ToLocalChecked();
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsNumber();
    }
};*/
template <> class Converter<bool>
{
  public:
    static bool ToCpp(jsvm::Env env, jsvm::Value value)
    {
        bool result{false};
        jsvm::GetValueBool(env, value, &result);
        // CHECK todo
        return result;
    }
    static jsvm::Value ToJs(jsvm::Env env, bool value, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::CreateUint32(env, static_cast<uint32_t>(value), &result); // why no create bool
        // CHECK todo
        return result;
    }
    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsBoolean();
    }*/
};

template <> class Converter<bool *>
{
  public:
    static bool ToCpp(jsvm::Env env, jsvm::Value value)
    {
        bool result{false};
        jsvm::GetValueBool(env, value, &result);
        // CHECK todo
        return result;
    }
    static jsvm::Value ToJs(jsvm::Env env, bool *value, bool callDestructor = true)
    {
        /*if (p_vl == nullptr)
        {
            return v8::Null(v8::Isolate::GetCurrent());
        }*/
        jsvm::Value result;
        jsvm::CreateUint32(env, static_cast<uint32_t>(*value), &result); // why no create bool
        // CHECK todo
        return result;
    }
    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsBoolean();
    }*/
};

/*template <> class Converter<float>
{
  public:
    static float ToCpp(v8::Local<v8::Value> p_vl)
    {
        return static_cast<float>(p_vl->NumberValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked());
        // return static_cast<float>(val.As<Number>()->Value());
    }
    static v8::Local<v8::Value> ToJs(float p_vl, bool callDestructor = true)
    {
        return v8::Number::New(v8::Isolate::GetCurrent(), p_vl);
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsNumber();
    }
};
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
    static double ToCpp(jsvm::Env env, jsvm::Value value)
    {
        double result{0.0};
        jsvm::GetValueDouble(env, value, &result);
        // CHECK todo
        return result;
    }
    static jsvm::Value ToJs(jsvm::Env env, double value, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::CreateDouble(env, value, &result);
        // CHECK todo
        return result;
    }
    /*static v8::Local<v8::Value> ToJsDate(double p_vl)
    {

        return v8::Date::New(v8::Isolate::GetCurrent()->GetCurrentContext(), (double)p_vl).ToLocalChecked();
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsNumber();
    }*/
};

template <> class Converter<jsvm::Value>
{
  public:
    static jsvm::Value ToCpp(jsvm::Env env, jsvm::Value value)
    {
        return value;
    }
    static jsvm::Value ToJs(jsvm::Env env, jsvm::Value value, bool callDestructor = true)
    {
        return value;
    }
    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return true;
    }*/
};

/*template <> class Converter<v8::Local<v8::ArrayBuffer>>
{
  public:
    static v8::Local<v8::ArrayBuffer> ToCpp(v8::Local<v8::Value> p_vl)
    {
        return p_vl.As<v8::ArrayBuffer>();
    }
    static v8::Local<v8::Value> ToJs(v8::Local<v8::ArrayBuffer> p_vl, bool callDestructor = true)
    {
        return p_vl.As<v8::Value>();
    }
    static bool is(v8::Local<v8::Value> p_vl)
    {
        return true;
    }
};*/

template <> class Converter<void>
{
  public:
    static void ToCpp(jsvm::Env env, jsvm::Value value)
    {
        return;
    }
    static jsvm::Value ToJs(jsvm::Env env, int value, bool callDestructor = true)
    {
        if (0 == value)
        {
            jsvm::Value result;
            jsvm::GetUndefined(env, &result);
            // CHECK todo
            return result;
        }
        else
        {
            jsvm::Value result;
            jsvm::GetNull(env, &result);
            // CHECK todo
            return result;
        }
    }
    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsNullOrUndefined();
    }*/
};
// const char* sColor = Converter<const char*>::ToCpp(args);         error  get right value address
// const std::string sColor = Converter<std::string>::ToCpp(args);   ok
// const std::string sColor = Converter<const char*>::ToCpp(args);   ok
template <> class Converter<const char *>
{
  public:
    /*class convertible_string
    {
      public:
        convertible_string(const char *str) : realString(str)
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
    static from_type ToCpp(Env env, Value value)
    {
        v8::String::Utf8Value const str(v8::Isolate::GetCurrent(), value);
        return from_type(reinterpret_cast<char const *>(*str));
    }*/
    static const char *ToCpp(jsvm::Env env, jsvm::Value value) = delete;
    static jsvm::Value ToJs(jsvm::Env env, std::string_view value, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::Status status;

        status = jsvm::CreateStringUtf8(env, value.data(), value.size(), &result);
        // CHECK todo
        return result;
    }
    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsString();
    }*/
};

template <> class Converter<std::string>
{
  public:
    static std::string ToCpp(jsvm::Env env, jsvm::Value value)
    {
        size_t len = 0;
        // Get the length
        jsvm::Status status = jsvm::GetValueStringUtf8(env, value, NULL, 0, &len);
        // assert(status == napi_ok);
        char *buf = new char[len + 1];

        status = jsvm::GetValueStringUtf8(env, value, buf, len + 1, &len);
        // assert(status == napi_ok);
        std::string utf8str(buf);
        delete[] buf;
        return utf8str;
    }
    static jsvm::Value ToJs(jsvm::Env env, std::string value, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::Status status;

        status = jsvm::CreateStringUtf8(env, value.c_str(), value.length(), &result);
        // CHECK todo
        return result;
    }
    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsString();
    }*/
};

// template<> class __TransferToCpp<laya::JSArrayBuffer*>{public:
//	static bool is(Local<Value> p_vl) { return p_vl->IsArrayBuffer() || p_vl->IsArrayBufferView(); }
//	static laya::JSArrayBuffer* ToCpp(Local<Value> p_vl) { return laya::JSArrayBuffer::fromeJSObj(p_vl); };
// };

/*template <> class Converter<v8::Local<v8::Primitive>>
{
  public:
    static v8::Local<v8::Value> ToJs(v8::Local<v8::Primitive> p_vl, bool callDestructor = true)
    {
        return p_vl;
    }
};

template <> class Converter<v8::Local<v8::Object>>
{
  public:
    static v8::Local<v8::Value> ToJs(v8::Local<v8::Object> p_vl, bool callDestructor = true)
    {
        return p_vl;
    }
};*/

// template <> class __TransferToJs<laya::JSArrayBuffer*>
//{public:static Handle<Value> ToJs( laya::JSArrayBuffer* p_vl ){return p_vl->toLocal();}};
template <typename T> class __JsArray
{
  public:
    static jsvm::Value ToJsArray(jsvm::Env env, const std::vector<T *> &value, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::Status status;

        int size = value.size();
        if (0 == size)
        {
            status = jsvm::CreateArrayWithLength(env, 0, &result);
            // CHECK todo
            return result;
        }
        else
        {
            status = jsvm::CreateArrayWithLength(env, size, &result);
            // CHECK todo
            for (int i = 0; i < size; i++)
            {
                jsvm::SetElement(env, result, i, Converter<T *>::ToJs(env, value.at(i), callDestructor));
            }
            return result;
        }
    }
    static jsvm::Value ToJsArray(jsvm::Env env, const std::vector<T> &value, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::Status status;
        int size = value.size();
        if (0 == size)
        {
            status = jsvm::CreateArrayWithLength(env, 0, &result);
            // CHECK todo
            return result;
        }
        else
        {
            status = CreateArrayWithLength(env, size, &result);
            // CHECK todo
            for (int i = 0; i < size; i++)
            {
                jsvm::SetElement(env, result, i, Converter<T>::ToJs(env, value.at(i), callDestructor));
            }
            return result;
        }
    }
    static void FillJsArray(jsvm::Env env, const std::vector<T> &value, jsvm::Value array, bool callDestructor = true)
    {
        jsvm::Value result;
        jsvm::Status status;
        int size = value.size();
        for (int i = 0; i < size; i++)
        {
            jsvm::SetElement(env, result, i, Converter<T>::ToJs(env, value.at(i), callDestructor));
        }
    }
    static void FromJsArray(jsvm::Env env, jsvm::Value array, std::vector<T *> &result)
    {
        bool isArray{false};
        jsvm::IsArray(env, array, isArray);
        // CHECK todo
        if (isArray)
        {
            value.clear();
            uint32_t length{0};
            jsvm::GetArrayLength(env, array, &length)
                // CHECK todo
                result.reserve(length);
            for (int i = 0; i < length; i++)
            {
                jsvm::Value element;
                jsvm::GetElement(env, array, i, &element);
                result.push_back(Converter<T *>::ToCpp(env, element));
            }
        }
    }
    static void FromJsArray(jsvm::Env env, jsvm::Value array, std::vector<T> &result)
    {
        bool isArray{false};
        jsvm::IsArray(env, array, isArray);
        // CHECK todo
        if (isArray)
        {
            result.clear();
            uint32_t length{0};
            jsvm::GetArrayLength(env, array, &length)
            // CHECK todo
            result.reserve(length);
            for (int i = 0; i < length; i++)
            {
                jsvm::Value element;
                jsvm::GetElement(env, array, i, &element);
                // CHECK todo
                result.push_back(Converter<T>::ToCpp(env, element));
            }
        }
    }
};
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

template <typename T> class Converter<std::vector<T>>
{
  public:
    static std::vector<T> ToCpp(jsvm::Env env, jsvm::Value value)
    {
        std::vector<T> vec;
        __JsArray<T>::FromJsArray(env, value, vec);
        return vec;
    }
    static jsvm::Value ToJs(jsvm::Env env, const std::vector<T> &value, bool callDestructor = true)
    {
        return __JsArray<T>::ToJsArray(env, value);
    }
};
template <typename T> class Converter<std::vector<T *>>
{
  public:
    static std::vector<T *> ToCpp(jsvm::Env env, jsvm::Value)
    {
        std::vector<T *> vec;
        __JsArray<T>::FromJsArray(env, Value, vec);
        return vec;
    }
    static jsvm::Value ToJs(jsvm::Env env, const std::vector<T *> &value, bool callDestructor = true)
    {
        return __JsArray<T>::ToJsArray(env, value, callDestructor);
    }
    /*static void ToCpp(v8::Local<v8::Value> p_vl, std::vector<T*>& vec)
    {
        return __JsArray<T>::FromJsArray(p_vl, vec);
    }*/

    /*static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsArray();
    }*/
};
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

/*inline v8::Local<v8::String> Js_Str(v8::Isolate* pIso, const char* str)
{
    return v8::String::NewFromUtf8(pIso, str).ToLocalChecked();
}*/

template <class T> jsvm::Value ToJSValue(jsvm::Env env, T t, bool callDestructor = true)
{
    return Converter<T>::ToJs(env, t, callDestructor);
}
} // namespace jsvm

#endif

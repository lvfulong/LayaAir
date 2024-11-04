#ifndef __JSBIND_ARRAY_H_
#define __JSBIND_ARRAY_H_

#include <binder/JSVM_Types.h>
#include <vector>
#include <binder/v8/internal/Converter.h>

namespace jsbind
{

    template <typename T> class Array
    {
    public:
        static jsvm::Value ToJs(const std::vector<T*>& value, bool callDestructor = true)
        {
            GET_ENV
                jsvm::Value result;
            jsvm::Status status;

            int size = value.size();
            if (0 == size)
            {
                status = jsvm::CreateArrayWithLength(env, 0, &result);
                DEBUG_CHECK(status == jsvm::Status::OK);
                return result;
            }
            else
            {
                status = jsvm::CreateArrayWithLength(env, size, &result);
                DEBUG_CHECK(status == jsvm::Status::OK);
                for (int i = 0; i < size; i++)
                {
                    jsvm::SetElement(env, result, i, Converter<T*>::ToJs(value.at(i), callDestructor));
                }
                return result;
            }
        }
        static jsvm::Value ToJs(const std::vector<T>& value, bool callDestructor = true)
        {
            GET_ENV
                jsvm::Value result;
            jsvm::Status status;
            int size = value.size();
            if (0 == size)
            {
                status = jsvm::CreateArrayWithLength(env, 0, &result);
                DEBUG_CHECK(status == jsvm::Status::OK);
                return result;
            }
            else
            {
                status = jsvm::CreateArrayWithLength(env, size, &result);
                DEBUG_CHECK(status == jsvm::Status::OK);
                for (int i = 0; i < size; i++)
                {
                    jsvm::SetElement(env, result, i, Converter<T>::ToJs(value.at(i), callDestructor));
                }
                return result;
            }
        }
        static void setData(const std::vector<T>& value, jsvm::Value array, bool callDestructor = true)
        {
            GET_ENV
                jsvm::Value result;
            jsvm::Status status;
            int size = value.size();
            for (int i = 0; i < size; i++)
            {
                jsvm::SetElement(env, result, i, Converter<T>::ToJs(value.at(i), callDestructor));
            }
        }
        static void getData(jsvm::Value array, std::vector<T*>& result)
        {
            GET_ENV
                bool isArray;
            jsvm::Status status;
            status = jsvm::IsArray(env, array, &isArray);
            DEBUG_CHECK(status == jsvm::Status::OK);
            if (isArray)
            {
                result.clear();
                uint32_t length;
                jsvm::GetArrayLength(env, array, &length);
                DEBUG_CHECK(status == jsvm::Status::OK);
                result.reserve(length);
                for (int i = 0; i < length; i++)
                {
                    jsvm::Value element;
                    jsvm::GetElement(env, array, i, &element);
                    result.push_back(Converter<T*>::ToCpp(element));
                }
            }
        }
        static void getData(jsvm::Value array, std::vector<T>& result)
        {
            GET_ENV
                bool isArray;
            jsvm::Status status;
            status = jsvm::IsArray(env, array, &isArray);
            DEBUG_CHECK(status == jsvm::Status::OK);
            if (isArray)
            {
                result.clear();
                uint32_t length;
                status = jsvm::GetArrayLength(env, array, &length);
                DEBUG_CHECK(status == jsvm::Status::OK);
                result.reserve(length);
                for (int i = 0; i < length; i++)
                {
                    jsvm::Value element;
                    status = jsvm::GetElement(env, array, i, &element);
                    DEBUG_CHECK(status == jsvm::Status::OK);
                    result.push_back(Converter<T>::ToCpp(element));
                }
            }
        }
    };
 
    template <typename T> class Converter<std::vector<T>>
    {
    public:
        static std::vector<T> ToCpp(jsvm::Value value)
        {
            std::vector<T> vec;
            Array<T>::getData(value, vec);
            return vec;
        }
        static jsvm::Value ToJs(const std::vector<T>& value, bool callDestructor = true)
        {
            return Array<T>::ToJs(value);
        }
    };
    template <typename T> class Converter<std::vector<T*>>
    {
    public:
        static std::vector<T*> ToCpp(jsvm::Value value)
        {
            std::vector<T*> vec;
            Array<T>::getData(value, vec);
            return vec;
        }
        static jsvm::Value ToJs(const std::vector<T*>& value, bool callDestructor = true)
        {
            return Array<T>::ToJs(value, callDestructor);
        }
        /*static void ToCpp(v8::Local<v8::Value> p_vl, std::vector<T*>& vec)
        {
            return Array<T>::getData(p_vl, vec);
        }*/

        /*static bool is(v8::Local<v8::Value> p_vl)
        {
            return p_vl->IsArray();
        }*/
    };
} // namespace jsbind

#endif

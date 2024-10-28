#ifndef __JSBIND_ARRAY_H__
#define __JSBIND_ARRAY_H__

#include <assert.h>
#include <binder/JSVM_Types.h>
#include <binder/v8/internal/Converter.h>
#include <binder/v8/Array.h>
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
        static void getData(jsvm::Env env, jsvm::Value array, std::vector<T*>& result)
        {
            bool isArray{ false };
            jsvm::IsArray(env, array, isArray);
            DEBUG_CHECK(status == jsvm::Status::OK);
            if (isArray)
            {
                value.clear();
                uint32_t length{ 0 };
                jsvm::GetArrayLength(env, array, &length) DEBUG_CHECK(status == jsvm::Status::OK);
                result.reserve(length);
                for (int i = 0; i < length; i++)
                {
                    jsvm::Value element;
                    jsvm::GetElement(env, array, i, &element);
                    result.push_back(Converter<T*>::ToCpp(element));
                }
            }
        }
        static void getData(jsvm::Env env, jsvm::Value array, std::vector<T>& result)
        {
            bool isArray{ false };
            jsvm::IsArray(env, array, isArray);
            DEBUG_CHECK(status == jsvm::Status::OK);
            if (isArray)
            {
                result.clear();
                uint32_t length{ 0 };
                jsvm::GetArrayLength(env, array, &length) DEBUG_CHECK(status == jsvm::Status::OK);
                result.reserve(length);
                for (int i = 0; i < length; i++)
                {
                    jsvm::Value element;
                    jsvm::GetElement(env, array, i, &element);
                    DEBUG_CHECK(status == jsvm::Status::OK);
                    result.push_back(Converter<T>::ToCpp(element));
                }
            }
        }
};
} // namespace jsbind
#endif

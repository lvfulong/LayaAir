#ifndef __JSBIND_ARRAY_H_
#define __JSBIND_ARRAY_H_

#include <jsbind/internal/ValueTraits.h>
#include <jsvm/JSVM_Types.h>
#include <vector>

namespace jsbind
{

template <typename T> class Array
{
  public:
    static jsvm_value ToJs(const std::vector<T *> &value, bool callDestructor = true)
    {
        GET_ENV
        jsvm_value result;
        jsvm_status status;

        int size = value.size();
        if (0 == size)
        {
            status = jsvm_create_array_with_length(env, 0, &result);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            return result;
        }
        else
        {
            status = jsvm_create_array_with_length(env, size, &result);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            for (int i = 0; i < size; i++)
            {
                jsvm_set_element(env, result, i, internal::ValueTraits<T *>::ToJs(value.at(i), callDestructor));
            }
            return result;
        }
    }
    static jsvm_value ToJs(const std::vector<T> &value, bool callDestructor = true)
    {
        GET_ENV
        jsvm_value result;
        jsvm_status status;
        int size = value.size();
        if (0 == size)
        {
            status = jsvm_create_array_with_length(env, 0, &result);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            return result;
        }
        else
        {
            status = jsvm_create_array_with_length(env, size, &result);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            for (int i = 0; i < size; i++)
            {
                status = jsvm_set_element(env, result, i, internal::ValueTraits<T>::ToJs(value.at(i), callDestructor));
                DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            }
            return result;
        }
    }
    static void setData(const std::vector<T> &value, jsvm_value array, bool callDestructor = true)
    {
        GET_ENV
        jsvm_value result;
        jsvm_status status;
        int size = value.size();
        for (int i = 0; i < size; i++)
        {
            status = jsvm_set_element(env, array, i, internal::ValueTraits<T>::ToJs(value.at(i), callDestructor));
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        }
    }
    static void getData(jsvm_value array, std::vector<T *> &result)
    {
        GET_ENV
        bool isArray;
        jsvm_status status;
        status = jsvm_is_array(env, array, &isArray);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (isArray)
        {
            result.clear();
            uint32_t length;
            status = jsvm_get_array_length(env, array, &length);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            result.reserve(length);
            for (int i = 0; i < length; i++)
            {
                jsvm_value element;
                status = jsvm_get_element(env, array, i, &element);
                DEBUG_CHECK(status == jsvm_status::jsvm_ok);
                result.push_back(internal::ValueTraits<T *>::ToCpp(element));
            }
        }
    }
    static void getData(jsvm_value array, std::vector<T> &result)
    {
        GET_ENV
        bool isArray;
        jsvm_status status;
        status = jsvm_is_array(env, array, &isArray);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (isArray)
        {
            result.clear();
            uint32_t length;
            status = jsvm_get_array_length(env, array, &length);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            result.reserve(length);
            for (int i = 0; i < length; i++)
            {
                jsvm_value element;
                status = jsvm_get_element(env, array, i, &element);
                DEBUG_CHECK(status == jsvm_status::jsvm_ok);
                result.push_back(internal::ValueTraits<T>::ToCpp(element));
            }
        }
    }
};
namespace internal
{
template <typename T> class ValueTraits<std::vector<T>>
{
  public:
    static std::vector<T> ToCpp(jsvm_value value)
    {
        std::vector<T> vec;
        Array<T>::getData(value, vec);
        return vec;
    }
    static jsvm_value ToJs(const std::vector<T> &value, bool callDestructor = true)
    {
        return Array<T>::ToJs(value, callDestructor);
    }
};
template <typename T> class ValueTraits<const std::vector<T> &> : public ValueTraits<std::vector<T>>
{
};
template <typename T> class ValueTraits<std::vector<T> &> : public ValueTraits<std::vector<T>>
{
};
template <typename T> class ValueTraits<std::vector<T *>>
{
  public:
    static std::vector<T *> ToCpp(jsvm_value value)
    {
        std::vector<T *> vec;
        Array<T>::getData(value, vec);
        return vec;
    }
    static jsvm_value ToJs(const std::vector<T *> &value, bool callDestructor = true)
    {
        return Array<T>::ToJs(value, callDestructor);
    }
    static bool is(jsvm_value value)
    {
        return isArray(value);
    }
};
template <typename T> class ValueTraits<const std::vector<T *> &> : public ValueTraits<std::vector<T *>>
{
};
template <typename T> class ValueTraits<std::vector<T *> &> : public ValueTraits<std::vector<T *>>
{
};
} // namespace internal
} // namespace jsbind

#endif

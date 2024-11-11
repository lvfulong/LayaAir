#ifndef __JSBIND_MAP_H_
#define __JSBIND_MAP_H_

#include <binder/JSVM_Types.h>
#include <binder/internal/Converter.h>
#include <unordered_map>

namespace jsbind
{

template <typename K, typename T> class Map
{
  public:
    static void FromJsMap(jsvm::Value value, std::unordered_map<K, T> &map)
    {

        GET_ENV
        jsvm::Status status;
        jsvm::Value names;

        status = jsvm::GetPropertyNames(env, value, &names);
        DEBUG_CHECK(status == jsvm::Status::OK);

        uint32_t length;
        status = jsvm::GetArrayLength(env, names, &length);
        DEBUG_CHECK(status == jsvm::Status::OK);

        for (auto index = 0; index < length; index++)
        {
            jsvm::Value keyNapi;
            status = jsvm::GetElement(env, names, index, &keyNapi);
            DEBUG_CHECK(status == jsvm::Status::OK);

            K k = Converter<K>::ToCpp(keyNapi);

            jsvm::Value valueNapi;
            status = jsvm::GetProperty(env, value, keyNapi, &valueNapi);
            DEBUG_CHECK(status == jsvm::Status::OK);
            map[k] = Converter<T>::ToCpp(valueNapi);
        }
    }
};

template <typename K, typename T> class Converter<std::unordered_map<K, T>>
{
  public:
    static std::unordered_map<K, T> ToCpp(jsvm::Value value)
    {
        std::unordered_map<K, T> map;
        Map<K, T>::FromJsMap(value, map);
        return map;
    }
    static jsvm::Value ToJs(const std::unordered_map<K, T> &map, bool callDestructor = true)
    {
        DEBUG_CHECK(false); // todo
        return internal::makeUndefined();
    }
};

template <typename T, typename R> class Converter<const std::unordered_map<T, R> &>
{
  public:
    static std::unordered_map<T, R> ToCpp(jsvm::Value value)
    {
        std::unordered_map<T, R> map;
        Map<T, R>::FromJsMap(value, map);
        return map;
    }
    static jsvm::Value ToJs(const std::unordered_map<T, R> &map, bool callDestructor = true)
    {
        DEBUG_CHECK(false); // todo
        return internal::makeUndefined();
    }
};

} // namespace jsbind

#endif

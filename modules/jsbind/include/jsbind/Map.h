#ifndef __JSBIND_MAP_H_
#define __JSBIND_MAP_H_

#include <jsbind/Local.h>
#include <jsbind/internal/ValueTraits.h>
#include <jsvm/JSVM_Types.h>
#include <unordered_map>

namespace jsbind
{

template <typename K, typename T> class Map
{
  public:
#if 0
    static void FromJsMap(jsvm_value value, std::unordered_map<K, T> &map)
    {

        GET_ENV
        jsvm_status status;
        jsvm_value names;

        jsvm_value keys = Local(value)["keys"].getHandle();
        Local global(jsvm::global());
        Local funcArrayFrom = global["Array"]["from"];
        names = funcArrayFrom.call<jsvm_value>(jsvm::global(), keys);
        
        bool isArray = true;
        jsvm::IsArray(env, names, &isArray);
        DEBUG_CHECK(isArray);
#if 0
        status = jsvm::GetPropertyNames(env, value, &names);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
#endif
        uint32_t length;
        status = jsvm::GetArrayLength(env, names, &length);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        for (auto index = 0; index < length; index++)
        {
            jsvm_value keyNapi;
            status = jsvm::GetElement(env, names, index, &keyNapi);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);

            K k = internal::ValueTraits<K>::ToCpp(keyNapi);

            jsvm_value valueNapi;
            status = jsvm::GetProperty(env, value, keyNapi, &valueNapi);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            map[k] = internal::ValueTraits<T>::ToCpp(valueNapi);
        }
    }
#endif
    static void FromJsMap(jsvm_value value, std::unordered_map<K, T> &map)
    {
        GET_ENV
        jsvm_status status;
        jsvm_valuetype type;
        jsvm_value forEachFunc;
        status = jsvm_get_named_property(env, value, "forEach", &forEachFunc);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        jsvm_typeof(env, forEachFunc, &type);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        if (type == jsvm_valuetype::jsvm_function)
        {
            auto forEachCallback = [](jsvm_env env, jsvm_callback_info info) -> jsvm_value {
                size_t argc = 2;
                jsvm_value argv[2];

                std::unordered_map<K, T> *ptrMap;
                jsvm_status status = jsvm_get_cb_info(env, info, &argc, argv, nullptr, (void **)&ptrMap);
                DEBUG_CHECK(status == jsvm_status::jsvm_ok);
                K k = internal::ValueTraits<K>::ToCpp(argv[1]);
                (*ptrMap)[k] = internal::ValueTraits<T>::ToCpp(argv[0]);
                return nullptr;
            };

            jsvm_value fn;
            status = jsvm_create_function(env, "callback", NAPI_AUTO_LENGTH, forEachCallback, &map, &fn);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);

            jsvm_value return_val;
            status = jsvm_call_function(env, value, forEachFunc, 1, &fn, &return_val);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        }
    }
};
namespace internal
{
template <typename K, typename T> class ValueTraits<std::unordered_map<K, T>>
{
  public:
    static std::unordered_map<K, T> ToCpp(jsvm_value value)
    {
        std::unordered_map<K, T> map;
        Map<K, T>::FromJsMap(value, map);
        return map;
    }
    static jsvm_value ToJs(const std::unordered_map<K, T> &map, bool callDestructor = true)
    {
        DEBUG_CHECK(false); // todo
        return internal::makeUndefined();
    }
};

template <typename T, typename R> class ValueTraits<const std::unordered_map<T, R> &>
{
  public:
    static std::unordered_map<T, R> ToCpp(jsvm_value value)
    {
        std::unordered_map<T, R> map;
        Map<T, R>::FromJsMap(value, map);
        return map;
    }
    static jsvm_value ToJs(const std::unordered_map<T, R> &map, bool callDestructor = true)
    {
        DEBUG_CHECK(false); // todo
        return internal::makeUndefined();
    }
};
} // namespace internal

} // namespace jsbind

#endif

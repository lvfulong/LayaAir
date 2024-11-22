#ifndef __JSBIND_MAP_H_
#define __JSBIND_MAP_H_

#include <jsbind/Local.h>
#include <jsbind/internal/Converter.h>
#include <jsvm/JSVM_Types.h>
#include <unordered_map>

namespace jsbind
{

template <typename K, typename T> class Map
{
  public:
#if 0
    static void FromJsMap(jsvm::Value value, std::unordered_map<K, T> &map)
    {

        GET_ENV
        jsvm::Status status;
        jsvm::Value names;

        jsvm::Value keys = Local(value)["keys"].getHandle();
        Local global(jsvm::global());
        Local funcArrayFrom = global["Array"]["from"];
        names = funcArrayFrom.call<jsvm::Value>(jsvm::global(), keys);
        
        bool isArray = true;
        jsvm::IsArray(env, names, &isArray);
        DEBUG_CHECK(isArray);
#if 0
        status = jsvm::GetPropertyNames(env, value, &names);
        DEBUG_CHECK(status == jsvm::Status::OK);
#endif
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
#endif
    static void FromJsMap(jsvm::Value value, std::unordered_map<K, T> &map)
    {
        GET_ENV
        jsvm::Status status;
        jsvm::ValueType type;
        jsvm::Value forEachFunc;
        status = jsvm::GetNamedProperty(env, value, "forEach", &forEachFunc);
        DEBUG_CHECK(status == jsvm::Status::OK);
        jsvm::Typeof(env, forEachFunc, &type);
        DEBUG_CHECK(status == jsvm::Status::OK);
        if (type == jsvm::ValueType::FUNCTION)
        {
            auto forEachCallback = [](jsvm::Env env, jsvm::CallbackInfo info) -> jsvm::Value {
                size_t argc = 2;
                jsvm::Value argv[2];

                std::unordered_map<K, T> *ptrMap;
                jsvm::Status status = jsvm::GetCbInfo(env, info, &argc, argv, nullptr, (void **)&ptrMap);
                DEBUG_CHECK(status == jsvm::Status::OK);
                K k = Converter<K>::ToCpp(argv[1]);
                (*ptrMap)[k] = Converter<T>::ToCpp(argv[0]);
                return nullptr;
            };

            jsvm::Value fn;
            status = jsvm::CreateFunction(env, "callback", NAPI_AUTO_LENGTH, forEachCallback, &map, &fn);
            DEBUG_CHECK(status == jsvm::Status::OK);

            jsvm::Value return_val;
            status = jsvm::CallFunction(env, value, forEachFunc, 1, &fn, &return_val);
            DEBUG_CHECK(status == jsvm::Status::OK);
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

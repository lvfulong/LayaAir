#ifndef __JSBIND_VALUE_H__
#define __JSBIND_VALUE_H__

#include <binder/JSEnv.h>
#include <binder/JSVM.h>
#include <binder/JSVM_Types.h>

namespace jsbind
{
    template <class T>  jsvm::Value Make(T t, bool callDestructor = true)
    {
        return Converter<T>::ToJs(t, callDestructor);
    }
     inline jsvm::Value MakeNull()
    {
        return internal::makeNull();
    }
     inline jsvm::Value MakeUndefined()
    {
        return internal::makeUndefined();
    }
     inline jsvm::Value MakeObject()
    {
        return internal::makeObject();
    }
     template<typename R, typename... Args>
     inline jsvm::Value MakeFunction(std::function<R(Args...)> value)
     {
         return internal::makeFunction(value);
     }
     inline jsvm::Value MakeFunctionRaw(std::function<jsvm::Value(jsvm::Env env, jsvm::CallbackInfo info)> value)
     {
         return internal::makeFunctionRaw(value);
     }

      /*template <typename T> T as(Local value) 
     {
         DEBUG_CHECK(value.isValid());
         GET_ENV
             return Converter<T>::ToCpp(value.getHandle());
     }
     template <typename T> T as(jsvm::Value value) 
     {
         DEBUG_CHECK(value != nullptr);
         GET_ENV
         return Converter<T>::ToCpp(value);
     }
    template <typename T> bool is(jsvm::Value value)
     {
        DEBUG_CHECK(value != nullptr);
         GET_ENV
             return Converter<T>::is(value);
     }*/

} // namespace jsbind
#endif

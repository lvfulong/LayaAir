#ifndef __JSBIND_VALUE_H__
#define __JSBIND_VALUE_H__

#include <assert.h>
#include <binder/JSVM_Types.h>
#include <binder/v8/Converter.h>

namespace jsbind
{
template <class T> jsvm::Value MakeJSValue(T t, bool callDestructor = true)
{
    GET_ENV
    return Converter<T>::ToJs(t, callDestructor);
}

template <typename T> bool is()
{
    GET_ENV
    return Converter<T>::is(env);
}
} // namespace jsbind
#endif

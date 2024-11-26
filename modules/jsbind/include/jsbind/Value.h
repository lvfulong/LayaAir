#ifndef __JSBIND_VALUE_H__
#define __JSBIND_VALUE_H__

#include <jsvm/JSEnv.h>
#include <jsvm/JSVM.h>
#include <jsvm/JSVM_Types.h>

namespace jsbind
{
template <class T> jsvm_value Make(T t, bool callDestructor = true)
{
    return internal::ValueTraits<T>::ToJs(t, callDestructor);
}
inline jsvm_value MakeNull()
{
    return internal::makeNull();
}
inline jsvm_value MakeUndefined()
{
    return internal::makeUndefined();
}
inline jsvm_value MakeObject()
{
    return internal::makeObject();
}

/*template <typename T> T as(Local value)
{
   DEBUG_CHECK(value.isValid());
   GET_ENV
       return ValueTraits<T>::ToCpp(value.getHandle());
}*/
template <typename T> T as(jsvm_value value)
{
    DEBUG_CHECK(value != nullptr);
    GET_ENV
    return internal::ValueTraits<T>::ToCpp(value);
}
/*template <typename T> bool is(jsvm_value value)
{
  DEBUG_CHECK(value != nullptr);
   GET_ENV
       return ValueTraits<T>::is(value);
}*/

} // namespace jsbind
#endif

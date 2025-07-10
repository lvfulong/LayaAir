#ifndef __JSBIND_H__
#define __JSBIND_H__

#include <jsbind/Array.h>
#include <jsbind/ArrayBuffer.h>
#include <jsbind/Class.h>
#include <jsbind/Date.h>
#include <jsbind/Enum.h>
#include <jsbind/Error.h>
#include <jsbind/Function.h>
#include <jsbind/Local.h>
#include <jsbind/Map.h>
#include <jsbind/Object.h>
#include <jsbind/Persistent.h>
#include <jsbind/Reference.h>
#include <jsbind/Promise.h>
#include <jsbind/Script.h>
#include <jsbind/Value.h>
#include <jsvm/JSEnv.h>
#include <jsvm/JSVM.h>
#include <jsvm/JSVM_Types.h>
#include <utils/JCMemorySurvey.h>
#if defined(JS_V8)
#include <jsvm/napi/js_native_api.h>
#include <jsvm/napi/js_native_api_v8.h>
#endif

namespace jsbind
{

extern void runDeinitializers();
template <typename ClassType> void makeWeak(ClassType *objectPointer)
{
    ClassRegistryManager::makeWeak<ClassType>(objectPointer);
}
template <typename ClassType> void makeStrong(ClassType *objectPointer)
{
    ClassRegistryManager::makeStrong<ClassType>(objectPointer);
}
template <typename ClassType> jsvm_value toLocal(ClassType *objectPointer)
{
    GET_ENV
    ClassRegistryBase *classRegistry = ClassRegistryManager::getClassRegistry(type_id<ClassType>());
    auto objectRegistry = classRegistry->getObjectRegistry(objectPointer);
    DEBUG_CHECK(objectRegistry != nullptr);
    jsvm_value result;
    jsvm_status status;
    status = jsvm_get_reference_value(env, objectRegistry->objectRef_, &result);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}

template <typename ClassType> Reference<ClassType> toReference(ClassType* objectPointer)
{
    GET_ENV
    ClassRegistryBase* classRegistry = ClassRegistryManager::getClassRegistry(type_id<ClassType>());
    auto objectRegistry = classRegistry->getObjectRegistry(objectPointer);
    if (objectRegistry != nullptr)
    {
        return Reference<ClassType>(objectRegistry->objectRef_);
    }
    else
    {
        jsvm_ref ref = nullptr;
        jsvm_status status = jsvm_create_reference(env, jsbind::internal::ValueTraits<ClassType*>::ToJs(objectPointer), 1, &ref);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        return Reference<ClassType>(ref);
    }
}
extern void AdjustAmountOfExternalAllocatedMemory(int p_nMemorySize);

template <class T> jsvm_value Make(T t, bool callDestructor = true)
{
    return jsbind::internal::ValueTraits<T>::ToJs(t, callDestructor);
}
template <typename T> T as(jsvm_value value)
{
    DEBUG_CHECK(value != nullptr);
    GET_ENV
    return jsbind::internal::ValueTraits<T>::ToCpp(value);
}
} // namespace jsbind
#endif

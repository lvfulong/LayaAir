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
#include <jsbind/Promise.h>
#include <jsbind/Script.h>
#include <jsbind/String.h>
#include <jsbind/Value.h>
#include <jsvm/JSEnv.h>
#include <jsvm/JSVM.h>
#include <jsvm/JSVM_Types.h>
#include <utils/JCMemorySurvey.h>

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
    ClassRegistry<ClassType> &classRegistry = ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());
    auto objectRegistry = classRegistry.getObjectRegistry(objectPointer);
    DEBUG_CHECK(objectRegistry != nullptr);
    jsvm_value result;
    jsvm_status status;
    status = jsvm_get_reference_value(env, objectRegistry->objectRef_, &result);
    DEBUG_CHECK(status == jsvm_status::ok);
    return result;
}
extern void AdjustAmountOfExternalAllocatedMemory(int p_nMemorySize);
} // namespace jsbind
#endif

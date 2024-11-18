#ifndef __JSBIND_H__
#define __JSBIND_H__

#include <utils/JCMemorySurvey.h>
#include <jsbind/Map.h>
#include <jsbind/Array.h>
#include <jsbind/ArrayBuffer.h>
#include <jsbind/Date.h>
#include <jsbind/Enum.h>
#include <jsvm/JSEnv.h>
#include <jsbind/Local.h>
#include <jsbind/Object.h>
#include <jsbind/Persistent.h>
#include <jsbind/Promise.h>
#include <jsbind/Script.h>
#include <jsbind/String.h>
#include <jsbind/Value.h>
#include <jsbind/Function.h>
#include <jsbind/Class.h>
#include <jsbind/Error.h>
#include <jsvm/JSVM.h>
#include <jsvm/JSVM_Types.h>
//#ifdef JS_V8_DEBUGGER
//#include <jsvm/v8debug/debug-agent.h>
//#endif

namespace jsbind
{

extern void runDeinitializers();
template <typename ClassType> void makeWeak(ClassType* objectPointer)
{
    ClassRegistryManager::makeWeak<ClassType>(objectPointer);
}
template <typename ClassType> void makeStrong(ClassType* objectPointer)
{
    ClassRegistryManager::makeStrong<ClassType>(objectPointer);
}
template <typename ClassType> jsvm::Value toLocal(ClassType *objectPointer)
{
    GET_ENV
    ClassRegistry<ClassType> &classRegistry = ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());
    auto objectRegistry = classRegistry.getObjectRegistry(objectPointer);
    DEBUG_CHECK(objectRegistry != nullptr);
    jsvm::Value result;
    jsvm::Status status;
    status = jsvm::GetReferenceValue(env, objectRegistry->objectRef_, &result);
    DEBUG_CHECK(status == jsvm::Status::OK);
    return result;
}
extern void AdjustAmountOfExternalAllocatedMemory(int p_nMemorySize);
} // namespace jsbind
#endif

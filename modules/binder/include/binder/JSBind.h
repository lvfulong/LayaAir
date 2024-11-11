#ifndef __JSBIND_H__
#define __JSBIND_H__

#include <utils/JCMemorySurvey.h>
#include <binder/Map.h>
#include <binder/Array.h>
#include <binder/ArrayBuffer.h>
#include <binder/Date.h>
#include <binder/Enum.h>
#include <binder/JSEnv.h>
#include <binder/Local.h>
#include <binder/Object.h>
#include <binder/Persistent.h>
#include <binder/Promise.h>
#include <binder/Script.h>
#include <binder/String.h>
#include <binder/Value.h>
#include <binder/Function.h>
#include <binder/Class.h>
#include <binder/JSVM.h>
#include <binder/JSVM_Types.h>
#ifdef JS_V8_DEBUGGER
#include <binder/v8debug/debug-agent.h>
#endif

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

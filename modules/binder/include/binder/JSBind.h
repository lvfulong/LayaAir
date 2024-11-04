#ifndef __JSBIND_H__
#define __JSBIND_H__

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
} // namespace jsbind
#endif

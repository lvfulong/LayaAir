#ifndef __JSBIND_CLASS_H__
#define __JSBIND_CLASS_H__

#include "Invoke.h"
#include "Utility.h"
#include <assert.h>
#include <functional>
#include <map>
#include <string>
#include <utils/Log.h>

namespace jsbind
{
template <typename ClassType> class ClassRegistry;
struct ObjectRegistry
{
    jsvm::Ref objectRef_;
    bool callDestructor = true;
};
class ClassRegistryBase
{
  public:
    std::unordered_map<void *, ObjectRegistry> objects_;
    // std::vector<ClassRegistryBase *> bases_;
    // std::vector<ClassRegistryBase *> derivatives_;
    jsvm::Ref classRef_ = nullptr;

  public:
    virtual ~ClassRegistryBase()
    {
    }
    template <typename ClassType> ObjectRegistry *getObjectRegistry(ClassType *objectPointer)
    {
        auto it = objects_.find(objectPointer);
        if (it != objects_.end())
        {
            return &it->second;
        }

        /*for (auto const info : derivatives_)
         {
             ObjectRegistry *result = info->getObjectRegistry(objectPointer);
             if (result != nullptr)
                 return result;
         }*/

        return nullptr;
    }
};
template <typename ClassType> class ClassRegistry : public ClassRegistryBase
{
  public:
    typedef ClassType *(*ConstructorFunctionType)(jsvm::Env env, jsvm::CallbackInfo info);
    ClassRegistry()
    {
    }
    virtual ~ClassRegistry()
    {
        auto it = objects_.begin();
        for (; it != objects_.end(); it++)
        {
            if (it->second.callDestructor)
            {
                internal::raw_destructor((ClassType *)it->first);
            }

            // it->second.pobj.ClearWeak();
            // it->second.pobj.Reset();
        }
        objects_.clear();

        // func_.Reset();
        // js_func_.Reset();
        GET_ENV
        jsvm::DeleteReference(env, classRef_);
    }

    void registerConstructor(uint32_t numPara, ConstructorFunctionType func)
    {
        assert(constructorFunctionMap_.find(numPara) == constructorFunctionMap_.end());
        constructorFunctionMap_.insert(std::make_pair(numPara, func));
    }
    ClassType *ConstructObject(uint32_t numPara, jsvm::Env env, jsvm::CallbackInfo info)
    {
        auto it = constructorFunctionMap_.find(numPara);
        if (it != constructorFunctionMap_.end())
        {
            // info.GetIsolate()->AdjustAmountOfExternalAllocatedMemory(static_cast<int64_t>(sizeof(ClassType)));
            return (it->second)(env, info);
        }
        DEBUG_CHECK(false);
        return nullptr;
    }

    jsvm::Value wrapCppObject(ClassType *objectPointer, bool callDestructor = true)
    {
        GET_ENV
        jsvm::Status status;
        jsvm::Value cons;
        status = jsvm::GetReferenceValue(env, classRef_, &cons);
        DEBUG_CHECK(status == jsvm::Status::OK);
        jsvm::Value instance;
        status = jsvm::NewInstance(env, cons, 0, nullptr, &instance);
        DEBUG_CHECK(status == jsvm::Status::OK);
        return instance;
    }
    void addBase(ClassRegistryBase *info)
    {
        /*auto it = std::find(bases_.begin(), bases_.end(), info);
        if (it != bases_.end())
        {
            assert(false && "duplicated inheritance");
            // throw std::runtime_error(class_name()+ " is already inherited from " + info.class_name());
        }
        bases_.emplace_back(info);
        info->derivatives_.emplace_back(this);*/
    }

    void removeObject(ClassType *objectPointer)
    {

        auto it = objects_.find((void *)objectPointer);
        DEBUG_CHECK(it != objects_.end());
        if (it != objects_.end())
        {
            if (it->second.callDestructor)
            {
                internal::raw_destructor(objectPointer);
            }
            // isolate_->AdjustAmountOfExternalAllocatedMemory(-static_cast<int64_t>(sizeof(ClassType)));
            // todo it->second.pobj.ClearWeak();
            // todo it->second.pobj.Reset();
            //  if (erase)
            //{
            objects_.erase(it);
            // }
        }
    }

  private:
    std::map<uint32_t, ConstructorFunctionType> constructorFunctionMap_;
};

class ClassRegistryManager
{
  public:
    template <typename ClassType> static ClassRegistry<ClassType> &getClassRegistry(type_info info)
    {
        auto it = classRegistryMap_.find(info.name().data());
        if (it != classRegistryMap_.end())
        {
            return *((ClassRegistry<ClassType> *)it->second);
        }
        auto item = new ClassRegistry<ClassType>();
        classRegistryMap_.insert(std::make_pair(info.name().data(), item));
        return *item;
    }
    template <typename ClassType> static void makeStrong(ClassType *objectPointer)
    {
        GET_ENV
        jsvm::Status status;
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        auto objectRegistry = classRegistry.getObjectRegistry(objectPointer);
        DEBUG_CHECK(objectRegistry != nullptr);
        uint32_t result;
        status = jsvm::ReferenceRef(env, objectRegistry->objectRef_, &result);
        DEBUG_CHECK(result > 0);
        DEBUG_CHECK(status == jsvm::Status::OK);
    }
    template <typename ClassType> static void makeWeak(ClassType *objectPointer)
    {
        GET_ENV
        jsvm::Status status;
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        auto objectRegistry = classRegistry.getObjectRegistry(objectPointer);
        DEBUG_CHECK(objectRegistry != nullptr);
        uint32_t result;
        status = jsvm::ReferenceUnref(env, objectRegistry->objectRef_, &result);
        DEBUG_CHECK(status == jsvm::Status::OK);
    }
    template <typename ClassType> static jsvm::Value wrapCppObject(ClassType *objectPointer, bool callDestructor = true)
    {
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        return classRegistry.wrapCppObject(objectPointer, callDestructor);
    }
    /*template <typename ClassType> static void removeObject(ClassType* objectPointer, bool callDestructor)
    {
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        classRegistry.removeObject(objectPointer, callDestructor);
    }*/
    template <typename ClassType> static bool isWrappedClassOf()
    {
        auto it = classRegistryMap_.find(type_id<ClassType>().name().data());
        if (it != classRegistryMap_.end())
        {
            return true;
        }
        return false;
    }
    static void cleanup()
    {
        auto it = classRegistryMap_.begin();
        for (; it != classRegistryMap_.end(); it++)
        {
            delete it->second;
        }
        classRegistryMap_.clear();
    }
    static std::unordered_map<std::string, ClassRegistryBase *> classRegistryMap_;
};
template <typename ClassType> static jsvm::Value New(jsvm::Env env, jsvm::CallbackInfo info)
{

    jsvm::Value newTarget;
    jsvm::GetNewTarget(env, info, &newTarget);
    // if (newTarget != nullptr)
    {
        // new MyObject(...)
        size_t argc = 16;
        jsvm::Value args[16];
        jsvm::Value jsThis;
        jsvm::GetCbInfo(env, info, &argc, args, &jsThis, nullptr);
        DEBUG_CHECK(argc <= 16);
        ClassRegistry<ClassType> &classRegistry =
            ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());
        // ClassType* object = classRegistry.ConstructObject(env, argc, args);
        ClassType *object = classRegistry.ConstructObject(argc, env, info);
        // v8::Global<v8::Object> pobj(isolate, obj);
        // pobj.SetWeak(this_, WeakCallback<ClassType>, v8::WeakCallbackType::kInternalFields);
        jsvm::Ref objectRef_;
        jsvm::Wrap(env, jsThis, reinterpret_cast<void *>(object), internal::destructor<ClassType>, nullptr,
                   &objectRef_);
        classRegistry.objects_.emplace(object, ObjectRegistry{objectRef_, true});
        return jsThis;
    }
#if 0
    else
    {
        // ʹ��`MyObject(...)`���÷�ʽ
        size_t argc = 1;
        napi_value args[1];
        napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

        napi_value cons;
        napi_get_reference_value(env, g_ref, &cons);
        napi_value instance;
        napi_new_instance(env, cons, argc, args, &instance);

        return instance;

        }
#endif
}
template <typename ClassType> class class_
{
  public:
    class_() : classRegistry_(ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>())) {};
    class_(class_ const &) = delete;
    class_ &operator=(class_ const &) = delete;

    class_(class_ &&) = default;
    class_ &operator=(class_ &&) = delete;
    ClassRegistry<ClassType> &classRegistry_;

    jsvm::Value ctor_;
    mutable std::vector<jsvm::PropertyDescriptor> propertyDescriptorVector_;

  public:
    template <typename... Args> class_ &constructor()
    {
        classRegistry_.registerConstructor(sizeof...(Args), internal::InvokeClassConstructor<ClassType, Args...>);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &function(const char *name, ReturnType (ClassType::*func)(Args...)) const
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeClassMethod<ClassType, ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::DEFAULT;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &function(const char *name, ReturnType (ClassType::*func)(Args...) const) const
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeClassMethod<ClassType, ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::DEFAULT;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &function_optional_override(const char *name, ReturnType (*func)(ClassType &, Args...)) const
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeClassMethodOptionalOverride<ClassType, ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::DEFAULT;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &class_function(const char *name, ReturnType (*func)(Args...)) const
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeMethodStatic<ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::STATIC;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    template <typename PropertyType>
    class_ &property(const char *name, PropertyType (ClassType::*get)(void),
                     void (ClassType::*set)(PropertyType value) = nullptr)
    {
        auto data = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([data]() { delete data; });
        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = internal::InvokeClassGetter<ClassType, PropertyType>;
        descriptor.setter = internal::InvokeClassSetter<ClassType, PropertyType>;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::DEFAULT;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    template <typename PropertyType>
    class_ &property_optional_override(const char *name, PropertyType (*get)(ClassType &),
                                       void (*set)(ClassType &, PropertyType value) = nullptr)
    {
        auto data = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([data]() { delete data; });
        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = internal::InvokeClassGetterOptionalOverride<ClassType, PropertyType>;
        descriptor.setter = internal::InvokeClassSetterOptionalOverride<ClassType, PropertyType>;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::DEFAULT;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    template <
        typename PropertyType /*, typename = typename std::enable_if<!std::is_function<PropertyType>::value>::type>*/>
    const class_ &property_field(const char *name, PropertyType ClassType::*field) const
    {
        auto data = new FuncInfo<decltype(field)>(field);
        internal::addDeinitializer([data]() { delete data; });
        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = internal::InvokeClassGetterField<ClassType, PropertyType>;
        descriptor.setter = internal::InvokeClassSetterField<ClassType, PropertyType>;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::DEFAULT;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    template <typename PropertyType>
    class_ &class_property(const char *name, PropertyType (*get)(void), void (*set)(PropertyType value) = nullptr)
    {
        auto data = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([data]() { delete data; });
        jsvm::PropertyDescriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = internal::InvokeClassGetterStatic<PropertyType>;
        descriptor.setter = internal::InvokeClassSetterStatic<PropertyType>;
        descriptor.value = NULL;
        descriptor.attributes = jsvm::PropertyAttributes::STATIC;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }

    template <typename BaseType> class_ &inherit()
    {
        static_assert(std::is_base_of<BaseType, ClassType>::value, "Class BaseType should be base for class ClassType");

        /*ClassRegistry<BaseType>& baseClassRegistry =
            ClassRegistryManager::getClassRegistry<BaseType>(type_id<BaseType>());
        classRegistry_.addBase(&baseClassRegistry);
        classRegistry_.js_function_template()->Inherit(baseClassRegistry.class_function_template());*/
        return *this;
    }
    jsvm::Value Register(jsvm::Env env, jsvm::Value exports, const char *className)
    {
        jsvm::Value cons;
        jsvm::DefineClass(env, className, NAPI_AUTO_LENGTH, New<ClassType>, propertyDescriptorVector_.size(),
                          propertyDescriptorVector_.data(), &cons);

        ClassRegistry<ClassType> &baseClassRegistry =
            ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());

        jsvm::CreateReference(env, cons, 1, &classRegistry_.classRef_);
        jsvm::SetNamedProperty(env, exports, className, cons);
        return exports;
    }
};
template <typename ClassType> jsvm::Value wrapCppObject(ClassType *objectPointer, bool callDestructor)
{
    return ClassRegistryManager::wrapCppObject<ClassType>(objectPointer, callDestructor);
}
template <typename ClassType> bool isWrappedClassOf()
{
    return ClassRegistryManager::isWrappedClassOf<ClassType>();
}
namespace internal
{
template <typename ClassType> static void destructor(jsvm::Env env, void *nativeObject, void * /*finalize_hint*/)
{
    ClassRegistry<ClassType> &classRegistry = ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());
    ClassType *object = static_cast<ClassType *>(nativeObject);
    DEBUG_CHECK(object != nullptr);
    classRegistry.removeObject(object);
}
} // namespace internal
} // namespace jsbind
#endif

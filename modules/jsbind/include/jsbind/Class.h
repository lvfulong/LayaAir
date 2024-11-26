#ifndef __JSBIND_CLASS_H__
#define __JSBIND_CLASS_H__

#include <assert.h>
#include <functional>
#include <jsbind/Error.h>
#include <jsbind/Local.h>
#include <jsbind/Utility.h>
#include <jsbind/internal/Invoke.h>
#include <jsvm/JSEnv.h>
#include <map>
#include <string>
#include <utils/Log.h>

namespace jsbind
{
template <typename ClassType> class ClassRegistry;
struct ObjectRegistry
{
    jsvm_ref objectRef_ = nullptr;
    bool callDestructor = true;
};
class ClassRegistryBase
{
  public:
    std::unordered_map<void *, ObjectRegistry> objects_;
    // std::vector<ClassRegistryBase *> bases_;
    // std::vector<ClassRegistryBase *> derivatives_;
    jsvm_ref classRef_ = nullptr;

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
    typedef ClassType *(*ConstructorFunctionType)(jsvm_env env, jsvm_callback_info info);
    ClassRegistry()
    {
    }
    virtual ~ClassRegistry()
    {
        GET_ENV
        auto it = objects_.begin();
        for (; it != objects_.end(); it++)
        {

            removeObjectRegistry(env, &it->second, (ClassType *)it->first);
        }
        objects_.clear();

        jsvm_delete_reference(env, classRef_);
    }

    void registerConstructor(uint32_t numPara, ConstructorFunctionType func)
    {
        assert(constructorFunctionMap_.find(numPara) == constructorFunctionMap_.end());
        constructorFunctionMap_.insert(std::make_pair(numPara, func));
    }
    ClassType *ConstructObject(uint32_t numPara, jsvm_env env, jsvm_callback_info info)
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
    jsvm_value wrapCppObject(ClassType *objectPointer, bool callDestructor)
    {
        GET_ENV
        jsvm_status status;
        // 这个会重复，直接返回
        auto it = objects_.find((void *)objectPointer);
        if (it != objects_.end())
        {
            jsvm_value obj;
            status = jsvm_get_reference_value(env, it->second.objectRef_, &obj);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            return obj;
        }

        jsvm_value cons;
        DEBUG_CHECK(classRef_ != nullptr);
        status = jsvm_get_reference_value(env, classRef_, &cons);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        this->isWrap_ = true;
        jsvm_value instance;
        status = jsvm_new_instance(env, cons, 0, nullptr, &instance);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        this->isWrap_ = false;

        jsvm_ref objectRef;
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        status = jsvm_wrap(env, instance, reinterpret_cast<void *>(objectPointer), internal::destructor<ClassType>,
                           nullptr, &objectRef);

        this->objects_.emplace(objectPointer, ObjectRegistry{objectRef, callDestructor});

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

    void removeObject(jsvm_env env, ClassType *objectPointer)
    {
        jsvm_status status;
        auto it = objects_.find((void *)objectPointer);
        // DEBUG_CHECK(it != objects_.end());
        if (it != objects_.end())
        {
            removeObjectRegistry(env, &it->second, objectPointer);
            objects_.erase(it);
        }
    }

    bool isWrap_ = false;

  private:
    void removeObjectRegistry(jsvm_env env, ObjectRegistry *registry, ClassType *objectPointer)
    {
        jsvm_status status;
        if (registry->callDestructor)
        {
            internal::raw_destructor(objectPointer);
            // isolate_->AdjustAmountOfExternalAllocatedMemory(-static_cast<int64_t>(sizeof(ClassType)));
            status = jsvm_delete_reference(env, registry->objectRef_);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
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
        jsvm_status status;
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        auto objectRegistry = classRegistry.getObjectRegistry(objectPointer);
        DEBUG_CHECK(objectRegistry != nullptr);
        uint32_t result;
        status = jsvm_reference_ref(env, objectRegistry->objectRef_, &result);
        DEBUG_CHECK(result > 0);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
    template <typename ClassType> static void makeWeak(ClassType *objectPointer)
    {
        GET_ENV
        jsvm_status status;
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        auto objectRegistry = classRegistry.getObjectRegistry(objectPointer);
        DEBUG_CHECK(objectRegistry != nullptr);
        uint32_t result;
        status = jsvm_reference_unref(env, objectRegistry->objectRef_, &result);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
    template <typename ClassType> static jsvm_value wrapCppObject(ClassType *objectPointer, bool callDestructor)
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
template <typename ClassType> static jsvm_value New(jsvm_env env, jsvm_callback_info info)
{
    jsvm_status status;
    jsvm_value newTarget;
    status = jsvm_get_new_target(env, info, &newTarget);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    DEBUG_CHECK(newTarget != nullptr);
    {

        ClassRegistry<ClassType> &classRegistry =
            ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());
        bool callDestructor = true;
        if (classRegistry.isWrap_)
        {
            jsvm_value jsThis;
            status = jsvm_get_cb_info(env, info, 0, nullptr, &jsThis, nullptr);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            return jsThis;
        }
        else
        {
            // new MyObject(...)
            size_t argc = 8;
            jsvm_value args[8];
            jsvm_value jsThis;
            status = jsvm_get_cb_info(env, info, &argc, args, &jsThis, nullptr);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            DEBUG_CHECK(argc <= 8);
            ClassType *object = classRegistry.ConstructObject(argc, env, info);
            jsvm_ref objectRef;

            status = jsvm_wrap(env, jsThis, reinterpret_cast<void *>(object), internal::destructor<ClassType>, nullptr,
                               &objectRef);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            classRegistry.objects_.emplace(object, ObjectRegistry{objectRef, callDestructor});
            return jsThis;
        }
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
template <typename ClassType> static jsvm_value NewGlobalClass(jsvm_env env, jsvm_callback_info info)
{
    // LOGI("jsvm NewGlobalClass %s", type_id<ClassType>().name().data());
    jsvm_value thisVar = nullptr;
    jsvm_get_cb_info(env, info, nullptr, nullptr, &thisVar, nullptr);

    return thisVar;
}
template <typename ClassType> class class_
{
  public:
    class_() : classRegistry_(ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>()))
    {
        mergedPropertyDescriptorMap_.clear();
    }
    ~class_()
    {
    }
    class_(class_ const &) = delete;
    class_ &operator=(class_ const &) = delete;

    class_(class_ &&) = default;
    class_ &operator=(class_ &&) = delete;
    ClassRegistry<ClassType> &classRegistry_;

    static std::unordered_map<const char *, jsvm_property_descriptor> mergedPropertyDescriptorMap_;
    mutable std::unordered_map<const char *, jsvm_property_descriptor> propertyDescriptorMap_;
    mutable jsvm_ref inheritBaseCons_ = nullptr;
    mutable std::unordered_map<const char *, jsvm_property_descriptor> propertyInheritBaseDescriptorMap_;

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
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeClassMethod<ClassType, ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = data;
        propertyDescriptorMap_.insert(std::make_pair(name, descriptor));
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &function(const char *name, ReturnType (ClassType::*func)(Args...) const) const
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeClassMethod<ClassType, ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::DEFAULT;
        descriptor.data = data;
        propertyDescriptorMap_.insert(std::make_pair(name, descriptor));
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &function_optional_override(const char *name, ReturnType (*func)(ClassType &, Args...)) const
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeClassMethodOptionalOverride<ClassType, ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = data;
        propertyDescriptorMap_.insert(std::make_pair(name, descriptor));
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &class_function(const char *name, ReturnType (*func)(Args...)) const
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeMethodStatic<ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_static;
        descriptor.data = data;
        propertyDescriptorMap_.insert(std::make_pair(name, descriptor));
        return *this;
    }
    template <typename PropertyType>
    class_ &property(const char *name, PropertyType (ClassType::*get)(void),
                     void (ClassType::*set)(PropertyType value) = nullptr)
    {
        auto data = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([data]() { delete data; });
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = internal::InvokeClassGetter<ClassType, PropertyType>;
        descriptor.setter = internal::InvokeClassSetter<ClassType, PropertyType>;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = data;
        propertyDescriptorMap_.insert(std::make_pair(name, descriptor));
        return *this;
    }
    template <typename PropertyType>
    class_ &property_optional_override(const char *name, PropertyType (*get)(ClassType &),
                                       void (*set)(ClassType &, PropertyType value) = nullptr)
    {
        auto data = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([data]() { delete data; });
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = internal::InvokeClassGetterOptionalOverride<ClassType, PropertyType>;
        descriptor.setter = internal::InvokeClassSetterOptionalOverride<ClassType, PropertyType>;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = data;
        propertyDescriptorMap_.insert(std::make_pair(name, descriptor));
        return *this;
    }
    template <
        typename PropertyType /*, typename = typename std::enable_if<!std::is_function<PropertyType>::value>::type>*/>
    const class_ &property_field(const char *name, PropertyType ClassType::*field) const
    {
        auto data = new FuncInfo<decltype(field)>(field);
        internal::addDeinitializer([data]() { delete data; });
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = internal::InvokeClassGetterField<ClassType, PropertyType>;
        descriptor.setter = internal::InvokeClassSetterField<ClassType, PropertyType>;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = data;
        propertyDescriptorMap_.insert(std::make_pair(name, descriptor));
        return *this;
    }
    template <typename PropertyType>
    class_ &class_property(const char *name, PropertyType (*get)(void), void (*set)(PropertyType value) = nullptr)
    {
        auto data = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([data]() { delete data; });
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = internal::InvokeClassGetterStatic<PropertyType>;
        descriptor.setter = internal::InvokeClassSetterStatic<PropertyType>;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::STATIC;
        descriptor.data = data;
        propertyDescriptorMap_.insert(std::make_pair(name, descriptor));
        return *this;
    }

    template <typename BaseType> class_ &inherit()
    {
        static_assert(std::is_base_of<BaseType, ClassType>::value, "Class BaseType should be base for class ClassType");

        ClassRegistry<BaseType> &baseClassRegistry =
            ClassRegistryManager::getClassRegistry<BaseType>(type_id<BaseType>());
        DEBUG_CHECK(inheritBaseCons_ == nullptr); // only inherit single base
        inheritBaseCons_ = baseClassRegistry.classRef_;
        DEBUG_CHECK(!class_<BaseType>::mergedPropertyDescriptorMap_.empty());
        propertyInheritBaseDescriptorMap_ = class_<BaseType>::mergedPropertyDescriptorMap_;
        return *this;
    }
    jsvm_value registerClass(jsvm_env env, jsvm_value exports, const char *className)
    {
        jsvm_status status;
        jsvm_value cons;

        // merge start
        for (const auto &pair : propertyInheritBaseDescriptorMap_)
        {
            mergedPropertyDescriptorMap_.insert(pair);
        }
        for (const auto &pair : propertyDescriptorMap_)
        {
            mergedPropertyDescriptorMap_.insert(pair);
        }

        std::vector<jsvm_property_descriptor> propertyDescriptorVector;
        propertyDescriptorVector.reserve(mergedPropertyDescriptorMap_.size());
        for (const auto &pair : mergedPropertyDescriptorMap_)
        {
            propertyDescriptorVector.push_back(pair.second);
        }
        // merge end

        status = jsvm_define_class(env, className, NAPI_AUTO_LENGTH, New<ClassType>, propertyDescriptorVector.size(),
                                   propertyDescriptorVector.data(), &cons);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        status = jsvm_create_reference(env, cons, 1, &classRegistry_.classRef_);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        status = jsvm_set_named_property(env, exports, className, cons);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        if (inheritBaseCons_ != nullptr)
        {
            jsvm_value consBase;
            status = jsvm_get_reference_value(env, inheritBaseCons_, &consBase);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            jsvm_value consBasePrototype = Local(consBase)["prototype"].getHandle();

            jsvm_value consDerived = cons;
            jsvm_value consDerivedPrototype = Local(consDerived)["prototype"].getHandle();

            Local global(jsvm::global());
            Local setPrototypeOf = global["Object"]["setPrototypeOf"];

            setPrototypeOf.call<void>(jsvm::global(), consDerived, consBase);
            setPrototypeOf.call<void>(jsvm::global(), consDerivedPrototype, consBasePrototype);
#if 0 
            
            status = jsvm::ObjectGetPrototypeOf(env, consBase, &consBasePrototype);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);

            status = jsvm::ObjectGetPrototypeOf(env, consDerived, &consDerivedPrototype);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok); 
            status = jsvm::ObjectSetPrototypeOf(env, consDerived, consBase);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);

            status = jsvm::ObjectSetPrototypeOf(env, consDerivedPrototype, conBasePrototype);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
#endif
            // classRegistry_.addBase(&baseClassRegistry);//todo
        }
        return exports;
    }
};

template <typename ClassType> class global_class_
{
  public:
    global_class_() : classRegistry_(ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>())) {};
    global_class_(global_class_ const &) = delete;
    global_class_ &operator=(global_class_ const &) = delete;

    global_class_(global_class_ &&) = default;
    global_class_ &operator=(global_class_ &&) = delete;
    ClassRegistry<ClassType> &classRegistry_;
    mutable std::vector<jsvm_property_descriptor> propertyDescriptorVector_;

  public:
    template <typename ReturnType, typename... Args>
    const global_class_ &class_function(const char *name, ReturnType (*func)(Args...)) const
    {
        FuncInfo<decltype(func)> *data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = internal::InvokeMethodStatic<ReturnType, Args...>;
        descriptor.getter = NULL;
        descriptor.setter = NULL;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }
    template <typename PropertyType>
    global_class_ &class_property(const char *name, PropertyType (*get)(void),
                                  void (*set)(PropertyType value) = nullptr)
    {
        auto data = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([data]() { delete data; });
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = NULL;
        descriptor.getter = internal::InvokeClassGetterStatic<PropertyType>;
        descriptor.setter = internal::InvokeClassSetterStatic<PropertyType>;
        descriptor.value = NULL;
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
        descriptor.data = data;
        propertyDescriptorVector_.push_back(descriptor);
        return *this;
    }

    jsvm_value registerClass(jsvm_env env, jsvm_value exports, const char *className)
    {
        jsvm_status status;
        jsvm_value cons;

        status = jsvm_define_class(env, className, NAPI_AUTO_LENGTH, NewGlobalClass<ClassType>,
                                   propertyDescriptorVector_.size(), propertyDescriptorVector_.data(), &cons);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        ClassRegistry<ClassType> &baseClassRegistry =
            ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());

        jsvm_value instanceValue = nullptr;
        status = jsvm_new_instance(env, cons, 0, nullptr, &instanceValue);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        status = jsvm_set_named_property(env, exports, className, instanceValue);
        // status = jsvm_set_named_property(env, exports, className, cons);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        return exports;
    }
};
template <typename ClassType> jsvm_value wrapCppObject(ClassType *objectPointer, bool callDestructor)
{
    return ClassRegistryManager::wrapCppObject<ClassType>(objectPointer, callDestructor);
}
template <typename ClassType> bool isWrappedClassOf()
{
    return ClassRegistryManager::isWrappedClassOf<ClassType>();
}

template <typename ClassType>
std::unordered_map<const char *, jsvm_property_descriptor> class_<ClassType>::mergedPropertyDescriptorMap_;
namespace internal
{
template <typename ClassType> static void destructor(jsvm_env env, void *nativeObject, void * /*finalize_hint*/)
{
    ClassRegistry<ClassType> &classRegistry = ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());
    ClassType *object = static_cast<ClassType *>(nativeObject);
    DEBUG_CHECK(object != nullptr);
    classRegistry.removeObject(env, object);
}
} // namespace internal
} // namespace jsbind
#endif

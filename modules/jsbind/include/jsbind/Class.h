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
#include <memory>

namespace jsbind
{
struct ObjectRegistry
{
    jsvm_ref objectRef_ = nullptr;
};
class ClassRegistryBase
{
  public:
    jsvm_ref classRef_ = nullptr;
    bool isWrap_ = false;
  public:
    virtual ~ClassRegistryBase()
    {
    }
    virtual ObjectRegistry* getObjectRegistry(void* objectPointer) = 0;
};
template <typename ClassType, typename Traits> class ClassRegistry : public ClassRegistryBase
{
  public:
    using pointer_type = typename Traits::pointer_type;
    using object_id = typename Traits::object_id;
    using object_pointer_type = typename Traits::template object_pointer_type<ClassType>;
    using ConstructorFunctionType = object_pointer_type(*)(jsvm_env env, jsvm_callback_info info);
    ClassRegistry()
    {
    }
    virtual ~ClassRegistry()
    {
        GET_ENV
        auto it = objects_.begin();
        for (; it != objects_.end(); it++)
        {

            removeObjectRegistry(env, it->second.get(), it->first);
        }
        objects_.clear();

        jsvm_delete_reference(env, classRef_);
    }

    void registerConstructor(uint32_t numPara, ConstructorFunctionType func)
    {
        assert(constructorFunctionMap_.find(numPara) == constructorFunctionMap_.end());
        constructorFunctionMap_.insert(std::make_pair(numPara, func));
    }
    object_pointer_type ConstructObject(uint32_t numPara, jsvm_env env, jsvm_callback_info info)
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
    jsvm_value wrapCppObject(object_id objectPointer, bool callDestructor)
    {
        GET_ENV
        jsvm_status status;
        // 这个会重复，直接返回
        auto it = objects_.find(Traits::to_pointer_type(objectPointer));
        if (it != objects_.end())
        {
            jsvm_value obj;
            status = jsvm_get_reference_value(env, it->second->objectRef_, &obj);
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
        status = jsvm_wrap(env, instance, reinterpret_cast<void *>(objectPointer), 
                          static_cast<jsvm_finalize>(internal::destructor<ClassType, Traits>),
                          nullptr, &objectRef);

        this->objects_.emplace(Traits::to_pointer_type(objectPointer), std::make_unique<ObjectRegistry>(ObjectRegistry{objectRef}));

        return instance;
    }
    object_pointer_type unwrapCppObject(jsvm_value value)
    {
        DEBUG_CHECK(value != nullptr);
        GET_ENV
        jsvm_status status;
        jsvm_valuetype valueType;
        status = jsvm_typeof(env, value, &valueType);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        DEBUG_CHECK(valueType == jsvm_valuetype::jsvm_object)
        //if (valueType != jsvm_valuetype::jsvm_object)
        //{
        //    return nullptr;
        //}
        ClassType *obj;
        status = jsvm_unwrap(env, value, (void **)(&obj));
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        auto it = objects_.find(Traits::to_pointer_type(obj));
        if (it != objects_.end())
        {
            return Traits::template static_pointer_cast<ClassType>(it->first);
        }
        return nullptr;
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

    void removeObject(jsvm_env env, object_id objectPointer)
    {
        jsvm_status status;
        auto it = objects_.find(Traits::to_pointer_type(objectPointer));
        // DEBUG_CHECK(it != objects_.end());
        if (it != objects_.end())
        {
            removeObjectRegistry(env, it->second.get(), it->first);
            objects_.erase(it);
        }
    }

    ObjectRegistry* getObjectRegistry(object_id objectPointer) override
    {
        auto it = objects_.find(Traits::to_pointer_type(objectPointer));
        if (it != objects_.end())
        {
            return it->second.get();
        }

        /*for (auto const info : derivatives_)
         {
             ObjectRegistry *result = info->getObjectRegistry(objectPointer);
             if (result != nullptr)
                 return result;
         }*/

        return nullptr;
    }
  private:
    void removeObjectRegistry(jsvm_env env, ObjectRegistry *registry, const pointer_type& objectPointer)
    {
        jsvm_status status;
        {
            if constexpr (std::is_same_v<Traits, jsbind::raw_ptr_traits>) 
            {
                internal::raw_destructor(static_cast<ClassType*>(objectPointer));
            } 
            else 
            {
                //objectPointer.reset();
            }
            // isolate_->AdjustAmountOfExternalAllocatedMemory(-static_cast<int64_t>(sizeof(ClassType)));
            // all refs delete in DeleteMe()
            //status = jsvm_delete_reference(env, registry->objectRef_);
            //DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        }
    }

  public:
      friend class ClassRegistryManager;
      std::map<uint32_t, ConstructorFunctionType> constructorFunctionMap_;
      std::unordered_map<pointer_type, std::unique_ptr<ObjectRegistry>> objects_;
      // std::vector<ClassRegistryBase *> bases_;
      // std::vector<ClassRegistryBase *> derivatives_;
};

class ClassRegistryManager
{
  public:
    
    static ClassRegistryBase* getClassRegistry(type_info info)
    {
        auto it = classRegistryMap_.find(info.name().data());
        if (it != classRegistryMap_.end())
        {
            return it->second;
        }
        return nullptr;
    }
    static void addClassRegistry(type_info info, ClassRegistryBase* classRegistry)
    {
        classRegistryMap_.insert(std::make_pair(info.name().data(), classRegistry));
    }
    template <typename ClassType> static void makeStrong(ClassType *objectPointer)
    {
        GET_ENV
        jsvm_status status;
        ClassRegistryBase* classRegistry = getClassRegistry(type_id<ClassType>());
        auto objectRegistry = classRegistry->getObjectRegistry(objectPointer);
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
        ClassRegistryBase* classRegistry = getClassRegistry(type_id<ClassType>());
        auto objectRegistry = classRegistry->getObjectRegistry(objectPointer);
        DEBUG_CHECK(objectRegistry != nullptr);
        uint32_t result;
        status = jsvm_reference_unref(env, objectRegistry->objectRef_, &result);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
    template <typename ClassType, typename Traits> static jsvm_value wrapCppObject(ClassType *objectPointer, bool callDestructor)
    {
        ClassRegistry<ClassType, Traits>* classRegistry = static_cast<ClassRegistry<ClassType, Traits>*>(getClassRegistry(type_id<ClassType>()));
        return classRegistry->wrapCppObject(objectPointer, callDestructor);
    }
    template <typename ClassType, typename Traits> static typename Traits::template object_pointer_type<ClassType> unwrapCppObject(jsvm_value value)
    {
        ClassRegistry<ClassType, Traits>* classRegistry = static_cast<ClassRegistry<ClassType, Traits>*>(getClassRegistry(type_id<ClassType>()));
        return classRegistry->unwrapCppObject(value);
    }
    template <typename ClassType, typename Traits> static void removeObject(ClassType* objectPointer)
    {
        GET_ENV
        ClassRegistry<ClassType, Traits>* classRegistry = static_cast<ClassRegistry<ClassType, Traits>*>(getClassRegistry(type_id<ClassType>()));
        classRegistry->removeObject(env, Traits::pointer_id(objectPointer));
    }
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
template <typename ClassType, typename Traits> static jsvm_value New(jsvm_env env, jsvm_callback_info info)
{
    jsvm_status status;
    jsvm_value newTarget;
    status = jsvm_get_new_target(env, info, &newTarget);
    DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    DEBUG_CHECK(newTarget != nullptr);
    {

        ClassRegistry<ClassType, Traits> *classRegistry =
            static_cast<ClassRegistry<ClassType, Traits>*>(ClassRegistryManager::getClassRegistry(type_id<ClassType>()));
        bool callDestructor = true;
        if (classRegistry->isWrap_)
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
            typename Traits::pointer_type object = classRegistry->ConstructObject(argc, env, info);
            jsvm_ref objectRef;

            status = jsvm_wrap(env, jsThis, Traits::pointer_id(object), 
                              static_cast<jsvm_finalize>(internal::destructor<ClassType, Traits>), nullptr,
                              &objectRef);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            classRegistry->objects_.emplace(object, std::make_unique<ObjectRegistry>(ObjectRegistry{objectRef}));
            return jsThis;
        }
    }
#if 0
    else
    {
        // ʹ`MyObject(...)`÷ʽ
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
template <typename ClassType, typename Traits = raw_ptr_traits> class class_
{
  public:
    class_()
    {
        classRegistry_ = new ClassRegistry<ClassType, Traits>();
        DEBUG_CHECK(ClassRegistryManager::getClassRegistry(type_id<ClassType>()) == nullptr);
        ClassRegistryManager::addClassRegistry(type_id<ClassType>(), classRegistry_);
        mergedPropertyDescriptorMap_.clear();
    }
    ~class_()
    {
    }
    class_(class_ const &) = delete;
    class_ &operator=(class_ const &) = delete;

    class_(class_ &&) = default;
    class_ &operator=(class_ &&) = delete;
    ClassRegistry<ClassType, Traits> *classRegistry_;

    static std::unordered_map<const char *, jsvm_property_descriptor> mergedPropertyDescriptorMap_;
    mutable std::unordered_map<const char *, jsvm_property_descriptor> propertyDescriptorMap_;
    mutable jsvm_ref inheritBaseCons_ = nullptr;
    mutable std::unordered_map<const char *, jsvm_property_descriptor> propertyInheritBaseDescriptorMap_;

  public:
    template <typename... Args> class_ &constructor()
    {
        classRegistry_->registerConstructor(sizeof...(Args), internal::InvokeClassConstructor<ClassType, Traits, Args...>);
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
        descriptor.attributes = jsvm_property_attributes::jsvm_default;
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
        descriptor.attributes = jsvm_property_attributes::jsvm_static;
        descriptor.data = data;
        propertyDescriptorMap_.insert(std::make_pair(name, descriptor));
        return *this;
    }

    template <typename BaseType> class_ &inherit()
    {
        static_assert(std::is_base_of<BaseType, ClassType>::value, "Class BaseType should be base for class ClassType");

        ClassRegistryBase* baseClassRegistry =
            ClassRegistryManager::getClassRegistry(type_id<BaseType>());
        DEBUG_CHECK(inheritBaseCons_ == nullptr); // only inherit single base
        inheritBaseCons_ = baseClassRegistry->classRef_;
        //DEBUG_CHECK(!class_<BaseType>::mergedPropertyDescriptorMap_.empty());
        propertyInheritBaseDescriptorMap_ = class_<BaseType>::mergedPropertyDescriptorMap_;
        return *this;
    }
    jsvm_value Export(jsvm_env env, jsvm_value exports, const char *className)
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

        status = jsvm_define_class(env, className, JSVM_AUTO_LENGTH, New<ClassType, Traits>, propertyDescriptorVector.size(),
                                   propertyDescriptorVector.data(), &cons);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        status = jsvm_create_reference(env, cons, 1, &classRegistry_->classRef_);
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

template <typename ClassType, typename Traits = raw_ptr_traits> class global_class_
{
  public:
    global_class_()
    {
        classRegistry_ = new ClassRegistry<ClassType, Traits>();
        DEBUG_CHECK(ClassRegistryManager::getClassRegistry(type_id<ClassType>()) == nullptr);
        ClassRegistryManager::addClassRegistry(type_id<ClassType>(), classRegistry_);
    }
    global_class_(global_class_ const &) = delete;
    global_class_ &operator=(global_class_ const &) = delete;

    global_class_(global_class_ &&) = default;
    global_class_ &operator=(global_class_ &&) = delete;
    ClassRegistry<ClassType, Traits> *classRegistry_;
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
    const global_class_& class_function_raw(const char* name, jsvm_value(*func)(jsvm_env env, jsvm_callback_info info)) const
    {
        FuncInfo<decltype(func)>* data = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([data]() { delete data; });
        data->name = name;
        jsvm_property_descriptor descriptor;
        descriptor.utf8name = name;
        descriptor.name = NULL;
        descriptor.method = func;
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

    jsvm_value Export(jsvm_env env, jsvm_value exports, const char *className)
    {
        jsvm_status status;
        jsvm_value cons;

        status = jsvm_define_class(env, className, JSVM_AUTO_LENGTH, NewGlobalClass<ClassType>,
                                   propertyDescriptorVector_.size(), propertyDescriptorVector_.data(), &cons);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        jsvm_value instanceValue = nullptr;
        status = jsvm_new_instance(env, cons, 0, nullptr, &instanceValue);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        status = jsvm_set_named_property(env, exports, className, instanceValue);
        // status = jsvm_set_named_property(env, exports, className, cons);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);

        return exports;
    }
};
template <typename ClassType, typename Traits> jsvm_value wrapCppObject(ClassType *objectPointer, bool callDestructor)
{
    return ClassRegistryManager::wrapCppObject<ClassType, Traits>(objectPointer, callDestructor);
}
template <typename ClassType, typename Traits> typename Traits::template object_pointer_type<ClassType> unwrapCppObject(jsvm_value value)
{
    return ClassRegistryManager::unwrapCppObject<ClassType, Traits>(value);
}
template <typename ClassType> bool isWrappedClassOf()
{
    return ClassRegistryManager::isWrappedClassOf<ClassType>();
}

template <typename ClassType, typename Traits>
std::unordered_map<const char *, jsvm_property_descriptor> class_<ClassType, Traits>::mergedPropertyDescriptorMap_;
namespace internal
{
template <typename ClassType, typename Traits> static void destructor(jsvm_env env, void *nativeObject, void * /*finalize_hint*/)
{
    ClassRegistry<ClassType, Traits> *classRegistry = static_cast<ClassRegistry<ClassType, Traits>*>(ClassRegistryManager::getClassRegistry(type_id<ClassType>()));
    ClassType *object = static_cast<ClassType *>(nativeObject);
    DEBUG_CHECK(object != nullptr);
    classRegistry->removeObject(env, object);
}
} // namespace internal
} // namespace jsbind
#endif

#ifndef __LayaConchPhysXAdapter_H__
#define __LayaConchPhysXAdapter_H__

#include "../../JCScriptRuntime.h"
#include <jsbind/JSBind.h>
#include <Bindings/JSRuntime.h>
#include "PxPhysicsAPI.h"
#include <JCConch.h>

namespace laya
{

struct NoBaseClass
{
    using baseType = void;
};
template <typename BaseClass> struct base
{

    using baseType = BaseClass;
};

namespace internal
{
template <typename WrapperType, typename ClassType /*, typename... */> WrapperType wrapped_new(jsvm_value args)
{
    return WrapperType(new ClassType(args));
}

} // namespace internal
template <typename T> class VectorWrapper
{
  public:
    void push_back(const T &value)
    {
        data_.push_back(value);
    }
    void resize(uint32_t size /*, const T &*/)
    {
        data_.resize(size);
    }
    uint32_t size() const
    {
        return data_.size();
    }

    jsvm_value get(uint32_t index)
    {
        if (index < data_.size())
        {
            return jsbind::Make<T>(data_[index]);
        }
        else
        {
            return jsbind::makeUndefined();
        }
    }

    bool set(uint32_t index, const T &value)
    {
        data_[index] = value;
        return true;
    }
    std::vector<T> data_;
};

int allow_raw_pointers();
class PhysxAdapter;
template <typename ClassType, typename BaseSpecifier = NoBaseClass> class Class_
{
  public:
    Class_(const char * name, PhysxAdapter *adapter);
    Class_(Class_ const &) = delete;
    Class_ &operator=(Class_ const &) = delete;

    Class_(Class_ &&) = default;
    Class_ &operator=(Class_ &&) = delete;

    ~Class_();
    template <typename... Args> Class_ &constructor()
    {
        mclass_->template constructor<Args...>();
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const Class_ &function(const char * name, ReturnType (ClassType::*func)(Args...) const,
                           int dummy = allow_raw_pointers()) const
    {
        mclass_->template function(name, func);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const Class_ &function(const char * name, ReturnType (ClassType::*func)(Args...),
                           int dummy = allow_raw_pointers()) const
    {
        mclass_->template function(name, func);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const Class_ &function_optional_override(const char * name, ReturnType (*func)(ClassType &, Args...),
                                             int dummy = allow_raw_pointers()) const
    {
        mclass_->template function_optional_override(name, func);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    Class_ &class_function(const char * name, ReturnType (*func)(Args...), int dummy = allow_raw_pointers())
    {
        mclass_->class_function(name, func);
        return *this;
    }
    template <typename PropertyType>
    Class_ &property(const char * name, PropertyType (ClassType::*get)(void),
                     void (ClassType::*set)(PropertyType data) = nullptr)
    {
        mclass_->property(name, get, set);
        return *this;
    }
    template <
        typename PropertyType /*, typename = typename std::enable_if<!std::is_function<PropertyType>::value>::type>*/>
    const Class_ &property(const char * name, PropertyType ClassType::*field) const
    {
        mclass_->property_field(name, field);
        return *this;
    }
    template <typename WrapperType>
    const Class_<WrapperType, base<ClassType>> allow_subclass(const char *wrapperClassName) const
    {

        Class_<WrapperType, base<ClassType>> c(wrapperClassName, adapter_);
        mclass_->class_function("implement", &internal::wrapped_new<WrapperType *, WrapperType>);
        return c;
    }

  public:
    jsbind::class_<ClassType>* mclass_;
    const char* name_;
    PhysxAdapter *adapter_;
};
class PhysxAdapter
{
  public:
    PhysxAdapter() : module_(jsbind::makeObject())
    {
    }
    template <typename Value> jsbind::Object &constant(const char * name, Value const &value)
    {
        return module_.constant(name, value);
    }
    template <typename ReturnType, typename... Args>
    void function(const char * name, ReturnType (*func)(Args...), int dummy = allow_raw_pointers())
    {
        module_.function(name, func);
    }
    template <typename ClassType, typename BaseSpecifier = NoBaseClass>
    Class_<ClassType, BaseSpecifier> class_(const char * name)
    {
        Class_<ClassType, BaseSpecifier> c(name, this);
        return c;
    }
    template <typename EnumType> jsbind::Enum_ enum_(const char * name)
    {
        return module_.enum_<EnumType>(name);
    }
    template <typename T> Class_<VectorWrapper<T>> register_vector(const char *name)
    {
        typedef VectorWrapper<T> VectorWrapperType;
        Class_<VectorWrapperType> c(name, this);
        c.template constructor<>()
            .function("push_back", &VectorWrapperType::push_back)
            .function("resize", &VectorWrapperType::resize)
            .function("size", &VectorWrapperType::size)
            .function("get", &VectorWrapperType::get)
            .function("set", &VectorWrapperType::set);

        return c;
    }
    void registerFunction(jsbind::type_info info, std::function<void()> callback)
    {
        auto it = functions_.find(info.name().data());
        if (it != functions_.end())
        {
            DEBUG_CHECK(false && "only once");
        }
        functions_.insert(std::make_pair(info.name().data(), callback));
    }

    void unregisterFunction(jsbind::type_info info)
    {
        auto it = functions_.find(info.name().data());
        if (it != functions_.end())
        {
            functions_.erase(it);
        }
    }
    void exportAll()
    {
        for (auto &kv : functions_)
        {
            kv.second();
        }
    }
    virtual ~PhysxAdapter()
    {
    }

  public:
    std::unordered_map<const char *, std::function<void()>> functions_;
    jsbind::Object module_;
};

template <typename ClassType, typename BaseSpecifier> Class_<ClassType, BaseSpecifier>::~Class_()
{
    if constexpr (std::is_same_v<BaseSpecifier, NoBaseClass>)
    {
    }
    else
    {
        assert(jsbind::ClassRegistryManager::isWrappedClassOf<typename BaseSpecifier::baseType>());
        mclass_->template inherit<typename BaseSpecifier::baseType>();
        // delete base info , if not will crash, because base don not need bind
        //adapter_->unregisterFunction(jsbind::type_id<typename BaseSpecifier::baseType>());
    }

    mclass_->function_optional_override("delete",
                                        jsbind::optional_override([](ClassType &THIS) { jsbind::ClassRegistryManager::removeObject<ClassType, jsbind::raw_ptr_traits>(&THIS); }));

    jsbind::ClassRegistry<ClassType, jsbind::raw_ptr_traits> *classRegistry = 
        static_cast<jsbind::ClassRegistry<ClassType, jsbind::raw_ptr_traits>*>(
            jsbind::ClassRegistryManager::getClassRegistry(jsbind::type_id<ClassType>())
        );

    jsbind::Object *m = &adapter_->module_;
    auto className = this->name_;
    
    GET_ENV
    mclass_->Export(env, m->getHandle(), className);
    delete mclass_;
}
template <typename ClassType, typename BaseSpecifier>
Class_<ClassType, BaseSpecifier>::Class_(const char * name, PhysxAdapter *adapter)
    : mclass_(new jsbind::class_<ClassType>()), name_(name), adapter_(adapter){};

namespace internal
{

class WrapperBase
{
  public:
  protected:
};

} // namespace internal

template <typename T> class wrapper : public T, public internal::WrapperBase
{
  public:
    typedef T class_type;

    template <typename... Args> explicit wrapper(jsvm_value wrapped, Args &&...args) : T(std::forward<Args>(args)...)
    {
        wrapped_.reset(wrapped); 
    }

    ~wrapper()
    {
    }

    template <typename ReturnType, typename... Args> ReturnType call(const char *name, Args &&...args) const
    {
        return wrapped_.call<ReturnType>(name, std::forward<Args>(args)...);
    }

  private:
    jsbind::Persistent wrapped_;
};
#define EMSCRIPTEN_WRAPPER(T)                                                                                          \
    template <typename... Args> T(jsvm_value v, Args &&...args) : wrapper(v, std::forward<Args>(args)...)                 \
    {                                                                                                                  \
    }

#define EMSCRIPTEN_BINDINGS(name) void PhysxAdapter_::exportJS_##name()

#define EMSCRIPTEN_BINDINGS_DEFINE_EXPORT_FUNCTION(name) void exportJS_##name();

#define EMSCRIPTEN_BINDINGS_EXPORT_JS_START PhysxAdapter_ binding_;

#define EMSCRIPTEN_BINDINGS_EXPORT_JS(name) binding_.exportJS_##name();

#define EMSCRIPTEN_BINDINGS_EXPORT_JS_END  \
    GET_ENV                                \
    binding_.exportAll();                  \
    binding_.module_.Export(env, context.getHandle(), "physx");

class PhysxAdapter_ : public PhysxAdapter
{
  public:
    PhysxAdapter_() : PhysxAdapter()
    {
    }
    EMSCRIPTEN_BINDINGS_DEFINE_EXPORT_FUNCTION(physx)
    EMSCRIPTEN_BINDINGS_DEFINE_EXPORT_FUNCTION(physx_math)
    EMSCRIPTEN_BINDINGS_DEFINE_EXPORT_FUNCTION(physx_actor)
    EMSCRIPTEN_BINDINGS_DEFINE_EXPORT_FUNCTION(physx_controller)
    EMSCRIPTEN_BINDINGS_DEFINE_EXPORT_FUNCTION(physx_cooking)
    EMSCRIPTEN_BINDINGS_DEFINE_EXPORT_FUNCTION(physx_joint)
    EMSCRIPTEN_BINDINGS_DEFINE_EXPORT_FUNCTION(physx_scene)
    EMSCRIPTEN_BINDINGS_DEFINE_EXPORT_FUNCTION(physx_shape)
};
} // namespace laya

/*namespace laya
{

template <> class ValueTraits<physx::PxRigidBodyFlag::Enum>
{
  public:
    static physx::PxRigidBodyFlag::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxRigidBodyFlag::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxRigidBodyFlag::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};

template <> class ValueTraits<physx::PxActorFlag::Enum>
{
  public:
    static physx::PxActorFlag::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxActorFlag::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxActorFlag::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};

template <> class ValueTraits<physx::PxRigidDynamicLockFlag::Enum>
{
  public:
    static physx::PxRigidDynamicLockFlag::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxRigidDynamicLockFlag::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxRigidDynamicLockFlag::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};

template <> class ValueTraits<physx::PxCapsuleClimbingMode::Enum>
{
  public:
    static physx::PxCapsuleClimbingMode::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxCapsuleClimbingMode::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxCapsuleClimbingMode::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};

template <> class ValueTraits<physx::PxControllerCollisionFlag::Enum>
{
  public:
    static physx::PxControllerCollisionFlag::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxControllerCollisionFlag::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxControllerCollisionFlag::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};

template <> class ValueTraits<physx::PxControllerShapeType::Enum>
{
  public:
    static physx::PxControllerShapeType::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxControllerShapeType::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxControllerShapeType::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};
template <> class ValueTraits<physx::PxHeightFieldFormat::Enum>
{
  public:
    static physx::PxHeightFieldFormat::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxHeightFieldFormat::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxHeightFieldFormat::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};

template <> class ValueTraits<physx::PxDefaultCpuDispatcherWaitForWorkMode::Enum>
{
  public:
    static physx::PxDefaultCpuDispatcherWaitForWorkMode::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxDefaultCpuDispatcherWaitForWorkMode::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxDefaultCpuDispatcherWaitForWorkMode::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};

template <> class ValueTraits<physx::PxShapeFlag::Enum>
{
  public:
    static physx::PxShapeFlag::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxShapeFlag::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxShapeFlag::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};
template <> class ValueTraits<physx::PxCombineMode::Enum>
{
  public:
    static physx::PxCombineMode::Enum ToCpp(v8::Local<v8::Value> p_vl)
    {
        return (physx::PxCombineMode::Enum)p_vl.As<v8::Int32>()->Value();
    }
    static v8::Local<v8::Value> ToJs(physx::PxCombineMode::Enum p_vl)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), p_vl);
    }

    static bool is(v8::Local<v8::Value> p_vl)
    {
        return p_vl->IsInt32();
    }
};

} // namespace laya*/
#endif

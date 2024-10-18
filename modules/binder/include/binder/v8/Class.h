#ifndef __V8_CLASS_H__
#define __V8_CLASS_H__

#include "Invocation.h"
#include "Utility.h"
#include "Value.h"
#include <assert.h>
#include <functional>
#include <map>
#include <string>
#include <utils/Log.h>
#include <utils/RTTI.h>
#include <v8.h>

namespace binder
{
namespace internal
{
template <typename ClassType> void raw_destructor(ClassType *pointer)
{
    delete pointer;
}
extern void addDeinitializer(std::function<void()> func);
extern void runDeinitializers();
template <typename T> struct remove_class;
template <typename C, typename R, typename... A> struct remove_class<R (C::*)(A...)>
{
    using type = R(A...);
};
template <typename C, typename R, typename... A> struct remove_class<R (C::*)(A...) const>
{
    using type = R(A...);
};
template <typename C, typename R, typename... A> struct remove_class<R (C::*)(A...) volatile>
{
    using type = R(A...);
};
template <typename C, typename R, typename... A> struct remove_class<R (C::*)(A...) const volatile>
{
    using type = R(A...);
};

template <typename LambdaType> using LambdaSignature = typename remove_class<decltype(&LambdaType::operator())>::type;
} // end namespace internal

// requires captureless lambda because implicitly coerces to function pointer
template <typename LambdaType> internal::LambdaSignature<LambdaType> *optional_override(const LambdaType &fp)
{
    return fp;
}

template <typename Signature> Signature *select_overload(Signature *fn)
{
    return fn;
}

template <typename Signature, typename ClassType> auto select_overload(Signature(ClassType::*fn)) -> decltype(fn)
{
    return fn;
}

template <typename ClassType, typename ReturnType, typename... Args>
auto select_const(ReturnType (ClassType::*method)(Args...) const) -> decltype(method)
{
    return method;
}

template <typename ClassType> class ClassRegistry;
template <typename ClassType> static void WeakCallback(const v8::WeakCallbackInfo<ClassRegistry<ClassType>> &data);
struct ObjectRegistry
{
    v8::Global<v8::Object> pobj;
    bool callDestructor = true;
};
class ClassRegistryBase
{
  protected:
    std::unordered_map<void *, ObjectRegistry> objects_;
    std::vector<ClassRegistryBase *> bases_;
    std::vector<ClassRegistryBase *> derivatives_;
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

        for (auto const info : derivatives_)
        {
            ObjectRegistry *result = info->getObjectRegistry(objectPointer);
            if (result != nullptr)
                return result;
        }

        return nullptr;
    }
};
template <typename ClassType> class ClassRegistry : public ClassRegistryBase
{
  public:
    typedef ClassType *(*ConstructorFunctionType)(const v8::FunctionCallbackInfo<v8::Value> &info);
    ClassRegistry(jsvm::Env env) : env_(env) {}
    virtual ~ClassRegistry()
    {
        auto it = objects_.begin();
        for (; it != objects_.end(); it++)
        {
            v8::HandleScope scope(isolate_);

            if (it->second.callDestructor)
            {
                internal::raw_destructor((ClassType *)it->first);
            }

            it->second.pobj.ClearWeak();
            it->second.pobj.Reset();
        }
        objects_.clear();

        //func_.Reset();
        //js_func_.Reset();
        jsvm::DeleteReference(env_, classRef_);
    }

    void registerConstructor(uint32_t numPara, ConstructorFunctionType func)
    {
        assert(constructorFunctionMap_.find(numPara) == constructorFunctionMap_.end());
        constructorFunctionMap_.insert(std::make_pair(numPara, func));
    }
    ClassType *ConstructObject(uint32_t numPara, const v8::FunctionCallbackInfo<v8::Value> &info)
    {
        auto it = constructorFunctionMap_.find(numPara);
        if (it != constructorFunctionMap_.end())
        {
            info.GetIsolate()->AdjustAmountOfExternalAllocatedMemory(static_cast<int64_t>(sizeof(ClassType)));
            return (it->second)(info);
        }
        /*else
        {
            return new ClassType();
        }*/
        assert(true);
        return nullptr;
    }

    jsvm::Value wrapCppObject(ClassType *objectPointer, bool callDestructor = true)
    {
        /*auto it = objects_.find((void *)objectPointer);
        if (it != objects_.end())
        {
            assert(false && "duplicate object");
        }*/

        /*v8::EscapableHandleScope scope(isolate_);

        v8::Local<v8::Context> context = isolate_->GetCurrentContext();
        v8::Local<v8::Function> func;
        v8::Local<v8::Object> obj;
        if (class_function_template()->GetFunction(context).ToLocal(&func) && func->NewInstance(context).ToLocal(&obj))
        {
            obj->SetAlignedPointerInInternalField(0, objectPointer);
            obj->SetAlignedPointerInInternalField(1, this);
            isolate_->AdjustAmountOfExternalAllocatedMemory(static_cast<int64_t>(sizeof(ClassType)));
            v8::Global<v8::Object> pobj(isolate_, obj);

            pobj.SetWeak(this, WeakCallback<ClassType>, v8::WeakCallbackType::kInternalFields);
            this->objects_.emplace(objectPointer, ObjectRegistry{std::move(pobj), callDestructor});
            return scope.Escape(obj);
        }
        */
        //size_t argc = 0;
        //jsvm::Value args[1];

        jsvm::Value cons;
        jsvm::GetReferenceValue(env, classRef_, &cons);
        jsvm::Value instance;
        jsvm::NewInstance(env, cons, 0, nullptr, &instance);

        jsvm::Wrap(env, jsThis, reinterpret_cast<void *>(objectPointer), MyObject::Destructor,
                  nullptr, // finalize_hint
                  &obj->wrapper_);

        return instance;
    }
    void addBase(ClassRegistryBase *info)
    {
        auto it = std::find(bases_.begin(), bases_.end(), info);
        if (it != bases_.end())
        {
            assert(false && "duplicated inheritance");
            // throw std::runtime_error(class_name()+ " is already inherited from " + info.class_name());
        }
        bases_.emplace_back(info);
        info->derivatives_.emplace_back(this);
    }

    void removeObject(ClassType *objectPointer, bool callDestructor);
    // ObjectRegistry *getObjectRegistry(ClassType *objectPointer);

    std::map<uint32_t, ConstructorFunctionType> constructorFunctionMap_;
    jsvm::Env env_ = nullptr;
    jsvm::Ref classRef_ = nullptr;
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
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        auto objectRegistry = classRegistry.getObjectRegistry(objectPointer);
        assert(objectRegistry != nullptr);
        objectRegistry->pobj.ClearWeak();
    }
    template <typename ClassType> static void makeWeak(ClassType *objectPointer)
    {
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        auto objectRegistry = classRegistry.getObjectRegistry(objectPointer);
        assert(objectRegistry != nullptr);
        objectRegistry->pobj.SetWeak(&classRegistry, WeakCallback, v8::WeakCallbackType::kInternalFields);
    }
    template <typename ClassType>
    static v8::Local<v8::Object> wrapCppObject(ClassType *objectPointer, bool callDestructor = true)
    {
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        return classRegistry.wrapCppObject(objectPointer, callDestructor);
    }
    template <typename ClassType> static void removeObject(ClassType *objectPointer, bool callDestructor)
    {
        ClassRegistry<ClassType> &classRegistry = getClassRegistry<ClassType>(type_id<ClassType>());
        classRegistry.removeObject(objectPointer, callDestructor);
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
template <typename ClassType> void makeWeak(ClassType *objectPointer)
{
    ClassRegistryManager::makeWeak<ClassType>(objectPointer);
}
template <typename ClassType> void makeStrong(ClassType *objectPointer)
{
    ClassRegistryManager::makeStrong<ClassType>(objectPointer);
}
/*template <typename ClassType> Local toLocal(ClassType* objectPointer)
{
    ClassRegistry<ClassType> &classRegistry = ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());
    auto objectRegistry = classRegistry.getObjectRegistry(objectPointer);
    assert(objectRegistry != nullptr);

    return Local(v8::Local<v8::Object>::New(v8::Isolate::GetCurrent(), objectRegistry->pobj));
}*/

template <typename ClassType> class class_
{
  public:
    class_() : classRegistry_(ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>())) {};
    class_(class_ const &) = delete;
    class_ &operator=(class_ const &) = delete;

    class_(class_ &&) = default;
    class_ &operator=(class_ &&) = delete;
    ClassRegistry<ClassType> &classRegistry_;
    // v8::Isolate *isolate_;

    jsvm::Value ctor_;

  public:
    template <typename... Args> class_ &constructor()
    {
        classRegistry_.registerConstructor(sizeof...(Args), internal::InvokeClassConstructor<ClassType, Args...>);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &function(std::string_view name, ReturnType (ClassType::*func)(Args...)) const
    {

        v8::HandleScope scope(isolate_);

        FuncInfo<decltype(func)> *info = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([info]() { delete info; });
        info->name = name;
        v8::Local<v8::Value> data = v8::External::New(isolate_, info);

        v8::Local<v8::FunctionTemplate> t =
            v8::FunctionTemplate::New(isolate_, internal::InvokeClassMethod<ClassType, ReturnType, Args...>, data);
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate_, name.data(), v8::NewStringType::kInternalized).ToLocalChecked();
        classRegistry_.class_function_template()->PrototypeTemplate()->Set(name_string, t);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &function(std::string_view name, ReturnType (ClassType::*func)(Args...) const) const
    {

        v8::HandleScope scope(isolate_);

        FuncInfo<decltype(func)> *info = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([info]() { delete info; });
        info->name = name;
        v8::Local<v8::Value> data = v8::External::New(isolate_, info);

        v8::Local<v8::FunctionTemplate> t =
            v8::FunctionTemplate::New(isolate_, internal::InvokeClassMethod<ClassType, ReturnType, Args...>, data);
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate_, name.data(), v8::NewStringType::kInternalized).ToLocalChecked();
        classRegistry_.class_function_template()->PrototypeTemplate()->Set(name_string, t);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &function_optional_override(std::string_view name, ReturnType (*func)(ClassType &, Args...)) const
    {

        v8::HandleScope scope(isolate_);

        FuncInfo<decltype(func)> *info = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([info]() { delete info; });
        info->name = name;
        v8::Local<v8::Value> data = v8::External::New(isolate_, info);

        v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(
            isolate_, internal::InvokeClassMethodOptionalOverride<ClassType, ReturnType, Args...>, data);
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate_, name.data(), v8::NewStringType::kInternalized).ToLocalChecked();
        classRegistry_.class_function_template()->PrototypeTemplate()->Set(name_string, t);
        return *this;
    }
    template <typename ReturnType, typename... Args>
    const class_ &class_function(std::string_view name, ReturnType (*func)(Args...)) const
    {

        v8::HandleScope scope(isolate_);

        FuncInfo<decltype(func)> *info = new FuncInfo<decltype(func)>(func);
        internal::addDeinitializer([info]() { delete info; });
#if 0
        pData->name = name;
#endif
        v8::Local<v8::Value> data = v8::External::New(isolate_, info);

        v8::Local<v8::FunctionTemplate> t =
            v8::FunctionTemplate::New(isolate_, internal::InvokeFunction<ReturnType, Args...>, data);
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate_, name.data(), v8::NewStringType::kInternalized).ToLocalChecked();
        classRegistry_.js_function_template()->Set(name_string, t);
        return *this;
    }
    template <typename PropertyType>
    class_ &property(std::string_view name, PropertyType (ClassType::*get)(void),
                     void (ClassType::*set)(PropertyType data) = nullptr)
    {

        auto info = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([info]() { delete info; });
        classRegistry_.class_function_template()->PrototypeTemplate()->SetAccessor(
            Js_Str(isolate_, name.data()), internal::InvokeClassGetProperty<ClassType, PropertyType>,
            set == nullptr ? nullptr : internal::InvokeClassSetProperty<ClassType, PropertyType>,
            v8::External::New(isolate_, (void *)info));

        return *this;
    }
    template <typename PropertyType>
    class_ &property_optional_override(std::string_view name, PropertyType (*get)(ClassType &),
                                       void (*set)(ClassType &, PropertyType data) = nullptr)
    {

        auto info = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([info]() { delete info; });
        classRegistry_.class_function_template()->PrototypeTemplate()->SetAccessor(
            Js_Str(isolate_, name.data()), internal::InvokeClassGetPropertyOptionalOverride<ClassType, PropertyType>,
            set == nullptr ? nullptr : internal::InvokeClassSetPropertyOptionalOverride<ClassType, PropertyType>,
            v8::External::New(isolate_, (void *)info));

        return *this;
    }
    template <
        typename PropertyType /*, typename = typename std::enable_if<!std::is_function<PropertyType>::value>::type>*/>
    const class_ &property_field(std::string_view name, PropertyType ClassType::*field) const
    {
        auto info = new FuncInfo<decltype(field)>(field);
        internal::addDeinitializer([info]() { delete info; });
        classRegistry_.class_function_template()->PrototypeTemplate()->SetAccessor(
            Js_Str(isolate_, name.data()), internal::InvokeGetPropertyField<ClassType, PropertyType>,
            internal::InvokeSetPropertyField<ClassType, PropertyType>, v8::External::New(isolate_, (void *)info));
        return *this;
    }
    template <typename PropertyType>
    class_ &class_property(std::string_view name, PropertyType (*get)(void), void (*set)(PropertyType data) = nullptr)
    {

        auto info = new PropFuncInfo<decltype(get), decltype(set)>(get, set);
        internal::addDeinitializer([info]() { delete info; });
        v8::Local<v8::String> name_string =
            v8::String::NewFromUtf8(isolate_, name.data(), v8::NewStringType::kInternalized).ToLocalChecked();
        classRegistry_.js_function_template()
            ->GetFunction(isolate_->GetCurrentContext())
            .ToLocalChecked()
            ->SetAccessor(isolate_->GetCurrentContext(), name_string, internal::InvokeGetProperty<PropertyType>,
                          set == nullptr ? nullptr : internal::InvokeSetProperty<PropertyType>,
                          v8::External::New(isolate_, (void *)info));
        return *this;
    }

    template <typename BaseType> class_ &inherit()
    {
        static_assert(std::is_base_of<BaseType, ClassType>::value, "Class BaseType should be base for class ClassType");

        ClassRegistry<BaseType> &baseClassRegistry =
            ClassRegistryManager::getClassRegistry<BaseType>(type_id<BaseType>());
        classRegistry_.addBase(&baseClassRegistry);
        classRegistry_.js_function_template()->Inherit(baseClassRegistry.class_function_template());
        return *this;
    }
    static napi_value MyObject(napi_env env, napi_callback_info info)
    {
        napi_value js_this;
        napi_ref *ref = malloc(sizeof(*ref));
        NODE_API_CALL(env, napi_get_cb_info(env, info, NULL, NULL, &js_this, NULL));
        NODE_API_CALL(env, napi_wrap(env, js_this, ref, MyObject_fini, NULL, ref));
        return NULL;
    }
    void exports()
    {
        // todo
    }
};
template <typename ClassType> void ClassRegistry<ClassType>::removeObject(ClassType *objectPointer, bool callDestructor)
{
    auto it = objects_.find((void *)objectPointer);
    // assert(it != objects_.end());
    if (it != objects_.end())
    {
        v8::HandleScope scope(isolate_);

        if (callDestructor)
        {
            internal::raw_destructor(objectPointer);
        }
        isolate_->AdjustAmountOfExternalAllocatedMemory(-static_cast<int64_t>(sizeof(ClassType)));
        it->second.pobj.ClearWeak();
        it->second.pobj.Reset();
        // if (erase)
        //{
        objects_.erase(it);
        // }
    }
}

/*template <typename ClassType> static void WeakCallback(const v8::WeakCallbackInfo<ClassRegistry<ClassType>>& data)
{
    ClassType *object = static_cast<ClassType *>(data.GetInternalField(0));
    ClassRegistry<ClassType> *this_ = static_cast<ClassRegistry<ClassType> *>(data.GetInternalField(1));
    ObjectRegistry *objectRegistry = this_->getObjectRegistry(object);
    assert(object != nullptr);
    assert(this_ != nullptr);
    this_->removeObject(object, objectRegistry->callDestructor);
}*/
template <typename ClassType>
static void WeakCallback(jsvm::Env env, void *nativeObject, [[maybe_unused]] void *finalize_hint)
{
    // OH_LOG_INFO(LOG_APP, "MyObject::Destructor called");
    reinterpret_cast<MyObject *>(nativeObject)->~MyObject();
}
template <typename ClassType> static void New(jsvm::Env env, jsvm::CallbackInfo info)
{

    jsvm::Value newTarget;
    jsvm::GetNewTarget(env, info, &newTarget);
    // if (newTarget != nullptr)
    {
        // new MyObject(...) 
        size_t argc = 1;
        jsvm::Value args[1];
        jsvm::Value jsThis;
        jsvm::GetCbInfo(env, info, &argc, args, &jsThis, nullptr);

        /*double value = 0.0;
        napi_valuetype valuetype;
        napi_typeof(env, args[0], &valuetype);
        if (valuetype != napi_undefined) {
            napi_get_value_double(env, args[0], &value);
        }*/

        ClassType *object = this_->ConstructObject(args.Length(), args);

        // v8::Global<v8::Object> pobj(isolate, obj);
        // pobj.SetWeak(this_, WeakCallback<ClassType>, v8::WeakCallbackType::kInternalFields);
        this_->objects_.emplace(object, ObjectRegistry{std::move(pobj)});

        MyObject *obj = new MyObject(value);

        obj->env_ = env;
        // ͨ��napi_wrap��ArkTS����jsThis��C++����obj��
        svm::Wrap(env, jsThis, reinterpret_cast<void *>(obj), MyObject::Destructor,
                  nullptr, // finalize_hint
                  &obj->wrapper_);

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

 /*   v8::HandleScope scope(isolate_);

    v8::Local<v8::FunctionTemplate> func = v8::FunctionTemplate::New(isolate_);
    v8::Local<v8::FunctionTemplate> js_func = v8::FunctionTemplate::New(
        isolate_,
        [](v8::FunctionCallbackInfo<v8::Value> const &args) {
            v8::Isolate *isolate = args.GetIsolate();
            v8::EscapableHandleScope scope(isolate);
            ClassRegistry<ClassType> *this_ = (ClassRegistry<ClassType> *)v8::External::Cast(*args.Data())->Value();

            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            v8::Local<v8::Function> func;
            v8::Local<v8::Object> obj;
            if (this_->class_function_template()->GetFunction(context).ToLocal(&func) &&
                func->NewInstance(context).ToLocal(&obj))
            {
                ClassType *object = this_->ConstructObject(args.Length(), args);
                obj->SetAlignedPointerInInternalField(0, object);
                obj->SetAlignedPointerInInternalField(1, this_);

                v8::Global<v8::Object> pobj(isolate, obj);
                pobj.SetWeak(this_, WeakCallback<ClassType>, v8::WeakCallbackType::kInternalFields);
                this_->objects_.emplace(object, ObjectRegistry{std::move(pobj)});
            }

            args.GetReturnValue().Set(scope.Escape(obj));
        },
        v8::External::New(isolate_, this));

    func_.Reset(isolate_, func);
    js_func_.Reset(isolate_, js_func);
    func->InstanceTemplate()->SetInternalFieldCount(2);
    func->Inherit(js_func);
}*/
template <typename ClassType> jsvm::Value wrapCppObject(ClassType *objectPointer, bool callDestructor)
{
    return ClassRegistryManager::wrapCppObject<ClassType>(objectPointer, callDestructor);
}
template <typename ClassType> bool isWrappedClassOf()
{
    return ClassRegistryManager::isWrappedClassOf<ClassType>();
}

} // namespace binder
#endif

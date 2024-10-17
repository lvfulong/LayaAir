#ifndef __V8_VALUE_H__
#define __V8_VALUE_H__

#include <assert.h>
#include <binder/JSVM_Types.h>

namespace laya
{
/*class Local
{
  public:
      jsvm::Value handle_;

    Local() = delete;
    ~Local() = default;

    Local(Local const &) = delete;
    Local &operator=(Local const &) = delete;

    Local(Local &&) = delete;
    Local &operator=(Local &&) = delete;

    template <typename ValueType> explicit Local(  jsvm::Value val) : handle_(val)
    {
    }

    template <typename ReturnType, typename... Args> ReturnType call(const jsvm::Value& this_, const Args &...args)
    {
        if (!this->handle_.IsEmpty() && this->handle_->IsFunction())
        {
            auto func = v8::Function::Cast(*this->handle_);
            auto result = internal::v8_call(this_.handle_, func, args...);
            return Converter<ReturnType>::ToCpp(result);
        }
        else
        {
            return ReturnType();
        }
    }
    template <typename ReturnType, typename... Args> ReturnType call(  jsvm::Value this_, const Args &...args)
    {
        if (!this->handle_.IsEmpty() && this->handle_->IsFunction())
        {
            auto func = v8::Function::Cast(*this->handle_);
            auto result = internal::v8_call(this_, func, args...);
            return Converter<ReturnType>::ToCpp(result);
        }
        else
        {
            return ReturnType();
        }
    }

    template <typename K>
    typename std::enable_if<std::is_integral<K>::value, Local>::type operator[](const K &key) const
    {
        uint32_t i = uint32_t(key);
        auto obj = v8::Object::Cast(*handle_);

        v8::Isolate *isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        return Local(obj->Get(context, i).ToLocalChecked());
    }

    template <typename K>
    typename std::enable_if<!std::is_integral<K>::value, Local>::type operator[](const K &key) const
    {
        auto k = Converter<K>::ToJs(key);
        auto obj = v8::Object::Cast(*handle_);
        v8::Isolate *isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        return Local(obj->Get(context, k).ToLocalChecked());
    }

    template <typename ReturnType, typename... Args> ReturnType call(const char *name, const Args &...args)
    {
        auto func = v8::Function::Cast(*(*this)[name].handle_);
        auto result = internal::v8_call(handle_, func, args...);
        return Converter<ReturnType>::ToCpp(result);
    }

    template <typename T> T as() const
    {
        return Converter<T>::ToCpp(this->handle_);
    }
    bool isString()
    {
        return this->handle_->IsString();
    }
    bool isArrayBuffer()
    {
        return this->handle_->IsArrayBuffer();
    }
  private:
    friend class Persistent;
}; */
class Persistent
{
  public:
    ~Persistent()
    {
        ///////////////reset();
    }
    Persistent() = default;
    /*explicit Persistent(jsvm::Value val) : handle_(val)
    {
         handle_.ClearWeak();
    }
    explicit Persistent(const Local &local)
    {
        reset(local);
    }*/

    Persistent(Persistent const &) = delete;
    Persistent &operator=(Persistent const &) = delete;

    Persistent(Persistent &&) = delete;
    Persistent &operator=(Persistent &&) = delete;

    void reset(jsvm::Env env)
    {
        /*if (!handle_.IsEmpty())
        {
            handle_.ClearWeak();
            handle_.Reset();
        }*/

        jsvm::Status status = jsvm::DeleteReference(env, ref_);
        if (status != jsvm::Status::OK)
        {
            napi_throw_error(env, nullptr, "napi_delete_reference fail");
        }
    }

    /*void reset(const Local &l)
    {
         handle_.Reset(v8::Isolate::GetCurrent(), l.handle_);
         handle_.ClearWeak();
    }*/

    bool reset(jsvm::Env env, jsvm::Value val)
    {
        // handle_.Reset(v8::Isolate::GetCurrent(), val);
        // handle_.ClearWeak();

        jsvm::Status status = jsvm::CreateReference(env, val, 1, &ref_);
        if (status != jsvm::Status::OK)
        {
            napi_throw_error(env, nullptr, "napi_create_reference fail");
            return false;
        }
        return true;
    }

    /*Local toLocal() const
    {
        // return *reinterpret_cast<const Local *>(&handle_);
        return Local(v8::Local<v8::Value>::New(v8::Isolate::GetCurrent(), handle_));
    }*/

    jsvm::Value toLocal(jsvm::Env env) const
    {
        jsvm::Value obj = nullptr;
        // ͨ������napi_get_reference_value��ȡ���õ�ArkTS����
        jsvm::Status status = jsvm::GetReferenceValue(env, ref_, &obj);
        assert(status != jsvm::Status::OK);
        //{
        //    napi_throw_error(env, nullptr, "napi_get_reference_value fail");
        //    return nullptr;
        //}
        return obj;
    }
    bool isEmpty() const
    {
        // return handle_.IsEmpty();
        return true;
    }

    template <typename ReturnType, typename... Args>
    ReturnType call(jsvm::Env env, jsvm::Value this_, const Args &...args)
    {
        // return toLocal().call<ReturnType>(this_, args...);

        jsvm::Value func = toLocal(env);
        jsvm::ValueType valueType;
        jsvm::Status status = Typeof(env, func, &valueType)

            if (func != nullptr && status == jsvm::Status::OK && valueType == jsvm::ValueType::FUNCTION)
        {
            auto result = internal::v8_call(this_, func, args...);
            return Converter<ReturnType>::ToCpp(result);
        }
        else
        {
            return ReturnType();
        }
    }
    /*template <typename ReturnType, typename... Args> ReturnType call(v8::Local<v8::Value> this_, const Args &...args)
    {
        return toLocal().call<ReturnType>(this_, args...);
    }
    template <typename ReturnType, typename... Args> ReturnType call(v8::Local<v8::Object> this_, const Args &...args)
    {
        return toLocal().call<ReturnType>(this_.As<v8::Value>(), args...);
    }
    template <typename ReturnType, typename... Args> ReturnType call(const char *name, const Args &...args)
    {
        return toLocal().call<ReturnType>(name, args...);
    }
    template <typename ReturnType, typename... Args> ReturnType call(const char *name, const Args &...args) const
    {
        return toLocal().call<ReturnType>(name, args...);
    }*/

  private:
    // v8::CopyablePersistentTraits<v8::Value>::CopyablePersistent handle_;
    // jsvm::Value handle_;
    jsvm::Ref ref_ = nullptr;
};

} // namespace laya
#endif

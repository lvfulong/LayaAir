#ifndef __JSBIND_LOCAL_H__
#define __JSBIND_LOCAL_H__

#include <assert.h>
#include <binder/JSVM_Types.h>
#include <binder/v8/Class.h>

namespace jsbind
{
class Local
{
  public:
    Local();
    Local(jsvm::Value);
    Local(Local const &) = default;
    Local &operator=(Local const &) = default;

    Local(Local &&) = default;
    Local &operator=(Local &&) = default;
    template <typename ReturnType, typename... Args> ReturnType call(jsvm::Value recv, const Args &...args)
    {
        GET_ENV
        return call(env, recv, ... args)
    }
    template <typename ClassType, typename ReturnType, typename... Args>
    ReturnType call(ClassType *recv, const Args &...args)
    {
        GET_ENV

        ClassRegistry<ClassType> &classRegistry =
            ClassRegistryManager::getClassRegistry<ClassType>(type_id<ClassType>());
        auto objectRegistry = classRegistry.getObjectRegistry(recv);
        DEBUG_CHECK(objectRegistry != nullptr);
        svm::Value result_recv;
        Status status = GetReferenceValue(env, objectRegistry, &result_recv);
        DEBUG_CHECK(status == jsvm::Status::OK);

        return call(env, result_recv, ... args)
    }
    jsvm::Value getHandle() const
    {
        return handle_;
    }
    inline bool isValid() const
    {
        return handle_ != nullptr;
    }
    bool isUndefined() const;

    bool isNull() const;

    bool isBool() const;

    bool isNumber() const;

    bool isString() const;

    bool isObject() const;

    bool isArray() const;

    bool isFunction() const;

    bool isError() const;

    bool isArrayBuffer() const;

    bool isArrayBufferView() const;

    bool isTypedArray() const;

    bool isDataView() const;

    template <typename T> T as() const
    {
        DEBUG_CHECK(isValid());
        GET_ENV
        return Converter<T>::ToCpp(env, this->handle_);
    }
    template <typename T> bool is()
    {
        DEBUG_CHECK(isValid());
        GET_ENV
        return Converter<T>::is(env);
    }

  private:
    template <typename ClassType, typename ReturnType, typename... Args>
    ReturnType call(jsvm::Env env, jsvm::Value recv, const Args &...args)
    {
        jsvm::ValueType valueType;
        jsvm::Status status = jsvm::Typeof(env, func, &valueType) DEBUG_CHECK(status == jsvm::Status::OK);
        if (handle_ != nullptr && valueType == jsvm::ValueType::FUNCTION)
        {
            auto result = internal::v8_call(recv, handle_, args...);
            return Converter<ReturnType>::ToCpp(result);
        }
        else
        {
            return ReturnType();
        }
    }

  private:
    jsvm::Value handle_ = nullptr;
};
} // namespace jsbind
#endif

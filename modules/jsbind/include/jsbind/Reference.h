#ifndef __JSBIND_REFERENCE_H__
#define __JSBIND_REFERENCE_H__

#include <jsbind/Class.h>
#include <jsbind/Local.h>
#include <jsbind/internal/ValueTraits.h>
#include <jsvm/JSVM_Types.h>
namespace jsbind
{
template <typename T> class Reference
{
  public:
    Reference() = default;

    explicit Reference(jsvm_ref value)
    {
        DEBUG_CHECK(ref_ == nullptr);
        DEBUG_CHECK(value != nullptr);
        GET_ENV
        ref_ = value;
        jsvm_status status;
        uint32_t count;
        status = jsvm_reference_ref(env, ref_, &count);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    }
    Reference(const Reference &that)
    {
        DEBUG_CHECK(ref_ == nullptr);
        GET_ENV
        if (that.ref_ != nullptr)
        {
            ref_ = that.ref_;
            jsvm_status status;
            uint32_t count;
            status = jsvm_reference_ref(env, ref_, &count);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        }
    }

    Reference &operator=(const Reference &that)
    {
        GET_ENV
        if (this == &that || this->ref_ == that.ref_)
        {
            return *this;
        }
        if (that.ref_ != nullptr)
        {
            if (ref_ != nullptr)
            {
                reset();
            }
            ref_ = that.ref_;
            jsvm_status status;
            uint32_t count;
            status = jsvm_reference_ref(env, ref_, &count);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        }
        return *this;
    }
    Reference(Reference &&that) //: ref_(that.ref_)
    {
        DEBUG_CHECK(ref_ == nullptr);
        ref_ = that.ref_;
        that.ref_ = nullptr;
    }
    Reference &operator=(Reference &&that)
    {
        if (this == &that || this->ref_ == that.ref_)
        {
            return *this;
        }
        if (ref_ != nullptr)
        {
            reset();
        }
        ref_ = that.ref_;
        that.ref_ = nullptr;

        return *this;
    }
    ~Reference()
    {
        reset();
    }
    void reset(T *value)
    {
        *this = toReference(value);
    }
    void reset()
    {
        if (ref_ != nullptr)
        {
            GET_ENV
            jsvm_status status;
            uint32_t count;
            status = jsvm_reference_unref(env, ref_, &count);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            ref_ = nullptr;
        }
    }
    /*T as(const char* name, const Args &...args)
    {
        if (isValid() && getLocal().isFunction())
        {
            return getLocal().call<ReturnType>(name, args...);
        }
        else
        {
            return ReturnType();
        }
    }*/
    inline bool isValid() const
    {
        return ref_ != nullptr;
    }
    explicit operator bool() const noexcept
    {
        return isValid() && !getLocal().isUndefined() && !getLocal().isNull();
    }
    inline jsvm_value getHandle() const
    {
        GET_ENV
        jsvm_status status;
        jsvm_value value;
        if (isValid())
        {
            status = jsvm_get_reference_value(env, ref_, &value);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            return value;
        }
        else
        {
            status = jsvm_get_null(env, &value);
            DEBUG_CHECK(status == jsvm_status::jsvm_ok);
            return value;
        }
    }
    inline Local getLocal() const
    {
        return Local(getHandle());
    }

  private:
    jsvm_ref ref_ = nullptr;
};
namespace internal
{
template <typename ClassType>
class ValueTraits<Reference<ClassType>, std::enable_if_t<internal::is_wrapped_class<ClassType>::value>>
{
  public:
    static jsvm_value ToJs(Reference<ClassType> value, bool callDestructor = true)
    {
        return value.getHandle();
    }
    static Reference<ClassType> ToCpp(jsvm_value value)
    {
        ClassRegistry<ClassType, raw_ptr_traits> *classRegistry =
            static_cast<ClassRegistry<ClassType, raw_ptr_traits> *>(getClassRegistry(type_id<ClassType>()));
        return Reference<ClassType>(classRegistry->getObjectRef(value));
    }
    static bool is(jsvm_value value)
    {
        return true;
    }
};

} // namespace internal
} // namespace jsbind
#endif

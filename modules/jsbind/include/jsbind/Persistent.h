#ifndef __JSBIND_PERSISTENT_H__
#define __JSBIND_PERSISTENT_H__

#include <jsbind/Class.h>
#include <jsbind/Local.h>
#include <jsvm/JSVM_Types.h>

namespace jsbind
{
class Persistent
{
  public:
    Persistent() = default;
    explicit Persistent(jsvm_ref);
    explicit Persistent(jsvm_value);

    Persistent(const Persistent &);

    Persistent &operator=(const Persistent &);

    ~Persistent();
    template <typename ReturnType, typename... Args> ReturnType call(jsvm_value recv, const Args &...args)
    {
        if (isValid() && getLocal().isFunction())
        {
            return getLocal().call<ReturnType>(recv, args...);
        }
        else
        {
            return ReturnType();
        }
    }
    template <typename ReturnType, typename... Args> ReturnType call(jsvm_value recv, const Args &...args) const
    {
        if (isValid() && getLocal().isFunction())
        {
            return getLocal().call<ReturnType>(recv, args...);
        }
        else
        {
            return ReturnType();
        }
    }
        template <typename ReturnType, typename... Args> ReturnType call(const char*  name, const Args &...args)
    {
        if (isValid() && getLocal().isFunction())
        {
            return getLocal().call<ReturnType>(name, args...);
        }
        else
        {
            return ReturnType();
        }
    }
    template <typename ReturnType, typename... Args> ReturnType call(const char*  name, const Args &...args) const
    {
        if (isValid() && getLocal().isFunction())
        {
            return getLocal().call<ReturnType>(name, args...);
        }
        else
        {
            return ReturnType();
        }
    }
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

    void reset();
    inline Local getLocal() const
    {
        return Local(getHandle());
    }

  private:
    jsvm_ref ref_ = nullptr;
};

} // namespace jsbind
#endif

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
    inline bool isValid() const
    {
        return ref_ != nullptr;
    }

    inline jsvm_value getHandle() const
    {
        GET_ENV
        jsvm_status status;
        jsvm_value value;

        status = jsvm_get_reference_value(env, ref_, &value);
        DEBUG_CHECK(status == jsvm_status::jsvm_ok);
        return value;
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

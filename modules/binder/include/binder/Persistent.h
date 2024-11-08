#ifndef __JSBIND_PERSISTENT_H__
#define __JSBIND_PERSISTENT_H__

#include <assert.h>
#include <binder/Class.h>
#include <binder/JSVM_Types.h>
#include <binder/Local.h>

namespace jsbind
{
class Persistent
{
  public:
    Persistent() = default;

    explicit Persistent(jsvm::Value);

    Persistent(const Persistent &);

    Persistent &operator=(const Persistent &);

    ~Persistent();
    template <typename ReturnType, typename... Args> ReturnType call(jsvm::Value recv, const Args &...args)
    {
        DEBUG_CHECK(isValid());
        return getLocal().call<ReturnType>(recv, args...);
    }
    template <typename ReturnType, typename... Args> ReturnType call(jsvm::Value recv, const Args &...args) const
    {
        DEBUG_CHECK(isValid());
        return getLocal().call<ReturnType>(recv, args...);
    }
    inline bool isValid() const
    {
        return ref_ != nullptr;
    }

    inline jsvm::Value getHandle() const
    {
        GET_ENV
        jsvm::Status status;
        jsvm::Value value;

        status = jsvm::GetReferenceValue(env, ref_, &value);
        DEBUG_CHECK(status == jsvm::Status::OK);
        return value;
    }

    void reset();
    inline Local getLocal() const
    {
        return Local(getHandle());
    }

  private:
    jsvm::Ref ref_ = nullptr;
};

} // namespace jsbind
#endif

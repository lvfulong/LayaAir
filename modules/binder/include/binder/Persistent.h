#ifndef __JSBIND_PERSISTENT_H__
#define __JSBIND_PERSISTENT_H__

#include <assert.h>
#include <binder/JSVM_Types.h>
#include <binder/Class.h>
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
        if (isValid())
        {
            return getLocal().call<ReturnType>(recv,  args...);
        }
        else
        {
            return ReturnType();
        }
    }
    template <typename ReturnType, typename... Args> ReturnType call(jsvm::Value recv, const Args &...args) const
    {
        if (isValid())
        {
            return getLocal().call<ReturnType>(recv, args...);
        }
        else
        {
            return ReturnType();
        }
    }
    /*template <typename ClassType, typename ReturnType, typename... Args>
    ReturnType call(ClassType *recv, const Args &...args)
    {
        if (isValid())
        {
            return getLocal().call<ClassType, ReturnType>(recv,  args...);
        }
        else
        {
            return ReturnType();
        }
    }
    template <typename ClassType, typename ReturnType, typename... Args>
    ReturnType call(ClassType* recv, const Args &...args) const
    {
        if (isValid())
        {
            return getLocal().call<ClassType, ReturnType>(recv, args...);
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

    jsvm::Value getHandle() const;

    void reset();
    inline const Local &getLocal() const
    {
        return local_;
    }

  private:
    jsvm::Ref ref_ = nullptr;
    Local local_;
};

} // namespace jsbind
#endif

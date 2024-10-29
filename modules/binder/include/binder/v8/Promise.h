#ifndef __JSBIND_PROMISE_H__
#define __JSBIND_PROMISE_H__

#include <binder/JSVM_Types.h>

namespace jsbind
{
class Promise
{
  public:
    static Promise Make();

    ~Promise() = default;

    template <typename T> void resolve(T &&t) const;

    template <typename T> void reject(T &&t) const;

    jsvm::Value getHandle() const
    {
        return promise_;
    }
    inline bool isValid() const
    {
        return promise_ != nullptr;
    }

  private:
    Promise();
    jsvm::Value promise_;
    jsvm::Deferred deferred_;
};
} // namespace jsbind
#endif

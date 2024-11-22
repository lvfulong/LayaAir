#ifndef __JSBIND_PROMISE_H__
#define __JSBIND_PROMISE_H__

#include <jsvm/JSVM_Types.h>

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
template <typename T> void Promise::resolve(T&& t) const
{
    GET_ENV
        jsvm::ResolveDeferred(env, deferred_, ValueTraits<T>::ToJs(std::forward<T>(t), true));
}

template <typename T> void Promise::reject(T&& t) const
{
    GET_ENV
        jsvm::RejectDeferred(env, deferred_, ValueTraits<T>::ToJs(std::forward<T>(t), true));
}
} // namespace jsbind
#endif

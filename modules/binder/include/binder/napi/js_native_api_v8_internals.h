#ifndef SRC_JS_NATIVE_API_V8_INTERNALS_H_
#define SRC_JS_NATIVE_API_V8_INTERNALS_H_

// The V8 implementation of N-API, including `js_native_api_v8.h` uses certain
// idioms which require definition here. For example, it uses a variant of
// persistent references which need not be reset in the constructor. It is the
// responsibility of this file to define these idioms. Optionally, this file
// may also define `NAPI_VERSION` and set it to the version of N-API to be
// exposed.

// In the case of the Node.js implementation of N-API some of the idioms are
// imported directly from Node.js by including `node_internals.h` below. Others
// are bridged to remove references to the `node` namespace. `node_version.h`,
// included below, defines `NAPI_VERSION`.

// #include "node_version.h"

// #include "env.h"
// #include "gtest/gtest_prod.h"
// #include "node_errors.h"
// #include "node_internals.h"
// #include "util.h"
#include <v8.h>
#include <unordered_set>
#define NODE_API_DEFAULT_MODULE_API_VERSION 8
namespace node
{
template <typename T, size_t N> constexpr size_t arraysize(const T (&)[N])
{
    return N;
}
class PersistentToLocal
{
  public:
    // If persistent.IsWeak() == false, then do not call persistent.Reset()
    // while the returned Local<T> is still in scope, it will destroy the
    // reference to the object.
    template <class TypeName>
    static inline v8::Local<TypeName> Default(v8::Isolate *isolate, const v8::PersistentBase<TypeName> &persistent)
    {
        if (persistent.IsWeak())
        {
            return PersistentToLocal::Weak(isolate, persistent);
        }
        else
        {
            return PersistentToLocal::Strong(persistent);
        }
    }

    // Unchecked conversion from a non-weak Persistent<T> to Local<T>,
    // use with care!
    //
    // Do not call persistent.Reset() while the returned Local<T> is still in
    // scope, it will destroy the reference to the object.
    template <class TypeName> static inline v8::Local<TypeName> Strong(const v8::PersistentBase<TypeName> &persistent)
    {
        // DCHECK(!persistent.IsWeak());
        return *reinterpret_cast<v8::Local<TypeName> *>(const_cast<v8::PersistentBase<TypeName> *>(&persistent));
    }

    template <class TypeName>
    static inline v8::Local<TypeName> Weak(v8::Isolate *isolate, const v8::PersistentBase<TypeName> &persistent)
    {
        return v8::Local<TypeName>::New(isolate, persistent);
    }
};
} // namespace node
#define NAPI_ARRAYSIZE(array) node::arraysize((array))

#define NAPI_FIXED_ONE_BYTE_STRING(isolate, string) node::FIXED_ONE_BYTE_STRING((isolate), (string))

#define NAPI_PRIVATE_KEY(context, suffix) (jsbind::JSEnv::getCurrent()->napi_##suffix())

namespace v8impl
{

template <typename T> using Persistent = v8::Global<T>;

using PersistentToLocal = node::PersistentToLocal;

[[noreturn]] inline void OnFatalError(const char *location, const char *message)
{
    // node::OnFatalError(location, message);
}

} // end of namespace v8impl

#endif // SRC_JS_NATIVE_API_V8_INTERNALS_H_

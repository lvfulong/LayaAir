#include "binder/JSVM.h"
#include <v8.h>
namespace JSVM
{

#define GET_RETURN_STATUS(env) (!try_catch.HasCaught() ? Status::OK : set_last_error((env), Status::PENDING_EXCEPTION))
namespace v8impl
{
class PersistentToLocal
{
  public:
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
    template <class TypeName> static inline v8::Local<TypeName> Strong(const v8::PersistentBase<TypeName> &persistent)
    {
        DEBUG_CHECK(!persistent.IsWeak());
        return *reinterpret_cast<v8::Local<TypeName> *>(const_cast<v8::PersistentBase<TypeName> *>(&persistent));
    }

    template <class TypeName>
    static inline v8::Local<TypeName> Weak(v8::Isolate *isolate, const v8::PersistentBase<TypeName> &persistent)
    {
        return v8::Local<TypeName>::New(isolate, persistent);
    }
};
template <typename T> using Persistent = v8::Global<T>;

inline Deferred JsDeferredFromNodePersistent(v8impl::Persistent<v8::Value> *local)
{
    return reinterpret_cast<Deferred>(local);
}
inline Value JsValueFromV8LocalValue(v8::Local<v8::Value> local)
{
    return reinterpret_cast<Value>(*local);
}
inline v8::Local<v8::Value> V8LocalValueFromJsValue(Value v)
{
    v8::Local<v8::Value> local;
    memcpy(static_cast<void *>(&local), &v, sizeof(v));
    return local;
}

} // namespace v8impl

#define RETURN_STATUS_IF_FALSE(env, condition, status)                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            return set_last_error((env), (status));                                                                    \
        }                                                                                                              \
    } while (0)
#define CHECK_ENV(env)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((env) == nullptr)                                                                                          \
        {                                                                                                              \
            return Status::INVALID_ARG;                                                                                \
        }                                                                                                              \
    } while (0)

#define CHECK_ENV_NOT_IN_GC(env)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        CHECK_ENV((env));                                                                                              \
        (env)->CheckGCAccess();                                                                                        \
    } while (0)
#define NAPI_PREAMBLE(env)                                                                                             \
    CHECK_ENV_NOT_IN_GC((env));                                                                                        \
    RETURN_STATUS_IF_FALSE((env), (env)->last_exception.IsEmpty(), Status::PENDING_EXCEPTION);                         \
    RETURN_STATUS_IF_FALSE((env), (env)->can_call_into_js(),                                                           \
                           (/*env->module_api_version == NAPI_VERSION_EXPERIMENTAL                                     \
                                ? napi_cannot_run_js                                                                   \
                                : */                                                                                   \
                            Status::PENDING_EXCEPTION));                                                               \
    clear_last_error((env));                                                                                           \
    v8impl::TryCatch try_catch((env))

struct Env__
{
    explicit Env__(v8::Local<v8::Context> context, int32_t module_api_version)
        : isolate(context->GetIsolate()), context_persistent(isolate, context), module_api_version(module_api_version)
    {
        // napi_clear_last_error(this);
    }

    inline v8::Local<v8::Context> context() const
    {
        return v8impl::PersistentToLocal::Strong(context_persistent);
    }
    virtual bool can_call_into_js() const
    {
        return true;
    }
    void CheckGCAccess()
    {
        // todo
    }
    v8::Isolate *const isolate;
    v8impl::Persistent<v8::Context> context_persistent;
    v8impl::Persistent<v8::Value> last_exception;
    int32_t module_api_version = 8; // NODE_API_DEFAULT_MODULE_API_VERSION;
    ExtendedErrorInfo last_error;

  protected:
    virtual ~Env__() = default;
};
inline Status clear_last_error(Env basic_env)
{
    Env env = const_cast<Env>(basic_env);
    env->last_error.error_code = Status::OK;
    env->last_error.engine_error_code = 0;
    env->last_error.engine_reserved = nullptr;
    env->last_error.error_message = nullptr;
    return Status::OK;
}
inline Status set_last_error(Env basic_env, Status error_code, uint32_t engine_error_code = 0,
                             void *engine_reserved = nullptr)
{
    Env env = const_cast<Env>(basic_env);
    env->last_error.error_code = error_code;
    env->last_error.engine_error_code = engine_error_code;
    env->last_error.engine_reserved = engine_reserved;
    return error_code;
}
namespace v8impl
{
class TryCatch : public v8::TryCatch
{
  public:
    explicit TryCatch(Env env) : v8::TryCatch(env->isolate), _env(env)
    {
    }

    ~TryCatch()
    {
        if (HasCaught())
        {
            _env->last_exception.Reset(_env->isolate, Exception());
        }
    }

  private:
    Env _env;
};
inline v8impl::Persistent<v8::Value> *NodePersistentFromJsDeferred(Deferred local)
{
    return reinterpret_cast<v8impl::Persistent<v8::Value> *>(local);
}
inline Status ConcludeDeferred(Env env, Deferred deferred, Value result, bool is_resolved)
{
    NAPI_PREAMBLE(env);
    // CHECK_ARG(env, result);

    v8::Local<v8::Context> context = env->context();
    v8impl::Persistent<v8::Value> *deferred_ref = NodePersistentFromJsDeferred(deferred);
    v8::Local<v8::Value> v8_deferred = v8::Local<v8::Value>::New(env->isolate, *deferred_ref);

    auto v8_resolver = v8_deferred.As<v8::Promise::Resolver>();

    v8::Maybe<bool> success = is_resolved ? v8_resolver->Resolve(context, v8impl::V8LocalValueFromJsValue(result))
                                          : v8_resolver->Reject(context, v8impl::V8LocalValueFromJsValue(result));

    delete deferred_ref;

    // RETURN_STATUS_IF_FALSE(env, success.FromMaybe(false), napi_generic_failure);

    return GET_RETURN_STATUS(env);
}
} // namespace v8impl
/*JSVM_EXTERN*/ Status CreatePromise(Env env, Deferred *deferred, Value *promise)
{
    NAPI_PREAMBLE(env);
    // CHECK_ARG(env, deferred);
    // CHECK_ARG(env, promise);

    auto maybe = v8::Promise::Resolver::New(env->context());
    // CHECK_MAYBE_EMPTY(env, maybe, napi_generic_failure);

    auto v8_resolver = maybe.ToLocalChecked();
    auto v8_deferred = new v8impl::Persistent<v8::Value>();
    v8_deferred->Reset(env->isolate, v8_resolver);

    *deferred = v8impl::JsDeferredFromNodePersistent(v8_deferred);
    *promise = v8impl::JsValueFromV8LocalValue(v8_resolver->GetPromise());
    return GET_RETURN_STATUS(env);
}
/*JSVM_EXTERN*/ Status ResolveDeferred(Env env, Deferred deferred, Value resolution)
{
    return v8impl::ConcludeDeferred(env, deferred, resolution, true);
}

/*JSVM_EXTERN*/ Status RejectDeferred(Env env, Deferred deferred, Value resolution)
{
    return v8impl::ConcludeDeferred(env, deferred, resolution, false);
}

/*JSVM_EXTERN*/ Status IsPromise(Env env, Value value, bool *isPromise)
{
    // CHECK_ENV_NOT_IN_GC(env);
    // CHECK_ARG(env, value);
    // CHECK_ARG(env, isPromise);

    *isPromise = v8impl::V8LocalValueFromJsValue(value)->IsPromise();

    return clear_last_error(env);
}
} // namespace JSVM
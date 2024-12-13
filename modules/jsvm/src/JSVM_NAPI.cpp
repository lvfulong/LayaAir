#include "jsvm/JSEnv.h"
#include "jsvm/JSVM.h"
#include "jsvm/ScriptThread.h"
#include "jsvm/napi/js_native_api.h"
#include "jsvm/napi/js_native_api_v8.h"
#include "v8debug/debug-agent.h"
#include <libplatform/libplatform.h>
#include <v8.h>


#if 0
// ���Ҫ��v8�߳��������ͷţ���Ϊv8������ʱ��������������ṩ�� Free �ӿ�
class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator
{
public:
    ArrayBufferAllocator();
    ~ArrayBufferAllocator();
    virtual void* Allocate(size_t length);
    virtual void* AllocateUninitialized(size_t length);
    virtual void Free(void* data, size_t length);
    // �������û�����壬v8�����ᶼ�ͷŵ� Heap::FreeDeadArrayBuffers
    // void FreeAllAlive();//�ͷ����еĻ�û���ͷŵ�ArrayBuffer
    static ArrayBufferAllocator* getInstance();
    // int _testGetID(void* pdata);
    // int getAliveBufferNum() {
    //     return m_vAliveBuffer.size();
    // }
protected:
    // std::vector<char*> m_vAliveBuffer;
};
ArrayBufferAllocator::ArrayBufferAllocator()
{
}

ArrayBufferAllocator::~ArrayBufferAllocator()
{
}

void* ArrayBufferAllocator::Allocate(size_t length)
{
    char* pRet = new char[length];
    memset(pRet, 0, length);
    return pRet;
}
void* ArrayBufferAllocator::AllocateUninitialized(size_t length)
{
    char* pRet = new char[length];
    return pRet;
};
void ArrayBufferAllocator::Free(void* data, size_t length)
{
    if (data != NULL || length > 0)
    {
        delete[]((char*)data);
    }
    else
    {
        LOGI("ArrayBufferAllocator::Free data=%d length=%d", (intptr_t)data, length);
    }
}

ArrayBufferAllocator* ArrayBufferAllocator::getInstance()
{
    return new ArrayBufferAllocator();
}
#endif

static v8::Platform *s_pPlatform = NULL;
static bool s_bV8InitializePlatform = false;

struct VM__
{
    v8::Isolate *isolate_;
};
struct VMScope__
{
    v8::Isolate *isolate_;
};
struct EnvScope__
{
};

jsvm_status jsvm_init(const jsvm_init_options* options)
{
    // m_pIsolate = NULL;
    // m_nListenPort = 0;

    if (!s_bV8InitializePlatform)
    {
        s_bV8InitializePlatform = true;
        s_pPlatform = v8::platform::NewDefaultPlatform().release();
        v8::V8::InitializePlatform(s_pPlatform);
        v8::V8::Initialize();
        std::string flags;
#if defined(OS_IOS) || defined(OS_OHOS)
        flags.append(" --jitless ");
#endif
        flags.append(" --expose-gc ");
        // flags.append(" --expose-gc-as=gc ");
        flags.append(" --no-flush-bytecode ");
        flags.append(" --no-lazy ");
        flags.append(" --turbo-fast-api-calls ");
        v8::V8::SetFlagsFromString(flags.c_str(), (size_t)flags.length());
    }
    return jsvm_status::jsvm_ok; // todo
}
#if 0
Status UnInit()
{
    return;
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete s_pPlatform;
}
#endif

class LayaNapiEnv : public napi_env__
{
  public:
    LayaNapiEnv(v8::Local<v8::Context> context, int32_t module_api_version) : napi_env__(context, module_api_version)
    {
    }
    ~LayaNapiEnv()
    {
    }
    // bool can_call_into_js() const override;
    void CallFinalizer(napi_finalize cb, void *data, void *hint) override;
    template <bool enforceUncaughtExceptionPolicy> void CallFinalizer(napi_finalize cb, void *data, void *hint);

    void EnqueueFinalizer(v8impl::RefTracker *finalizer) override;
    void DrainFinalizerQueue();

    void trigger_fatal_exception(v8::Local<v8::Value> local_err);
    template <bool enforceUncaughtExceptionPolicy, typename T> void CallbackIntoModule(T &&call);

    void DeleteMe() override;

    /*inline node::Environment* node_env() const {
        return node::Environment::GetCurrent(context());
    }*/
    inline const char *GetFilename() const
    {
        return filename.c_str();
    }

    std::string filename;
    bool destructing = false;
    bool finalization_scheduled = false;
};
void LayaNapiEnv::DeleteMe()
{
    destructing = true;
    DrainFinalizerQueue();
    napi_env__::DeleteMe();
}

// bool LayaNapiEnv::can_call_into_js() const {
//     return node_env()->can_call_into_js();
// }

void LayaNapiEnv::CallFinalizer(napi_finalize cb, void *data, void *hint)
{
    CallFinalizer<true>(cb, data, hint);
}

template <bool enforceUncaughtExceptionPolicy> void LayaNapiEnv::CallFinalizer(napi_finalize cb, void *data, void *hint)
{
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(context());
    CallbackIntoModule<enforceUncaughtExceptionPolicy>([&](napi_env env) { cb(env, data, hint); });
}

void LayaNapiEnv::EnqueueFinalizer(v8impl::RefTracker *finalizer)
{
    napi_env__::EnqueueFinalizer(finalizer);
    // Schedule a second pass only when it has not been scheduled, and not
    // destructing the env.
    // When the env is being destructed, queued finalizers are drained in the
    // loop of `node_napi_env__::DrainFinalizerQueue`.
    if (!finalization_scheduled && !destructing)
    {
        finalization_scheduled = true;
        //Ref();
        /////node_env()->SetImmediate([this](node::Environment* node_env) {
        ////    finalization_scheduled = false;
        ////    Unref();
        //////    DrainFinalizerQueue();
        ///////    });
    }
    DrainFinalizerQueue();
}

void LayaNapiEnv::DrainFinalizerQueue()
{
    // As userland code can delete additional references in one finalizer,
    // the list of pending finalizers may be mutated as we execute them, so
    // we keep iterating it until it is empty.
    while (!pending_finalizers.empty())
    {
        v8impl::RefTracker *ref_tracker = *pending_finalizers.begin();
        pending_finalizers.erase(ref_tracker);
        ref_tracker->Finalize();
    }
}

void LayaNapiEnv::trigger_fatal_exception(v8::Local<v8::Value> local_err)
{
    v8::Local<v8::Message> local_msg = v8::Exception::CreateMessage(isolate, local_err);
    //////node::errors::TriggerUncaughtException(isolate, local_err, local_msg);
}

// The option enforceUncaughtExceptionPolicy is added for not breaking existing
// running Node-API add-ons.
template <bool enforceUncaughtExceptionPolicy, typename T> void LayaNapiEnv::CallbackIntoModule(T &&call)
{
    CallIntoModule(call, [](napi_env env_, v8::Local<v8::Value> local_err) {
#if 0
        node_napi_env__* env = static_cast<node_napi_env__*>(env_);
        if (env->terminatedOrTerminating()) {
            return;
        }
        node::Environment* node_env = env->node_env();
        // If the module api version is less than NAPI_VERSION_EXPERIMENTAL,
        // and the option --force-node-api-uncaught-exceptions-policy is not
        // specified, emit a warning about the uncaught exception instead of
        // triggering uncaught exception event.
        if (env->module_api_version < NAPI_VERSION_EXPERIMENTAL &&
            !node_env->options()->force_node_api_uncaught_exceptions_policy &&
            !enforceUncaughtExceptionPolicy) {
            ProcessEmitDeprecationWarning(
                node_env,
                "Uncaught N-API callback exception detected, please run node "
                "with option --force-node-api-uncaught-exceptions-policy=true "
                "to handle those exceptions properly.",
                "DEP0168");
            return;
        }
        // If there was an unhandled exception in the complete callback,
        // report it as a fatal exception. (There is no JavaScript on the
        // call stack that can possibly handle it.)
        env->trigger_fatal_exception(local_err);
#endif
        });
}
std::string stackTraceToString(v8::Local<v8::StackTrace> stack)
{
    std::string stackStr;
    if (stack.IsEmpty())
    {
        return stackStr;
    }

    char tmp[100] = {0};
    for (int i = 0, e = stack->GetFrameCount(); i < e; ++i)
    {
        v8::Local<v8::StackFrame> frame = stack->GetFrame(v8::Isolate::GetCurrent(), i);
        v8::Local<v8::String> script = frame->GetScriptName();
        std::string scriptName;
        if (!script.IsEmpty())
        {
            scriptName = *v8::String::Utf8Value(v8::Isolate::GetCurrent(), script);
        }

        v8::Local<v8::String> func = frame->GetFunctionName();
        std::string funcName;
        if (!func.IsEmpty())
        {
            funcName = *v8::String::Utf8Value(v8::Isolate::GetCurrent(), func);
        }

        stackStr += " - [";
        snprintf(tmp, sizeof(tmp), "%d", i);
        stackStr += tmp;
        stackStr += "]";
        stackStr += (funcName.empty() ? "anonymous" : funcName.c_str());
        stackStr += "@";
        stackStr += (scriptName.empty() ? "[no filename]" : scriptName.c_str());
        stackStr += ":";
        snprintf(tmp, sizeof(tmp), "%d", frame->GetLineNumber());
        stackStr += tmp;

        if (i < (e - 1))
        {
            stackStr += "\n";
        }
    }

    return stackStr;
}
static void PromiseRejectHandlerInMainThread(v8::PromiseRejectMessage data)
{
    v8::Local<v8::Promise> promise = data.GetPromise();
    v8::Isolate *isolate = promise->GetIsolate();
    if (data.GetEvent() == v8::kPromiseHandlerAddedAfterReject)
    {
        return;
    }
    else if (data.GetEvent() == v8::kPromiseRejectAfterResolved || data.GetEvent() == v8::kPromiseResolveAfterResolved)
    {
        // Ignore reject/resolve after resolved.
        return;
    }
    v8::Local<v8::Value> exception = data.GetValue();
    // const char *error_message = nullptr;
    std::string error_message;
    v8::Local<v8::Message> message = v8::Exception::CreateMessage(isolate, exception);

    if (!message.IsEmpty())
    {
        if (message->Get().IsEmpty() || message->Get()->IsNull())
        {
        }
        else
        {
            v8::String::Utf8Value utf8(isolate, message->Get().As<v8::String>());
            error_message.assign(*utf8);
            // error_message = ValueTraits<const char*>::ToCpp(message->Get().As<v8::String>());
        }
    }
    // std::string kBuf = "if(conch.onunhandledrejection){conch.onunhandledrejection('";
    // kBuf += UrlEncode(error_message != nullptr ? error_message : "");
    // kBuf += "');};";
    //__JSRun::Run(kBuf.c_str());
    v8::HandleScope hs(v8::Isolate::GetCurrent());
    v8::Local<v8::StackTrace> stack =
        v8::StackTrace::CurrentStackTrace(v8::Isolate::GetCurrent(), 20, v8::StackTrace::kOverview);
    std::string str = stackTraceToString(stack);
    const char *s = str.c_str();

    LOGI("unhandledrejection stack %s", s);
    LOGE("unhandledrejection %s", error_message.c_str()); // != nullptr ? error_message : "no message");
    // IsolateData *pIsolateData = IsolateData::From(isolate);
    // Javascript *pJavascript = (Javascript *)pIsolateData->m_data;
    // pJavascript->m_promiseRejectHandler(data.GetPromise(), data.GetValue(), "unhandledrejection");
}
jsvm_status jsvm_create_vm(const jsvm_create_vm_options* options, jsvm_vm* result)
{
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    *result = new VM__;
    (*result)->isolate_ = v8::Isolate::New(create_params);
    // m_pIsolate->Enter();
    // v8::HandleScope handle_scope(m_pIsolate);
    // v8::Local<v8::Context> context = v8::Context::New(m_pIsolate);
    // m_context.Reset(m_pIsolate, context);
    // m_IsolateData = new IsolateData(m_pIsolate, NULL);
    // m_IsolateData->m_data = (void *)this;
    (*result)->isolate_->SetPromiseRejectCallback(PromiseRejectHandlerInMainThread);
    // context->Enter();
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_destroy_vm(jsvm_vm vm)
{
    {
        // v8::HandleScope handle_scope(m_pIsolate);
        // v8::Local<v8::Context> context = m_context.Get(m_pIsolate);
        // context->Exit();
        // m_context.Reset();
        // delete m_IsolateData;
        // m_pIsolate->Exit();
    }
    DEBUG_CHECK(vm->isolate_ != nullptr);
    vm->isolate_->Dispose();
    vm->isolate_ = nullptr;
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_open_vm_scope(jsvm_vm vm, jsvm_vm_scope* result)
{
    *result = new VMScope__;
    (*result)->isolate_ = vm->isolate_;
    (*result)->isolate_->Enter();
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_close_vm_scope(jsvm_vm vm, jsvm_vm_scope scope)
{
    scope->isolate_ = vm->isolate_;
    scope->isolate_->Exit();
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_open_env_scope(jsvm_env env, jsvm_env_scope* result)
{
    env->context()->Enter();
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_close_env_scope(jsvm_env env, jsvm_env_scope scope)
{
    env->context()->Exit();
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_create_env(jsvm_vm vm, size_t propertyCount, const jsvm_property_descriptor* properties, jsvm_env* result)
{
    // todo properties
    v8::HandleScope handle_scope(vm->isolate_);
    v8::Local<v8::Context> context = v8::Context::New(vm->isolate_);
    *result = new LayaNapiEnv(context, NAPI_VERSION);

    jsvm::IsolateData *isolateData = new jsvm::IsolateData(vm->isolate_);     // delete ?
    jsvm::JSEnv *jsEnv = new jsvm::JSEnv(isolateData, vm->isolate_, *result); // delete ?
    jsvm::JSEnv::setCurrent(jsEnv);

    return jsvm_status::jsvm_ok;
}
jsvm_status jsvm_destroy_env(jsvm_env env)
{
    DEBUG_CHECK(env != nullptr);
    env->DeleteMe();
    return jsvm_status::jsvm_ok; // todo
}

jsvm_status jsvm_open_handle_scope(jsvm_env env, jsvm_handle_scope* result)
{
    return static_cast<jsvm_status>(napi_open_handle_scope(env, result));
}
jsvm_status jsvm_close_handle_scope(jsvm_env env, jsvm_handle_scope scope)
{
    return static_cast<jsvm_status>(napi_close_handle_scope(env, scope));
}
jsvm_status  jsvm_get_and_clear_last_exception(jsvm_env env, jsvm_value*result)
{
    return static_cast<jsvm_status>(napi_get_and_clear_last_exception(env, result));
}
jsvm_status jsvm_throw_error(jsvm_env env, const char* code, const char* msg)
{
    return static_cast<jsvm_status>(napi_throw_error(env, code, msg));
}
jsvm_status jsvm_get_array_length(jsvm_env env, jsvm_value value, uint32_t* result)
{
    return static_cast<jsvm_status>(napi_get_array_length(env, value, result));
}
jsvm_status jsvm_is_array(jsvm_env env, jsvm_value value, bool* result)
{
    return static_cast<jsvm_status>(napi_is_array(env, value, result));
}
jsvm_status jsvm_create_promise(jsvm_env env, jsvm_deferred* deferred, jsvm_value* promise)
{
    return static_cast<jsvm_status>(napi_create_promise(env, deferred, promise));
}
jsvm_status jsvm_create_array(jsvm_env env, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_array(env, result));
}
jsvm_status jsvm_create_array_with_length(jsvm_env env, size_t length, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_array_with_length(env, length, result));
}
jsvm_status jsvm_create_double(jsvm_env env, double value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_double(env, value, result));
}
jsvm_status jsvm_create_int32(jsvm_env env, int32_t value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_int32(env, value, result));
}
jsvm_status jsvm_create_uint32(jsvm_env env, uint32_t value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_uint32(env, value, result));
}
jsvm_status jsvm_create_int64(jsvm_env env, int64_t value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_int64(env, value, result));
}

jsvm_status jsvm_get_value_double(jsvm_env env, jsvm_value value, double* result)
{
    return static_cast<jsvm_status>(napi_get_value_double(env, value, result));
}

jsvm_status jsvm_get_value_int32(jsvm_env env, jsvm_value value, int32_t* result)
{
    return static_cast<jsvm_status>(napi_get_value_int32(env, value, result));
}
jsvm_status jsvm_get_value_uint32(jsvm_env env, jsvm_value value, uint32_t* result)
{
    return static_cast<jsvm_status>(napi_get_value_uint32(env, value, result));
}
jsvm_status jsvm_get_value_int64(jsvm_env env, jsvm_value value, int64_t* result)
{
    return static_cast<jsvm_status>(napi_get_value_int64(env, value, result));
}
jsvm_status jsvm_get_value_bool(jsvm_env env, jsvm_value value, bool* result)
{
    return static_cast<jsvm_status>(napi_get_value_bool(env, value, result));
}
jsvm_status jsvm_get_null(jsvm_env env, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_null(env, result));
}
jsvm_status jsvm_get_undefined(jsvm_env env, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_undefined(env, result));
}
jsvm_status jsvm_get_element(jsvm_env env, jsvm_value object, uint32_t index, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_element(env, object, index, result));
}
jsvm_status jsvm_set_element(jsvm_env env, jsvm_value object, uint32_t index, jsvm_value value)
{
    return static_cast<jsvm_status>(napi_set_element(env, object, index, value));
}
jsvm_status jsvm_create_string_utf8(jsvm_env env, const char* value, size_t length, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_string_utf8(env, value, length, result));
}
jsvm_status jsvm_get_value_string_utf8(jsvm_env env, jsvm_value value, char* buf, size_t bufsize, size_t* result)
{
    return static_cast<jsvm_status>(napi_get_value_string_utf8(env, value, buf, bufsize, result));
}
jsvm_status jsvm_adjust_external_memory(jsvm_env env, int64_t changeInBytes, int64_t* result)
{
    return static_cast<jsvm_status>(napi_adjust_external_memory(env, changeInBytes, result));
}
jsvm_status jsvm_is_set(jsvm_env env, jsvm_value value, bool* isSet)
{
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_create_set(jsvm_env env, jsvm_value* result)
{
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_get_cb_info(jsvm_env env, jsvm_callback_info cbinfo, size_t* argc, jsvm_value* argv, jsvm_value* thisArg, void** data)
{
    return static_cast<jsvm_status>(napi_get_cb_info(env, cbinfo, argc, argv, thisArg, data));
}
jsvm_status jsvm_get_new_target(jsvm_env env, jsvm_callback_info cbinfo, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_new_target(env, cbinfo, result));
}
jsvm_status jsvm_wrap(jsvm_env env, jsvm_value jsObject, void* nativeObject, jsvm_finalize finalizeCb, void* finalizeHint,
    jsvm_ref* result)
{
    return static_cast<jsvm_status>(napi_wrap(env, jsObject, nativeObject, finalizeCb, finalizeHint, result));
}
jsvm_status jsvm_unwrap(jsvm_env env, jsvm_value jsObject, void** result)
{
    return static_cast<jsvm_status>(napi_unwrap(env, jsObject, result));
}
jsvm_status jsvm_remove_wrap(jsvm_env env, jsvm_value jsObject, void** result)
{
    return static_cast<jsvm_status>(napi_remove_wrap(env, jsObject, result));
}
jsvm_status jsvm_create_reference(jsvm_env env, jsvm_value value, uint32_t initialRefcount, jsvm_ref* result)
{
    return static_cast<jsvm_status>(napi_create_reference(env, value, initialRefcount, result));
}
jsvm_status jsvm_delete_reference(jsvm_env env, jsvm_ref ref)
{
    return static_cast<jsvm_status>(napi_delete_reference(env, ref));
}
jsvm_status jsvm_reference_ref(jsvm_env env, jsvm_ref ref, uint32_t* result)
{
    return static_cast<jsvm_status>(napi_reference_ref(env, ref, result));
}
jsvm_status jsvm_reference_unref(jsvm_env env, jsvm_ref ref, uint32_t* result)
{
    return static_cast<jsvm_status>(napi_reference_unref(env, ref, result));
}
jsvm_status jsvm_get_reference_value(jsvm_env env, jsvm_ref ref, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_reference_value(env, ref, result));
}
jsvm_status jsvm_new_instance(jsvm_env env, jsvm_value constructor, size_t argc, const jsvm_value* argv, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_new_instance(env, constructor, argc, argv, result));
}
jsvm_status jsvm_is_exception_pending(jsvm_env env, bool* result)
{
    return static_cast<jsvm_status>(napi_is_exception_pending(env, result));
}

jsvm_status jsvm_define_class(jsvm_env env, const char* utf8name, size_t length, jsvm_callback constructor,
    size_t propertyCount, const jsvm_property_descriptor* properties, jsvm_value* result)
{
    std::vector<napi_property_descriptor> napi_properties;
    napi_properties.reserve(propertyCount);
    for (int i = 0; i < propertyCount; i++)
    {
        napi_property_descriptor property;
        property.utf8name = properties[i].utf8name;
        property.name = properties[i].name;
        property.method = properties[i].method;
        property.getter = properties[i].getter;
        property.setter = properties[i].setter;
        property.value = properties[i].value;
        property.attributes = static_cast<napi_property_attributes>(properties[i].attributes);
        property.data = properties[i].data;
        napi_properties.push_back(property);
    }

    return static_cast<jsvm_status>(
        napi_define_class(env, utf8name, length, constructor, nullptr, propertyCount, napi_properties.data(), result));
}
jsvm_status jsvm_define_properties(jsvm_env env, jsvm_value object, size_t propertyCount,
    const jsvm_property_descriptor* properties)
{
    std::vector<napi_property_descriptor> napi_properties;
    napi_properties.reserve(propertyCount);
    for (int i = 0; i < propertyCount; i++)
    {
        napi_property_descriptor property;
        property.utf8name = properties[i].utf8name;
        property.name = properties[i].name;
        property.method = properties[i].method;
        property.getter = properties[i].getter;
        property.setter = properties[i].setter;
        property.value = properties[i].value;
        property.attributes = static_cast<napi_property_attributes>(properties[i].attributes);
        property.data = properties[i].data;
        napi_properties.push_back(property);
    }
    return static_cast<jsvm_status>(napi_define_properties(env, object, propertyCount, napi_properties.data()));
}
jsvm_status jsvm_call_function(jsvm_env env, jsvm_value recv, jsvm_value func, size_t argc, const jsvm_value* argv, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_call_function(env, recv, func, argc, argv, result));
}
jsvm_status jsvm_create_function(jsvm_env env, const char* utf8name, size_t length, jsvm_callback cb, void* data,
    jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_function(env, utf8name, length, cb, data, result));
}
jsvm_status jsvm_typeof(jsvm_env env, jsvm_value value, jsvm_valuetype* result)
{

    napi_valuetype napi_result;
    auto status = napi_typeof(env, value, &napi_result);
    *result = static_cast<jsvm_valuetype>(napi_result);
    return static_cast<jsvm_status>(status);
}
jsvm_status jsvm_create_arraybuffer(jsvm_env env, size_t byteLength, void** data, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_arraybuffer(env, byteLength, data, result));
}
jsvm_status jsvm_is_arraybuffer(jsvm_env env, jsvm_value value, bool* result)
{
    return static_cast<jsvm_status>(napi_is_arraybuffer(env, value, result));
}
jsvm_status jsvm_is_typedarray(jsvm_env env, jsvm_value value, bool* result)
{
    return static_cast<jsvm_status>(napi_is_typedarray(env, value, result));
}
jsvm_status jsvm_is_dataview(jsvm_env env, jsvm_value value, bool* result)
{
    return static_cast<jsvm_status>(napi_is_dataview(env, value, result));
}
jsvm_status jsvm_create_typedarray(jsvm_env env, jsvm_typedarray_type type, size_t length, jsvm_value arraybuffer,
    size_t byteOffset, jsvm_value* result)
{
    return static_cast<jsvm_status>(
        napi_create_typedarray(env, static_cast<napi_typedarray_type>(type), length, arraybuffer, byteOffset, result));
}
jsvm_status jsvm_create_dataview(jsvm_env env, size_t length, jsvm_value arraybuffer, size_t byteOffset, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_dataview(env, length, arraybuffer, byteOffset, result));
}
jsvm_status jsvm_get_arraybuffer_info(jsvm_env env, jsvm_value arraybuffer, void** data, size_t* byteLength)
{
    return static_cast<jsvm_status>(napi_get_arraybuffer_info(env, arraybuffer, data, byteLength));
}
jsvm_status jsvm_get_typedarray_info(jsvm_env env, jsvm_value typedarray, jsvm_typedarray_type* type, size_t* length, void** data,
    jsvm_value* arraybuffer, size_t* byteOffset)
{
    napi_typedarray_type napi_type;
    auto status = static_cast<jsvm_status>(
        napi_get_typedarray_info(env, typedarray, &napi_type, length, data, arraybuffer, byteOffset));
    *type = static_cast<jsvm_typedarray_type>(napi_type);
    return status;
}
jsvm_status jsvm_get_dataview_info(jsvm_env env, jsvm_value dataview, size_t* bytelength, void** data, jsvm_value* arraybuffer,
    size_t* byteOffset)
{

    return static_cast<jsvm_status>(napi_get_dataview_info(env, dataview, bytelength, data, arraybuffer, byteOffset));
}
jsvm_status jsvm_get_global(jsvm_env env, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_global(env, result));
}
jsvm_status jsvm_set_instance_data(jsvm_env env, void* data, jsvm_finalize finalizeCb, void* finalizeHint)
{
    return static_cast<jsvm_status>(napi_set_instance_data(env, data, finalizeCb, finalizeHint));
}
jsvm_status jsvm_get_instance_data(jsvm_env env, void** data)
{
    return static_cast<jsvm_status>(napi_get_instance_data(env, data));
}
jsvm_status jsvm_create_date(jsvm_env env, double time, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_date(env, time, result));
}
jsvm_status jsvm_is_date(jsvm_env env, jsvm_value value, bool* isDate)
{
    return static_cast<jsvm_status>(napi_is_date(env, value, isDate));
}
jsvm_status jsvm_get_date_value(jsvm_env env, jsvm_value value, double* result)
{
    return static_cast<jsvm_status>(napi_get_date_value(env, value, result));
}
jsvm_status jsvm_is_error(jsvm_env env, jsvm_value value, bool* result)
{
    return static_cast<jsvm_status>(napi_is_error(env, value, result));
}
jsvm_status jsvm_create_string_utf16(jsvm_env env, const char16_t* str, size_t length, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_string_utf16(env, str, length, result));
}
jsvm_status jsvm_get_value_string_utf16(jsvm_env env, jsvm_value value, char16_t* buf, size_t bufsize, size_t* result)
{
    return static_cast<jsvm_status>(napi_get_value_string_utf16(env, value, buf, bufsize, result));
}
jsvm_status jsvm_create_string_latin1(jsvm_env env, const char* str, size_t length, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_string_latin1(env, str, length, result));
}
jsvm_status jsvm_get_value_string_latin1(jsvm_env env, jsvm_value value, char* buf, size_t bufsize, size_t* result)
{
    return static_cast<jsvm_status>(napi_get_value_string_latin1(env, value, buf, bufsize, result));
}
jsvm_status jsvm_create_bigint_int64(jsvm_env env, int64_t value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_bigint_int64(env, value, result));
}
jsvm_status jsvm_get_value_bigint_int64(jsvm_env env, jsvm_value value, int64_t* result, bool* lossless)
{
    return static_cast<jsvm_status>(napi_get_value_bigint_int64(env, value, result, lossless));
}
jsvm_status jsvm_create_bigint_uint64(jsvm_env env, uint64_t value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_bigint_uint64(env, value, result));
}
jsvm_status jsvm_get_value_bigint_uint64(jsvm_env env, jsvm_value value, uint64_t* result, bool* lossless)
{
    return static_cast<jsvm_status>(napi_get_value_bigint_uint64(env, value, result, lossless));
}
jsvm_status jsvm_run_script(jsvm_env env, jsvm_value script, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_run_script(env, script, result));
}
jsvm_status jsvm_pump_messageloop(jsvm_vm vm, bool* result)
{
    DEBUG_CHECK(vm->isolate_ != nullptr);
    *result = v8::platform::PumpMessageLoop(s_pPlatform, vm->isolate_, v8::platform::MessageLoopBehavior::kDoNotWait);
            
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_perform_microtask_checkpoint(jsvm_vm vm)
{

    DEBUG_CHECK(vm->isolate_ != nullptr);
    vm->isolate_->PerformMicrotaskCheckpoint();
    return jsvm_status::jsvm_ok; // todo
}
jsvm_status jsvm_get_property(jsvm_env env, jsvm_value object, jsvm_value key, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_property(env, object, key, result));
}
jsvm_status jsvm_set_named_property(jsvm_env env, jsvm_value object, const char* utf8name, jsvm_value value)
{
    return static_cast<jsvm_status>(napi_set_named_property(env, object, utf8name, value));
}
jsvm_status jsvm_get_named_property(jsvm_env env, jsvm_value object, const char* utf8name, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_named_property(env, object, utf8name, result));
}
jsvm_status jsvm_has_named_property(jsvm_env env, jsvm_value object, const char* utf8name, bool* result)
{
    return static_cast<jsvm_status>(napi_has_named_property(env, object, utf8name, result));
}
jsvm_status jsvm_create_object(jsvm_env env, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_object(env, result));
}
jsvm_status jsvm_resolve_deferred(jsvm_env env, jsvm_deferred deferred, jsvm_value resolution)
{
    return static_cast<jsvm_status>(napi_resolve_deferred(env, deferred, resolution));
}
jsvm_status jsvm_reject_deferred(jsvm_env env, jsvm_deferred deferred, jsvm_value resolution)
{
    return static_cast<jsvm_status>(napi_reject_deferred(env, deferred, resolution));
}
jsvm_status jsvm_create_external(jsvm_env env, void* data, jsvm_finalize finalizeCb, void* finalizeHint, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_create_external(env, data, finalizeCb, finalizeHint, result));
}
jsvm_status jsvm_get_value_external(jsvm_env env, jsvm_value value, void** result)
{
    return static_cast<jsvm_status>(napi_get_value_external(env, value, result));
}
jsvm_status jsvm_add_finalizer(jsvm_env env, jsvm_value jsObject, void* finalizeData, jsvm_finalize finalizeCb,
    void* finalizeHint, jsvm_ref* result)
{
    return static_cast<jsvm_status>(napi_add_finalizer(env, jsObject, finalizeData, finalizeCb, finalizeHint, result));
}
jsvm_status jsvm_coerce_to_bool(jsvm_env env, jsvm_value value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_coerce_to_bool(env, value, result));
}
jsvm_status jsvm_coerce_to_number(jsvm_env env, jsvm_value value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_coerce_to_number(env, value, result));
}
jsvm_status jsvm_coerce_to_object(jsvm_env env, jsvm_value value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_coerce_to_object(env, value, result));
}
jsvm_status jsvm_coerce_to_string(jsvm_env env, jsvm_value value, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_coerce_to_string(env, value, result));
}
jsvm_status jsvm_get_property_names(jsvm_env env, jsvm_value object, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_property_names(env, object, result));
}
static napi_status NAPI_CDECL napi_set_prototype(napi_env env, napi_value object, napi_value prototype)
{
    NAPI_PREAMBLE(env);

    v8::Local<v8::Context> context = env->context();

    v8::Local<v8::Object> obj;
    CHECK_TO_OBJECT(env, context, obj, object);

    v8::Local<v8::Value> a = v8impl::V8LocalValueFromJsValue(prototype);
    v8::Maybe<bool> set_maybe = obj->SetPrototype(context, a);
    RETURN_STATUS_IF_FALSE(env, set_maybe.FromMaybe(false), napi_generic_failure);
    return GET_RETURN_STATUS(env);
}
static napi_status NAPI_CDECL napi_object_set_prototype_of(napi_env env, napi_value object, napi_value prototype)
{
    NAPI_PREAMBLE(env);

    v8::Local<v8::Context> context = env->context();

    v8::Local<v8::Object> obj;
    CHECK_TO_OBJECT(env, context, obj, object);

    v8::Local<v8::Value> a = v8impl::V8LocalValueFromJsValue(prototype);
    v8::Maybe<bool> set_maybe = obj->SetPrototype(context, a);
    RETURN_STATUS_IF_FALSE(env, set_maybe.FromMaybe(false), napi_generic_failure);
    return GET_RETURN_STATUS(env);
}
jsvm_status jsvm_object_set_prototype_of(jsvm_env env, jsvm_value object, jsvm_value prototype)
{
    return static_cast<jsvm_status>(napi_object_set_prototype_of(env, object, prototype));
}
jsvm_status jsvm_object_get_prototype_of(jsvm_env env, jsvm_value object, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_prototype(env, object, result));
}
jsvm_status jsvm_get_prototype(jsvm_env env, jsvm_value object, jsvm_value* result)
{
    return static_cast<jsvm_status>(napi_get_prototype(env, object, result));
}
laya::DebuggerAgent *pDbgAgent = nullptr;
jsvm_status jsvm_open_inspector(jsvm_env env, int port)
{
#ifdef JS_V8_DEBUGGER
    // std::shared_ptr<jsvm::ScriptThread> scriptThread
    pDbgAgent = new laya::DebuggerAgent("layabox", port);
    pDbgAgent->onJSStart(env->scriptThread);
#endif
    return jsvm_status::jsvm_ok;
}
jsvm_status jsvm_wait_for_debugger(jsvm_env env, bool breakNextLine)
{
#ifdef JS_V8_DEBUGGER
    pDbgAgent->WaitForDebugger(breakNextLine);
#endif
    return jsvm_status::jsvm_ok;
}
// 关闭调试
jsvm_status jsvm_close_inspector(jsvm_env env)
{
#ifdef JS_V8_DEBUGGER
    if (pDbgAgent != nullptr)
    {
        pDbgAgent->Shutdown();
        delete pDbgAgent;
        pDbgAgent = nullptr;
    }
#endif
    return jsvm_status::jsvm_ok;
}
jsvm_status jsvm_get_last_error_info(jsvm_env env, const jsvm_extended_error_info **result)
{
    const napi_extended_error_info *info;
    jsvm_status status = static_cast<jsvm_status>(napi_get_last_error_info(env, &info));
    static jsvm_extended_error_info s_info;
    s_info.error_message = info->error_message;
    s_info.engine_reserved = info->engine_reserved;
    s_info.engine_error_code = info->engine_error_code;
    s_info.error_code = static_cast<jsvm_status>(info->error_code);

    *result = &s_info;
    return status;
}
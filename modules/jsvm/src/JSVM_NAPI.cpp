#include "jsvm/JSEnv.h"
#include "jsvm/JSVM.h"
#include "jsvm/ScriptThread.h"
#include "jsvm/napi/js_native_api.h"
#include "jsvm/napi/js_native_api_v8.h"
#include "v8debug/debug-agent.h"
#include <libplatform/libplatform.h>
#include <v8.h>

namespace jsvm
{
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

Status Init(const InitOptions *options)
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
    return Status::OK; // todo
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
        Ref();
        /////node_env()->SetImmediate([this](node::Environment* node_env) {
        ////    finalization_scheduled = false;
        ////    Unref();
        //////    DrainFinalizerQueue();
        ///////    });
    }
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
    /*CallIntoModule(call, [](napi_env env_, v8::Local<v8::Value> local_err) {
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
        });*/
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
Status CreateVM(const CreateVMOptions *options, VM *result)
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
    return Status::OK; // todo
}
Status DestroyVM(VM vm)
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
    return Status::OK; // todo
}
Status OpenVMScope(VM vm, VMScope *result)
{
    *result = new VMScope__;
    (*result)->isolate_ = vm->isolate_;
    (*result)->isolate_->Enter();
    return Status::OK; // todo
}
Status CloseVMScope(VM vm, VMScope scope)
{
    scope->isolate_ = vm->isolate_;
    scope->isolate_->Exit();
    return Status::OK; // todo
}
Status OpenEnvScope(Env env, EnvScope *result)
{
    env->context()->Enter();
    return Status::OK; // todo
}
Status CloseEnvScope(Env env, EnvScope scope)
{
    env->context()->Exit();
    return Status::OK; // todo
}
Status CreateEnv(VM vm, size_t propertyCount, const PropertyDescriptor *properties, Env *result)
{
    // todo properties
    v8::HandleScope handle_scope(vm->isolate_);
    v8::Local<v8::Context> context = v8::Context::New(vm->isolate_);
    *result = new LayaNapiEnv(context, NAPI_VERSION);

    jsvm::IsolateData *isolateData = new jsvm::IsolateData(vm->isolate_);     // delete ?
    jsvm::JSEnv *jsEnv = new jsvm::JSEnv(isolateData, vm->isolate_, *result); // delete ?
    jsvm::JSEnv::setCurrent(jsEnv);

    return Status::OK;
}
Status DestroyEnv(Env env)
{
    DEBUG_CHECK(env != nullptr);
    env->DeleteMe();
    return Status::OK; // todo
}

Status OpenHandleScope(Env env, HandleScope *result)
{
    return static_cast<Status>(napi_open_handle_scope(env, result));
}
Status CloseHandleScope(Env env, HandleScope scope)
{
    return static_cast<Status>(napi_close_handle_scope(env, scope));
}
Status GetAndClearLastException(Env env, Value *result)
{
    return static_cast<Status>(napi_get_and_clear_last_exception(env, result));
}
Status ThrowError(Env env, const char *code, const char *msg)
{
    return static_cast<Status>(napi_throw_error(env, code, msg));
}
Status GetArrayLength(Env env, Value value, uint32_t *result)
{
    return static_cast<Status>(napi_get_array_length(env, value, result));
}
Status IsArray(Env env, Value value, bool *result)
{
    return static_cast<Status>(napi_is_array(env, value, result));
}
Status CreatePromise(Env env, Deferred *deferred, Value *promise)
{
    return static_cast<Status>(napi_create_promise(env, deferred, promise));
}
Status CreateArray(Env env, Value *result)
{
    return static_cast<Status>(napi_create_array(env, result));
}
Status CreateArrayWithLength(Env env, size_t length, Value *result)
{
    return static_cast<Status>(napi_create_array_with_length(env, length, result));
}
Status CreateDouble(Env env, double value, Value *result)
{
    return static_cast<Status>(napi_create_double(env, value, result));
}
Status CreateInt32(Env env, int32_t value, Value *result)
{
    return static_cast<Status>(napi_create_int32(env, value, result));
}
Status CreateUint32(Env env, uint32_t value, Value *result)
{
    return static_cast<Status>(napi_create_uint32(env, value, result));
}
Status CreateInt64(Env env, int64_t value, Value *result)
{
    return static_cast<Status>(napi_create_int64(env, value, result));
}

Status GetValueDouble(Env env, Value value, double *result)
{
    return static_cast<Status>(napi_get_value_double(env, value, result));
}

Status GetValueInt32(Env env, Value value, int32_t *result)
{
    return static_cast<Status>(napi_get_value_int32(env, value, result));
}
Status GetValueUint32(Env env, Value value, uint32_t *result)
{
    return static_cast<Status>(napi_get_value_uint32(env, value, result));
}
Status GetValueInt64(Env env, Value value, int64_t *result)
{
    return static_cast<Status>(napi_get_value_int64(env, value, result));
}
Status GetValueBool(Env env, Value value, bool *result)
{
    return static_cast<Status>(napi_get_value_bool(env, value, result));
}
Status GetNull(Env env, Value *result)
{
    return static_cast<Status>(napi_get_null(env, result));
}
Status GetUndefined(Env env, Value *result)
{
    return static_cast<Status>(napi_get_undefined(env, result));
}
Status GetElement(Env env, Value object, uint32_t index, Value *result)
{
    return static_cast<Status>(napi_get_element(env, object, index, result));
}
Status SetElement(Env env, Value object, uint32_t index, Value value)
{
    return static_cast<Status>(napi_set_element(env, object, index, value));
}
Status CreateStringUtf8(Env env, const char *value, size_t length, Value *result)
{
    return static_cast<Status>(napi_create_string_utf8(env, value, length, result));
}
Status GetValueStringUtf8(Env env, Value value, char *buf, size_t bufsize, size_t *result)
{
    return static_cast<Status>(napi_get_value_string_utf8(env, value, buf, bufsize, result));
}
Status AdjustExternalMemory(Env env, int64_t changeInBytes, int64_t *adjustedValue)
{
    return static_cast<Status>(napi_adjust_external_memory(env, changeInBytes, adjustedValue));
}
Status IsSet(Env env, Value value, bool *isSet)
{
    return Status::OK; // todo
}
Status CreateSet(Env env, Value *result)
{
    return Status::OK; // todo
}
Status GetCbInfo(Env env, CallbackInfo cbinfo, size_t *argc, Value *argv, Value *thisArg, void **data)
{
    return static_cast<Status>(napi_get_cb_info(env, cbinfo, argc, argv, thisArg, data));
}
Status GetNewTarget(Env env, CallbackInfo cbinfo, Value *result)
{
    return static_cast<Status>(napi_get_new_target(env, cbinfo, result));
}
Status Wrap(Env env, Value jsObject, void *nativeObject, Finalize finalizeCb, void *finalizeHint, Ref *result)
{
    return static_cast<Status>(napi_wrap(env, jsObject, nativeObject, finalizeCb, finalizeHint, result));
}
Status Unwrap(Env env, Value jsObject, void **result)
{
    return static_cast<Status>(napi_unwrap(env, jsObject, result));
}
Status RemoveWrap(Env env, Value jsObject, void **result)
{
    return static_cast<Status>(napi_remove_wrap(env, jsObject, result));
}
Status CreateReference(Env env, Value value, uint32_t initialRefcount, Ref *result)
{
    return static_cast<Status>(napi_create_reference(env, value, initialRefcount, result));
}
Status DeleteReference(Env env, Ref ref)
{
    return static_cast<Status>(napi_delete_reference(env, ref));
}
Status ReferenceRef(Env env, Ref ref, uint32_t *result)
{
    return static_cast<Status>(napi_reference_ref(env, ref, result));
}
Status ReferenceUnref(Env env, Ref ref, uint32_t *result)
{
    return static_cast<Status>(napi_reference_unref(env, ref, result));
}
Status GetReferenceValue(Env env, Ref ref, Value *result)
{
    return static_cast<Status>(napi_get_reference_value(env, ref, result));
}
Status NewInstance(Env env, Value constructor, size_t argc, const Value *argv, Value *result)
{
    return static_cast<Status>(napi_new_instance(env, constructor, argc, argv, result));
}
Status DefineClass(Env env, const char *utf8name, size_t length, Callback constructor, size_t propertyCount,
                   const PropertyDescriptor *properties, Value *result)
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

    return static_cast<Status>(
        napi_define_class(env, utf8name, length, constructor, nullptr, propertyCount, napi_properties.data(), result));
}
Status DefineProperties(Env env, Value object, size_t propertyCount, const PropertyDescriptor *properties)
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
    return static_cast<Status>(napi_define_properties(env, object, propertyCount, napi_properties.data()));
}
Status CallFunction(Env env, Value recv, Value func, size_t argc, const Value *argv, Value *result)
{
    return static_cast<Status>(napi_call_function(env, recv, func, argc, argv, result));
}
Status CreateFunction(Env env, const char *utf8name, size_t length, Callback cb, void *data, Value *result)
{
    return static_cast<Status>(napi_create_function(env, utf8name, length, cb, data, result));
}
Status Typeof(Env env, Value value, ValueType *result)
{

    napi_valuetype napi_result;
    auto status = napi_typeof(env, value, &napi_result);
    *result = static_cast<ValueType>(napi_result);
    return static_cast<Status>(status);
}
Status CreateArraybuffer(Env env, size_t byteLength, void **data, Value *result)
{
    return static_cast<Status>(napi_create_arraybuffer(env, byteLength, data, result));
}
Status IsArraybuffer(Env env, Value value, bool *result)
{
    return static_cast<Status>(napi_is_arraybuffer(env, value, result));
}
Status IsTypedarray(Env env, Value value, bool *result)
{
    return static_cast<Status>(napi_is_typedarray(env, value, result));
}
Status IsDataview(Env env, Value value, bool *result)
{
    return static_cast<Status>(napi_is_dataview(env, value, result));
}
Status CreateTypedarray(Env env, TypedarrayType type, size_t length, Value arraybuffer, size_t byteOffset,
                        Value *result)
{
    return static_cast<Status>(
        napi_create_typedarray(env, static_cast<napi_typedarray_type>(type), length, arraybuffer, byteOffset, result));
}
Status CreateDataview(Env env, size_t length, Value arraybuffer, size_t byteOffset, Value *result)
{
    return static_cast<Status>(napi_create_dataview(env, length, arraybuffer, byteOffset, result));
}
Status GetArraybufferInfo(Env env, Value arraybuffer, void **data, size_t *byteLength)
{
    return static_cast<Status>(napi_get_arraybuffer_info(env, arraybuffer, data, byteLength));
}
Status GetTypedarrayInfo(Env env, Value typedarray, TypedarrayType *type, size_t *length, void **data,
                         Value *arraybuffer, size_t *byteOffset)
{
    napi_typedarray_type napi_type;
    auto status = static_cast<Status>(
        napi_get_typedarray_info(env, typedarray, &napi_type, length, data, arraybuffer, byteOffset));
    *type = static_cast<TypedarrayType>(napi_type);
    return status;
}
Status GetDataviewInfo(Env env, Value dataview, size_t *bytelength, void **data, Value *arraybuffer, size_t *byteOffset)
{

    return static_cast<Status>(napi_get_dataview_info(env, dataview, bytelength, data, arraybuffer, byteOffset));
}
Status GetGlobal(Env env, Value *result)
{
    return static_cast<Status>(napi_get_global(env, result));
}
Status SetInstanceData(Env env, void *data, Finalize finalizeCb, void *finalizeHint)
{
    return static_cast<Status>(napi_set_instance_data(env, data, finalizeCb, finalizeHint));
}
Status GetInstanceData(Env env, void **data)
{
    return static_cast<Status>(napi_get_instance_data(env, data));
}
Status CreateDate(Env env, double time, Value *result)
{
    return static_cast<Status>(napi_create_date(env, time, result));
}
Status IsDate(Env env, Value value, bool *isDate)
{
    return static_cast<Status>(napi_is_date(env, value, isDate));
}
Status GetDateValue(Env env, Value value, double *result)
{
    return static_cast<Status>(napi_get_date_value(env, value, result));
}
Status IsError(Env env, Value value, bool *result)
{
    return static_cast<Status>(napi_is_error(env, value, result));
}
Status CreateStringUtf16(Env env, const char16_t *str, size_t length, Value *result)
{
    return static_cast<Status>(napi_create_string_utf16(env, str, length, result));
}
Status GetValueStringUtf16(Env env, Value value, char16_t *buf, size_t bufsize, size_t *result)
{
    return static_cast<Status>(napi_get_value_string_utf16(env, value, buf, bufsize, result));
}
Status CreateStringLatin1(Env env, const char *str, size_t length, Value *result)
{
    return static_cast<Status>(napi_create_string_latin1(env, str, length, result));
}
Status GetValueStringLatin1(Env env, Value value, char *buf, size_t bufsize, size_t *result)
{
    return static_cast<Status>(napi_get_value_string_latin1(env, value, buf, bufsize, result));
}
Status CreateBigintInt64(Env env, int64_t value, Value *result)
{
    return static_cast<Status>(napi_create_bigint_int64(env, value, result));
}
Status GetValueBigintInt64(Env env, Value value, int64_t *result, bool *lossless)
{
    return static_cast<Status>(napi_get_value_bigint_int64(env, value, result, lossless));
}
Status CreateBigintUint64(Env env, uint64_t value, Value *result)
{
    return static_cast<Status>(napi_create_bigint_uint64(env, value, result));
}
Status GetValueBigintUint64(Env env, Value value, uint64_t *result, bool *lossless)
{
    return static_cast<Status>(napi_get_value_bigint_uint64(env, value, result, lossless));
}
Status RunScript(Env env, Value script, Value *result)
{
    return static_cast<Status>(napi_run_script(env, script, result));
}
Status PumpMessageLoop(VM vm, bool *result)
{
    return Status::OK; // todo
}
Status PerformMicrotaskCheckpoint(VM vm)
{
    return Status::OK; // todo
}
Status GetProperty(Env env, Value object, Value key, Value *result)
{
    return static_cast<Status>(napi_get_property(env, object, key, result));
}
Status SetNamedProperty(Env env, Value object, const char *utf8name, Value value)
{
    return static_cast<Status>(napi_set_named_property(env, object, utf8name, value));
}
Status GetNamedProperty(Env env, Value object, const char *utf8name, Value *result)
{
    return static_cast<Status>(napi_get_named_property(env, object, utf8name, result));
}
Status HasNamedProperty(Env env, Value object, const char *utf8name, bool *result)
{
    return static_cast<Status>(napi_has_named_property(env, object, utf8name, result));
}
Status CreateObject(Env env, Value *result)
{
    return static_cast<Status>(napi_create_object(env, result));
}
Status ResolveDeferred(Env env, Deferred deferred, Value resolution)
{
    return static_cast<Status>(napi_resolve_deferred(env, deferred, resolution));
}
Status RejectDeferred(Env env, Deferred deferred, Value resolution)
{
    return static_cast<Status>(napi_reject_deferred(env, deferred, resolution));
}
Status CreateExternal(Env env, void *data, Finalize finalizeCb, void *finalizeHint, Value *result)
{
    return static_cast<Status>(napi_create_external(env, data, finalizeCb, finalizeHint, result));
}
Status GetValueExternal(Env env, Value value, void **result)
{
    return static_cast<Status>(napi_get_value_external(env, value, result));
}
Status AddFinalizer(Env env, Value jsObject, void *finalizeData, Finalize finalizeCb, void *finalizeHint, Ref *result)
{
    return static_cast<Status>(napi_add_finalizer(env, jsObject, finalizeData, finalizeCb, finalizeHint, result));
}
Status CoerceToBool(Env env, Value value, Value *result)
{
    return static_cast<Status>(napi_coerce_to_bool(env, value, result));
}
Status CoerceToNumber(Env env, Value value, Value *result)
{
    return static_cast<Status>(napi_coerce_to_number(env, value, result));
}
Status CoerceToObject(Env env, Value value, Value *result)
{
    return static_cast<Status>(napi_coerce_to_object(env, value, result));
}
Status CoerceToString(Env env, Value value, Value *result)
{
    return static_cast<Status>(napi_coerce_to_string(env, value, result));
}
Status GetPropertyNames(Env env, Value object, Value *result)
{
    return static_cast<Status>(napi_get_property_names(env, object, result));
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
Status ObjectSetPrototypeOf(Env env, Value object, Value prototype)
{
    return static_cast<Status>(napi_object_set_prototype_of(env, object, prototype));
}
Status ObjectGetPrototypeOf(Env env, Value object,  Value *result)
{
    return static_cast<Status>(napi_get_prototype(env, object, result));
}
Status GetPrototype(Env env, Value object, Value *result)
{
    return static_cast<Status>(napi_get_prototype(env, object, result));
}
const char *ToCString(const v8::String::Utf8Value &value)
{
    return *value ? *value : "<string conversion failed>";
}
void ReportException(v8::Isolate *isolate, v8::Local<v8::Value> e)
{
    v8::HandleScope handle_scope(isolate);
    // v8::String::Utf8Value exception(isolate, try_catch->Exception());
    v8::String::Utf8Value exception(isolate, e);
    const char *exception_string = ToCString(exception);
    v8::Local<v8::Message> message; // = try_catch->Message();
    static char errInfo[2048];
    int curpos = 0;
    // if (message.IsEmpty())
    if (true)
    {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        int off = snprintf(errInfo, sizeof(errInfo), "%s\n", exception_string);
#if 0
        // 通知全局错误处理脚本
        std::string kBuf = "if(conch.onerror){conch.onerror('";
        kBuf += UrlEncode(exception_string);
        kBuf += "','undefined','undefined','undefined','";
        kBuf += UrlEncode(exception_string);
        kBuf += "');};";
        __JSRun::Run(kBuf.c_str());
#endif
    }
#if 0
    else
    {
        auto ctx = isolate->GetCurrentContext();
        v8::String::Utf8Value fnstr(isolate, message->GetScriptResourceName());
        const char* filename_string = ToCString(fnstr);
        v8::MaybeLocal<v8::String> source_line_maybe = message->GetSourceLine(ctx);
        v8::String::Utf8Value srclinestr(isolate, source_line_maybe.ToLocalChecked());
        const char* sourceline_string = ToCString(srclinestr);
        int linenum = message->GetLineNumber(ctx).FromJust();
        int start = message->GetStartColumn(ctx).FromMaybe(0);
        int end = message->GetEndColumn(ctx).FromMaybe(0);
        v8::ScriptOrigin origin = message->GetScriptOrigin();
        int lineoff = origin.LineOffset();
        int startcol = origin.ColumnOffset();
        if (start > startcol)
        {
            start -= startcol;
            end -= startcol;
        }

        // 错误行可能非常长，只取一部分
        char errLineSrc[128 + 1];
        if (strlen(sourceline_string) > 128)
        {
            int startoff = start > 50 ? (start - 50) : 0;
            start -= startoff;
            end -= startoff;
            if (end >= 128)
                end = 127;

            memcpy(errLineSrc, sourceline_string + startoff, 128);
            errLineSrc[128] = '\0';
            sourceline_string = errLineSrc;
        }
        curpos += snprintf(errInfo, sizeof(errInfo), "%s:%i:\n%s\n%s\n", filename_string, linenum, exception_string,
            sourceline_string);
        // 打印具体哪一行，哪一列
        if (curpos < sizeof(errInfo))
        {
            int st = curpos;
            int srclen = snprintf(errInfo + curpos, sizeof(errInfo) - curpos, "%s\n", sourceline_string);
            curpos += srclen;
            if (curpos < sizeof(errInfo))
            {
                for (int si = 0; si < srclen; si++)
                {
                    char& c = errInfo[st + si];
                    if (c != ' ' && c != '\t' && c != '\r')
                        c = ' ';
                    if (si >= start && si <= end)
                        c = '^';
                }
            }
        }
        curpos += snprintf(errInfo + curpos, sizeof(errInfo) - curpos, "\n");
        v8::Local<v8::Value> stack_trace_string;
        if (try_catch->StackTrace(ctx).ToLocal(&stack_trace_string) && stack_trace_string->IsString() &&
            v8::Local<v8::String>::Cast(stack_trace_string)->Length() > 0)
        {
            v8::String::Utf8Value stack_trace(isolate, stack_trace_string);
            const char* stack_trace_string = ToCString(stack_trace);
            if (curpos < sizeof(errInfo))
            {
                curpos += snprintf(errInfo + curpos, sizeof(errInfo) - curpos, "%s", stack_trace_string);
            }
        }
#endif
    // 通知全局错误处理脚本
#if 0
        std::string kBuf = "if(conch.onerror){conch.onerror('";
        kBuf += UrlEncode(exception_string);
        kBuf += "','";
        kBuf += UrlEncode(filename_string);
        kBuf += "','";
        // kBuf += std::to_string(linenum);
        std::ostringstream os;
        os << linenum;
        kBuf += os.str();
        kBuf += "','";
        kBuf += "undefined";
        kBuf += "','";
        kBuf += UrlEncode(errInfo);
        kBuf += "');};";
        __JSRun::Run(kBuf.c_str());
 }
#endif

    // if (gbAlertException)
    {
        // JSAlert(errInfo);
    }
    LOGE("==JSERROR:\n%s", errInfo);
}
Status ReportException(Env env)
{
    bool isExceptionPending;
    auto status = napi_is_exception_pending(env, &isExceptionPending);
    DEBUG_CHECK(status == napi_ok);

    if (isExceptionPending)
    {
        napi_value result = nullptr;
        status = napi_get_and_clear_last_exception(env, &result);
        DEBUG_CHECK(status == napi_ok);
        v8::Local<v8::Value> val = v8impl::V8LocalValueFromJsValue(result);
        ReportException(env->isolate, val);
    }

    return Status::OK; // todo
}

laya::DebuggerAgent *pDbgAgent;
void OpenInspector(Env env, int port)
{
#ifdef JS_V8_DEBUGGER
    // std::shared_ptr<jsvm::ScriptThread> scriptThread
    pDbgAgent = new laya::DebuggerAgent("layabox", port);
    pDbgAgent->onJSStart(env->scriptThread);
#endif
}
void WaitForDebugger(Env env, bool breakNextLine)
{
#ifdef JS_V8_DEBUGGER
    pDbgAgent->WaitForDebugger(breakNextLine);
#endif
}
// 关闭调试
void CloseInspector(Env env)
{
#ifdef JS_V8_DEBUGGER
    pDbgAgent->Shutdown();
    delete pDbgAgent;
    pDbgAgent = nullptr;
#endif
}

} // namespace jsvm
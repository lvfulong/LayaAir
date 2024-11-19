#include <jsvm/JSEnv.h>
#include <jsvm/JSVM.h>

namespace jsvm
{

static bool s_bV8InitializePlatform = false;

Status Init(const InitOptions *options)
{

    if (!s_bV8InitializePlatform)
    {
#if 0
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
#endif
        return static_cast<Status>(OH_JSVM_Init(options));
    }
    return Status::OK;
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

Status CreateVM(const CreateVMOptions *options, VM *result)
{
    return static_cast<Status>(OH_JSVM_CreateVM(options, result));
}
Status DestroyVM(VM vm)
{
    return static_cast<Status>(OH_JSVM_DestroyVM(vm));
}
Status OpenVMScope(VM vm, VMScope *result)
{
    return static_cast<Status>(OH_JSVM_OpenVMScope(vm, result));
}
Status CloseVMScope(VM vm, VMScope scope)
{
    return static_cast<Status>(OH_JSVM_CloseVMScope(vm, scope));
}
Status OpenEnvScope(Env env, EnvScope *result)
{
    return static_cast<Status>(OH_JSVM_OpenEnvScope(env, result));
}
Status CloseEnvScope(Env env, EnvScope scope)
{
    return static_cast<Status>(OH_JSVM_CloseEnvScope(env, scope));
}
Status CreateEnv(VM vm, size_t propertyCount, const PropertyDescriptor *properties, Env *result)
{

    // todo properties
    auto status = static_cast<Status>(OH_JSVM_CreateEnv(vm, 0, nullptr, result));

    if (status == Status::OK)
    {
        jsvm::IsolateData *isolateData = new jsvm::IsolateData();   // delete ?
        jsvm::JSEnv *jsEnv = new jsvm::JSEnv(isolateData, *result); // delete ?
        jsvm::JSEnv::setCurrent(jsEnv);
    }

    return status;
}
Status DestroyEnv(Env env)
{
    return static_cast<Status>(OH_JSVM_DestroyEnv(env));
}

Status OpenHandleScope(Env env, HandleScope *result)
{
    return static_cast<Status>(OH_JSVM_OpenHandleScope(env, result));
}
Status CloseHandleScope(Env env, HandleScope scope)
{
    return static_cast<Status>(OH_JSVM_CloseHandleScope(env, scope));
}
Status GetAndClearLastException(Env env, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetAndClearLastException(env, result));
}
Status ThrowError(Env env, const char *code, const char *msg)
{
    return static_cast<Status>(OH_JSVM_ThrowError(env, code, msg));
}
Status GetArrayLength(Env env, Value value, uint32_t *result)
{
    return static_cast<Status>(OH_JSVM_GetArrayLength(env, value, result));
}
Status IsArray(Env env, Value value, bool *result)
{
    return static_cast<Status>(OH_JSVM_IsArray(env, value, result));
}
Status CreatePromise(Env env, Deferred *deferred, Value *promise)
{
    return static_cast<Status>(OH_JSVM_CreatePromise(env, deferred, promise));
}
Status CreateArray(Env env, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateArray(env, result));
}
Status CreateArrayWithLength(Env env, size_t length, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateArrayWithLength(env, length, result));
}
Status CreateDouble(Env env, double value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateDouble(env, value, result));
}
Status CreateInt32(Env env, int32_t value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateInt32(env, value, result));
}
Status CreateUint32(Env env, uint32_t value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateUint32(env, value, result));
}
Status CreateInt64(Env env, int64_t value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateInt64(env, value, result));
}

Status GetValueDouble(Env env, Value value, double *result)
{
    return static_cast<Status>(OH_JSVM_GetValueDouble(env, value, result));
}

Status GetValueInt32(Env env, Value value, int32_t *result)
{
    return static_cast<Status>(OH_JSVM_GetValueInt32(env, value, result));
}
Status GetValueUint32(Env env, Value value, uint32_t *result)
{
    return static_cast<Status>(OH_JSVM_GetValueUint32(env, value, result));
}
Status GetValueInt64(Env env, Value value, int64_t *result)
{
    return static_cast<Status>(OH_JSVM_GetValueInt64(env, value, result));
}
Status GetValueBool(Env env, Value value, bool *result)
{
    return static_cast<Status>(OH_JSVM_GetValueBool(env, value, result));
}
Status GetNull(Env env, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetNull(env, result));
}
Status GetUndefined(Env env, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetUndefined(env, result));
}
Status GetElement(Env env, Value object, uint32_t index, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetElement(env, object, index, result));
}
Status SetElement(Env env, Value object, uint32_t index, Value value)
{
    return static_cast<Status>(OH_JSVM_SetElement(env, object, index, value));
}
Status CreateStringUtf8(Env env, const char *value, size_t length, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateStringUtf8(env, value, length, result));
}
Status GetValueStringUtf8(Env env, Value value, char *buf, size_t bufsize, size_t *result)
{
    return static_cast<Status>(OH_JSVM_GetValueStringUtf8(env, value, buf, bufsize, result));
}
Status AdjustExternalMemory(Env env, int64_t changeInBytes, int64_t *adjustedValue)
{
    return static_cast<Status>(OH_JSVM_AdjustExternalMemory(env, changeInBytes, adjustedValue));
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
    return static_cast<Status>(OH_JSVM_GetCbInfo(env, cbinfo, argc, argv, thisArg, data));
}
Status GetNewTarget(Env env, CallbackInfo cbinfo, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetNewTarget(env, cbinfo, result));
}
Status Wrap(Env env, Value jsObject, void *nativeObject, Finalize finalizeCb, void *finalizeHint, Ref *result)
{
    return static_cast<Status>(OH_JSVM_Wrap(env, jsObject, nativeObject, finalizeCb, finalizeHint, result));
}
Status Unwrap(Env env, Value jsObject, void **result)
{
    return static_cast<Status>(OH_JSVM_Unwrap(env, jsObject, result));
}
Status RemoveWrap(Env env, Value jsObject, void **result)
{
    return static_cast<Status>(OH_JSVM_RemoveWrap(env, jsObject, result));
}
Status CreateReference(Env env, Value value, uint32_t initialRefcount, Ref *result)
{
    return static_cast<Status>(OH_JSVM_CreateReference(env, value, initialRefcount, result));
}
Status DeleteReference(Env env, Ref ref)
{
    return static_cast<Status>(OH_JSVM_DeleteReference(env, ref));
}
Status ReferenceRef(Env env, Ref ref, uint32_t *result)
{
    return static_cast<Status>(OH_JSVM_ReferenceRef(env, ref, result));
}
Status ReferenceUnref(Env env, Ref ref, uint32_t *result)
{
    return static_cast<Status>(OH_JSVM_ReferenceUnref(env, ref, result));
}
Status GetReferenceValue(Env env, Ref ref, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetReferenceValue(env, ref, result));
}
Status NewInstance(Env env, Value constructor, size_t argc, const Value *argv, Value *result)
{
    return static_cast<Status>(OH_JSVM_NewInstance(env, constructor, argc, argv, result));
}
Status DefineClass(Env env, const char *utf8name, size_t length, Callback constructor, size_t propertyCount,
                   const PropertyDescriptor *properties, Value *result)
{
    auto jsenv = jsvm::JSEnv::getCurrent();
    DEBUG_CHECK(nullptr != jsenv);

    auto it = jsenv->jsvm_properties_map_.find(utf8name);
    if (it != jsenv->jsvm_properties_map_.end())
    {
        LOGW("warning: jsvm DefineClass % more than once", utf8name);
    }
    auto jsvm_properties = std::make_unique<std::vector<JSVM_PropertyDescriptor>>();
    jsvm_properties->reserve(propertyCount);

    auto jsvm_callbacks = std::make_unique<std::vector<JSVM_CallbackStruct>>();
    jsvm_callbacks->reserve(3 * propertyCount);

    for (int i = 0; i < propertyCount; i++)
    {
        JSVM_PropertyDescriptor property;
        memset(&property, 0, sizeof(property));
        property.utf8name = properties[i].utf8name;
        property.name = properties[i].name;
        if (properties[i].method != nullptr)
        {
            JSVM_CallbackStruct s;
            s.callback = properties[i].method;
            s.data = properties[i].data;
            jsvm_callbacks->push_back(s);
            property.method = &jsvm_callbacks->back();
        }
        else
        {
            property.method = nullptr;
        }

        if (properties[i].getter != nullptr)
        {
            JSVM_CallbackStruct s;
            s.callback = properties[i].getter;
            s.data = properties[i].data;
            jsvm_callbacks->push_back(s);
            property.getter = &jsvm_callbacks->back();
        }
        else
        {
            property.getter = nullptr;
        }
        if (properties[i].setter != nullptr)
        {
            JSVM_CallbackStruct s;
            s.callback = properties[i].setter;
            s.data = properties[i].data;
            jsvm_callbacks->push_back(s);
            property.setter = &jsvm_callbacks->back();
        }
        else
        {
            property.setter = nullptr;
        }
        property.value = properties[i].value;
        property.attributes = static_cast<JSVM_PropertyAttributes>(properties[i].attributes);
        jsvm_properties->push_back(property);
    }
    auto jsvm_constructor = std::make_unique<JSVM_CallbackStruct>();

    jsvm_constructor->callback = constructor;
    jsvm_constructor->data = nullptr;

    auto s = static_cast<Status>(OH_JSVM_DefineClass(env, utf8name, length, jsvm_constructor.get(), propertyCount,
                                                     jsvm_properties->data(), result));
    jsenv->jsvm_properties_map_[utf8name] = std::move(jsvm_properties);
    jsenv->jsvm_callbacks_map_[utf8name] = std::move(jsvm_callbacks);
    jsenv->jsvm_constructor_map_[utf8name] = std::move(jsvm_constructor);
    return s;
}
Status DefineProperties(Env env, Value object, size_t propertyCount, const PropertyDescriptor *properties)
{
    auto jsenv = jsvm::JSEnv::getCurrent();
    DEBUG_CHECK(nullptr != jsenv);

    auto it = jsenv->jsvm_object_properties_map_.find(object);
    if (it != jsenv->jsvm_object_properties_map_.end())
    {
        LOGW("warning: jsvm DefineProperties %p more than once", object);
    }
    auto jsvm_properties = std::make_unique<std::vector<JSVM_PropertyDescriptor>>();
    jsvm_properties->reserve(propertyCount);

    auto jsvm_callbacks = std::make_unique<std::vector<JSVM_CallbackStruct>>();
    jsvm_callbacks->reserve(3 * propertyCount);

    for (int i = 0; i < propertyCount; i++)
    {
        JSVM_PropertyDescriptor property;
        memset(&property, 0, sizeof(property));
        property.utf8name = properties[i].utf8name;
        property.name = properties[i].name;
        if (properties[i].method != nullptr)
        {
            JSVM_CallbackStruct s;
            s.data = properties[i].data;
            s.callback = properties[i].method;
            jsvm_callbacks->push_back(s);
            property.method = &jsvm_callbacks->back();
        }
        else
        {
            property.method = nullptr;
        }
        if (properties[i].getter != nullptr)
        {
            JSVM_CallbackStruct s;
            s.data = properties[i].data;
            s.callback = properties[i].getter;
            jsvm_callbacks->push_back(s);
            property.getter = &jsvm_callbacks->back();
        }
        else
        {
            property.getter = nullptr;
        }

        if (properties[i].setter != nullptr)
        {
            JSVM_CallbackStruct s;
            s.data = properties[i].data;
            s.callback = properties[i].setter;
            jsvm_callbacks->push_back(s);
            property.setter = &jsvm_callbacks->back();
        }
        else
        {
            property.setter = nullptr;
        }
        property.value = properties[i].value;
        property.attributes = static_cast<JSVM_PropertyAttributes>(properties[i].attributes);
        jsvm_properties->push_back(property);
    }
    auto s = static_cast<Status>(OH_JSVM_DefineProperties(env, object, propertyCount, jsvm_properties->data()));
    jsenv->jsvm_object_callbacks_map_[object] = std::move(jsvm_callbacks);
    jsenv->jsvm_object_properties_map_[object] = std::move(jsvm_properties);
    return s;
}
Status CallFunction(Env env, Value recv, Value func, size_t argc, const Value *argv, Value *result)
{
    return static_cast<Status>(OH_JSVM_CallFunction(env, recv, func, argc, argv, result));
}
Status CreateFunction(Env env, const char *utf8name, size_t length, Callback cb, void *data, Value *result)
{
    JSVM_Callback jsvmCallback;
    jsvmCallback->callback = cb;
    jsvmCallback->data = data;
    // todo
    return static_cast<Status>(OH_JSVM_CreateFunction(env, utf8name, length, jsvmCallback, result));
}
Status Typeof(Env env, Value value, ValueType *result)
{

    JSVM_ValueType napi_result;
    auto status = OH_JSVM_Typeof(env, value, &napi_result);
    *result = static_cast<ValueType>(napi_result);
    return static_cast<Status>(status);
}
Status CreateArraybuffer(Env env, size_t byteLength, void **data, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateArraybuffer(env, byteLength, data, result));
}
Status IsArraybuffer(Env env, Value value, bool *result)
{
    return static_cast<Status>(OH_JSVM_IsArraybuffer(env, value, result));
}
Status IsTypedarray(Env env, Value value, bool *result)
{
    return static_cast<Status>(OH_JSVM_IsTypedarray(env, value, result));
}
Status IsDataview(Env env, Value value, bool *result)
{
    return static_cast<Status>(OH_JSVM_IsDataview(env, value, result));
}
Status CreateTypedarray(Env env, TypedarrayType type, size_t length, Value arraybuffer, size_t byteOffset,
                        Value *result)
{
    return static_cast<Status>(
        OH_JSVM_CreateTypedarray(env, static_cast<JSVM_TypedarrayType>(type), length, arraybuffer, byteOffset, result));
}
Status CreateDataview(Env env, size_t length, Value arraybuffer, size_t byteOffset, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateDataview(env, length, arraybuffer, byteOffset, result));
}
Status GetArraybufferInfo(Env env, Value arraybuffer, void **data, size_t *byteLength)
{
    return static_cast<Status>(OH_JSVM_GetArraybufferInfo(env, arraybuffer, data, byteLength));
}
Status GetTypedarrayInfo(Env env, Value typedarray, TypedarrayType *type, size_t *length, void **data,
                         Value *arraybuffer, size_t *byteOffset)
{
    JSVM_TypedarrayType napi_type;
    auto status = static_cast<Status>(
        OH_JSVM_GetTypedarrayInfo(env, typedarray, &napi_type, length, data, arraybuffer, byteOffset));
    *type = static_cast<TypedarrayType>(napi_type);
    return status;
}
Status GetDataviewInfo(Env env, Value dataview, size_t *bytelength, void **data, Value *arraybuffer, size_t *byteOffset)
{

    return static_cast<Status>(OH_JSVM_GetDataviewInfo(env, dataview, bytelength, data, arraybuffer, byteOffset));
}
Status GetGlobal(Env env, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetGlobal(env, result));
}
Status SetInstanceData(Env env, void *data, Finalize finalizeCb, void *finalizeHint)
{
    return static_cast<Status>(OH_JSVM_SetInstanceData(env, data, finalizeCb, finalizeHint));
}
Status GetInstanceData(Env env, void **data)
{
    return static_cast<Status>(OH_JSVM_GetInstanceData(env, data));
}
Status CreateDate(Env env, double time, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateDate(env, time, result));
}
Status IsDate(Env env, Value value, bool *isDate)
{
    return static_cast<Status>(OH_JSVM_IsDate(env, value, isDate));
}
Status GetDateValue(Env env, Value value, double *result)
{
    return static_cast<Status>(OH_JSVM_GetDateValue(env, value, result));
}
Status IsError(Env env, Value value, bool *result)
{
    return static_cast<Status>(OH_JSVM_IsError(env, value, result));
}
Status CreateStringUtf16(Env env, const char16_t *str, size_t length, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateStringUtf16(env, str, length, result));
}
Status GetValueStringUtf16(Env env, Value value, char16_t *buf, size_t bufsize, size_t *result)
{
    return static_cast<Status>(OH_JSVM_GetValueStringUtf16(env, value, buf, bufsize, result));
}
Status CreateStringLatin1(Env env, const char *str, size_t length, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateStringLatin1(env, str, length, result));
}
Status GetValueStringLatin1(Env env, Value value, char *buf, size_t bufsize, size_t *result)
{
    return static_cast<Status>(OH_JSVM_GetValueStringLatin1(env, value, buf, bufsize, result));
}
Status CreateBigintInt64(Env env, int64_t value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateBigintInt64(env, value, result));
}
Status GetValueBigintInt64(Env env, Value value, int64_t *result, bool *lossless)
{
    return static_cast<Status>(OH_JSVM_GetValueBigintInt64(env, value, result, lossless));
}
Status CreateBigintUint64(Env env, uint64_t value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateBigintUint64(env, value, result));
}
Status GetValueBigintUint64(Env env, Value value, uint64_t *result, bool *lossless)
{
    return static_cast<Status>(OH_JSVM_GetValueBigintUint64(env, value, result, lossless));
}
Status RunScript(Env env, Value script, Value *result)
{
    JSVM_Script jsvm_script;
    JSVM_Status status = OH_JSVM_CompileScript(env, script, nullptr, 0, true, nullptr, &jsvm_script);
    if (status != JSVM_OK)
    {
        // return napi_set_last_error((env), JSVM_GENERIC_FAILURE);
        return static_cast<Status>(JSVM_GENERIC_FAILURE); // lvtodo throw ???
    }
    status = OH_JSVM_RunScript(env, jsvm_script, result);
    return static_cast<Status>(status);
}
Status PumpMessageLoop(VM vm, bool *result)
{
    return static_cast<Status>(OH_JSVM_PumpMessageLoop(vm, result));
}
Status PerformMicrotaskCheckpoint(VM vm)
{
    return static_cast<Status>(OH_JSVM_PerformMicrotaskCheckpoint(vm));
}
Status GetProperty(Env env, Value object, Value key, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetProperty(env, object, key, result));
}
Status SetNamedProperty(Env env, Value object, const char *utf8name, Value value)
{
    return static_cast<Status>(OH_JSVM_SetNamedProperty(env, object, utf8name, value));
}
Status GetNamedProperty(Env env, Value object, const char *utf8name, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetNamedProperty(env, object, utf8name, result));
}
Status HasNamedProperty(Env env, Value object, const char *utf8name, bool *result)
{
    return static_cast<Status>(OH_JSVM_HasNamedProperty(env, object, utf8name, result));
}
Status CreateObject(Env env, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateObject(env, result));
}
Status ResolveDeferred(Env env, Deferred deferred, Value resolution)
{
    return static_cast<Status>(OH_JSVM_ResolveDeferred(env, deferred, resolution));
}
Status RejectDeferred(Env env, Deferred deferred, Value resolution)
{
    return static_cast<Status>(OH_JSVM_RejectDeferred(env, deferred, resolution));
}
Status CreateExternal(Env env, void *data, Finalize finalizeCb, void *finalizeHint, Value *result)
{
    return static_cast<Status>(OH_JSVM_CreateExternal(env, data, finalizeCb, finalizeHint, result));
}
Status GetValueExternal(Env env, Value value, void **result)
{
    return static_cast<Status>(OH_JSVM_GetValueExternal(env, value, result));
}
Status AddFinalizer(Env env, Value jsObject, void *finalizeData, Finalize finalizeCb, void *finalizeHint, Ref *result)
{
    return static_cast<Status>(OH_JSVM_AddFinalizer(env, jsObject, finalizeData, finalizeCb, finalizeHint, result));
}
Status CoerceToBool(Env env, Value value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CoerceToBool(env, value, result));
}
Status CoerceToNumber(Env env, Value value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CoerceToNumber(env, value, result));
}
Status CoerceToObject(Env env, Value value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CoerceToObject(env, value, result));
}
Status CoerceToString(Env env, Value value, Value *result)
{
    return static_cast<Status>(OH_JSVM_CoerceToString(env, value, result));
}
Status GetPropertyNames(Env env, Value object, Value *result)
{
    return static_cast<Status>(OH_JSVM_GetPropertyNames(env, object, result));
}

Status ReportException(Env env)
{
#if 0
    bool isExceptionPending;
    auto status = napi_is_exception_pending(env, &isExceptionPending);
    DEBUG_CHECK(status == napi_ok);

    if (isExceptionPending)
    {
        napi_value result = nullptr;
        status = napi_get_and_clear_last_exception(env, &result);
        DEBUG_CHECK(status == napi_ok);
        //v8::Local<v8::Value> val = v8impl::V8LocalValueFromJsValue(result);
        //ReportException(env->isolate, val);
    }
#endif
    return Status::OK; // todo
}

} // namespace jsvm
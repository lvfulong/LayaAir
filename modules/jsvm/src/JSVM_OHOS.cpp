#include <jsvm/JSEnv.h>
#include <jsvm/JSVM.h>

static bool s_bV8InitializePlatform = false;

jsvm_status jsvm_init(const jsvm_init_options *options)
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
        return static_cast<jsvm_status>(OH_JSVM_Init(options));
    }
    return jsvm_status::jsvm_ok;
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

jsvm_status jsvm_create_vm(const jsvm_create_vm_options *options, jsvm_vm *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateVM(options, result));
}
jsvm_status jsvm_destroy_vm(jsvm_vm vm)
{
    return static_cast<jsvm_status>(OH_JSVM_DestroyVM(vm));
}
jsvm_status jsvm_open_vm_scope(jsvm_vm vm, jsvm_vm_scope *result)
{
    return static_cast<jsvm_status>(OH_JSVM_OpenVMScope(vm, result));
}
jsvm_status jsvm_close_vm_scope(jsvm_vm vm, jsvm_vm_scope scope)
{
    return static_cast<jsvm_status>(OH_JSVM_CloseVMScope(vm, scope));
}
jsvm_status jsvm_open_env_scope(jsvm_env env, jsvm_env_scope *result)
{
    return static_cast<jsvm_status>(OH_JSVM_OpenEnvScope(env, result));
}
jsvm_status jsvm_close_env_scope(jsvm_env env, jsvm_env_scope scope)
{
    return static_cast<jsvm_status>(OH_JSVM_CloseEnvScope(env, scope));
}
jsvm_status jsvm_create_env(jsvm_vm vm, size_t propertyCount, const jsvm_property_descriptor *properties,
                            jsvm_env *result)
{

    // todo properties
    auto status = static_cast<jsvm_status>(OH_JSVM_CreateEnv(vm, 0, nullptr, result));

    if (status == jsvm_status::jsvm_ok)
    {
        jsvm::IsolateData *isolateData = new jsvm::IsolateData();   // delete ?
        jsvm::JSEnv *jsEnv = new jsvm::JSEnv(isolateData, *result); // delete ?
        jsvm::JSEnv::setCurrent(jsEnv);
    }

    return status;
}
jsvm_status jsvm_destroy_env(jsvm_env env)
{
    return static_cast<jsvm_status>(OH_JSVM_DestroyEnv(env));
}

jsvm_status jsvm_open_handle_scope(jsvm_env env, jsvm_handle_scope *result)
{
    return static_cast<jsvm_status>(OH_JSVM_OpenHandleScope(env, result));
}
jsvm_status jsvm_close_handle_scope(jsvm_env env, jsvm_handle_scope scope)
{
    return static_cast<jsvm_status>(OH_JSVM_CloseHandleScope(env, scope));
}
jsvm_status jsvm_get_and_clear_last_exception(jsvm_env env, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetAndClearLastException(env, result));
}
jsvm_status jsvm_throw_error(jsvm_env env, const char *code, const char *msg)
{
    return static_cast<jsvm_status>(OH_JSVM_ThrowError(env, code, msg));
}
jsvm_status jsvm_get_array_length(jsvm_env env, jsvm_value value, uint32_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetArrayLength(env, value, result));
}
jsvm_status jsvm_is_array(jsvm_env env, jsvm_value value, bool *result)
{
    return static_cast<jsvm_status>(OH_JSVM_IsArray(env, value, result));
}
jsvm_status jsvm_create_promise(jsvm_env env, jsvm_deferred *deferred, jsvm_value *promise)
{
    return static_cast<jsvm_status>(OH_JSVM_CreatePromise(env, deferred, promise));
}
jsvm_status jsvm_create_array(jsvm_env env, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateArray(env, result));
}
jsvm_status jsvm_create_array_with_length(jsvm_env env, size_t length, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateArrayWithLength(env, length, result));
}
jsvm_status jsvm_create_double(jsvm_env env, double value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateDouble(env, value, result));
}
jsvm_status jsvm_create_int32(jsvm_env env, int32_t value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateInt32(env, value, result));
}
jsvm_status jsvm_create_uint32(jsvm_env env, uint32_t value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateUint32(env, value, result));
}
jsvm_status jsvm_create_int64(jsvm_env env, int64_t value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateInt64(env, value, result));
}

jsvm_status jsvm_get_value_double(jsvm_env env, jsvm_value value, double *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueDouble(env, value, result));
}

jsvm_status jsvm_get_value_int32(jsvm_env env, jsvm_value value, int32_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueInt32(env, value, result));
}
jsvm_status jsvm_get_value_uint32(jsvm_env env, jsvm_value value, uint32_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueUint32(env, value, result));
}
jsvm_status jsvm_get_value_int64(jsvm_env env, jsvm_value value, int64_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueInt64(env, value, result));
}
jsvm_status jsvm_get_value_bool(jsvm_env env, jsvm_value value, bool *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueBool(env, value, result));
}
jsvm_status jsvm_get_null(jsvm_env env, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetNull(env, result));
}
jsvm_status jsvm_get_undefined(jsvm_env env, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetUndefined(env, result));
}
jsvm_status jsvm_get_element(jsvm_env env, jsvm_value object, uint32_t index, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetElement(env, object, index, result));
}
jsvm_status jsvm_set_element(jsvm_env env, jsvm_value object, uint32_t index, jsvm_value value)
{
    return static_cast<jsvm_status>(OH_JSVM_SetElement(env, object, index, value));
}
jsvm_status jsvm_create_string_utf8(jsvm_env env, const char *value, size_t length, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateStringUtf8(env, value, length, result));
}
jsvm_status jsvm_get_value_string_utf8(jsvm_env env, jsvm_value value, char *buf, size_t bufsize, size_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueStringUtf8(env, value, buf, bufsize, result));
}
jsvm_status jsvm_adjust_external_memory(jsvm_env env, int64_t changeInBytes, int64_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_AdjustExternalMemory(env, changeInBytes, result));
}
jsvm_status jsvm_is_set(jsvm_env env, jsvm_value value, bool *isSet)
{
    return static_cast<jsvm_status>(OH_JSVM_IsSet(env, value, isSet));
}
jsvm_status jsvm_create_set(jsvm_env env, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateSet(env, result));
}
jsvm_status jsvm_get_cb_info(jsvm_env env, jsvm_callback_info cbinfo, size_t *argc, jsvm_value *argv,
                             jsvm_value *thisArg, void **data)
{
    return static_cast<jsvm_status>(OH_JSVM_GetCbInfo(env, cbinfo, argc, argv, thisArg, data));
}
jsvm_status jsvm_get_new_target(jsvm_env env, jsvm_callback_info cbinfo, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetNewTarget(env, cbinfo, result));
}
jsvm_status jsvm_wrap(jsvm_env env, jsvm_value jsObject, void *nativeObject, jsvm_finalize finalizeCb,
                      void *finalizeHint, jsvm_ref *result)
{
    return static_cast<jsvm_status>(OH_JSVM_Wrap(env, jsObject, nativeObject, finalizeCb, finalizeHint, result));
}
jsvm_status jsvm_unwrap(jsvm_env env, jsvm_value jsObject, void **result)
{
    return static_cast<jsvm_status>(OH_JSVM_Unwrap(env, jsObject, result));
}
jsvm_status jsvm_remove_wrap(jsvm_env env, jsvm_value jsObject, void **result)
{
    return static_cast<jsvm_status>(OH_JSVM_RemoveWrap(env, jsObject, result));
}
jsvm_status jsvm_create_reference(jsvm_env env, jsvm_value value, uint32_t initialRefcount, jsvm_ref *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateReference(env, value, initialRefcount, result));
}
jsvm_status jsvm_delete_reference(jsvm_env env, jsvm_ref ref)
{
    return static_cast<jsvm_status>(OH_JSVM_DeleteReference(env, ref));
}
jsvm_status jsvm_reference_ref(jsvm_env env, jsvm_ref ref, uint32_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_ReferenceRef(env, ref, result));
}
jsvm_status jsvm_reference_unref(jsvm_env env, jsvm_ref ref, uint32_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_ReferenceUnref(env, ref, result));
}
jsvm_status jsvm_get_reference_value(jsvm_env env, jsvm_ref ref, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetReferenceValue(env, ref, result));
}
jsvm_status jsvm_new_instance(jsvm_env env, jsvm_value constructor, size_t argc, const jsvm_value *argv,
                              jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_NewInstance(env, constructor, argc, argv, result));
}
jsvm_status jsvm_define_class(jsvm_env env, const char *utf8name, size_t length, jsvm_callback constructor,
                              size_t propertyCount, const jsvm_property_descriptor *properties, jsvm_value *result)
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

    auto s = static_cast<jsvm_status>(OH_JSVM_DefineClass(env, utf8name, length, jsvm_constructor.get(), propertyCount,
                                                          jsvm_properties->data(), result));
    jsenv->jsvm_properties_map_[utf8name] = std::move(jsvm_properties);
    jsenv->jsvm_callbacks_map_[utf8name] = std::move(jsvm_callbacks);
    jsenv->jsvm_constructor_map_[utf8name] = std::move(jsvm_constructor);
    return s;
}
jsvm_status jsvm_define_properties(jsvm_env env, jsvm_value object, size_t propertyCount,
                                   const jsvm_property_descriptor *properties)
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
    auto s = static_cast<jsvm_status>(OH_JSVM_DefineProperties(env, object, propertyCount, jsvm_properties->data()));
    jsenv->jsvm_object_callbacks_map_[object] = std::move(jsvm_callbacks);
    jsenv->jsvm_object_properties_map_[object] = std::move(jsvm_properties);
    return s;
}
jsvm_status jsvm_call_function(jsvm_env env, jsvm_value recv, jsvm_value func, size_t argc, const jsvm_value *argv,
                               jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CallFunction(env, recv, func, argc, argv, result));
}
jsvm_status jsvm_create_function(jsvm_env env, const char *utf8name, size_t length, jsvm_callback cb, void *data,
                                 jsvm_value *result)
{
    JSVM_Callback jsvmCallback = new JSVM_CallbackStruct;
    jsvmCallback->callback = cb;
    jsvmCallback->data = data;

    auto status = static_cast<jsvm_status>(OH_JSVM_CreateFunction(env, utf8name, length, jsvmCallback, result));
    if (status != jsvm_status::jsvm_ok)
    {
        return status;
    }
    OH_JSVM_AddFinalizer(
        env, *result, reinterpret_cast<void *>(jsvmCallback),
        [](JSVM_Env env, void *data, void *hint) -> void { delete static_cast<JSVM_Callback>(data); }, nullptr,
        nullptr);
    return status;
}
jsvm_status jsvm_typeof(jsvm_env env, jsvm_value value, jsvm_valuetype *result)
{

    JSVM_ValueType napi_result;
    auto status = OH_JSVM_Typeof(env, value, &napi_result);
    *result = static_cast<jsvm_valuetype>(napi_result);
    return static_cast<jsvm_status>(status);
}
jsvm_status jsvm_create_arraybuffer(jsvm_env env, size_t byteLength, void **data, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateArraybuffer(env, byteLength, data, result));
}
jsvm_status jsvm_is_arraybuffer(jsvm_env env, jsvm_value value, bool *result)
{
    return static_cast<jsvm_status>(OH_JSVM_IsArraybuffer(env, value, result));
}
jsvm_status jsvm_is_typedarray(jsvm_env env, jsvm_value value, bool *result)
{
    return static_cast<jsvm_status>(OH_JSVM_IsTypedarray(env, value, result));
}
jsvm_status jsvm_is_dataview(jsvm_env env, jsvm_value value, bool *result)
{
    return static_cast<jsvm_status>(OH_JSVM_IsDataview(env, value, result));
}
jsvm_status jsvm_create_typedarray(jsvm_env env, jsvm_typedarray_type type, size_t length, jsvm_value arraybuffer,
                                   size_t byteOffset, jsvm_value *result)
{
    return static_cast<jsvm_status>(
        OH_JSVM_CreateTypedarray(env, static_cast<JSVM_TypedarrayType>(type), length, arraybuffer, byteOffset, result));
}
jsvm_status jsvm_create_dataview(jsvm_env env, size_t length, jsvm_value arraybuffer, size_t byteOffset,
                                 jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateDataview(env, length, arraybuffer, byteOffset, result));
}
jsvm_status jsvm_get_arraybuffer_info(jsvm_env env, jsvm_value arraybuffer, void **data, size_t *byteLength)
{
    return static_cast<jsvm_status>(OH_JSVM_GetArraybufferInfo(env, arraybuffer, data, byteLength));
}
jsvm_status jsvm_get_typedarray_info(jsvm_env env, jsvm_value typedarray, jsvm_typedarray_type *type, size_t *length,
                                     void **data, jsvm_value *arraybuffer, size_t *byteOffset)
{
    JSVM_TypedarrayType napi_type;
    auto status = static_cast<jsvm_status>(
        OH_JSVM_GetTypedarrayInfo(env, typedarray, &napi_type, length, data, arraybuffer, byteOffset));
    *type = static_cast<jsvm_typedarray_type>(napi_type);
    return status;
}
jsvm_status jsvm_get_dataview_info(jsvm_env env, jsvm_value dataview, size_t *bytelength, void **data,
                                   jsvm_value *arraybuffer, size_t *byteOffset)
{

    return static_cast<jsvm_status>(OH_JSVM_GetDataviewInfo(env, dataview, bytelength, data, arraybuffer, byteOffset));
}
jsvm_status jsvm_get_global(jsvm_env env, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetGlobal(env, result));
}
jsvm_status jsvm_set_instance_data(jsvm_env env, void *data, jsvm_finalize finalizeCb, void *finalizeHint)
{
    return static_cast<jsvm_status>(OH_JSVM_SetInstanceData(env, data, finalizeCb, finalizeHint));
}
jsvm_status jsvm_get_instance_data(jsvm_env env, void **data)
{
    return static_cast<jsvm_status>(OH_JSVM_GetInstanceData(env, data));
}
jsvm_status jsvm_create_date(jsvm_env env, double time, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateDate(env, time, result));
}
jsvm_status jsvm_is_date(jsvm_env env, jsvm_value value, bool *isDate)
{
    return static_cast<jsvm_status>(OH_JSVM_IsDate(env, value, isDate));
}
jsvm_status jsvm_get_date_value(jsvm_env env, jsvm_value value, double *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetDateValue(env, value, result));
}
jsvm_status jsvm_is_error(jsvm_env env, jsvm_value value, bool *result)
{
    return static_cast<jsvm_status>(OH_JSVM_IsError(env, value, result));
}
jsvm_status jsvm_create_string_utf16(jsvm_env env, const char16_t *str, size_t length, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateStringUtf16(env, str, length, result));
}
jsvm_status jsvm_get_value_string_utf16(jsvm_env env, jsvm_value value, char16_t *buf, size_t bufsize, size_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueStringUtf16(env, value, buf, bufsize, result));
}
jsvm_status jsvm_create_string_latin1(jsvm_env env, const char *str, size_t length, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateStringLatin1(env, str, length, result));
}
jsvm_status jsvm_get_value_string_latin1(jsvm_env env, jsvm_value value, char *buf, size_t bufsize, size_t *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueStringLatin1(env, value, buf, bufsize, result));
}
jsvm_status jsvm_create_bigint_int64(jsvm_env env, int64_t value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateBigintInt64(env, value, result));
}
jsvm_status jsvm_get_value_bigint_int64(jsvm_env env, jsvm_value value, int64_t *result, bool *lossless)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueBigintInt64(env, value, result, lossless));
}
jsvm_status jsvm_create_bigint_uint64(jsvm_env env, uint64_t value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateBigintUint64(env, value, result));
}
jsvm_status jsvm_get_value_bigint_uint64(jsvm_env env, jsvm_value value, uint64_t *result, bool *lossless)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueBigintUint64(env, value, result, lossless));
}
jsvm_status jsvm_run_script(jsvm_env env, jsvm_value script, jsvm_value *result)
{
    JSVM_Script jsvm_script;
    JSVM_Status status = OH_JSVM_CompileScript(env, script, nullptr, 0, true, nullptr, &jsvm_script);
    if (status != JSVM_OK)
    {
        // return napi_set_last_error((env), JSVM_GENERIC_FAILURE);
        return static_cast<jsvm_status>(JSVM_GENERIC_FAILURE); // lvtodo throw ???
    }
    status = OH_JSVM_RunScript(env, jsvm_script, result);
    return static_cast<jsvm_status>(status);
}
jsvm_status jsvm_pump_messageloop(jsvm_vm vm, bool *result)
{
    return static_cast<jsvm_status>(OH_JSVM_PumpMessageLoop(vm, result));
}
jsvm_status jsvm_perform_microtask_checkpoint(jsvm_vm vm)
{
    return static_cast<jsvm_status>(OH_JSVM_PerformMicrotaskCheckpoint(vm));
}
jsvm_status jsvm_get_property(jsvm_env env, jsvm_value object, jsvm_value key, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetProperty(env, object, key, result));
}
jsvm_status jsvm_set_named_property(jsvm_env env, jsvm_value object, const char *utf8name, jsvm_value value)
{
    return static_cast<jsvm_status>(OH_JSVM_SetNamedProperty(env, object, utf8name, value));
}
jsvm_status jsvm_get_named_property(jsvm_env env, jsvm_value object, const char *utf8name, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetNamedProperty(env, object, utf8name, result));
}
jsvm_status jsvm_has_named_property(jsvm_env env, jsvm_value object, const char *utf8name, bool *result)
{
    return static_cast<jsvm_status>(OH_JSVM_HasNamedProperty(env, object, utf8name, result));
}
jsvm_status jsvm_create_object(jsvm_env env, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateObject(env, result));
}
jsvm_status jsvm_resolve_deferred(jsvm_env env, jsvm_deferred deferred, jsvm_value resolution)
{
    return static_cast<jsvm_status>(OH_JSVM_ResolveDeferred(env, deferred, resolution));
}
jsvm_status jsvm_reject_deferred(jsvm_env env, jsvm_deferred deferred, jsvm_value resolution)
{
    return static_cast<jsvm_status>(OH_JSVM_RejectDeferred(env, deferred, resolution));
}
jsvm_status jsvm_create_external(jsvm_env env, void *data, jsvm_finalize finalizeCb, void *finalizeHint,
                                 jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CreateExternal(env, data, finalizeCb, finalizeHint, result));
}
jsvm_status jsvm_get_value_external(jsvm_env env, jsvm_value value, void **result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetValueExternal(env, value, result));
}
jsvm_status jsvm_add_finalizer(jsvm_env env, jsvm_value jsObject, void *finalizeData, jsvm_finalize finalizeCb,
                               void *finalizeHint, jsvm_ref *result)
{
    return static_cast<jsvm_status>(
        OH_JSVM_AddFinalizer(env, jsObject, finalizeData, finalizeCb, finalizeHint, result));
}
jsvm_status jsvm_coerce_to_bool(jsvm_env env, jsvm_value value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CoerceToBool(env, value, result));
}
jsvm_status jsvm_coerce_to_number(jsvm_env env, jsvm_value value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CoerceToNumber(env, value, result));
}
jsvm_status jsvm_coerce_to_object(jsvm_env env, jsvm_value value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CoerceToObject(env, value, result));
}
jsvm_status jsvm_coerce_to_string(jsvm_env env, jsvm_value value, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_CoerceToString(env, value, result));
}
jsvm_status jsvm_get_property_names(jsvm_env env, jsvm_value object, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetPropertyNames(env, object, result));
}
jsvm_status jsvm_object_set_prototype_of(jsvm_env env, jsvm_value object, jsvm_value prototype)
{
    return static_cast<jsvm_status>(OH_JSVM_ObjectSetPrototypeOf(env, object, prototype));
}
jsvm_status jsvm_object_get_prototype_of(jsvm_env env, jsvm_value object, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_GetPrototype(env, object, result));
}
jsvm_status jsvm_get_prototype(jsvm_env env, jsvm_value object, jsvm_value *result)
{
    return static_cast<jsvm_status>(OH_JSVM_ObjectGetPrototypeOf(env, object, result));
}
jsvm_status jsvm_is_exception_pending(jsvm_env env, bool* result)
{
    return static_cast<jsvm_status>(OH_JSVM_IsExceptionPending(env, result));
}

#ifndef __JSVM_TYPES_H__
#define __JSVM_TYPES_H__

#if defined(JS_OHOS_JSVM)
#include "ark_runtime/jsvm.h"
#endif
#if defined(JS_V8)
#include <jsvm/napi/js_native_api.h>
#endif

enum jsvm_status
{
    jsvm_ok,
    jsvm_invalid_arg,
    jsvm_object_expected,
    jsvm_string_expected,
    jsvm_name_expected,
    jsvm_function_expected,
    jsvm_number_expected,
    jsvm_boolean_expected,
    jsvm_array_expected,
    jsvm_generic_failure,
    jsvm_pending_exception,
    jsvm_cancelled,
    jsvm_escape_called_twice,
    jsvm_handle_scope_mismatch,
    jsvm_callback_scope_mismatch,
    jsvm_queue_full,
    jsvm_closing,
    jsvm_bigint_expected,
    jsvm_date_expected,
    jsvm_arraybuffer_expected,
    jsvm_detachable_arraybuffer_expected,
    jsvm_would_deadlock, // unused
    jsvm_no_external_buffers_allowed,
    jsvm_cannot_run_js,
};
enum jsvm_valuetype
{
    jsvm_undefined,
    jsvm_null,
    jsvm_boolean,
    jsvm_number,
    jsvm_string,
    jsvm_symbol,
    jsvm_object,
    jsvm_function,
    jsvm_external,
    jsvm_bigint,
};

enum jsvm_typedarray_type
{
    jsvm_int8_array,
    jsvm_uint8_array,
    jsvm_uint8_clamped_array,
    jsvm_int16_array,
    jsvm_uint16_array,
    jsvm_int32_array,
    jsvm_uint32_array,
    jsvm_float32_array,
    jsvm_float64_array,
    jsvm_bigint64_array,
    jsvm_biguint64_array,
};

#if defined(JS_OHOS_JSVM)
typedef JSVM_Env jsvm_env;
typedef JSVM_Deferred jsvm_deferred;
typedef JSVM_Value jsvm_value;
typedef JSVM_CallbackInfo jsvm_callback_info;
typedef JSVM_Finalize jsvm_finalize;
typedef JSVM_Ref jsvm_ref;
typedef JSVM_Value(JSVM_CDECL *jsvm_callback)(JSVM_Env env, JSVM_CallbackInfo info);
typedef JSVM_VM jsvm_vm;
typedef JSVM_VMScope jsvm_vm_scope;
typedef JSVM_EnvScope jsvm_env_scope;
typedef JSVM_HandleScope jsvm_handle_scope;
typedef JSVM_InitOptions jsvm_init_options;
typedef JSVM_CreateVMOptions jsvm_create_vm_options;
#endif
#if defined(JS_V8)
#define JSVM_AUTO_LENGTH SIZE_MAX
typedef napi_env jsvm_env;
typedef napi_deferred jsvm_deferred;
typedef napi_value jsvm_value;
typedef napi_callback_info jsvm_callback_info;
typedef node_api_basic_finalize jsvm_finalize;
typedef napi_ref jsvm_ref;
typedef napi_value(NAPI_CDECL *jsvm_callback)(napi_env env, napi_callback_info info);
typedef struct VM__ *jsvm_vm;
typedef struct VMScope__ *jsvm_vm_scope;
typedef struct EnvScope__ *jsvm_env_scope;
typedef napi_handle_scope  jsvm_handle_scope;
struct jsvm_init_options
{
    const intptr_t *externalReferences;
    int *argc;
    char **argv;
    bool removeFlags;
};
struct jsvm_create_vm_options
{
    size_t maxOldGenerationSize;
    size_t maxYoungGenerationSize;
    size_t initialOldGenerationSize;
    size_t initialYoungGenerationSize;
    const char *snapshotBlobData;
    size_t snapshotBlobSize;
    bool isForSnapshotting;
};
#endif
enum jsvm_property_attributes
{
    jsvm_default = 0,
    jsvm_writable = 1 << 0,
    jsvm_enumerable = 1 << 1,
    jsvm_configurable = 1 << 2,
    jsvm_static = 1 << 10,
    jsvm_default_method = jsvm_writable | jsvm_configurable,
    jsvm_default_jsproperty = jsvm_writable | jsvm_enumerable | jsvm_configurable,
};

struct jsvm_property_descriptor
{
    const char *utf8name;
    jsvm_value name;

    jsvm_callback method;
    jsvm_callback getter;
    jsvm_callback setter;
    jsvm_value value;

    jsvm_property_attributes attributes;
    void *data;
};
struct jsvm_extended_error_info
{
  const char* error_message;
  void* engine_reserved;
  uint32_t engine_error_code;
  jsvm_status error_code;
};
#endif
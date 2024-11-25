#ifndef __JSVM_TYPES_H__
#define __JSVM_TYPES_H__

#if defined(JS_OHOS_JSVM)
#include "ark_runtime/jsvm.h"
#endif
#if defined(JS_V8)
#include <jsvm/napi/js_native_api.h>
#include <jsvm/napi/js_native_api_v8.h>
#include <libplatform/libplatform.h>
#include <v8.h>
#endif


enum jsvm_status
{
    ok,                              // 成功状态。
    INVALID_ARG,                     //	无效的状态。
    OBJECT_EXPECTED,                 //	期待传入对象类型。
    STRING_EXPECTED,                 //	期望传入字符串类型。
    NAME_EXPECTED,                   //	期望传入名字类型。
    FUNCTION_EXPECTED,               //	期待传入函数类型。
    NUMBER_EXPECTED,                 // 期待传入数字类型。
    BOOLEAN_EXPECTED,                //	期待传入布尔类型。
    ARRAY_EXPECTED,                  //	期待传入数组类型。
    GENERIC_FAILURE,                 //	泛型失败状态。
    PENDING_EXCEPTION,               //	挂起异常状态。
    CANCELLED,                       //	取消状态。
    ESCAPE_CALLED_TWICE,             //	转义调用了两次。
    HANDLE_SCOPE_MISMATCH,           //	句柄作用域不匹配。
    CALLBACK_SCOPE_MISMATCH,         //	回调作用域不匹配。
    QUEUE_FULL,                      //	队列满。
    CLOSING,                         //	关闭中。
    BIGINT_EXPECTED,                 //	期望传入Bigint类型。
    DATE_EXPECTED,                   //	期望传入日期类型。
    ARRAYBUFFER_EXPECTED,            //	期望传入ArrayBuffer类型。
    DETACHABLE_ARRAYBUFFER_EXPECTED, //	可分离的数组缓冲区预期状态。
    WOULD_DEADLOCK,                  //	将死锁状态。
    NO_EXTERNAL_BUFFERS_ALLOWED,     //	不允许外部缓冲区。
    CANNOT_RUN_JS,                   //	不能执行JS。
};
enum jsvm_valuetype
{
    jsvm_undefined, // 未定义类型。
    jsvm_null,      // Null类型。
    jsvm_boolean,   //	布尔类型。
    jsvm_number,    //	数字类型。
    jsvm_string,    //	字符串类型。
    jsvm_symbol,    //	符号类型。
    jsvm_object,    //	对象类型。
    jsvm_function,  //	函数类型。
    jsvm_external,  //	外部类型。
    jsvm_bigint,    //	bigint类型。
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
using jsvm_env = JSVM_Env;
using jsvm_deferred = JSVM_Deferred;
using jsvm_value = JSVM_Value;
using jsvm_callback_info = JSVM_CallbackInfo;
using jsvm_finalize = JSVM_Finalize;
using jsvm_ref = JSVM_Ref;
typedef JSVM_Value(JSVM_CDECL * jsvm_callback)(JSVM_Env env, JSVM_CallbackInfo info);
// using Script = JSVM_Script;
using jsvm_vm = JSVM_VM;
using jsvm_vm_scope = JSVM_VMScope;
using jsvm_env_scope = JSVM_EnvScope;
using jsvm_handle_scope = JSVM_HandleScope;
using jsvm_init_options = JSVM_InitOptions;
using jsvm_create_vm_options = JSVM_CreateVMOptions;
#endif
#if defined(JS_V8)
typedef napi_env jsvm_env;
typedef napi_deferred jsvm_deferred;
typedef napi_value jsvm_value;
typedef napi_callback_info jsvm_callback_info;
typedef node_api_basic_finalize jsvm_finalize;
typedef napi_ref jsvm_ref;
typedef napi_value(NAPI_CDECL * jsvm_callback)(napi_env env, napi_callback_info info);

typedef struct VM__ * jsvm_vm;
typedef struct VMScope__ * jsvm_vm_scope;
typedef struct EnvScope__ * jsvm_env_scope;
using jsvm_handle_scope = napi_handle_scope;
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
    jsvm_default_method = napi_writable | napi_configurable,
    jsvm_default_jsproperty = napi_writable | napi_enumerable | napi_configurable,
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

#endif
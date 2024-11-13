#ifndef __JSVM_TYPES_H__
#define __JSVM_TYPES_H__

#if defined(JS_OHOS_JSVM)
#include "ark_runtime/jsvm.h"
#endif
#if defined(JS_V8)
#include <binder/napi/js_native_api.h>
#include <binder/napi/js_native_api_v8.h>
#include <libplatform/libplatform.h>
#include <v8.h>
#endif

namespace jsvm
{

enum class Status
{
    OK,                              // 成功状态。
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
enum class ValueType
{
    UNDEFINED, // 未定义类型。
    Null,      // Null类型。
    BOOLEAN,   //	布尔类型。
    NUMBER,    //	数字类型。
    STRING,    //	字符串类型。
    SYMBOL,    //	符号类型。
    OBJECT,    //	对象类型。
    FUNCTION,  //	函数类型。
    EXTERNAL,  //	外部类型。
    BIGINT,    //	bigint类型。
};

enum class TypedarrayType
{
    INT8_ARRAY,
    UINT8_ARRAY,
    UINT8_CLAMPED_ARRAY,
    INT16_ARRAY,
    UINT16_ARRAY,
    INT32_ARRAY,
    UINT32_ARRAY,
    FLOAT32_ARRAY,
    FLOAT64_ARRAY,
    BIGINT64_ARRAY,
    BIGUINT64_ARRAY,
};

#if 0

struct ExtendedErrorInfo
{
    const char *error_message;
    void *engine_reserved;
    uint32_t engine_error_code;
    Status error_code;
};

#endif
#if defined(JS_OHOS_JSVM)
using Env = JSVM_Env;
using Deferred = JSVM_Deferred;
using Value = JSVM_Value;
using CallbackInfo = JSVM_CallbackInfo;
using Finalize = JSVM_Finalize;
using Ref = JSVM_Ref;
typedef JSVM_Value(JSVM_CDECL* Callback)(JSVM_Env env, JSVM_CallbackInfo info);//using Callback = JSVM_Callback;
//using Script = JSVM_Script;
using VM = JSVM_VM;
using VMScope = JSVM_VMScope;
using EnvScope = JSVM_EnvScope;
using HandleScope = JSVM_HandleScope;
using InitOptions = JSVM_InitOptions;
using CreateVMOptions = JSVM_CreateVMOptions;
#endif
#if defined(JS_V8)
using Env = napi_env;
using Deferred = napi_deferred;
using Value = napi_value;
using CallbackInfo = napi_callback_info;
using Finalize = node_api_basic_finalize;//typedef node_api_nogc_finalize Finalize;
using Ref = napi_ref;
typedef Value (NAPI_CDECL *Callback)(Env env, CallbackInfo info);
//using Script = napi_value;

typedef struct VM__ *VM;
typedef struct VMScope__ *VMScope;
typedef struct EnvScope__ *EnvScope;
using HandleScope = napi_handle_scope;
struct InitOptions
{
    const intptr_t *externalReferences;
    int *argc;
    char **argv;
    bool removeFlags;
};
struct CreateVMOptions
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
enum class PropertyAttributes
{
    DEFAULT = 0,
    WRITABLE = 1 << 0,
    ENUMERABLE = 1 << 1,
    CONFIGURABLE = 1 << 2,
    STATIC = 1 << 10,
    DEFAULT_METHOD = WRITABLE | CONFIGURABLE,
    DEFAULT_JSPROPERTY = WRITABLE | ENUMERABLE | CONFIGURABLE,
};

struct PropertyDescriptor
{
    const char *utf8name;
    Value name;

    Callback method;
    Callback getter;
    Callback setter;
    Value value;

    PropertyAttributes attributes;
    void *data;
};

} // namespace jsvm

#endif
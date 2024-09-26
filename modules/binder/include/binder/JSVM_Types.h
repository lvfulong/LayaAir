#ifndef __JSVM_TYPES_H__
#define __JSVM_TYPES_H__

#if defined(OS_OHOS)
#include "ark_runtime/jsvm.h"
#else
#include "napi/js_native_api.h"
#endif

namespace JSVM
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
#if 0
enum PropertyAttributes
{
    DEFAULT = 0,
    WRITABLE = 1 << 0,
    ENUMERABLE = 1 << 1,
    CONFIGURABLE = 1 << 2,

    // Used with napi_define_class to distinguish static properties
    // from instance properties. Ignored by napi_define_properties.
    STATIC = 1 << 10,

    // #if NAPI_VERSION >= 8
    //  Default for class methods.
    DEFAULT_METHOD = WRITABLE | CONFIGURABLE,

    // Default for object properties, like in JS obj[prop].
    DEFAULT_JSPROPERTY = WRITABLE | ENUMERABLE | CONFIGURABLE,
    // #endif  // NAPI_VERSION >= 8
};

struct ExtendedErrorInfo
{
    const char *error_message;
    void *engine_reserved;
    uint32_t engine_error_code;
    Status error_code;
};
struct PropertyDescriptor
{
    // One of utf8name or name should be NULL.
    const char *utf8name;
    Value name;

    Callback method;
    Callback getter;
    Callback setter;
    Value value;

    PropertyAttributes attributes;
    void *data;
};
#endif
#if defined(OS_OHOS)
using Env = JSVM_Env;
using Deferred = JSVM_Deferred;
using Value = JSVM_Value;
using PropertyDescriptor = JSVM_PropertyDescriptor;
// using CallbackInfo=  JSVM_CallbackInfo;
#else
using Env = napi_env;
using Deferred = napi_deferred;
using Value = napi_value;
using PropertyDescriptor = napi_property_descriptor;
//using CallbackInfo = struct CallbackInfo__ *;
//typedef Value(/*NAPI_CDECL*/ *Callback)(Env env, CallbackInfo info);


#endif
} // namespace JSVM

#endif
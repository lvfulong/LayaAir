#ifndef __JSVM_TYPES_H__
#define __JSVM_TYPES_H__

#if defined(OS_OHOS)
#include "ark_runtime/jsvm.h"
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
struct ExtendedErrorInfo
{
    const char *error_message;
    void *engine_reserved;
    uint32_t engine_error_code;
    Status error_code;
};
#if defined(OS_OHOS)

// using Status = JSVM_Status;
using Env = JSVM_Env;
using Deferred = JSVM_Deferred;
using Value = JSVM_Value;
#else
using Env = struct Env__ *;
using Deferred = struct Deferred__ *;
using Value = struct Value__ *;
#endif
} // namespace JSVM

#endif
#ifndef __JSVM_H__
#define __JSVM_H__

#include <cstdint>
#if defined(OS_OHOS)
#include "ark_runtime/jsvm.h"
#endif
#include "JSVM_Types.h"

namespace JSVM
{
#if defined(OS_OHOS)
using Env = JSVM_Env;
using Deferred = JSVM_Deferred;
using Value = JSVM_Value;
#endif

/*JSVM_EXTERN*/ Status CreatePromise(Env env, Deferred *deferred, Value *promise);

#if defined(OS_OHOS)
inline JSVM_Status ConvertToJSVM_Status(Status value)
{
    switch (value)
    {
    case Status::OK:
        return JSVM_OK;
    case Status::INVALID_ARG:
        return JSVM_INVALID_ARG;
    case Status::OBJECT_EXPECTED:
        return JSVM_OBJECT_EXPECTED;
    case Status::STRING_EXPECTED:
        return JSVM_STRING_EXPECTED;
    case Status::NAME_EXPECTED:
        return JSVM_NAME_EXPECTED;
    case Status::FUNCTION_EXPECTED:
        return JSVM_FUNCTION_EXPECTED;
    case Status::NUMBER_EXPECTED:
        return JSVM_NUMBER_EXPECTED;
    case Status::BOOLEAN_EXPECTED:
        return JSVM_BOOLEAN_EXPECTED;
    case Status::ARRAY_EXPECTED:
        return JSVM_ARRAY_EXPECTED;
    case Status::GENERIC_FAILURE:
        return JSVM_GENERIC_FAILURE;
    case Status::PENDING_EXCEPTION:
        return JSVM_PENDING_EXCEPTION;
    case Status::CANCELLED:
        return JSVM_CANCELLED;
    case Status::ESCAPE_CALLED_TWICE:
        return JSVM_ESCAPE_CALLED_TWICE;
    case Status::HANDLE_SCOPE_MISMATCH:
        return JSVM_HANDLE_SCOPE_MISMATCH;
    case Status::CALLBACK_SCOPE_MISMATCH:
        return JSVM_CALLBACK_SCOPE_MISMATCH;
    case Status::QUEUE_FULL:
        return JSVM_QUEUE_FULL;
    case Status::CLOSING:
        return JSVM_CLOSING;
    case Status::BIGINT_EXPECTED:
        return JSVM_BIGINT_EXPECTED;
    case Status::DATE_EXPECTED:
        return JSVM_DATE_EXPECTED;
    case Status::ARRAYBUFFER_EXPECTED:
        return JSVM_ARRAYBUFFER_EXPECTED;
    case Status::DETACHABLE_ARRAYBUFFER_EXPECTED:
        return JSVM_DETACHABLE_ARRAYBUFFER_EXPECTED;
    case Status::WOULD_DEADLOCK:
        return JSVM_WOULD_DEADLOCK;
    case Status::NO_EXTERNAL_BUFFERS_ALLOWED:
        return JSVM_NO_EXTERNAL_BUFFERS_ALLOWED;
    case Status::CANNOT_RUN_JS:
        return JSVM_CANNOT_RUN_JS;
    default:
        DEBUG_CHECK(false);
        break;
    }
}

inline Status ConvertToStatus(JSVM_Status value)
{
    switch (value)
    {
    case JSVM_OK:
        return Status::OK;
    case JSVM_INVALID_ARG:
        return Status::INVALID_ARG;
    case JSVM_OBJECT_EXPECTED:
        return Status::OBJECT_EXPECTED;
    case JSVM_STRING_EXPECTED:
        return Status::STRING_EXPECTED;
    case JSVM_NAME_EXPECTED:
        return Status::NAME_EXPECTED;
    case JSVM_FUNCTION_EXPECTED:
        return Status::FUNCTION_EXPECTED;
    case JSVM_NUMBER_EXPECTED:
        return Status::NUMBER_EXPECTED;
    case JSVM_BOOLEAN_EXPECTED:
        return Status::BOOLEAN_EXPECTED;
    case JSVM_ARRAY_EXPECTED:
        return Status::ARRAY_EXPECTED;
    case JSVM_GENERIC_FAILURE:
        return Status::GENERIC_FAILURE;
    case JSVM_PENDING_EXCEPTION:
        return Status::PENDING_EXCEPTION;
    case JSVM_CANCELLED:
        return Status::CANCELLED;
    case JSVM_ESCAPE_CALLED_TWICE:
        return Status::ESCAPE_CALLED_TWICE;
    case JSVM_HANDLE_SCOPE_MISMATCH:
        return Status::HANDLE_SCOPE_MISMATCH;
    case JSVM_CALLBACK_SCOPE_MISMATCH:
        return Status::CALLBACK_SCOPE_MISMATCH;
    case JSVM_QUEUE_FULL:
        return Status::QUEUE_FULL;
    case JSVM_CLOSING:
        return Status::CLOSING;
    case JSVM_BIGINT_EXPECTED:
        return Status::BIGINT_EXPECTED;
    case JSVM_DATE_EXPECTED:
        return Status::DATE_EXPECTED;
    case JSVM_ARRAYBUFFER_EXPECTED:
        return Status::ARRAYBUFFER_EXPECTED;
    case JSVM_DETACHABLE_ARRAYBUFFER_EXPECTED:
        return Status::DETACHABLE_ARRAYBUFFER_EXPECTED;
    case JSVM_WOULD_DEADLOCK:
        return Status::WOULD_DEADLOCK;
    case JSVM_NO_EXTERNAL_BUFFERS_ALLOWED:
        return Status::NO_EXTERNAL_BUFFERS_ALLOWED;
    case JSVM_CANNOT_RUN_JS:
        return Status::CANNOT_RUN_JS;
    default:
        DEBUG_CHECK(false);
        break;
    }
}
inline /*JSVM_EXTERN*/ Status CreatePromise(Env env, Deferred *deferred, Value *promise)
{
    return ConvertToStatus(OH_JSVM_CreatePromise(env, deferred, promise));
}
#endif
} // namespace JSVM

#endif
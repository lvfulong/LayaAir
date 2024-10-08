#ifndef __JSVM_H__
#define __JSVM_H__

#include <cstdint>
#if defined(OS_OHOS)
#include "ark_runtime/jsvm.h"
#endif
#include "JSVM_Types.h"

namespace jsvm
{
/*JSVM_EXTERN*/ // Status Init(const InitOptions *options);
/*JSVM_EXTERN*/ Status CreatePromise(Env env, Deferred *deferred, Value *promise);
/*JSVM_EXTERN*/ Status ResolveDeferred(Env env, Deferred deferred, Value resolution);
/*JSVM_EXTERN*/ Status RejectDeferred(Env env, Deferred deferred, Value resolution);
/*JSVM_EXTERN*/ Status RunScript(Env env, Value script, Value *result);
/*JSVM_EXTERN*/ Status IsPromise(Env env, Value value, bool *isPromise);
/*JSVM_EXTERN*/ Status IsArray(Env env, Value value, bool *result);
/*JSVM_EXTERN*/ Status GetArrayLength(Env env, Value value, uint32_t *result);
/*JSVM_EXTERN*/ Status CreateObject(Env env, Value *result);
/*JSVM_EXTERN*/ Status IsSet(Env env, Value value, bool *isSet);
/*JSVM_EXTERN*/ Status CreateSet(Env env, Value *result);
/*JSVM_EXTERN*/ Status DefineProperties(Env env, Value object, size_t propertyCount,
                                        const PropertyDescriptor *properties);
/*JSVM_EXTERN*/ Status SetProperty(Env env, Value object, Value key, Value value);
/*JSVM_EXTERN*/ Status CreateArray(Env env, Value *result);
/*JSVM_EXTERN*/ Status CreateArrayWithLength(Env env, size_t length, Value *result);
/*JSVM_EXTERN*/ Status CreateDouble(Env env, double value, Value *result);
/*JSVM_EXTERN*/ Status CreateInt32(Env env, int32_t value, Value *result);
/*JSVM_EXTERN*/ Status CreateUint32(Env env, int32_t value, Value *result);
/*JSVM_EXTERN*/ Status CreateInt64(Env env, int64_t value, Value *result);
/*JSVM_EXTERN*/ Status GetValueDouble(Env env, Value value, double *result);
/*JSVM_EXTERN*/ Status GetValueInt32(Env env, Value value, int32_t *result);
/*JSVM_EXTERN*/ Status GetValueUint32(Env env, Value value, uint32_t *result);
/*JSVM_EXTERN*/ Status GetValueInt64(Env env, Value value, int64_t *result);
/*JSVM_EXTERN*/ Status GetValueBool(Env env, Value value, bool *result);
/*JSVM_EXTERN*/ Status GetNull(Env env, Value *result);
/*JSVM_EXTERN*/ Status GetUndefined(Env env, Value *result);
/*JSVM_EXTERN*/ Status SetElement(Env env, Value object, uint32_t index, Value value);
/*JSVM_EXTERN*/ Status GetElement(Env env, Value object, uint32_t index, Value *result);
/*JSVM_EXTERN*/ Status CreateStringUtf8(Env env, const char *value, size_t length, Value *result);
/*JSVM_EXTERN*/ Status GetValueStringUtf8(Env env, Value value, char *buf, size_t bufsize, size_t *result);
/*JSVM_EXTERN*/ Status AdjustExternalMemory(Env env, int64_t changeInBytes, int64_t *result);
/*JSVM_EXTERN*/ Status GetCbInfo (Env env, CallbackInfo cbinfo, size_t *argc,  Value *argv,  Value *thisArg, void **data);
} // namespace jsvm

#endif
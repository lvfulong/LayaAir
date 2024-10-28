#ifndef __JSVM_H__
#define __JSVM_H__

#include <cstdint>
#if defined(OS_OHOS)
#include "ark_runtime/jsvm.h"
#endif
#include "JSVM_Types.h"

namespace jsvm
{
/*JSVM_EXTERN*/ Status CreateEnv(VM vm, size_t propertyCount, const PropertyDescriptor *properties, Env *result);
/*JSVM_EXTERN*/ Status DestroyEnv(Env env);
/*JSVM_EXTERN*/ // Status Init(const InitOptions *options);
/*JSVM_EXTERN*/ Status CreatePromise(Env env, Deferred *deferred, Value *promise);
/*JSVM_EXTERN*/ Status ResolveDeferred(Env env, Deferred deferred, Value resolution);
/*JSVM_EXTERN*/ Status RejectDeferred(Env env, Deferred deferred, Value resolution);
/*JSVM_EXTERN*/ Status RunScript(Env env, Value script, Value *result);
/*JSVM_EXTERN*/ Status IsPromise(Env env, Value value, bool *isPromise);
/*JSVM_EXTERN*/ Status IsArray(Env env, Value value, bool *result);
/*JSVM_EXTERN*/ Status IsError(Env env, Value value, bool *result);
/*JSVM_EXTERN*/ Status GetArrayLength(Env env, Value value, uint32_t *result);
/*JSVM_EXTERN*/ Status CreateObject(Env env, Value *result);
/*JSVM_EXTERN*/ Status IsSet(Env env, Value value, bool *isSet);
/*JSVM_EXTERN*/ Status IsDate(Env env, Value value, bool *isDate);
/*JSVM_EXTERN*/ Status GetDateValue(Env env, Value value, double *result);
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
/*JSVM_EXTERN*/ Status CreateDate(Env env, double time, Value *result);
/*JSVM_EXTERN*/ Status GetValueDouble(Env env, Value value, double *result);
/*JSVM_EXTERN*/ Status GetValueInt32(Env env, Value value, int32_t *result);
/*JSVM_EXTERN*/ Status GetValueUint32(Env env, Value value, uint32_t *result);
/*JSVM_EXTERN*/ Status CreateStringLatin1(Env env, const char* str, size_t length, Value* result);
/*JSVM_EXTERN*/ Status GetValueStringLatin1(Env env, Value value, char* buf, size_t bufsize, size_t* result);
/*JSVM_EXTERN*/ Status CreateStringUtf16(Env env, const char16_t* str, size_t length, Value* result);
/*JSVM_EXTERN*/ Status GetValueStringUtf16(Env env, Value value, char16_t* buf, size_t bufsize, size_t* result);
/*JSVM_EXTERN*/ Status GetValueInt64(Env env, Value value, int64_t *result);
/*JSVM_EXTERN*/ Status GetValueBool(Env env, Value value, bool *result);
/*JSVM_EXTERN*/ Status GetNull(Env env, Value *result);
/*JSVM_EXTERN*/ Status GetUndefined(Env env, Value *result);
/*JSVM_EXTERN*/ Status SetElement(Env env, Value object, uint32_t index, Value value);
/*JSVM_EXTERN*/ Status GetElement(Env env, Value object, uint32_t index, Value *result);
/*JSVM_EXTERN*/ Status CreateStringUtf8(Env env, const char *value, size_t length, Value *result);
/*JSVM_EXTERN*/ Status GetValueStringUtf8(Env env, Value value, char *buf, size_t bufsize, size_t *result);
/*JSVM_EXTERN*/ Status AdjustExternalMemory(Env env, int64_t changeInBytes, int64_t *result);
/*JSVM_EXTERN*/ Status GetCbInfo(Env env, CallbackInfo cbinfo, size_t *argc, Value *argv, Value *thisArg, void **data);
/*JSVM_EXTERN*/ Status SetNamedProperty(Env env, Value object, const char *utf8name, Value value);
/*JSVM_EXTERN*/ Status GetNewTarget(Env env, CallbackInfo cbinfo, Value *result);
/*JSVM_EXTERN*/ Status Wrap(Env env, Value jsObject, void *nativeObject, Finalize finalizeCb, void *finalizeHint,
                            Ref *result);
/*JSVM_EXTERN*/ Status Unwrap(Env env, Value jsObject, void **result);
/*JSVM_EXTERN*/ Status CreateReference(Env env, Value value, uint32_t initialRefcount, Ref *result);
/*JSVM_EXTERN*/ Status DeleteReference(Env env, Ref ref);
/*JSVM_EXTERN*/ Status ReferenceRef(Env env, Ref ref, uint32_t *result);
/*JSVM_EXTERN*/ Status ReferenceUnref(Env env, Ref ref, uint32_t *result);
/*JSVM_EXTERN*/ Status GetReferenceValue(Env env, Ref ref, Value *result);
/*JSVM_EXTERN*/ Status NewInstance(Env env, Value constructor, size_t argc, const Value *argv, Value *result);
/*JSVM_EXTERN*/ Status DefineClass(Env env, const char *utf8name, size_t length, Callback constructor,
                                   size_t propertyCount, const PropertyDescriptor *properties, Value *result);
/*JSVM_EXTERN*/ Status CallFunction(Env env, Value recv, Value func, size_t argc, const Value *argv, Value *result);
/*JSVM_EXTERN*/ Status CreateFunction(Env env, const char *utf8name, size_t length, Callback cb, Value *result);
/*JSVM_EXTERN*/ Status Typeof(Env env, Value value, ValueType *result);
/*JSVM_EXTERN*/ Status CreateArraybuffer(Env env, size_t byteLength, void **data, Value *result);
/*JSVM_EXTERN*/ Status IsArraybuffer(Env env, Value value, bool *result);
/*JSVM_EXTERN*/ Status IsTypedarray(Env env, Value value, bool *result);
/*JSVM_EXTERN*/ Status IsDataview(Env env, Value value, bool *result);
/*JSVM_EXTERN*/ Status CreateTypedarray(Env env, TypedarrayType type, size_t length, Value arraybuffer,
                                        size_t byteOffset, Value *result);
/*JSVM_EXTERN*/ Status CreateDataview(Env env, size_t length, Value arraybuffer, size_t byteOffset, Value *result);
/*JSVM_EXTERN*/ Status GetArraybufferInfo(Env env, Value arraybuffer, void **data, size_t *byteLength);
/*JSVM_EXTERN*/ Status GetTypedarrayInfo(Env env, Value typedarray, TypedarrayType *type, size_t *length, void **data,
                                         Value *arraybuffer, size_t *byteOffset);
/*JSVM_EXTERN*/ Status GetDataviewInfo(Env env, Value dataview, size_t *bytelength, void **data, Value *arraybuffer,
                                       size_t *byteOffset);
/*JSVM_EXTERN*/ Status GetGlobal(Env env, Value *result);
/*JSVM_EXTERN*/ Status SetInstanceData(Env env, void *data, Finalize finalizeCb, void *finalizeHint);
/*JSVM_EXTERN*/ Status GetInstanceData(Env env, void **data);
} // namespace jsvm

#endif
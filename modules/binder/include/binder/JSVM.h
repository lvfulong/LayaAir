#ifndef __JSVM_H__
#define __JSVM_H__

#include <cstdint>
#if defined(OS_OHOS)
#include "ark_runtime/jsvm.h"
#endif
#include "JSVM_Types.h"

namespace JSVM
{
/*JSVM_EXTERN*/ // Status Init(const InitOptions *options);
/*JSVM_EXTERN*/ Status CreatePromise(Env env, Deferred *deferred, Value *promise);
/*JSVM_EXTERN*/ Status ResolveDeferred(Env env, Deferred deferred, Value resolution);
/*JSVM_EXTERN*/ Status RejectDeferred(Env env, Deferred deferred, Value resolution);
/*JSVM_EXTERN*/ Status RunScript(Env env, Value script, Value *result);
/*JSVM_EXTERN*/ Status IsPromise(Env env, Value value, bool *isPromise);
/*JSVM_EXTERN*/ Status CreateObject(Env env, Value *result);
/*JSVM_EXTERN*/ Status DefineProperties(Env env, Value object, size_t propertyCount,
                                        const PropertyDescriptor *properties);
/*JSVM_EXTERN*/ Status SetProperty(Env env, Value object, Value key, Value value);
} // namespace JSVM

#endif
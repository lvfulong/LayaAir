#ifndef __JSInterface_H__
#define __JSInterface_H__

#include <utils/JCMemorySurvey.h>
#ifdef JS_JSC
#include "jsc/JSCArrayBuffer.h"
#include "jsc/JSCBinder.h"
#include "jsc/JSCEnv.h"
#elif JS_V8
#include <binder/JSBind.h>
#include <binder/v8/ArrayBuffer.h>
#include <binder/v8/Class.h>
#include <binder/v8/Date.h>
#include <binder/v8/JSEnv.h>
#include <binder/v8/Local.h>
#include <binder/v8/Object.h>
#include <binder/v8/Persistent.h>
#include <binder/v8/Promise.h>
#include <binder/v8/Script.h>
#include <binder/v8/String.h>
#include <binder/v8/Array.h>
#include <binder/v8/Value.h>
#include <binder/v8/Enum.h>
#ifdef JS_V8_DEBUGGER
#include <binder/v8/v8debug/debug-agent.h>
#endif
#endif

namespace laya
{
#ifdef JS_JSC
#elif JS_V8
typedef jsvm::Value JsValue;
// typedef v8::Local<v8::String> JsString;
// typedef v8::FunctionCallbackInfo<v8::Value> JsFuncArgs;
//typedef v8::Local<v8::Function> JsFunction;
typedef jsvm::Value JSValueAsParam;
// typedef v8::Local<v8::Object> JsObject;
// #define JSP_THROW(str) __JsThrow::Throw(str);
// #define JSP_RUN_SCRIPT(script) laya::__JSRun::Run(script);
#define JSP_TO_JS_UNDEFINE ((v8::Undefined(v8::Isolate::GetCurrent())))
#define JSP_TO_JS_PROMISE (v8::Local<v8::Promise>().As<v8::Value>())
#define JSP_TO_JS_BYTE_ARRAY(vl, sz) (__JsByteArray::ToJsByteArray(vl, sz))
#define JSP_TO_JS(tp, v) (Converter<tp>::ToJs(v))
// #define JS_TO_CPP(tp, v) (Converter<tp>::ToCpp(v))
#define JSP_TO_JS_STR(str) (v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), str).ToLocalChecked())
/*#define JS_TRY \
    v8::Isolate *isolate = v8::Isolate::GetCurrent();                                                                  \
    v8::HandleScope handle_scope(isolate);                                                                             \
    v8::TryCatch try_catch(isolate);

#define JS_CATCH                                                                                                       \
    if (try_catch.HasCaught())                                                                                         \
    {                                                                                                                  \
        LOGE("JS onFrame error\n");                                                                                    \
        __JSRun::ReportException(isolate, &try_catch);                                                                 \
    }*/
#endif
void AdjustAmountOfExternalAllocatedMemory(int p_nMemorySize)
{
    // todo
}

} // namespace laya
#endif

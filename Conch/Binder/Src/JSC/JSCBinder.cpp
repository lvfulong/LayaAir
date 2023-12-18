#ifdef JS_JSC
#include "JSCBinder.h"

thread_local long* JSClassMgr::s_tls_curThread = nullptr;

JSObjBaseJSC::JSObjBaseJSC()
{
}

JSObjBaseJSC::~JSObjBaseJSC() {
    if (!m_bWeakThis){
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        JSValueUnprotect(ctx, mpJsThis);
    }
}

void JSObjBaseJSC::makeStrong()
{
    JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
    JSValueProtect(ctx, mpJsThis);
}

void JSObjBaseJSC::makeWeak()
{
    JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
    JSValueUnprotect(ctx, mpJsThis);
}

JSValueRef JSObjBaseJSC::callJsFunc(JSValueRef func) {
	return _callJsFunc(func, 0, nullptr);
}

bool JsObjHandleJSC::Empty() {
	if (!m_pObj)
		return true;
    return m_pValue == nullptr;
}
#endif

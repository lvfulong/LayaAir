#include "JSPromiseRejectionEvent.h"
#include <Binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../../JCScriptRuntime.h"

namespace laya
{
    JSPromiseRejectionEvent::JSPromiseRejectionEvent()
    {
	    AdjustAmountOfExternalAllocatedMemory(128);
	    JCMemorySurvey::GetInstance()->newClass( "JSPromiseRejectionEvent",10240,this );
    }
    JSPromiseRejectionEvent::~JSPromiseRejectionEvent()
    {
		m_pReason.reset();
		m_pPromise.reset();
	    JCMemorySurvey::GetInstance()->releaseClass( "JSPromiseRejectionEvent",this );
    }
    JsValue JSPromiseRejectionEvent::getReason()
    {
	    return m_pReason.toLocal().handle_;
    }
    JsValue JSPromiseRejectionEvent::getPromise()
    {
        return m_pPromise.toLocal().handle_;
    }
	void JSPromiseRejectionEvent::setReason(JSValueAsParam pObj)
	{
		m_pReason.reset(pObj);
	}
	void JSPromiseRejectionEvent::setPromise(JSValueAsParam pObj)
	{
		m_pPromise.reset(pObj);
	}
	void JSPromiseRejectionEvent::setType(const char* strType)
	{
		m_strType = strType;
	}
	const char* JSPromiseRejectionEvent::getType()
	{
		return m_strType.c_str();
	}
    void JSPromiseRejectionEvent::exportJS(Context& context)
    {
		class_<JSPromiseRejectionEvent> class_binding;
		class_binding.constructor<>();
        class_binding.property("reason", &JSPromiseRejectionEvent::getReason);
        class_binding.property("promise", &JSPromiseRejectionEvent::getPromise);
        class_binding.property("type", &JSPromiseRejectionEvent::getType);
		context.class_("PromiseRejectionEvent", class_binding);
    }
}

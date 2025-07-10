#include "JSPromiseRejectionEvent.h"
#include <jsbind/JSBind.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "../../JCScriptRuntime.h"

namespace laya
{
    JSPromiseRejectionEvent::JSPromiseRejectionEvent()
    {
	    jsbind::AdjustAmountOfExternalAllocatedMemory(128);
	    JCMemorySurvey::GetInstance()->newClass( "JSPromiseRejectionEvent",10240,this );
    }
    JSPromiseRejectionEvent::~JSPromiseRejectionEvent()
    {
		m_pReason.reset();
		m_pPromise.reset();
	    JCMemorySurvey::GetInstance()->releaseClass( "JSPromiseRejectionEvent",this );
    }
    jsvm_value JSPromiseRejectionEvent::getReason()
    {
	    return m_pReason.getHandle();
    }
    jsvm_value JSPromiseRejectionEvent::getPromise()
    {
        return m_pPromise.getHandle();
    }
	void JSPromiseRejectionEvent::setReason(jsvm_value pObj)
	{
		m_pReason.reset(pObj); 
	}
	void JSPromiseRejectionEvent::setPromise(jsvm_value pObj)
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
    void JSPromiseRejectionEvent::exportJS(jsbind::Object& context)
    {
		jsbind::class_<JSPromiseRejectionEvent> class_binding;
		class_binding.constructor<>();
        class_binding.property("reason", &JSPromiseRejectionEvent::getReason);
        class_binding.property("promise", &JSPromiseRejectionEvent::getPromise);
        class_binding.property("type", &JSPromiseRejectionEvent::getType);
		context.class_("PromiseRejectionEvent", class_binding);
    }
}

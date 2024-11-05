#include "JSPromiseRejectionEvent.h"
#include <binder/JSBind.h>
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
    jsvm::Value JSPromiseRejectionEvent::getReason()
    {
	    return m_pReason.getHandle();
    }
    jsvm::Value JSPromiseRejectionEvent::getPromise()
    {
        return m_pPromise.getHandle();
    }
	void JSPromiseRejectionEvent::setReason(jsvm::Value pObj)
	{
		m_pReason = jsbind::Persistent(pObj);
	}
	void JSPromiseRejectionEvent::setPromise(jsvm::Value pObj)
	{
		m_pPromise = jsbind::Persistent(pObj);
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

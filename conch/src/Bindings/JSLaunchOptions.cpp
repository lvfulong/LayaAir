#include "JSLaunchOptions.h"
#include <jsbind/JSBind.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "../../JCScriptRuntime.h"

namespace laya
{
	JSLaunchOptions::JSLaunchOptions()
    {
	    jsbind::AdjustAmountOfExternalAllocatedMemory(10240);
	    JCMemorySurvey::GetInstance()->newClass( "JSLaunchOptions",10240,this );
    }
	JSLaunchOptions::~JSLaunchOptions()
    {
	    JCMemorySurvey::GetInstance()->releaseClass( "JSLaunchOptions",this );
    }
	int JSLaunchOptions::getScene()
	{
		return -1;
	}
	jsvm_value JSLaunchOptions::getQuery()
	{
		return jsbind::makeUndefined();
	}
	const char* JSLaunchOptions::getShareTicket()
	{
		return "";
	}
	jsvm_value JSLaunchOptions::getReferrerInfo()
	{
		return jsbind::makeUndefined();
	}
    void JSLaunchOptions::exportJS(jsbind::Object& context)
    {
		jsbind::class_<JSLaunchOptions> class_binding;
		class_binding.constructor<>();
		class_binding.property("scene", &JSLaunchOptions::getScene);
		class_binding.property("query", &JSLaunchOptions::getQuery);
		class_binding.property("shareTicket", &JSLaunchOptions::getShareTicket);
		class_binding.property("referrerInfo", &JSLaunchOptions::getReferrerInfo);
		context.class_("launchOptions", class_binding);
    }
}

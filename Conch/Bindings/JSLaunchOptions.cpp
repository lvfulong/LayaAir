#include "JSLaunchOptions.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "../../JCScriptRuntime.h"

namespace laya
{
	JSLaunchOptions::JSLaunchOptions()
    {
	    AdjustAmountOfExternalAllocatedMemory(10240);
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
	JsValue JSLaunchOptions::getQuery()
	{
		return JSP_TO_JS_UNDEFINE;
	}
	const char* JSLaunchOptions::getShareTicket()
	{
		return "";
	}
	JsValue JSLaunchOptions::getReferrerInfo()
	{
		return JSP_TO_JS_UNDEFINE;
	}
    void JSLaunchOptions::exportJS(Context& context)
    {
		class_<JSLaunchOptions> class_binding;
		class_binding.constructor<>();
		class_binding.property("scene", &JSLaunchOptions::getScene);
		class_binding.property("query", &JSLaunchOptions::getQuery);
		class_binding.property("shareTicket", &JSLaunchOptions::getShareTicket);
		class_binding.property("referrerInfo", &JSLaunchOptions::getReferrerInfo);
		context.class_("launchOptions", class_binding);
    }
}

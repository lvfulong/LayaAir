#ifndef __JSPromiseRejectionEvent_H__
#define __JSPromiseRejectionEvent_H__


#include <stdio.h>
#include <string>
#include <map>
#include <binder/JSInterface.h>

namespace laya
{
    class JSPromiseRejectionEvent
    {
    public:
	    static void exportJS(jsbind::Object& context);

        JSPromiseRejectionEvent();

	    ~JSPromiseRejectionEvent();

		void setReason(JSValueAsParam pObj);
		
		JsValue getReason();
		
		void setPromise(JSValueAsParam pObj);
		
		JsValue getPromise();
		
		const char* getType();
		
		void setType(const char* strType);
    public:
		std::string m_strType;
		jsbind::Persistent m_pReason;
		jsbind::Persistent m_pPromise;
    };
}
#endif
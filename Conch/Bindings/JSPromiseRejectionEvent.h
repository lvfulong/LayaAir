#ifndef __JSPromiseRejectionEvent_H__
#define __JSPromiseRejectionEvent_H__


#include <stdio.h>
#include <string>
#include <map>
#include <Binder/JSInterface.h>

namespace laya
{
    class JSPromiseRejectionEvent
    {
    public:
	    static void exportJS(Context& context);

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
		Persistent m_pReason;
		Persistent m_pPromise;
    };
}
#endif
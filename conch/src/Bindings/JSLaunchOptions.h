#ifndef __JSLaunchOptions_H__
#define __JSLaunchOptions_H__


#include <stdio.h>
#include <string>
#include <map>
#include <binder/JSInterface.h>

namespace laya
{
    class JSLaunchOptions
    {
    public:
	    static void exportJS(Context& context);

		JSLaunchOptions();

	    ~JSLaunchOptions();

		int getScene();

		JsValue getQuery();

		const char* getShareTicket();

		JsValue getReferrerInfo();
    };
}
#endif
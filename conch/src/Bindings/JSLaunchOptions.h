#ifndef __JSLaunchOptions_H__
#define __JSLaunchOptions_H__


#include <stdio.h>
#include <string>
#include <map>
#include <binder/JSBind.h>

namespace laya
{
    class JSLaunchOptions
    {
    public:
	    static void exportJS(jsbind::Object& context);

		JSLaunchOptions();

	    ~JSLaunchOptions();

		int getScene();

		jsvm::Value getQuery();

		const char* getShareTicket();

		jsvm::Value getReferrerInfo();
    };
}
#endif
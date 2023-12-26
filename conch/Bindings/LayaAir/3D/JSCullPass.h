#ifndef __JSCullPass_H__
#define __JSCullPass_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include "JSRenderNode.h"

namespace laya
{
	class JSCullPass
	{
	public:
		static void exportJS(Context& context);
		JSCullPass();
		~JSCullPass();
		int cullByCameraCullInfo(JSValueAsParam cameraCullInfo, JSValueAsParam renderManager);
		int cullByShadowCullInfo(JSValueAsParam shadowCullInfo, JSValueAsParam renderManager);
		int cullingSpotShadow(JSValueAsParam cameraCullInfo, JSValueAsParam renderManager);
		JsValue getCullList();
	private:
#if JS_V8
		Persistent m_pJSCullList;
#endif
	};
}
#endif //__JSCullPass_H__
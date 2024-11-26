#ifndef __JSSubmitScene3D_H__
#define __JSSubmitScene3D_H__

#include <stdio.h>
#include <jsbind/JSBind.h>
#include "../../LayaAir/ISubmit.h"

namespace laya
{
	class JSSubmitScene3D :public ISubmit
	{
	public:
		static void exportJS(jsbind::Object& context);
		JSSubmitScene3D();
		JSSubmitScene3D(jsvm_value pCallback);
		~JSSubmitScene3D();
		int renderSubmit() override;
		void recycle() override;
	private:
		jsbind::Persistent					m_pJSFunctionRenderSubmit;
	};
}
#endif //__JSSubmitScene3D_H__
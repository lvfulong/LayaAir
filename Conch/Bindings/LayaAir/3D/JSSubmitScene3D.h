#ifndef __JSSubmitScene3D_H__
#define __JSSubmitScene3D_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../LayaAir/ISubmit.h"

namespace laya
{
	class JSSubmitScene3D :public ISubmit
	{
	public:
		static void exportJS(Context& context);
		JSSubmitScene3D();
		JSSubmitScene3D(JSValueAsParam pCallback);
		~JSSubmitScene3D();
		int renderSubmit() override;
		void recycle() override;
	private:
		Persistent					m_pJSFunctionRenderSubmit;
	};
}
#endif //__JSSubmitScene3D_H__
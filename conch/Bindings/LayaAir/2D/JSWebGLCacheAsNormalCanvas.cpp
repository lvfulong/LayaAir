#include "JSWebGLCacheAsNormalCanvas.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "../../../LayaAir/2D/Context2D.h"
#include "JSContext2D.h"

namespace laya
{
	JSWebGLCacheAsNormalCanvas::JSWebGLCacheAsNormalCanvas()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("_conchWebGLCacheAsNormalCanvas", 4, this);
	}
	//------------------------------------------------------------------------------
	JSWebGLCacheAsNormalCanvas::JSWebGLCacheAsNormalCanvas(JSValueAsParam ctx, int sp)
	{
		JSContext2D* pJSContext2D = (JSContext2D*)Converter<JSContext2D*>::ToCpp(ctx);
		m_canvas = new WebGLCacheAsNormalCanvas(pJSContext2D->m_context);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("_conchWebGLCacheAsNormalCanvas", 4, this);
	}
	//------------------------------------------------------------------------------
	JSWebGLCacheAsNormalCanvas::~JSWebGLCacheAsNormalCanvas()
	{
		if (m_canvas)
		{
			delete m_canvas;
			m_canvas = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("_conchWebGLCacheAsNormalCanvas", this);
	}
	void JSWebGLCacheAsNormalCanvas::startRec()
	{
		m_canvas->startRec();
	}
	void JSWebGLCacheAsNormalCanvas::endRec()
	{
		m_canvas->endRec();
	}
	bool JSWebGLCacheAsNormalCanvas::isCacheValid()
	{
		return m_canvas->isCacheValid();
	}
	bool JSWebGLCacheAsNormalCanvas::isTextNeedRestore()
	{
		return m_canvas->isTextNeedRestore();
	}
	//------------------------------------------------------------------------------
	void JSWebGLCacheAsNormalCanvas::exportJS(Context& context)
	{
		 class_<JSWebGLCacheAsNormalCanvas> class_binding;
		 class_binding.constructor<>();
		 class_binding.constructor<JSValueAsParam, int>();
		 class_binding.function("startRec", &JSWebGLCacheAsNormalCanvas::startRec);
		 class_binding.function("endRec", &JSWebGLCacheAsNormalCanvas::endRec);
		 class_binding.function("isCacheValid", &JSWebGLCacheAsNormalCanvas::isCacheValid); 
		 class_binding.function("isTextNeedRestore", &JSWebGLCacheAsNormalCanvas::isTextNeedRestore); 
		 context.class_("_conchWebGLCacheAsNormalCanvas", class_binding);
	}
}
//------------------------------------------------------------------------------

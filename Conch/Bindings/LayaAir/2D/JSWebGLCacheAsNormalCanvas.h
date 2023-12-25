#ifndef __JSWebGLCacheAsNormalCanvas_H__
#define __JSWebGLCacheAsNormalCanvas_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include "../../../LayaAir/2D/WebGLCacheAsNormalCanvas.h"

namespace laya
{
	class WebGLCacheAsNormalCanvas;
	class JSWebGLCacheAsNormalCanvas
	{
	public:
		static void exportJS(Context& context);
		JSWebGLCacheAsNormalCanvas();
		JSWebGLCacheAsNormalCanvas(JSValueAsParam ctx, int sp);
		~JSWebGLCacheAsNormalCanvas();
		void startRec();
		void endRec();
		bool isCacheValid();
		bool isTextNeedRestore();
	public:
		WebGLCacheAsNormalCanvas* m_canvas = nullptr;
	};
}
#endif //__JSWebGLCacheAsNormalCanvas_H__
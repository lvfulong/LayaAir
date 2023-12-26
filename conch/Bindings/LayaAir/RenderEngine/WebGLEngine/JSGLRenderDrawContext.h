#ifndef __JSGLRenderDrawContext_H__
#define __JSGLRenderDrawContext_H__

#include <stdio.h>
#include <binder/JSInterface.h>

namespace laya
{
	class IRenderDrawContext;
	class WebGLEngine;
	class JSGLRenderDrawContext 
	{
	public:
		static void exportJS(Context& context);
		JSGLRenderDrawContext();
		JSGLRenderDrawContext(JSValueAsParam engine);
		~JSGLRenderDrawContext();

	public:
		void drawElementsInstanced(int mode, int count, int type, int offset, int instanceCount);
		void drawArraysInstanced(int mode, int first, int count, int instanceCount);
		void drawArrays(int mode, int first, int count);
		void drawElements(int mode, int count, int type, int offset);
		void drawGeometryElement(JSValueAsParam pGeometryElement);

	public:
		WebGLEngine*				m_pEngine;
		IRenderDrawContext*			m_pRenderDrawContext;
	};
}
#endif //__JSGLRenderDrawContext_H__
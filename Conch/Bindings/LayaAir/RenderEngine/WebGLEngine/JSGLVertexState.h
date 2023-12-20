#ifndef __JSGLVertexState_H__
#define __JSGLVertexState_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include <render/driver/gles/GLVertexState.h>

namespace laya
{
	class JSGLVertexState
	{
	public:

		static void exportJS(Context& context);

		JSGLVertexState();

		JSGLVertexState(JSValueAsParam engine);

		~JSGLVertexState();

		void bindVertexArray();

		void unbindVertexArray();

		void applyVertexBuffer(JSValueAsParam pVertexBuffers);

		void applyIndexBuffer(JSValueAsParam pIndexBuffer);

		void destroy();

	public:

		GLVertexState*	m_GLVertexState;

	};
}
#endif //__JSGLVertexState_H__
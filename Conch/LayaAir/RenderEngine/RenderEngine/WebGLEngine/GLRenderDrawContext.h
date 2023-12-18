#ifndef __GLRenderDrawContext_H__
#define __GLRenderDrawContext_H__

#include "../../RenderEnum/RenderPologyMode.h"
#include "../../RenderEnum/IndexFormat.h"
#include "GLObject.h"
#include "../../RenderInterface/IRenderDrawContext.h"

namespace laya
{
	class ANGLEInstancedArraysExt;
	class WebGLEngine;
	class RenderGeometryElement;
	class GLRenderDrawContext: public GLObject, public IRenderDrawContext
	{
	public:
		GLRenderDrawContext(WebGLEngine* engine);
		void drawElementsInstanced(MeshTopology mode, int count, IndexFormat type, int offset, int instanceCount) override;
		void drawArraysInstanced(MeshTopology mode, int first, int count, int instanceCount) override;
		void drawArrays(MeshTopology mode, int first, int count) override;
		void drawElements(MeshTopology mode, int count, IndexFormat type , int offset) override;
		void drawGeometryElement(RenderGeometryElement* geometryElement) override;
	private:
		ANGLEInstancedArraysExt* m_angleInstancedArrays;
	};
}
#endif //__GLRenderDrawContext_H__



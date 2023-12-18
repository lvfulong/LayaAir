#ifndef __IRenderDrawContext_H__
#define __IRenderDrawContext_H__

#include "../RenderEnum/RenderPologyMode.h"
#include "../RenderEnum/IndexFormat.h"

namespace laya
{
	class RenderGeometryElement;
	class IRenderDrawContext
	{
	public:
		virtual ~IRenderDrawContext() {}
		virtual void drawElementsInstanced(MeshTopology mode, int count, IndexFormat type, int offset, int instanceCount) = 0;
		virtual void drawArraysInstanced(MeshTopology mode, int first, int count, int instanceCount) = 0;
		virtual void drawArrays(MeshTopology mode, int first, int count) = 0;
		virtual void drawElements(MeshTopology mode, int count, IndexFormat type, int offset) = 0;
		virtual void drawGeometryElement(RenderGeometryElement* geometryElement) = 0;
	};
}
#endif
#ifndef __JSRenderGeometryElement_H__
#define __JSRenderGeometryElement_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../../LayaAir/3D/RenderGeometryElement.h"

namespace laya
{
	class JSRenderGeometryElement
	{
	public:
		static void exportJS(Context& context);
		JSRenderGeometryElement();
		JSRenderGeometryElement(int nMeshPologyType,int nDrawType);
		~JSRenderGeometryElement();

	public:
		void setDrawArrayParams(int first, int count);
		void setDrawElemenParams(int count, int offset);
		void clearRenderParams();
		void destroy();

		void setMeshTopology(int nMode);
		int getMeshTopology();
		
		void setDrawType(int nDrawType);
		int getDrawType();

		void setInstanceCount(int number);
		int getInstanceCount();

		void setIndexFormat(int indexFormat);
		int getIndexFormat();

		void setBufferState3D(JSValueAsParam bufferState3D);
		JsValue getBufferState3D();

	public:

		Persistent		        m_pJSBufferState3D;
		RenderGeometryElement*	m_pRenderGeometryElement;

	};
}
#endif //__JSRenderGeometryElement_H__
#ifndef __JSRenderQueue_H__
#define __JSRenderQueue_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include <render/3D/temp/RenderQueue.h>

namespace laya
{
	class JSRenderQueue
	{
	public:

		static void exportJS(Context& context);

		JSRenderQueue();
		JSRenderQueue(bool isTransparent);
		~JSRenderQueue();

	public:
		
		void renderQueue(JSValueAsParam pContext);

		void addRenderElement(JSValueAsParam renderElement, JSValueAsParam renderNode, int nRenderQueue, float fSortingFudge);
		
		void clear();
	
		void setSortPass(JSValueAsParam sortpass);

		JsValue getSortPass();

		void destroy();

	public:
	
		RenderQueue*					m_pRenderQueue;
		//std::vector<Persistent*>		m_vJSRenderElements;
		Persistent						m_pJSSortPass;
	};
}
#endif //__JSRenderQueue_H__
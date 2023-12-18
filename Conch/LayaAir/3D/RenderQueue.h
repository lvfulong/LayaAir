#ifndef __RenderQueue_H__
#define __RenderQueue_H__

#include <stdio.h>
#ifdef __APPLE__
	#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
	#include <GLES3/gl3.h>
#endif
#include "RenderElement.h"
#include <Utils/JCSingletonList.h>
#include "RenderElement.h"

namespace laya
{
	class ISortPass;
	class RenderQueue
	{
	public:
		RenderQueue(bool isTransparent);
		
		~RenderQueue();
		
		void renderQueue(RenderContext3D* pContext);
		void addRenderElement(RenderElement* pRenderElement);
		void clear();

	private:

		void _batchQueue();

		void _sort();

	public:

		JCSingletonList<RenderElement*>* 	m_pElementList;
		ISortPass*							m_pSortPass = nullptr;
		bool								m_bIsTransparent;
	};
}
#endif //__RenderQueue_H__

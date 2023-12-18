#ifndef __RenderGeometryElement_H__
#define __RenderGeometryElement_H__

#include <stdio.h>
#ifdef __APPLE__
	#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
	#include <GLES3/gl3.h>
#endif

#include "../RenderEngine/RenderEnum/RenderPologyMode.h"
#include "../RenderEngine/RenderEnum/DrawType.h"
#include "../RenderEngine/RenderEnum/IndexFormat.h"
#include <Utils/JCSingletonList.h>

namespace laya
{
	class IRenderVertexState;
	class RenderGeometryElement
	{
	public:
		RenderGeometryElement(MeshTopology mode,DrawType drawType);
		~RenderGeometryElement();
		void setDrawArrayParams(int first, int count);
		void setDrawElemenParams(int count, int offset);
		void clearRenderParams();
		void destroy();
	public:
		IRenderVertexState*		m_pBufferState3D;
		MeshTopology			m_nRenderMode;
		DrawType				m_nDrawType;
		IndexFormat				m_nIndexFormat;
		int						m_nInstanceCount;
		JCSingletonList<int>*	m_pDrawParams;
	};
}
#endif //__RenderGeometryElement_H__

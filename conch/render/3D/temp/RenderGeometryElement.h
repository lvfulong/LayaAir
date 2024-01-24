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

#include <render/3D/design/renderEnum/RenderPologyMode.h>
#include <render/3D/design/renderEnum/DrawType.h>
#include <render/3D/design/renderEnum/IndexFormat.h>
#include <utils/JCSingletonList.h>
#include "ResourceBase.h"

namespace laya
{
	class IRenderVertexState;
	class RenderGeometryElement: public ResourceBase<RenderGeometryElement>
	{
	public:
		RenderGeometryElement(MeshTopology mode,DrawType drawType);
		~RenderGeometryElement();
		void setDrawArrayParams(int first, int count);
		void setDrawElementParams(int count, int offset);
		void clearRenderParams();
		void destroy();
		static RenderGeometryElement *getRenderGeometryElement(uint32_t id);
		void setIndexFormat(IndexFormat indexFormat)
		{
			m_nIndexFormat = indexFormat;
			//todo this._glindexFormat = (LayaGL.renderDrawContext as GLRenderDrawContext).getIndexType(this._indexFormat);
		}
		IndexFormat getIndexFormat()
		{
			return m_nIndexFormat;
		}
		void setMeshTopology(MeshTopology nMode)
		{
			m_nRenderMode = nMode;
			//todo this._glmode = (LayaGL.renderDrawContext as GLRenderDrawContext).getMeshTopology(this._mode);
		}
		MeshTopology getMeshTopology()
		{
			return m_nRenderMode;
		}
		void setDrawType(DrawType nDrawType)
		{
			m_nDrawType = nDrawType;
		}
		DrawType getDrawType()
		{
			return m_nDrawType;
		}
		void setInstanceCount(int number)
		{
			m_nInstanceCount = number;
		}
		int getInstanceCount()
		{
			return m_nInstanceCount;
		}
	public:
		IRenderVertexState*		m_pBufferState3D;
		MeshTopology			m_nRenderMode;
		DrawType				m_nDrawType;
		IndexFormat				m_nIndexFormat;
		int						m_nInstanceCount;
		laya::JCSingletonList<int>*m_pDrawParams;
	};
}
#endif //__RenderGeometryElement_H__

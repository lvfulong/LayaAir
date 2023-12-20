#include "RenderGeometryElement.h"


namespace laya
{
	RenderGeometryElement::RenderGeometryElement(MeshTopology mode, DrawType drawType)
		:m_nRenderMode(mode),m_nDrawType(drawType)
	{
		m_pDrawParams = new laya::JCSingletonList<int>(false);
	}
	RenderGeometryElement::~RenderGeometryElement()
	{	
		destroy();
	}
	void RenderGeometryElement::setDrawArrayParams(int first, int count)
	{
		m_pDrawParams->add(first);
		m_pDrawParams->add(count);
	}
	void RenderGeometryElement::setDrawElemenParams(int count, int offset)
	{
		m_pDrawParams->add(offset);
		m_pDrawParams->add(count);
	}
	void RenderGeometryElement::clearRenderParams()
	{
		m_pDrawParams->resetLength();
	}
	void RenderGeometryElement::destroy()
	{
		m_pBufferState3D = nullptr;
		if (m_pDrawParams)
		{
			delete m_pDrawParams;
			m_pDrawParams = NULL;
		}
	}
}
//------------------------------------------------------------------------------

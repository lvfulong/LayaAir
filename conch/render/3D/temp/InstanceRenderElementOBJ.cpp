#include "InstanceRenderElementOBJ.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <render/3D/design/renderInterface/IRenderDrawContext.h>
#include <Bindings/LayaAir/3D/JSTransform.h>
#include "RenderGeometryElement.h"
#include <render/3D/design/renderInterface/IRenderVertexState.h>
#include "VertexBuffer3D.h"

namespace laya
{
	InstanceRenderElementOBJ::InstanceRenderElementOBJ(GLESEngine* pGLESEngine): RenderElement(pGLESEngine)
	{
		m_vertexBuffer3D.reserve(10);
	}
	InstanceRenderElementOBJ::~InstanceRenderElementOBJ()
	{
	}
	void InstanceRenderElementOBJ::clear()
	{
		m_vertexBuffer3D.clear();
	}
	int InstanceRenderElementOBJ::getDrawCount()
	{
		return m_nDrawCount;
	}
	void InstanceRenderElementOBJ::setDrawCount(int nValue)
	{
		m_nDrawCount = nValue;
	}
	void InstanceRenderElementOBJ::addUpdateBuffer(VertexBuffer3D* vb, int length)
	{
		m_vertexBuffer3D.push_back(std::make_pair(vb, length));
	}
	void InstanceRenderElementOBJ::getUpdateData(int index, const char* pData)
	{
		if (m_updateData.size() < index + 1)
			m_updateData.resize(index + 1);
		m_updateData[index] = pData;
	}
	void InstanceRenderElementOBJ::drawGeometry(GLESShaderInstance* shaderIns)
	{
		//��instance�����ر��ʱ������Ҫһ��һ����������,������Ҫ���¶�������
		const char* data = nullptr;
		VertexBuffer3D* buffer = nullptr;
		int updateDataSize = m_updateData.size();
		for (int i = 0, size = m_vertexBuffer3D.size(); i < size; i++)
		{
			buffer = m_vertexBuffer3D[i].first;
			if (!buffer) 
				break;
			if (i < updateDataSize)
			{
				data = m_updateData[i];
				buffer->orphanStorage();
				buffer->setData(data, 0, 0, m_nDrawCount * m_vertexBuffer3D[i].second * 4);
			}
		}
		m_pGLESEngine->getDrawContext()->drawGeometryElement(m_pGeometry);
	}
}
//------------------------------------------------------------------------------

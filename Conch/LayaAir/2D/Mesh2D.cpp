#include "Mesh2D.h"
#include "IndexBuffer2D.h"
#include "VertexBuffer2D.h"
#include "BufferStateBase.h"
#include <render/3D/design/renderEnum/BufferTargetType.h>

namespace laya
{
	Mesh2D::Mesh2D(WebGLEngine* engine, int stride, int vballoc, int iballoc, bool createIndexBuffer)
	{
		m_createIndexBuffer = createIndexBuffer;
		m_pWebGLEngine = engine;
		m_stride = stride;
		m_vb = new VertexBuffer2D(engine, stride, BufferUsage::Dynamic);
		//if (vballoc) 
		//{
			m_vb->m_buffer2D->_resizeBuffer(vballoc, false);
		//}
		//else 
		//{
		//	Config.webGL2D_MeshAllocMaxMem && m_vb->buffer2D._resizeBuffer(64 * 1024 * stride, false);
		//}
		if (createIndexBuffer)
		{
			m_ib = new IndexBuffer2D(engine, BufferUsage::Static);
			//if (iballoc)
			//{
			m_ib->m_buffer2D->_resizeBuffer(iballoc, false);
			//}
		}
	}
	Mesh2D::~Mesh2D()
	{
		if (m_vb)
		{
			delete m_vb;
			m_vb = nullptr;
		}
		if (m_createIndexBuffer && m_ib)
		{
			delete m_ib;
			m_ib = nullptr;
		}
		if (m_vao)
		{
			delete m_vao;
			m_vao = nullptr;
		}
	}
	void Mesh2D::configVAO()
	{
		if (m_applied)
			return;
		m_applied = true;
		if (!m_vao)
		{
			m_vao = new BufferStateBase(m_pWebGLEngine);
		}
		m_vao->applyState(m_stride, m_attribInfo, m_vb, m_ib);
	}
	void Mesh2D::useMesh()
	{
		if (m_vao && !m_vao->isBind())
		{
			if (BufferStateBase::m_curBindedBufferState != nullptr) 
			{
				BufferStateBase::m_curBindedBufferState->unBind();
			}
		}
		//Ҫ��bind����bufferData
		if (!m_applied)
		{
			configVAO();
		}
		m_ib->m_buffer2D->_bind_upload();
		m_vb->m_buffer2D->_bind_upload();
		m_vao->bind();
	}
	//ֱ�Ӵ���һ���̶���ib�����չ̶��ı��ε�������
	void Mesh2D::createQuadIB(int quadNum)
	{
		//this._quadNum = quadNum;
		m_ib->m_buffer2D->_resizeBuffer(quadNum * 6 * 2, false);	//short����
		m_ib->m_buffer2D->setByteLength(m_ib->m_buffer2D->getBufferLength());

		uint16_t* bd = (uint16_t*)m_ib->m_buffer2D->getData();
		int idx = 0;
		int curvert = 0;
		for (int i = 0; i < quadNum; i++) 
		{
			bd[idx++] = curvert;
			bd[idx++] = curvert + 2;
			bd[idx++] = curvert + 1;
			bd[idx++] = curvert;
			bd[idx++] = curvert + 3;
			bd[idx++] = curvert + 2;
			curvert += 4;
		}

		m_ib->m_buffer2D->setNeedUpload();
	}
}
//------------------------------------------------------------------------------

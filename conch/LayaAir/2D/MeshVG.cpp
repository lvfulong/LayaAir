#include "MeshVG.h"
#include "IndexBuffer2D.h"
#include "VertexBuffer2D.h"

namespace laya
{
	std::stack<MeshVG*> MeshVG::_POOL;
	//------------------------------------------------------------------------------
	MeshVG::MeshVG(GLESEngine* engine): Mesh2D(engine, MeshVG::const_stride, 4, 4, true)
	{
		m_canReuse = true;
		m_attribInfo.push_back({ GL_FLOAT, 2, 0 });
		m_attribInfo.push_back({ GL_UNSIGNED_BYTE, 4, 8 });
	}
	//------------------------------------------------------------------------------
	MeshVG::~MeshVG()
	{
		if (m_ib)
		{
			delete m_ib;
			m_ib = nullptr;
		}
		if (m_vb)
		{
			delete m_vb;
			m_vb = nullptr;
		}
	}
    //------------------------------------------------------------------------------
    void MeshVG::onDestroy()
    {
        while (!MeshVG::_POOL.empty())
        {
            delete MeshVG::_POOL.top();
            MeshVG::_POOL.pop();
        }
    }
	//------------------------------------------------------------------------------
	MeshVG* MeshVG::getAMesh(GLESEngine* engine, bool mainctx)
	{
		MeshVG* ret;
		if (!MeshVG::_POOL.empty())
		{
			ret = MeshVG::_POOL.top();
			MeshVG::_POOL.pop();
		}
		else
		{
			ret = new MeshVG(engine);
		}
		if (mainctx)
		{
			ret->m_vb->m_buffer2D->_resizeBuffer(64 * 1024 * MeshVG::const_stride, false);
		}
		return ret;
	}
	//------------------------------------------------------------------------------
	void MeshVG::recycle()
	{
		m_vb->m_buffer2D->setByteLength(0);
		m_ib->m_buffer2D->setByteLength(0);
		m_vertNum = 0;
		m_indexNum = 0;
		MeshVG::_POOL.push(this);
	}
	//------------------------------------------------------------------------------
	void MeshVG::addVertAndIBToMesh(const std::vector<float>& points, uint32_t rgba, const std::vector<uint16_t>& ib)
	{
		int startpos = m_vb->m_buffer2D->needSize(points.size() / 2 * MeshVG::const_stride);//vb����㡣
		int f32pos = startpos >> 2;
		float* vbdata = (float*)m_vb->m_buffer2D->getData();
		uint32_t* vbu32Arr = (uint32_t*)m_vb->m_buffer2D->getData();
		int ci = 0;
		//vb
		//var clipinfo:Array = ctx.getTransedClipInfo();
		int sz = points.size() / 2;
		for (int i = 0; i < sz; i++) 
		{
			vbdata[f32pos++] = points[ci]; vbdata[f32pos++] = points[ci + 1]; ci += 2;
			vbu32Arr[f32pos++] = rgba;
			/*
			//�ü���Ϣ��
			vbdata[f32pos++] = clipinfo[2] ; vbdata[f32pos++] = clipinfo[3]; vbdata[f32pos++] = clipinfo[4]; vbdata[f32pos++] = clipinfo[5];//cliprect�ķ���
			vbdata[f32pos++] = clipinfo[0]; vbdata[f32pos++] = clipinfo[1]; //cliprect��λ��
			*/
		}
		m_vb->m_buffer2D->setNeedUpload();

		int idxLength = ib.size();
		int stib = m_ib->m_buffer2D->needSize(idxLength * sizeof(uint16_t));
		uint16_t* cidx = (uint16_t*)m_ib->m_buffer2D->getData();
		int stibid = stib >> 1;	// indexbuffer����ʼλ��

		{
			memcpy(cidx + stibid, &ib[0], idxLength * sizeof(uint16_t));
		}
		m_ib->m_buffer2D->setNeedUpload();

		//this->_ib.append(new Uint16Array(ib));
		//this->_ib.setNeedUpload();

		m_vertNum += sz;
		m_indexNum += idxLength;
	}
	//------------------------------------------------------------------------------
	void MeshVG::addVertAndIBToMesh(const std::vector<std::array<float, 2> >& points, uint32_t rgba, const std::vector<uint16_t>& ib)
	{
		int startpos = m_vb->m_buffer2D->needSize(points.size() * MeshVG::const_stride);//vb����㡣
		int f32pos = startpos >> 2;
		float* vbdata = (float*)m_vb->m_buffer2D->getData();
		uint32_t* vbu32Arr = (uint32_t*)m_vb->m_buffer2D->getData();
		int ci = 0;
		//vb
		//var clipinfo:Array = ctx.getTransedClipInfo();
		int sz = points.size();
		for (int i = 0; i < sz; i++)
		{
			vbdata[f32pos++] = points[i][0]; vbdata[f32pos++] = points[i][1]; ci += 2;
			vbu32Arr[f32pos++] = rgba;
			/*
			//�ü���Ϣ��
			vbdata[f32pos++] = clipinfo[2] ; vbdata[f32pos++] = clipinfo[3]; vbdata[f32pos++] = clipinfo[4]; vbdata[f32pos++] = clipinfo[5];//cliprect�ķ���
			vbdata[f32pos++] = clipinfo[0]; vbdata[f32pos++] = clipinfo[1]; //cliprect��λ��
			*/
		}
		m_vb->m_buffer2D->setNeedUpload();

		int idxLength = ib.size();
		int stib = m_ib->m_buffer2D->needSize(idxLength * sizeof(uint16_t));
		uint16_t* cidx = (uint16_t*)m_ib->m_buffer2D->getData();
		int stibid = stib >> 1;	// indexbuffer����ʼλ��

		{
			memcpy(cidx + stibid, &ib[0], idxLength * sizeof(uint16_t));
		}
		m_ib->m_buffer2D->setNeedUpload();

		//this->_ib.append(new Uint16Array(ib));
		//this->_ib.setNeedUpload();

		m_vertNum += sz;
		m_indexNum += idxLength;
	}
}


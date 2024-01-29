#include "MeshTexture.h"
#include "IndexBuffer2D.h"
#include "VertexBuffer2D.h"

namespace laya
{
	std::stack<MeshTexture*> MeshTexture::_POOL;
	//------------------------------------------------------------------------------
	MeshTexture::MeshTexture(WebGLEngine* engine): Mesh2D(engine, MeshTexture::const_stride, 4, 4, true)	//x,y,u,v,rgba
	{
		m_canReuse = true;
		m_attribInfo.push_back({ GL_FLOAT, 4, 0 });			//pos,uv
		m_attribInfo.push_back({ GL_UNSIGNED_BYTE, 4, 16 });	//color
		m_attribInfo.push_back({ GL_UNSIGNED_BYTE, 4, 20 });	//alpha
	}
	//------------------------------------------------------------------------------
	MeshTexture::~MeshTexture()
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
	MeshTexture* MeshTexture::getAMesh(WebGLEngine* engine, bool mainctx)
	{
		MeshTexture* ret;
		if (!MeshTexture::_POOL.empty()) 
		{
			ret = MeshTexture::_POOL.top();
			MeshTexture::_POOL.pop();
		}
		else
		{
			ret = new MeshTexture(engine);
		}
		if (mainctx)
		{
			ret->m_vb->m_buffer2D->_resizeBuffer(64 * 1024 * MeshTexture::const_stride, false);
		}
		return ret;
	}
	//------------------------------------------------------------------------------
	void MeshTexture::recycle()
	{
		m_vb->m_buffer2D->setByteLength(0);
		m_ib->m_buffer2D->setByteLength(0);
		m_vertNum = 0;
		m_indexNum = 0;
		MeshTexture::_POOL.push(this);
	}
    //------------------------------------------------------------------------------
    void MeshTexture::onDestroy()
    {
        while (!MeshTexture::_POOL.empty())
        {
            delete MeshTexture::_POOL.top();
            MeshTexture::_POOL.pop();
        }
    }
	//------------------------------------------------------------------------------
	void MeshTexture::addData(float* vertices, int verticesLength, float* uvs, int uvLength, uint16_t* idx, int idxLength, const Matrix& matrix, uint32_t rgba)
	{
		int vertsz = verticesLength >> 1;
		int startpos = m_vb->m_buffer2D->needSize(vertsz * MeshTexture::const_stride);//vb����㡣			
		int f32pos = startpos >> 2;
		float* vbdata = (float*)m_vb->m_buffer2D->getData();
		uint32_t* vbu32Arr = (uint32_t*)m_vb->m_buffer2D->getData();
		int ci = 0;
		float m00 = matrix.a;
		float m01 = matrix.b;
		float m10 = matrix.c;
		float m11 = matrix.d;
		float tx = matrix.tx;
		float ty = matrix.ty;
		for (int i = 0; i < vertsz; i++) 
		{
			float x = vertices[ci];
			float y  = vertices[ci + 1];
			vbdata[f32pos] = x * m00 + y * m10 + tx;
			vbdata[f32pos + 1] = x * m01 + y * m11 + ty;
			vbdata[f32pos + 2] = uvs[ci];
			vbdata[f32pos + 3] = uvs[ci + 1];
			vbu32Arr[f32pos + 4] = rgba;
			vbu32Arr[f32pos + 5] = 0xff;
			f32pos += 6;
			//�ü���Ϣ��
			//vbdata[f32pos++] = clipinfo[2] ; vbdata[f32pos++] = clipinfo[3]; vbdata[f32pos++] = clipinfo[4]; vbdata[f32pos++] = clipinfo[5];//cliprect�ķ���
			//vbdata[f32pos++] = clipinfo[0]; vbdata[f32pos++] = clipinfo[1];	//cliprect��λ��
			ci += 2;
		}
		m_vb->m_buffer2D->setNeedUpload();

		int vertN = m_vertNum;
		int sz = idxLength;
		int stib = m_ib->m_buffer2D->needSize(idxLength * sizeof(uint16_t));
		uint16_t* cidx = (uint16_t*)m_ib->m_buffer2D->getData();
		int stibid = stib >> 1;	// indexbuffer����ʼλ��
		if (vertN > 0) 
		{
			int end = stibid + sz;
			int si = 0;
			for (int i = stibid; i < end; i++, si++)
			{
				cidx[i] = idx[si] + vertN;
			}
		}
		else
		{
			memcpy(cidx + stibid, idx, idxLength * sizeof(uint16_t));
		}
		m_ib->m_buffer2D->setNeedUpload();

		m_vertNum += vertsz;
		m_indexNum += idxLength;
	}
}
//------------------------------------------------------------------------------

#include "MeshQuadTexture.h"
#include "IndexBuffer2D.h"
#include "VertexBuffer2D.h"

namespace laya
{
	IndexBuffer2D*					MeshQuadTexture::s_fixib = nullptr;//delete ?
	std::stack<MeshQuadTexture*>	MeshQuadTexture::_POOL;
	//------------------------------------------------------------------------------
	MeshQuadTexture::MeshQuadTexture(WebGLEngine* engine): Mesh2D(engine, MeshQuadTexture::const_stride, 4, 4, false)	//x,y,u,v,rgba
	{
		m_canReuse = true;
		m_attribInfo.push_back({ GL_FLOAT, 4, 0 });			//pos,uv
		m_attribInfo.push_back({ GL_UNSIGNED_BYTE, 4, 16 });	//color
		m_attribInfo.push_back({ GL_UNSIGNED_BYTE, 4, 20 });	//alpha
		if (MeshQuadTexture::s_fixib == nullptr)
		{
			MeshQuadTexture::s_fixib = m_ib = new IndexBuffer2D(engine, BufferUsage::Static);
			createQuadIB(MeshQuadTexture::MAX_IB);
		}
		else
		{
			m_ib = MeshQuadTexture::s_fixib;
		}
	}
	//------------------------------------------------------------------------------
	MeshQuadTexture::~MeshQuadTexture()
	{
		if (m_vb)
		{
			delete m_vb;
			m_vb = nullptr;
		}
		/*if (m_ib)
		{
			delete m_ib;
			m_ib = nullptr;
		}*/
	}
	//------------------------------------------------------------------------------
	MeshQuadTexture* MeshQuadTexture::getAMesh(WebGLEngine* engine, bool mainctx)
	{
		MeshQuadTexture* ret;
		if (!MeshQuadTexture::_POOL.empty())
		{
			ret = MeshQuadTexture::_POOL.top();
			MeshQuadTexture::_POOL.pop();
		}
		else
		{
			ret = new MeshQuadTexture(engine);
		}
		if (mainctx)
		{
			ret->m_vb->m_buffer2D->_resizeBuffer(64 * 1024 * MeshQuadTexture::const_stride, false);
		}
		return ret;
	}
	//------------------------------------------------------------------------------
	void MeshQuadTexture::recycle()
	{
		m_vb->m_buffer2D->setByteLength(0);
		m_vertNum = 0;
		m_indexNum = 0;
		MeshQuadTexture::_POOL.push(this);
	}

    void MeshQuadTexture::onDestroy()
    {
        while (!MeshQuadTexture::_POOL.empty())
        {
            delete MeshQuadTexture::_POOL.top();
            MeshQuadTexture::_POOL.pop();
        }
        delete MeshQuadTexture::s_fixib;
        MeshQuadTexture::s_fixib = nullptr;
    }
	//------------------------------------------------------------------------------
	void MeshQuadTexture::addQuad(float pos[8], float uv[8], uint32_t color, bool useTex)
	{
		uint32_t vpos = m_vb->m_byteLength >> 2;	//float������±�
													//x,y,u,v,rgba
		m_vb->m_buffer2D->setByteLength((vpos + MeshQuadTexture::const_stride) << 2); //��һ���ı��εĴ�С��Ҳ���������Ĵ�С
		float* vbdata = (float*)m_vb->m_buffer2D->getData();
		uint32_t* vbu32Arr = (uint32_t*)m_vb->m_buffer2D->getData();
		int cpos = vpos;
		uint32_t useTexVal = useTex ? 0xff : 0;
		vbdata[cpos++] = pos[0]; vbdata[cpos++] = pos[1]; vbdata[cpos++] = uv[0]; vbdata[cpos++] = uv[1]; vbu32Arr[cpos++] = color; vbu32Arr[cpos++] = useTexVal;
		vbdata[cpos++] = pos[2]; vbdata[cpos++] = pos[3]; vbdata[cpos++] = uv[2]; vbdata[cpos++] = uv[3]; vbu32Arr[cpos++] = color; vbu32Arr[cpos++] = useTexVal;
		vbdata[cpos++] = pos[4]; vbdata[cpos++] = pos[5]; vbdata[cpos++] = uv[4]; vbdata[cpos++] = uv[5]; vbu32Arr[cpos++] = color; vbu32Arr[cpos++] = useTexVal;
		vbdata[cpos++] = pos[6]; vbdata[cpos++] = pos[7]; vbdata[cpos++] = uv[6]; vbdata[cpos++] = uv[7]; vbu32Arr[cpos++] = color; vbu32Arr[cpos++] = useTexVal;
		m_vb->m_buffer2D->setNeedUpload();
	}
}
//------------------------------------------------------------------------------

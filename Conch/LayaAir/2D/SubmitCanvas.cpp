#include "SubmitCanvas.h"
#include "Mesh2D.h"
#include "../RenderEngine/RenderStateContext.h"
#include "Shader2D.h"
#include "RenderState2D.h"
#include "WebGLCacheAsNormalCanvas.h"

namespace laya
{
	std::stack<SubmitCanvas*> SubmitCanvas::POOL;
	SubmitCanvas* SubmitCanvas::create(WebGLCacheAsNormalCanvas* context/*, float alpha*/)
	{
		SubmitCanvas* o = nullptr;
		if (SubmitCanvas::POOL.empty())
		{
			o = new SubmitCanvas(nullptr);
		}
		else
		{
			o = SubmitCanvas::POOL.top();
			SubmitCanvas::POOL.pop();
		}
		/*if (o->m_shaderBindAttrib.empty())
		{
			o->m_shaderBindAttrib.push_back(std::make_pair("posuv", 0));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribColor", 1));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribFlags", 2));
		}
		o->m_mesh = mesh;*/
		o->m_context = context;
		o->m_key.m_submitType = KEY_DRAWCANVAS;
		//o->m_key.m_blendMode = blendMode;
		//o->m_key.m_other = -1;
		o->m_elementNum = 0;
		//o->m_startIndex = o->m_mesh->indexNum * Mesh2D::BYTES_PER_INDEX;
		o->m_shaderDefines.clear();
		return o;
	}
	SubmitCanvas::SubmitCanvas(WebGLEngine* pWebglEngine): SubmitBase(pWebglEngine)
	{

	}
	//------------------------------------------------------------------------------
	void SubmitCanvas::recycle()
	{
		SubmitCanvas::POOL.push(this);
	}
    //------------------------------------------------------------------------------
    void SubmitCanvas::onDestory()
    {
        while (!SubmitCanvas::POOL.empty())
        {
            delete SubmitCanvas::POOL.top();
            SubmitCanvas::POOL.pop();
        }
    }
	//------------------------------------------------------------------------------
	int SubmitCanvas::renderSubmit()
	{
		
		float* preMatrix4 = RenderState2D::worldMatrix4;
		Matrix* preMatrix = RenderState2D::worldMatrix;

		Matrix::mul(m_matrix, *preMatrix, m_worldMatrix);
		m_matrix4[0] = m_worldMatrix.a;
		m_matrix4[1] = m_worldMatrix.b;
		m_matrix4[4] = m_worldMatrix.c;
		m_matrix4[5] = m_worldMatrix.d;
		m_matrix4[12] = m_worldMatrix.tx;
		m_matrix4[13] = m_worldMatrix.ty;

		RenderState2D::worldMatrix = &m_worldMatrix;
		RenderState2D::worldMatrix4 = m_matrix4;

		m_context->flushsubmit();


		RenderState2D::worldMatrix4 = preMatrix4;
		RenderState2D::worldMatrix = preMatrix;

		return 1;
	}
	
}

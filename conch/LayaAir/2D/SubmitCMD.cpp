#include "SubmitCMD.h"
#include "Mesh2D.h"
#include <render/3D/temp/RenderStateContext.h>
#include "Shader2D.h"

namespace laya
{
	std::stack<SubmitCMD*> SubmitCMD::POOL;
	SubmitCMD* SubmitCMD::create(std::function<void()> func)
	{
		SubmitCMD* o = nullptr;
		if (SubmitCMD::POOL.empty())
		{
			o = new SubmitCMD(nullptr);
		}
		else
		{
			o = SubmitCMD::POOL.top();
			SubmitCMD::POOL.pop();
		}
		o->m_func = func;
		/*if (o->m_shaderBindAttrib.empty())
		{
			o->m_shaderBindAttrib.push_back(std::make_pair("position", 0));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribColor", 1));
		}
		o->m_mesh = mesh;
		o->m_key.m_submitType = submitType;
		o->m_key.m_blendMode = blendMode;
		o->m_key.m_other = -1;
		o->m_glTexture = -1;
		o->m_elementNum = 0;
		o->m_renderTarget = renderTarget;
		o->m_startIndex = o->m_mesh->indexNum * Mesh2D::BYTES_PER_INDEX;
		o->m_shaderDefines.clear();*/
		return o;
	}
	SubmitCMD::SubmitCMD(WebGLEngine* pWebglEngine) : SubmitBase(pWebglEngine)
	{

	}
	//------------------------------------------------------------------------------
	void SubmitCMD::recycle()
	{
		SubmitCMD::POOL.push(this);
	}
    //------------------------------------------------------------------------------
    void SubmitCMD::onDestory()
    {
        while (!SubmitCMD::POOL.empty())
        {
            delete SubmitCMD::POOL.top();
            SubmitCMD::POOL.pop();
        }
    }
	//------------------------------------------------------------------------------
	int SubmitCMD::renderSubmit()
	{
		m_func();
		return 1;
	}
	
}

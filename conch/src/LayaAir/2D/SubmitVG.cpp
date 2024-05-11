#include "SubmitVG.h"
#include "Mesh2D.h"
#include <render/3D/temp/RenderStateContext.h>
#include "Shader2D.h"
#include "RenderState2D.h"
#include "Context2D.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderDrawContext.h"
namespace laya
{
	std::stack<SubmitVG*> SubmitVG::POOL;
	SubmitVG* SubmitVG::create(int submitType, Mesh2D* mesh, BlendMode blendMode, bool renderTarget, Context2D* ctx)
	{
		SubmitVG* o = nullptr;
		if (SubmitVG::POOL.empty())
		{
			o = new SubmitVG(ctx->m_pGLESEngine);
		}
		else
		{
			o = SubmitVG::POOL.top();
			SubmitVG::POOL.pop();
		}
		if (o->m_shaderBindAttrib.empty())
		{
			o->m_shaderBindAttrib.push_back(std::make_pair("position", 0));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribColor", 1));
		}
		o->m_mesh = mesh;
		o->m_key.m_submitType = submitType;
		o->m_key.m_blendMode = blendMode;
		//o->m_key.m_other = -1;
		o->m_glTexture = -1;
		o->m_elementNum = 0;
		o->m_renderTarget = renderTarget;
		o->m_startIndex = o->m_mesh->m_indexNum * Mesh2D::BYTES_PER_INDEX;
		o->m_shaderDefines.clear();

		o->m_clipMatDir[0] = Context2D::MAXSIZE;
		o->m_clipMatDir[1] = 0;
		o->m_clipMatDir[2] = 0;
		o->m_clipMatDir[3] = Context2D::MAXSIZE;
		o->m_clipMatPos[0] = o->m_clipMatPos[1] = 0;
		o->m_clipOff[0] = o->m_clipOff[1] = 0;
		return o;
	}
	SubmitVG::SubmitVG(GLESEngine* pWebglEngine): SubmitBase(pWebglEngine)
	{

	}
	//------------------------------------------------------------------------------
	void SubmitVG::recycle()
	{
		SubmitVG::POOL.push(this);
	}
    //------------------------------------------------------------------------------
    void SubmitVG::onDestroy()
    {
        while (!SubmitVG::POOL.empty())
        {
            delete SubmitVG::POOL.top();
            SubmitVG::POOL.pop();
        }
    }
	//------------------------------------------------------------------------------
	int SubmitVG::renderSubmit()
	{
		if (m_elementNum == 0)
		{
			return 1;
		}

		m_mesh->useMesh();

		RenderStateContext::setBlend(true);
		if (m_renderTarget)
		{
			RenderStateContext::setBlendFunc(BlendModes[(int)m_key.m_blendMode].sourceTarget, BlendModes[(int)m_key.m_blendMode].destinationTarget);
		}
		else
		{
			RenderStateContext::setBlendFunc(BlendModes[(int)m_key.m_blendMode].source, BlendModes[(int)m_key.m_blendMode].destination);
		}
		// ����о���Ļ��������� WORLDMAT ��
		if (RenderState2D::worldMatrix4 != RenderState2D::TEMPMAT4_ARRAY)
		{
			m_shaderDefines.add(Shader2DDefines::WORLDMAT);
		}
		Shader2D* shader = Shader2D::withCompile2D(m_pGLESEngine, Shader2DDefines::PRIMITIVE, m_shaderDefines, m_shaderBindAttrib);
		shader->use(); 
		// ����о���Ļ��������� WORLDMAT ��
		if (RenderState2D::worldMatrix4 != RenderState2D::TEMPMAT4_ARRAY)
		{
			shader->uniformMatrix4fv("mmat", RenderState2D::worldMatrix4);
		}
		shader->uniform4f("clipMatDir", m_clipMatDir[0], m_clipMatDir[1], m_clipMatDir[2], m_clipMatDir[3]);
		shader->uniform2f("clipMatPos", m_clipMatPos[0], m_clipMatPos[1]);
		shader->uniform2f("clipOff", m_clipOff[0], m_clipOff[1]);
		shader->uniform2f("size", RenderState2D::width, RenderState2D::height);
		m_pGLESEngine->getDrawContext()->drawElements2DTemp(MeshTopology::Triangles, m_elementNum, IndexFormat::UInt16, m_startIndex);
		return 1;
	}
	
}

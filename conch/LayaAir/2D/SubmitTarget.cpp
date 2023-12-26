#include "SubmitTarget.h"
#include "Mesh2D.h"
#include <render/3D/temp/RenderStateContext.h>
#include "Shader2D.h"
#include "RenderTexture2D.h"
#include "RenderState2D.h"
#include <render/driver/gles/WebGLInternalTex.h>
#include "Context2D.h"
#include <render/driver/gles/WebGLEngine.h>
#include <render/3D/design/renderInterface/IRenderDrawContext.h>
#include "JCConch.h"

namespace laya
{
	std::stack<SubmitTarget*> SubmitTarget::POOL;
	SubmitTarget* SubmitTarget::create(Mesh2D* mesh, BlendMode blendMode, RenderTexture2D* target, Context2D* ctx)
	{
		SubmitTarget* o = nullptr;
		if (SubmitTarget::POOL.empty())
		{
			o = new SubmitTarget(ctx->m_pWebGLEngine);
		}
		else
		{
			o = SubmitTarget::POOL.top();
			SubmitTarget::POOL.pop();
		}
		o->m_pWebGLEngine = ctx->m_pWebGLEngine;
		if (o->m_shaderBindAttrib.empty())
		{
			o->m_shaderBindAttrib.push_back(std::make_pair("posuv", 0));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribColor", 1));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribFlags", 2));
		}
		o->m_mesh = mesh;
		o->m_key.m_submitType = KEY_DRAWTARGET;
		o->m_key.m_blendMode = blendMode;
		o->m_key.m_other = target->getID();
        o->m_textureID = target->m_texture->getID();
		o->m_elementNum = 0;
		//o->m_renderTarget = renderTarget;
		o->m_startIndex = o->m_mesh->m_indexNum * Mesh2D::BYTES_PER_INDEX;
		o->m_shaderDefines.clear();


		o->m_clipMatDir[0] = Context2D::MAXSIZE;
		o->m_clipMatDir[1] = 0;
		o->m_clipMatDir[2] = 0;
		o->m_clipMatDir[3] = Context2D::MAXSIZE;
		o->m_clipMatPos[0] = o->m_clipMatPos[1] = 0;
		o->m_clipOff[0] = o->m_clipOff[1] = 0;

		if(ctx->getCurrentState().colorFiler)
		{
			o->m_shaderDefines.add(Shader2DDefines::FILTERCOLOR);
			memcpy(o->m_colorMat, ctx->getCurrentState().colorMat, sizeof(float) * 16);
			memcpy(o->m_colorAlpha, ctx->getCurrentState().colorAlpha, sizeof(float) * 4);
		}

		o->m_bGamma = true;
		return o;
	}
	SubmitTarget* SubmitTarget::create(Mesh2D* mesh, BlendMode blendMode, WebGLInternalTex* pTexture, WebGLEngine* pWebGLEngine, bool gamma)
	{
		SubmitTarget* o = nullptr;
		if (SubmitTarget::POOL.empty())
		{
			o = new SubmitTarget(pWebGLEngine);
		}
		else
		{
			o = SubmitTarget::POOL.top();
			SubmitTarget::POOL.pop();
		}
		o->m_pWebGLEngine = pWebGLEngine;
		if (o->m_shaderBindAttrib.empty())
		{
			o->m_shaderBindAttrib.push_back(std::make_pair("posuv", 0));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribColor", 1));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribFlags", 2));
		}
		o->m_mesh = mesh;
		o->m_key.m_submitType = KEY_DRAWTARGET;
		o->m_key.m_blendMode = blendMode;
		o->m_key.m_other = -1;
        o->m_textureID = pTexture->getID();
		o->m_elementNum = 0;
		//o->m_renderTarget = renderTarget;
		o->m_startIndex = o->m_mesh->m_indexNum * Mesh2D::BYTES_PER_INDEX;
		o->m_shaderDefines.clear();


		o->m_clipMatDir[0] = Context2D::MAXSIZE;
		o->m_clipMatDir[1] = 0;
		o->m_clipMatDir[2] = 0;
		o->m_clipMatDir[3] = Context2D::MAXSIZE;
		o->m_clipMatPos[0] = o->m_clipMatPos[1] = 0;
		o->m_clipOff[0] = o->m_clipOff[1] = 0;

		o->m_bGamma = gamma;
		return o;
	}
	SubmitTarget::SubmitTarget(WebGLEngine* pWebglEngine):SubmitBase(pWebglEngine)
	{
		
	}
	//------------------------------------------------------------------------------
	void SubmitTarget::recycle()
	{
		SubmitTarget::POOL.push(this);
	}
    //------------------------------------------------------------------------------
    void SubmitTarget::onDestory()
    {
        while (!SubmitTarget::POOL.empty())
        {
            delete SubmitTarget::POOL.top();
            SubmitTarget::POOL.pop();
        }
    }
	//------------------------------------------------------------------------------
	int SubmitTarget::renderSubmit()
	{
		if (m_elementNum == 0)
		{
			return 1;
		}

        WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(m_textureID);
        if (texture == nullptr)
            return 1;
        
		m_mesh->useMesh();
		if (m_key.m_blendMode == BlendMode::disable)
		{
			RenderStateContext::setBlend(m_pWebGLEngine, false);
		}
		else
		{
			RenderStateContext::setBlend(m_pWebGLEngine, true);

			RenderStateContext::setBlendFunc(m_pWebGLEngine, BlendModes[(int)m_key.m_blendMode].source, BlendModes[(int)m_key.m_blendMode].destination);
		}
		// ����о���Ļ��������� WORLDMAT ��
		if (RenderState2D::worldMatrix4 != RenderState2D::TEMPMAT4_ARRAY)
		{
			m_shaderDefines.add(Shader2DDefines::WORLDMAT);
		}
		bool returnGamma = !(RenderTexture2D::m_currentActive);
		returnGamma = returnGamma && m_bGamma && texture->m_gammaCorrection == 1;
		if (returnGamma)
		{
			m_shaderDefines.add(Shader2DDefines::GAMMASPACE);
		}
		else
		{
			m_shaderDefines.remove(Shader2DDefines::GAMMASPACE);
		}

		Shader2D* shader = Shader2D::withCompile2D(m_pWebGLEngine, Shader2DDefines::TEXTURE2D, m_shaderDefines, m_shaderBindAttrib);
		if (!shader)
		{
			return 1;
		}
		shader->use(); 

		if (RenderState2D::worldMatrix4 != RenderState2D::TEMPMAT4_ARRAY)
		{
			shader->uniformMatrix4fv("mmat", RenderState2D::worldMatrix4);
		}
		if (m_shaderDefines.isDefine(Shader2DDefines::FILTERCOLOR))
		{
			shader->uniform4f("colorAlpha", m_colorAlpha[0], m_colorAlpha[1], m_colorAlpha[2], m_colorAlpha[3]);
			shader->uniformMatrix4fv("colorMat", m_colorMat);
		}
		if (m_shaderDefines.isDefine(Shader2DDefines::FILTERBLUR))
		{
			shader->uniform4f("strength_sig2_2sig2_gauss1", m_strength_sig2_2sig2_gauss1[0], m_strength_sig2_2sig2_gauss1[1], m_strength_sig2_2sig2_gauss1[2], m_strength_sig2_2sig2_gauss1[3]);
			shader->uniform2f("blurInfo", m_blurinfo[0], m_blurinfo[1]);
		}
		shader->uniform4f("clipMatDir", m_clipMatDir[0], m_clipMatDir[1], m_clipMatDir[2], m_clipMatDir[3]);
		shader->uniform2f("clipMatPos", m_clipMatPos[0], m_clipMatPos[1]);
		shader->uniform2f("clipOff", m_clipOff[0], m_clipOff[1]);
		shader->uniform2f("size", RenderState2D::width, RenderState2D::height);
		shader->uniform_sampler2D("texture", texture);
		m_pWebGLEngine->getDrawContext()->drawElements(MeshTopology::Triangles, m_elementNum, IndexFormat::UInt16, m_startIndex);
		return 1;
	}
	
}

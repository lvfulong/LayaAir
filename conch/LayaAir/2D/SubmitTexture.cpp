#include "SubmitTexture.h"
#include "Mesh2D.h"
#include <render/3D/temp/RenderStateContext.h>
#include "Shader2D.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderDrawContext.h"
#include "RenderState2D.h"
#include "Context2D.h"
#include "RenderTexture2D.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include "JCConch.h"

namespace laya
{
	std::stack<SubmitTexture*> SubmitTexture::POOL;
	SubmitTexture* SubmitTexture::create(int submitType, Mesh2D* mesh, Context2D* ctx, WebGLInternalTex* texture)
	{
		SubmitTexture* o = nullptr;
		if (SubmitTexture::POOL.empty())
		{
			o = new SubmitTexture(ctx->m_pWebGLEngine);
		}
		else
		{
			o = SubmitTexture::POOL.top();
			SubmitTexture::POOL.pop();
		}
		o->m_pWebGLEngine = ctx->m_pWebGLEngine;
		if (o->m_shaderBindAttrib.empty())
		{
			o->m_shaderBindAttrib.push_back(std::make_pair("posuv", 0));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribColor", 1));
			o->m_shaderBindAttrib.push_back(std::make_pair("attribFlags", 2));
		}
		o->m_mesh = mesh;
		o->m_key.m_submitType = submitType;
		o->m_key.m_blendMode = ctx->getCurrentState().blendMode;
		o->m_key.m_other = texture ? texture->getID() : -1;
        o->m_textureID = texture->getID();
		o->m_elementNum = 0;
		o->m_renderTarget = ctx->m_target != nullptr;
		o->m_startIndex = o->m_mesh->m_indexNum * Mesh2D::BYTES_PER_INDEX;
		o->m_shaderDefines.clear();

		o->m_clipMatDir[0] = Context2D::MAXSIZE;
		o->m_clipMatDir[1] = 0;
		o->m_clipMatDir[2] = 0;
		o->m_clipMatDir[3] = Context2D::MAXSIZE;
		o->m_clipMatPos[0] = o->m_clipMatPos[1] = 0;
		o->m_clipOff[0] = o->m_clipOff[1] = 0;

		if (ctx->getCurrentState().colorFiler) 
		{
			o->m_shaderDefines.add(Shader2DDefines::FILTERCOLOR);
			memcpy(o->m_colorMat, ctx->getCurrentState().colorMat, sizeof(float) *16);
			memcpy(o->m_colorAlpha, ctx->getCurrentState().colorAlpha, sizeof(float) * 4);
		}
		return o;
	}
	SubmitTexture::SubmitTexture(WebGLEngine* pWebglEngine) :SubmitBase(pWebglEngine)
	{

	}
	//------------------------------------------------------------------------------
	void SubmitTexture::recycle()
	{
		SubmitTexture::POOL.push(this);
	}
    //------------------------------------------------------------------------------
    void SubmitTexture::onDestory()
    {
        while (!SubmitTexture::POOL.empty())
        {
            delete SubmitTexture::POOL.top();
            SubmitTexture::POOL.pop();
        }
    }
	//------------------------------------------------------------------------------
	int SubmitTexture::renderSubmit()
	{
		if (m_elementNum == 0)
		{
			return 1;
		}

        WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(m_textureID);
        if (texture == nullptr)
            return 1;
        
        
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

		bool returnGamma = !(RenderTexture2D::m_currentActive);
		returnGamma = returnGamma && texture->m_gammaCorrection == 1;
		if (returnGamma)
		{
			m_shaderDefines.add(Shader2DDefines::GAMMASPACE);
		}
		else
		{
			m_shaderDefines.remove(Shader2DDefines::GAMMASPACE);
		}

		Shader2D* shader = Shader2D::withCompile2D(m_pWebGLEngine, Shader2DDefines::TEXTURE2D, m_shaderDefines, m_shaderBindAttrib);
		shader->use(); 
		// ����о���Ļ��������� WORLDMAT ��
		if (RenderState2D::worldMatrix4 != RenderState2D::TEMPMAT4_ARRAY)
		{
			shader->uniformMatrix4fv("mmat", RenderState2D::worldMatrix4);
		}
		if (m_shaderDefines.isDefine(Shader2DDefines::FILTERCOLOR))
		{
			shader->uniform4f("colorAlpha", m_colorAlpha[0], m_colorAlpha[1], m_colorAlpha[2], m_colorAlpha[3]);
			shader->uniformMatrix4fv("colorMat", m_colorMat);
		}
		shader->uniform4f("clipMatDir", m_clipMatDir[0], m_clipMatDir[1], m_clipMatDir[2], m_clipMatDir[3]);
		shader->uniform2f("clipMatPos", m_clipMatPos[0], m_clipMatPos[1]);
		shader->uniform2f("clipOff", m_clipOff[0], m_clipOff[1]);
		shader->uniform2f("size", RenderState2D::width, RenderState2D::height);
        
  
		shader->uniform_sampler2D("texture", texture);
	
		m_pWebGLEngine->getDrawContext()->drawElements2DTemp(MeshTopology::Triangles, m_elementNum, IndexFormat::UInt16, m_startIndex);
		return 1;
	}
	
}

#include "GLESRenderElement2D.h"
#include <render/RenderDriver/OpenGLESDriver/2DRenderPass/GLESRenderContext2D.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>
#include "render/Property.h"
#include "GLESRenderContext2D.h"
namespace laya
{
	RTDefineDatas* GLESRenderElement2D::_compileDefines = nullptr;
	void GLESRenderElement2D::setCompileDefine(RTDefineDatas* value)
	{
		GLESRenderElement2D::_compileDefines = value;
	}

	GLESRenderElement2D::GLESRenderElement2D()
	{


	}

	GLESRenderElement2D::~GLESRenderElement2D()
	{

	}

	void GLESRenderElement2D::destroy() {
		_shaderInstances.clear();
		GLESShaderData* materialShaderData = nullptr;
		GLESShaderData* value2DShaderData = nullptr;
		GLESRenderGeometryElement* geometry = nullptr;
		RTSubShader* subshader = nullptr;
		
	}

	void GLESRenderElement2D::_preUpdatePre(GLESRenderContext2D* context)
	{
		_compileShader(context);
	}
	
	void GLESRenderElement2D::_render(GLESRenderContext2D* context)
	{
		if (_shaderInstances.getLength() == 1) {
			_renderByShaderInstance(_shaderInstances.m_vElements[0], context);
		}
		else {
			for (uint32_t j = 0, m = _shaderInstances.getLength(); j < m; j++) {
				_renderByShaderInstance(_shaderInstances.m_vElements[j], context);
			}
		}
	}

	void GLESRenderElement2D::_compileShader(GLESRenderContext2D* context)
	{
		std::vector<RTShaderPass*> passes = subShader->shaderpasses;
		_shaderInstances.clear();
		for (uint32_t j = 0, m = passes.size(); j < m; j++)
		{
			RTShaderPass* pass = passes[j];
			if (pass->pipelineMode != context->pipelineMode)
				continue;
			RTDefineDatas* comDef = GLESRenderElement2D::_compileDefines;
			if (context->sceneData)
			{
				context->sceneData->_defineDatas->cloneTo(comDef);
			}
			else
			{
				if(context->globalConfigShaderData) context->globalConfigShaderData->cloneTo(comDef);
			}

			bool returnGamma;
			if (context->destRT)
				returnGamma = static_cast<bool>(context->destRT->m_textures.front()->gammaCorrection != 1);
			else
				returnGamma = true;
			
			if (returnGamma)
				comDef->add(ShaderDefines2D::GAMMASPACE);
			else
				comDef->remove(ShaderDefines2D::GAMMASPACE);

			if(context->invertY)
				comDef->add(ShaderDefines2D::INVERTY);
			else
				comDef->remove(ShaderDefines2D::INVERTY);
		
			if (value2DShaderData) {
				comDef->addDefineDatas(value2DShaderData->_defineDatas);
				pass->nodeCommonMap = _commonUniformMap;
			}
			if (materialShaderData) {
				comDef->addDefineDatas(materialShaderData->_defineDatas);
			}
			pass->is2D = true;
			RTShaderPass::CacheShaderItem* item = pass->getCacheShader(comDef);
			GLESShaderInstance* shader;
			if (item)
				shader = item->_glesShaderInstance;
			else
				shader = pass->callCreateShaderInstanceFunction();
			assert(shader != nullptr);
			_shaderInstances.add(shader);
		}
	}
	void GLESRenderElement2D::_renderByShaderInstance(GLESShaderInstance* shader, GLESRenderContext2D* context)
	{
		if (!shader->complete())
			return;
		shader->bind();
		if (value2DShaderData) {
			shader->uploadUniforms(&(shader->m_sprite2DUniformParamsMap), value2DShaderData, true);
		}
		
		if (context->sceneData) {
			shader->uploadUniforms(&(shader->m_sceneUniformParamsMap), context->sceneData, true);
		}
		if (materialShaderData) {
			shader->uploadUniforms(&(shader->m_materialUniformParamsMap),materialShaderData, true);
		}

		if (renderStateIsBySprite || materialShaderData==nullptr) {
			shader->uploadRenderStateBlendDepth(value2DShaderData);
			shader->uploadRenderStateFrontFace(value2DShaderData, false, context->invertY);
		}
		else {
			shader->uploadRenderStateBlendDepth(materialShaderData);
			shader->uploadRenderStateFrontFace(materialShaderData, false, context->invertY);
		}
		LayaGL::m_pWebglEngine->getDrawContext()->drawGeometryElement(geometry);
	}
} // namespace laya

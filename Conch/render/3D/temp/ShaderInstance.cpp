#include "ShaderInstance.h"
#include "CommandUniformMap.h"
#include <render/driver/gles/GLStateMap.h>
#include "RenderState.h"
#include <render/3D/temp/RenderStateContext.h>
#include "Shader3D.h"
#include <Utils/Log.h>
namespace laya
{
	ShaderInstance::ShaderInstance(WebGLEngine* engine, const char* vs, const char* ps, GLAttributeMap* pAttributeMap, GLStateMap* pStateMap, RenderState* pRenderState)
	{
		m_pWebGLEngine = engine;
		m_stateParamsMap = pStateMap;
		m_renderState = pRenderState;
		m_GLShaderInstance = new GLShaderInstance(engine, vs, ps, pAttributeMap);
		_create();
	}
	ShaderInstance::~ShaderInstance()
	{
		if (m_GLShaderInstance)
		{
			delete m_GLShaderInstance;
			m_GLShaderInstance = nullptr;
		}
	}
	bool ShaderInstance::bind()
	{
		return m_GLShaderInstance->bind();
	}
	void ShaderInstance::destroy()
	{
		m_GLShaderInstance->destroy();
	}
	void ShaderInstance::_create()
	{
		CommandUniformMap* sceneParams = CommandUniformMap::createGlobalUniformMap("Scene3D");
		CommandUniformMap* spriteParms = CommandUniformMap::createGlobalUniformMap("Sprite3D");
		CommandUniformMap* cameraParams = CommandUniformMap::createGlobalUniformMap("BaseCamera");
		CommandUniformMap* customParams = CommandUniformMap::createGlobalUniformMap("Custom");

		std::vector<ShaderVariable*>& data = m_GLShaderInstance->getUniformMap();
		for (int i = 0, n = data.size(); i < n; i++)
		{
			ShaderVariable* one = data[i];
			if (sceneParams->hasPtrID(one->dataOffset))
			{
				m_sceneUniformParamsMap.addShaderUniform(one);
			}
			else if (cameraParams->hasPtrID(one->dataOffset))
			{
				m_cameraUniformParamsMap.addShaderUniform(one);
			}
			else if (spriteParms->hasPtrID(one->dataOffset))
			{
				m_spriteUniformParamsMap.addShaderUniform(one);
			}
			else if (customParams->hasPtrID(one->dataOffset))
			{
				m_customUniformParamsMap[one->dataOffset] = one;
			}
			else
			{
				m_materialUniformParamsMap.addShaderUniform(one);
			}
		}
	}
	void ShaderInstance::_disposeResource()
	{
		//this._renderShaderInstance.destroy();
		m_sceneUniformParamsMap.clear();
		m_cameraUniformParamsMap.clear();
		m_spriteUniformParamsMap.clear();
		m_materialUniformParamsMap.clear();
		m_customUniformParamsMap.clear();
		m_stateParamsMap = nullptr;
		m_renderState = nullptr;

		m_uploadMaterial = nullptr;
		m_uploadRender = nullptr;
		m_uploadCameraShaderValue = nullptr;
		m_uploadScene = -1;
		m_uploadMark = -1;
	}
	ShaderData::DataInfo* ShaderInstance::_getRenderState(ShaderData* shaderDatas, int stateIndex)
	{
		int stateID = m_stateParamsMap->getData(stateIndex);
		if (stateID == -1)
			return nullptr;
		else
			return shaderDatas->getData(stateID);
	}

	int ShaderInstance::uploadUniforms(CommandEncoder* shaderUniform, ShaderData* shaderDatas, bool uploadUnTexture)
	{
		return m_pWebGLEngine->uploadUniforms(m_GLShaderInstance, shaderUniform, shaderDatas, uploadUnTexture);
	}
	int ShaderInstance::uploadCustomUniforms(int index, char* data, int byteSize)
	{
		return m_pWebGLEngine->uploadCustomUniforms(m_GLShaderInstance, m_customUniformParamsMap, index, data, byteSize);
	}
	void ShaderInstance::uploadRenderStateBlendDepth(ShaderData* shaderDatas)
	{
		ShaderData::DataInfo* depthWrite = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_DEPTH_WRITE);
		ShaderData::DataInfo* depthTest = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_DEPTH_TEST);
		ShaderData::DataInfo* blend = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND);
		ShaderData::DataInfo* stencilRef = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_STENCIL_REF);
		ShaderData::DataInfo* stencilTest = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_STENCIL_TEST);
		ShaderData::DataInfo* stencilWrite = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_STENCIL_WRITE);
		ShaderData::DataInfo* stencilOp = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_STENCIL_OP);

		int nDepthWrite;
		int nDepthTest;

		int nBlend;

		int nStencilRef;

		int nStencilTest;

		int nStencilWrite;

		int nStencilOpX;
		int nStencilOpY;
		int nStencilOpZ;


		if (m_stateParamsMap->m_shaderPassStatefirst)
		{
			m_renderState->depthWrite != nullptr ? nDepthWrite = *m_renderState->depthWrite : 0;
			m_renderState->depthTest != nullptr ? nDepthTest = *m_renderState->depthTest : 0;
			m_renderState->blend != nullptr ? nBlend = *m_renderState->blend : 0;
			m_renderState->stencilRef != nullptr ? nStencilRef = *m_renderState->stencilRef : 0;
			m_renderState->stencilTest != nullptr ? nStencilTest = *m_renderState->stencilTest : 0;
			m_renderState->stencilWrite != nullptr ? nStencilWrite = *m_renderState->stencilWrite : 0;
			m_renderState->stencilOpX != nullptr ? nStencilOpX = *m_renderState->stencilOpX : 0;
			m_renderState->stencilOpY != nullptr ? nStencilOpY = *m_renderState->stencilOpY : 0;
			m_renderState->stencilOpZ != nullptr ? nStencilOpZ = *m_renderState->stencilOpZ : 0;
		}

		if (depthWrite == nullptr)
		{
			if (m_renderState->depthWrite != nullptr)
				nDepthWrite = *m_renderState->depthWrite;
			else
				nDepthWrite = *RenderState::Default.depthWrite;
		}
		else
		{
			nDepthWrite = *(float*)depthWrite->data;
		}

		if (depthTest == nullptr)
		{
			if (m_renderState->depthTest != nullptr)
				nDepthTest = *m_renderState->depthTest;
			else
				nDepthTest = *RenderState::Default.depthTest;
		}
		else
		{
			nDepthTest = *(float*)depthTest->data;
		}

		if (blend == nullptr)
		{
			if (m_renderState->blend != nullptr)
				nBlend = *m_renderState->blend;
			else
				nBlend = *RenderState::Default.blend;
		}
		else
		{
			nBlend = *(float*)blend->data;
		}

		if (stencilRef == nullptr)
		{
			if (m_renderState->stencilRef != nullptr)
				nStencilRef = *m_renderState->stencilRef;
			else
				nStencilRef = *RenderState::Default.stencilRef;
		}
		else
		{
			nStencilRef = *(float*)stencilRef->data;
		}

		if (stencilTest == nullptr)
		{
			if (m_renderState->stencilTest != nullptr)
				nStencilTest = *m_renderState->stencilTest;
			else
				nStencilTest = *RenderState::Default.stencilTest;
		}
		else
		{
			nStencilTest = *(float*)stencilTest->data;
		}

		if (stencilWrite == nullptr)
		{
			if (m_renderState->stencilWrite != nullptr)
				nStencilWrite = *m_renderState->stencilWrite;
			else
				nStencilWrite = *RenderState::Default.stencilWrite;
		}
		else
		{
			nStencilWrite = *(float*)stencilWrite->data;
		}

		if (stencilOp == nullptr)
		{
			if (m_renderState->stencilOpX != nullptr)
			{
				nStencilOpX = *m_renderState->stencilOpX;
				nStencilOpY = *m_renderState->stencilOpY;
				nStencilOpZ = *m_renderState->stencilOpZ;
			}
			else
			{
				nStencilOpX = *RenderState::Default.stencilOpX;
				nStencilOpY = *RenderState::Default.stencilOpY;
				nStencilOpZ = *RenderState::Default.stencilOpZ;
			}
		}
		else
		{
			nStencilOpX = *(float*)stencilOp->data;
			nStencilOpY = *((float*)stencilOp->data + 1);
			nStencilOpZ = *((float*)stencilOp->data + 2);
		}


		RenderStateContext::setDepthMask(m_pWebGLEngine, nDepthWrite > 0 ? true : false);
		if (nDepthTest == RenderState::DEPTHTEST_OFF)
			RenderStateContext::setDepthTest(m_pWebGLEngine, false);
		else 
		{
			RenderStateContext::setDepthTest(m_pWebGLEngine, true);
			RenderStateContext::setDepthFunc(m_pWebGLEngine, (CompareFunction)nDepthTest);
		}
		//blend
		switch (nBlend)
		{
		case RenderState::BLEND_DISABLE:
			RenderStateContext::setBlend(m_pWebGLEngine, false);
			break;
		case RenderState::BLEND_ENABLE_ALL:
			{
				ShaderData::DataInfo* blendEquation = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND_EQUATION);
				ShaderData::DataInfo* srcBlend = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND_SRC);
				ShaderData::DataInfo* dstBlend = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND_DST);

				int nBlendEquation;
				int nSrcBlend;
				int nDstBlend;

				if (m_stateParamsMap->m_shaderPassStatefirst)
				{
					m_renderState->blendEquation != nullptr ? nBlendEquation = *m_renderState->blendEquation : 0;
					m_renderState->srcBlend != nullptr ? nSrcBlend = *m_renderState->srcBlend : 0;
					m_renderState->dstBlend != nullptr ? nDstBlend = *m_renderState->dstBlend : 0;
				}
				if (blendEquation == nullptr)
				{
					if (m_renderState->blendEquation != nullptr)
						nBlendEquation = *m_renderState->blendEquation;
					else
						nBlendEquation = *RenderState::Default.blendEquation;
				}
				else
				{
					nBlendEquation = *(float*)blendEquation->data;
				}

				if (srcBlend == nullptr)
				{
					if (m_renderState->srcBlend != nullptr)
						nSrcBlend = *m_renderState->srcBlend;
					else
						nSrcBlend = *RenderState::Default.srcBlend;
				}
				else
				{
					nSrcBlend = *(float*)srcBlend->data;
				}

				if (dstBlend == nullptr)
				{
					if (m_renderState->srcBlend != nullptr)
						nDstBlend = *m_renderState->dstBlend;
					else
						nDstBlend = *RenderState::Default.dstBlend;
				}
				else
				{
					nDstBlend = *(float*)dstBlend->data;
				}

				RenderStateContext::setBlend(m_pWebGLEngine, true);
				RenderStateContext::setBlendEquation(m_pWebGLEngine, (BlendEquationSeparate)nBlendEquation);
				RenderStateContext::setBlendFunc(m_pWebGLEngine, (BlendFactor)nSrcBlend, (BlendFactor)nDstBlend);
			}
			break;
		case RenderState::BLEND_ENABLE_SEPERATE:
			ShaderData::DataInfo* blendEquationRGB = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND_EQUATION_RGB);
			ShaderData::DataInfo* blendEquationAlpha = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND_EQUATION_ALPHA);
			ShaderData::DataInfo* srcRGB = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND_SRC_RGB);
			ShaderData::DataInfo* dstRGB = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND_DST_RGB);
			ShaderData::DataInfo* srcAlpha = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND_SRC_ALPHA);
			ShaderData::DataInfo* dstAlpha = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_BLEND_DST_ALPHA);

			int nBlendEquationRGB;
			int nBlendEquationAlpha;
			int nSrcRGB;
			int nDstRGB;
			int nSrcAlpha;
			int nDstAlpha;

			if (m_stateParamsMap->m_shaderPassStatefirst)
			{
				m_renderState->blendEquationRGB != nullptr ? nBlendEquationRGB = *m_renderState->blendEquationRGB : 0;
				m_renderState->blendEquationAlpha != nullptr ? nBlendEquationAlpha = *m_renderState->blendEquationAlpha : 0;
				m_renderState->srcBlendRGB != nullptr ? nSrcRGB = *m_renderState->srcBlendRGB : 0;
				m_renderState->dstBlendRGB != nullptr ? nDstRGB = *m_renderState->dstBlendRGB : 0;
				m_renderState->srcBlendAlpha != nullptr ? nSrcAlpha = *m_renderState->srcBlendAlpha : 0;
				m_renderState->dstBlendAlpha != nullptr ? nDstAlpha = *m_renderState->dstBlendAlpha : 0;
			}
			if (blendEquationRGB == nullptr)
			{
				if (m_renderState->blendEquationRGB != nullptr)
					nBlendEquationRGB = *m_renderState->blendEquationRGB;
				else
					nBlendEquationRGB = *RenderState::Default.blendEquationRGB;
			}
			else
			{
				nBlendEquationRGB = *(float*)blendEquationRGB->data;
			}

			if (blendEquationAlpha == nullptr)
			{
				if (m_renderState->blendEquationAlpha != nullptr)
					nBlendEquationAlpha = *m_renderState->blendEquationAlpha;
				else
					nBlendEquationAlpha = *RenderState::Default.blendEquationAlpha;
			}
			else
			{
				nBlendEquationAlpha = *(float*)blendEquationAlpha->data;
			}

			if (srcRGB == nullptr)
			{
				if (m_renderState->srcBlendRGB != nullptr)
					nSrcRGB = *m_renderState->srcBlendRGB;
				else
					nSrcRGB = *RenderState::Default.srcBlendRGB;
			}
			else
			{
				nSrcRGB = *(float*)srcRGB->data;
			}

			if (dstRGB == nullptr)
			{
				if (m_renderState->dstBlendRGB != nullptr)
					nDstRGB = *m_renderState->dstBlendRGB;
				else
					nDstRGB = *RenderState::Default.dstBlendRGB;
			}
			else
			{
				nDstRGB = *(float*)dstRGB->data;
			}

			if (srcAlpha == nullptr)
			{
				if (m_renderState->srcBlendAlpha != nullptr)
					nSrcAlpha = *m_renderState->srcBlendAlpha;
				else
					nSrcAlpha = *RenderState::Default.srcBlendAlpha;
			}
			else
			{
				nSrcAlpha = *(float*)srcAlpha->data;
			}

			if (dstAlpha == nullptr)
			{
				if (m_renderState->dstBlendAlpha != nullptr)
					nDstAlpha = *m_renderState->dstBlendAlpha;
				else
					nDstAlpha = *RenderState::Default.dstBlendAlpha;
			}
			else
			{
				nDstAlpha = *(float*)dstAlpha->data;
			}

			RenderStateContext::setBlend(m_pWebGLEngine, true);
			RenderStateContext::setBlendEquationSeparate(m_pWebGLEngine, (BlendEquationSeparate)nBlendEquationRGB, (BlendEquationSeparate)nBlendEquationAlpha);
			RenderStateContext::setBlendFuncSeperate(m_pWebGLEngine, (BlendFactor)nSrcRGB, (BlendFactor)nDstRGB, (BlendFactor)nSrcAlpha, (BlendFactor)nDstAlpha);
			break;
		}

		//Stencil
		RenderStateContext::setStencilMask(m_pWebGLEngine, nStencilWrite > 0 ? true : false);
		if (nStencilTest == RenderState::STENCILTEST_OFF)
		{
			RenderStateContext::setStencilTest(m_pWebGLEngine, false);
		}
		else 
		{
			RenderStateContext::setStencilTest(m_pWebGLEngine, true);
			RenderStateContext::setStencilFunc(m_pWebGLEngine, (CompareFunction)nStencilTest, nStencilRef);
		}
		RenderStateContext::setstencilOp(m_pWebGLEngine, (StencilOperation)nStencilOpX, (StencilOperation)nStencilOpY, (StencilOperation)nStencilOpZ);
	}
	void ShaderInstance::uploadRenderStateFrontFace(ShaderData* shaderDatas, bool isTarget, bool invertFront)
	{
		m_cullStateCMD.clear();
		ShaderData::DataInfo* pCull = _getRenderState(shaderDatas, (int)Shader3D::RENDER_STATE_CULL);
		
		/*if (pCull)
		{
			float cc = *(float*)pCull->data;
			LOGI("cc %f %p", cc, pCull->data);
		}*/
        float* pfCull = (pCull != nullptr ? (float*)pCull->data : nullptr);
		if (m_stateParamsMap->m_shaderPassStatefirst)
		{
			if (m_renderState->cull != nullptr )
			{
				pfCull = m_renderState->cull;
			}
			
		}
		int nCull;
		if (pfCull != nullptr)
		{
            nCull = *(float*)pfCull;
		}
		else
		{
            nCull = *RenderState::Default.cull;
		}

		int forntFace;
		switch (nCull)
		{
		case RenderState::CULL_NONE:
			m_cullStateCMD.addCMDInt1(RenderStateType::CullFace, false);
				if (isTarget != invertFront)
					forntFace = (int)CullMode::Front;//gl.CCW
				else
					forntFace = (int)CullMode::Back;
				m_cullStateCMD.addCMDInt1(RenderStateType::FrontFace, forntFace);
			break;
		case RenderState::CULL_FRONT:
			m_cullStateCMD.addCMDInt1(RenderStateType::CullFace, true);
			if (isTarget == invertFront)
				forntFace = (int)CullMode::Front;//gl.CCW
			else
				forntFace = (int)CullMode::Back;
			m_cullStateCMD.addCMDInt1(RenderStateType::FrontFace, forntFace);
			break;
		case RenderState::CULL_BACK:
			m_cullStateCMD.addCMDInt1(RenderStateType::CullFace, true);
			if (isTarget != invertFront)
				forntFace = (int)CullMode::Front;//gl.CCW
			else
				forntFace = (int)CullMode::Back;
			m_cullStateCMD.addCMDInt1(RenderStateType::FrontFace, forntFace);
			break;
		}
		m_pWebGLEngine->applyRenderStateCMD(m_cullStateCMD);
	}
}
//------------------------------------------------------------------------------

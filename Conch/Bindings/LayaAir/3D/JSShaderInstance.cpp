#include "JSShaderInstance.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "../RenderEngine/WebGLEngine/JSWebGLEngine.h"
#include "JSAttributeMap.h"
#include "JSShaderData.h"
#include "JSRenderState.h"

namespace laya
{
	enum class UniformParamsMapType
	{
		Scene = 0,
		Camera,
		Sprite,
		Material,
	};

	JSShaderInstance::JSShaderInstance()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchShaderInstance", 128, this);
	}
	JSShaderInstance::JSShaderInstance(JSValueAsParam engine, const char* sVS,const char* sPS, JSValueAsParam arributeMap, JSValueAsParam renderState)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(engine);
		m_pJSAttributeMapHandle.reset(arributeMap);
		JSAttributeMap* pAttributeMap = (JSAttributeMap*)Converter<JSAttributeMap*>::ToCpp(arributeMap);
        m_pJSRenderStateHandle.reset(renderState);
		JSRenderState* pRenderState = (JSRenderState*)Converter<JSRenderState*>::ToCpp(renderState);
		m_pShaderInstance = new ShaderInstance(pWebglEngine->m_pEngine, sVS, sPS, &pAttributeMap->m_pAttributeMap, &pAttributeMap->m_pStateMap, &pRenderState->m_renderState);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchShaderInstance", 128, this);
	}
	//------------------------------------------------------------------------------
	JSShaderInstance::~JSShaderInstance()
	{
		m_pJSAttributeMapHandle.reset();
        m_pJSRenderStateHandle.reset();
		if (m_pShaderInstance)
		{
			delete m_pShaderInstance;
			m_pShaderInstance = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchShaderInstance", this);
	}
	bool JSShaderInstance::bind()
	{
		return m_pShaderInstance->bind();
	}
	void JSShaderInstance::destroy()
	{
		m_pShaderInstance->destroy();
	}
	int JSShaderInstance::uploadUniforms(int shaderUniform, JSValueAsParam pShaderDatas, bool uploadUnTexture)
	{
		JSShaderData* pShaderData = (JSShaderData*)Converter<JSShaderData*>::ToCpp(pShaderDatas);
		switch ((UniformParamsMapType)shaderUniform)
		{
		case UniformParamsMapType::Scene:
			return m_pShaderInstance->uploadUniforms(&m_pShaderInstance->m_sceneUniformParamsMap, pShaderData->m_pShaderData, uploadUnTexture);
			break;
		case UniformParamsMapType::Camera:
			return m_pShaderInstance->uploadUniforms(&m_pShaderInstance->m_cameraUniformParamsMap, pShaderData->m_pShaderData, uploadUnTexture);
			break;
		case UniformParamsMapType::Sprite:
			return m_pShaderInstance->uploadUniforms(&m_pShaderInstance->m_spriteUniformParamsMap, pShaderData->m_pShaderData, uploadUnTexture);
			break;
		case UniformParamsMapType::Material:
			return m_pShaderInstance->uploadUniforms(&m_pShaderInstance->m_materialUniformParamsMap, pShaderData->m_pShaderData, uploadUnTexture);
			break;
		default:
			assert(true);
			return 0;
			break;
		}
		
	}
	int JSShaderInstance::uploadCustomUniforms(int index, JSValueAsParam pData)
	{
		char* pArrayBuffer = NULL;
		int nArrayBufferSize = 0;
		bool bIsArrayBuffer = extractJSAB(pData, pArrayBuffer, nArrayBufferSize);
		if (bIsArrayBuffer)
		{
			m_pShaderInstance->uploadCustomUniforms(index, pArrayBuffer, nArrayBufferSize);
		}
		return 0;
	}
	void JSShaderInstance::uploadRenderStateBlendDepth(JSValueAsParam pShaderDatas)
	{
		JSShaderData* pShaderData = (JSShaderData*)Converter<JSShaderData*>::ToCpp(pShaderDatas);
		m_pShaderInstance->uploadRenderStateBlendDepth(pShaderData->m_pShaderData);
	}
	void JSShaderInstance::uploadRenderStateFrontFace(JSValueAsParam pShaderDatas, bool isTarget, bool invertFront)
	{
		JSShaderData* pShaderData = (JSShaderData*)Converter<JSShaderData*>::ToCpp(pShaderDatas);
		m_pShaderInstance->uploadRenderStateFrontFace(pShaderData->m_pShaderData, isTarget, invertFront);
	}
	//------------------------------------------------------------------------------
	void JSShaderInstance::exportJS(Context& context)
	{
		class_<JSShaderInstance> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam, const char*, const char*, JSValueAsParam, JSValueAsParam>();
		class_binding.function("bind", &JSShaderInstance::bind);
		class_binding.function("destroy", &JSShaderInstance::destroy);
		class_binding.function("uploadUniforms", &JSShaderInstance::uploadUniforms);
	 	class_binding.function("uploadCustomUniforms", &JSShaderInstance::uploadCustomUniforms);
		class_binding.function("uploadRenderStateBlendDepth", &JSShaderInstance::uploadRenderStateBlendDepth);
		class_binding.function("uploadRenderStateFrontFace", &JSShaderInstance::uploadRenderStateFrontFace);
		context.class_("conchShaderInstance", class_binding);
	}
}
//------------------------------------------------------------------------------

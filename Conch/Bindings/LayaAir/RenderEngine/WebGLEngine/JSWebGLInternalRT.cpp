#include "JSWebGLInternalRT.h"
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../../../../LayaAir/RenderEngine/RenderEngine/WebGLEngine/WebGLInternalRT.h"
#include "JSWebGLInternalTex.h"

namespace laya
{

	JSWebGLInternalRT::JSWebGLInternalRT()
	{
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchWebGLInternalRT", 4, this);
	}
	//------------------------------------------------------------------------------
	JSWebGLInternalRT::JSWebGLInternalRT(WebGLInternalRT* texture, bool bNeedDelete)
	{
		m_pRenderTexture = texture;
		m_bNeedDelete = bNeedDelete;
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchWebGLInternalRT", 4, this);
	}
	//------------------------------------------------------------------------------
	JSWebGLInternalRT::~JSWebGLInternalRT()
	{
		m_pJSDepthTexture.reset();
		m_pJSTextures.reset();
		if (m_bNeedDelete && m_pRenderTexture)
		{
			delete m_pRenderTexture;
			m_pRenderTexture = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchWebGLInternalRT", this);
	}
	JsValue JSWebGLInternalRT::getTextures()
	{
		if (m_pJSTextures.isEmpty())
		{
			int size = m_pRenderTexture->m_textures.size();
			std::vector<JSWebGLInternalTex*> vec;
			vec.reserve(size);
			for (int i = 0; i < size; i++)
			{
				vec.push_back(new JSWebGLInternalTex(m_pRenderTexture->m_textures[i]));
			}
			m_pJSTextures.reset(Converter<std::vector<JSWebGLInternalTex*>>::ToJs(vec));
			return m_pJSTextures.toLocal().handle_;
		}
		else
		{
			return m_pJSTextures.toLocal().handle_;
		}
	}
	bool JSWebGLInternalRT::generateMipmap()
	{
		return m_pRenderTexture->m_generateMipmap;
	}
	bool JSWebGLInternalRT::isCube()
	{
		return m_pRenderTexture->m_isCube;
	}
	int JSWebGLInternalRT::getSamples()
	{
		return m_pRenderTexture->m_samples;
	}
	void JSWebGLInternalRT::dispose()
	{
		m_pRenderTexture->dispose();
	}
	int JSWebGLInternalRT::getDepthStencilFormat()
	{
		return (int)m_pRenderTexture->m_depthStencilFormat;
	}
	int JSWebGLInternalRT::getColorFormat()
	{
		return (int)m_pRenderTexture->m_colorFormat;
	}
	JsValue JSWebGLInternalRT::getDepthTexture()
	{
		if (m_pJSDepthTexture.isEmpty())
		{
			JSWebGLInternalTex* pTex = new JSWebGLInternalTex(m_pRenderTexture->m_depthTexture);
			m_pJSDepthTexture.reset(JSP_TO_JS(JSWebGLInternalTex*, pTex));
			return m_pJSDepthTexture.toLocal().handle_;
		}
		else
		{
			return m_pJSDepthTexture.toLocal().handle_;
		}
	}
	bool JSWebGLInternalRT::getDeleteRT()
	{
		return m_bNeedDelete;
	}
	void JSWebGLInternalRT::setDeleteRT(bool value)
	{
		m_bNeedDelete = value;
	}
	int JSWebGLInternalRT::getGpuMemory()
	{
		return m_pRenderTexture->getGpuMemory();
	}
	//------------------------------------------------------------------------------
	void JSWebGLInternalRT::exportJS(Context& context)
	{
		class_<JSWebGLInternalRT> class_binding;
		class_binding.constructor<>();
		class_binding.property("_deleteRT", &JSWebGLInternalRT::getDeleteRT, &JSWebGLInternalRT::setDeleteRT);
		class_binding.property("_textures", &JSWebGLInternalRT::getTextures);
		class_binding.property("_generateMipmap", &JSWebGLInternalRT::generateMipmap);
		class_binding.property("_isCube", &JSWebGLInternalRT::isCube);
		class_binding.property("_samples", &JSWebGLInternalRT::getSamples);
		class_binding.property("depthStencilFormat", &JSWebGLInternalRT::getDepthStencilFormat);
		class_binding.property("colorFormat", &JSWebGLInternalRT::getColorFormat);
		class_binding.property("_depthTexture", &JSWebGLInternalRT::getDepthTexture);
		class_binding.property("gpuMemory", &JSWebGLInternalRT::getGpuMemory);
		class_binding.function("dispose", &JSWebGLInternalRT::dispose);
		context.class_("conchWebGLInternalRT", class_binding);
	}
	//------------------------------------------------------------------------------
}

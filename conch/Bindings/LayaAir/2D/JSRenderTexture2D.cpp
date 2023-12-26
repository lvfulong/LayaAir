#include "JSRenderTexture2D.h"
#include <utils/Log.h>
#include "../RenderEngine/WebGLEngine/JSWebGLEngine.h"
#include <render/driver/gles/WebGLEngine.h>
#include <render/driver/gles/GLTextureContext.h>
#include "../RenderEngine/WebGLEngine/JSGLTextureContext.h"
#include "../RenderEngine/WebGLEngine/JSWebGLInternalRT.h"

namespace laya
{
	JSRenderTexture2D::JSRenderTexture2D()
	{
		m_renderTexture = nullptr;
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchRenderTexture2D", 4, this);
	}
	//------------------------------------------------------------------------------
	JSRenderTexture2D::JSRenderTexture2D(WebGLEngine* pWebGLEngine, std::shared_ptr<RenderTexture2D> renderTexture)
	{
		m_pWebGLEngine = pWebGLEngine;
		m_renderTexture = renderTexture;
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchRenderTexture2D", 4, this);
	}
	//------------------------------------------------------------------------------
	JSRenderTexture2D::JSRenderTexture2D(JSValueAsParam pEngine, int width, int height, int format, int depthStencilFormat)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(pEngine);
		m_pWebGLEngine = pWebglEngine->m_pEngine;
		m_renderTexture = std::make_shared<RenderTexture2D>(pWebglEngine->m_pEngine, width, height, (RenderTargetFormat)format, (RenderTargetFormat)depthStencilFormat);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchRenderTexture2D", 4, this);
	}
	//------------------------------------------------------------------------------
	JSRenderTexture2D::~JSRenderTexture2D()
	{
		destroy();
		JCMemorySurvey::GetInstance()->releaseClass("conchRenderTexture2D", this);
	}
	void JSRenderTexture2D::destroy()
	{
		m_renderTexture.reset();
	}
	void JSRenderTexture2D::recycle()
	{

	}

	//------------------------------------------------------------------------------
	/*int JSRenderTexture2D::getID()
	{
		return m_textureContext->getID();
	}*/
	bool JSRenderTexture2D::getIsCube()
	{
		return m_renderTexture->m_renderTarget->m_isCube;
	}

	int JSRenderTexture2D::getSamples()
	{
		return m_renderTexture->m_renderTarget->m_samples;
	}

	bool JSRenderTexture2D::getGenerateMipmap()
	{
		return m_renderTexture->m_renderTarget->m_generateMipmap;
	}
	void JSRenderTexture2D::start()
	{
		m_renderTexture->start();
	}
	void JSRenderTexture2D::end()
	{
		m_renderTexture->end();
	}
	void JSRenderTexture2D::restore()
	{
		m_renderTexture->restore();
	}
	void JSRenderTexture2D::clear(float r, float g, float b, float a)
	{
		m_renderTexture->clear(r, g, b, a);
	}
	JsValue JSRenderTexture2D::getData(int x, int y, int width, int height)
	{
		std::vector<uint8_t> buffer;
		m_pWebGLEngine->getTextureContext()->getRenderTextureData(m_renderTexture->m_renderTarget, x, y, width, height, buffer);
		return JSGLTextureContext::_getRenderTextureData(buffer, width, height, (int)m_renderTexture->m_colorFormat);
	}
	void JSRenderTexture2D::_disposeResource()
	{
		//this._renderTarget && this._renderTarget.dispose();
	}
	bool JSRenderTexture2D::getDeleteRT()
	{
		return true;// m_bNeedDelete;
	}
	void JSRenderTexture2D::setDeleteRT(bool value)
	{
		//m_bNeedDelete = value;
	}
	JsValue JSRenderTexture2D::getRenderTarget()
	{
		return JSP_TO_JS(JSWebGLInternalRT*, new JSWebGLInternalRT(m_renderTexture->m_renderTarget, false));
	}
	//------------------------------------------------------------------------------
	void JSRenderTexture2D::exportJS(Context& context)
	{
		class_<JSRenderTexture2D> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam, int, int, int, int>();
		class_binding.property("_deleteRT", &JSRenderTexture2D::getDeleteRT, &JSRenderTexture2D::setDeleteRT);
		//class_binding.property(id, JSRenderTexture2D, getID);
		class_binding.property("_renderTarget", &JSRenderTexture2D::getRenderTarget);
		class_binding.property("isCube", &JSRenderTexture2D::getIsCube);
		class_binding.property("samples", &JSRenderTexture2D::getSamples);
		class_binding.property("generateMipmap", &JSRenderTexture2D::getGenerateMipmap);
		class_binding.function("destroy", &JSRenderTexture2D::destroy);
		class_binding.function("recycle", &JSRenderTexture2D::recycle);
		class_binding.function("start", &JSRenderTexture2D::start);
		class_binding.function("end", &JSRenderTexture2D::end);
		class_binding.function("restore", &JSRenderTexture2D::restore);
		class_binding.function("clear", &JSRenderTexture2D::clear);
		class_binding.function("getData", &JSRenderTexture2D::getData);
		class_binding.function("_disposeResource", &JSRenderTexture2D::_disposeResource);
		context.class_("conchRenderTexture2D", class_binding);
	}
	//------------------------------------------------------------------------------
}

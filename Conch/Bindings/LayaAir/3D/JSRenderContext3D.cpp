#include "JSRenderContext3D.h"
#include <Binder/JSInterface.h>
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../RenderEngine/WebGLEngine/JSWebGLEngine.h"
#include "JSShaderData.h"
#include "../RenderEngine/WebGLEngine/JSWebGLInternalRT.h"

namespace laya
{
	JSRenderContext3D::JSRenderContext3D()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(64);
		JCMemorySurvey::GetInstance()->newClass("conchRenderContext3D", 64, this);
	}
	JSRenderContext3D::JSRenderContext3D(JSValueAsParam pEngine)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(pEngine);
		m_pRenderContext3D = new RenderContext3D(pWebglEngine->m_pEngine);
		AdjustAmountOfExternalAllocatedMemory(64);
		JCMemorySurvey::GetInstance()->newClass("conchRenderContext3D", 64, this);
	}
	//------------------------------------------------------------------------------
	JSRenderContext3D::~JSRenderContext3D()
	{
		m_pJSCameraShaderData.reset();
		m_pJSSceneShaderData.reset();
		m_pJSGlobalShaderData.reset();
		m_pJSDstTarget.reset();
		if (m_pRenderContext3D)
		{
			delete m_pRenderContext3D;
			m_pRenderContext3D = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchRenderContext3D", this);
	}
	void JSRenderContext3D::changeViewport(float x, float y, float width, float height)
	{
		Viewport* pViewport = m_pRenderContext3D->m_pViewport;
		pViewport->x = x;
		pViewport->y = y;
		pViewport->width = width;
		pViewport->height = height;
	}
	void JSRenderContext3D::changeScissor(float x, float y, float width, float height)
	{
		Vector4* pScissor = m_pRenderContext3D->m_pScissor;
		pScissor->x = x;
		pScissor->y = y;
		pScissor->z = width;
		pScissor->w = height;
	}
	void JSRenderContext3D::setCameraShaderData(JSValueAsParam pShaderData)
	{
		JSShaderData* pJSShaderData = (JSShaderData*)Converter<JSShaderData*>::ToCpp(pShaderData);
		if (pJSShaderData && pJSShaderData->m_pShaderData)
		{
			m_pJSCameraShaderData.reset(pShaderData);
			m_pRenderContext3D->m_pCameraShaderData = pJSShaderData->m_pShaderData;
		}
		else
		{
			m_pJSCameraShaderData.reset();
			m_pRenderContext3D->m_pCameraShaderData = nullptr;
		}
	}
	JsValue JSRenderContext3D::getCameraShaderData()
	{
		return m_pJSCameraShaderData.toLocal().handle_;
	}
	void JSRenderContext3D::setSceneShaderData(JSValueAsParam pShaderData)
	{
		JSShaderData* pJSShaderData = (JSShaderData*)Converter<JSShaderData*>::ToCpp(pShaderData);
		if (pJSShaderData && pJSShaderData->m_pShaderData)
		{
			m_pJSSceneShaderData.reset(pShaderData);
			m_pRenderContext3D->m_pSceneShaderData = pJSShaderData->m_pShaderData;
		}
		else
		{
			m_pJSSceneShaderData.reset();
			m_pRenderContext3D->m_pSceneShaderData = nullptr;
		}
	}
	JsValue JSRenderContext3D::getSceneShaderData()
	{
		return m_pJSSceneShaderData.toLocal().handle_;
	}
	//------------------------------------------------------------------------------
	void JSRenderContext3D::setDestTarget(JSValueAsParam target)
	{
		JSWebGLInternalRT* pJSRT = (JSWebGLInternalRT*)Converter<JSWebGLInternalRT*>::ToCpp(target);
		if (pJSRT && pJSRT->m_pRenderTexture)
		{
			m_pJSDstTarget.reset(target);
			m_pRenderContext3D->m_pDstTarget = pJSRT->m_pRenderTexture;
		}
		else
		{
			m_pJSDstTarget.reset();
			m_pRenderContext3D->m_pDstTarget = nullptr;
		}
	}
	//------------------------------------------------------------------------------
	JsValue JSRenderContext3D::getDestTarget()
	{
		return m_pJSDstTarget.toLocal().handle_;
	}
	void JSRenderContext3D::JSRenderContext3D::applyContext(int cameraUpdateMark)
	{
		m_pRenderContext3D->applyContext(cameraUpdateMark);
	}
	void  JSRenderContext3D::setInvertY(bool value)
	{
		m_pRenderContext3D->m_bInvertY = value;
	}
	bool JSRenderContext3D::getInvertY()
	{
		return m_pRenderContext3D->m_bInvertY;
	}
	void  JSRenderContext3D::setPipelineMode(const char* value)
	{
		m_pRenderContext3D->m_sPipelineMode = value;
	}
	const char* JSRenderContext3D::getPipelineMode()
	{
		return m_pRenderContext3D->m_sPipelineMode.c_str();
	}
	void JSRenderContext3D::setSceneID(int value)
	{
		m_pRenderContext3D->m_nSceneID = value;
	}
	int JSRenderContext3D::getSceneID()
	{
		return m_pRenderContext3D->m_nSceneID;
	}
	void JSRenderContext3D::setCameraUpdateMark(int value)
	{
		m_pRenderContext3D->m_nCameraUpdateMark = value;
	}
	int JSRenderContext3D::getCameraUpdateMark()
	{
		return m_pRenderContext3D->m_nCameraUpdateMark;
	}
	void JSRenderContext3D::setGlobalShaderData(JSValueAsParam pShaderData)
	{
		JSShaderData* pJSShaderData = (JSShaderData*)Converter<JSShaderData*>::ToCpp(pShaderData);
		if (pJSShaderData && pJSShaderData->m_pShaderData)
		{
			m_pJSGlobalShaderData.reset(pShaderData);
			m_pRenderContext3D->m_pGlobalShaderData = pJSShaderData->m_pShaderData;
		}
		else {
			m_pJSGlobalShaderData.reset();
			m_pRenderContext3D->m_pGlobalShaderData = nullptr;
		}
	}
	JsValue JSRenderContext3D::getGlobalShaderData()
	{
		return m_pJSGlobalShaderData.toLocal().handle_;
	}
	//------------------------------------------------------------------------------
	void JSRenderContext3D::exportJS(Context& context)
	{
		class_<JSRenderContext3D> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam>();
		class_binding.property("destTarget", &JSRenderContext3D::getDestTarget, &JSRenderContext3D::setDestTarget);
		class_binding.property("invertY", &JSRenderContext3D::getInvertY, &JSRenderContext3D::setInvertY);
		class_binding.property("pipelineMode", &JSRenderContext3D::getPipelineMode, &JSRenderContext3D::setPipelineMode);
		class_binding.property("cameraShaderData", &JSRenderContext3D::getCameraShaderData, &JSRenderContext3D::setCameraShaderData);
		class_binding.property("sceneID", &JSRenderContext3D::getSceneID, &JSRenderContext3D::setSceneID);
		class_binding.property("cameraUpdateMark", &JSRenderContext3D::getCameraUpdateMark, &JSRenderContext3D::setCameraUpdateMark);
		class_binding.property("sceneShaderData", &JSRenderContext3D::getSceneShaderData, &JSRenderContext3D::setSceneShaderData);
		class_binding.property("globalShaderData", &JSRenderContext3D::getGlobalShaderData, &JSRenderContext3D::setGlobalShaderData);
		class_binding.function("changeViewport", &JSRenderContext3D::changeViewport);
		class_binding.function("changeScissor", &JSRenderContext3D::changeScissor);
		class_binding.function("setCameraShaderData", &JSRenderContext3D::setCameraShaderData);
		class_binding.function("setSceneShaderData", &JSRenderContext3D::setSceneShaderData);
		class_binding.function("applyContext", &JSRenderContext3D::applyContext);
		context.class_("conchRenderContext3D", class_binding);

	}
}
//------------------------------------------------------------------------------

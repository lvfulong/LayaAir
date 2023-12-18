#include "JSWebGLEngine.h"
#include <Utils/Log.h>
#include <Utils/JCMemorySurvey.h>
#include "../../../../LayaAir/RenderEngine/RenderEngine/WebGLEngine/WebGLInternalTex.h"
#include "../../../../LayaAir/RenderEngine/RenderEnum/RenderCapable.h"
#include "../../../../LayaAir/RenderEngine/RenderEnum/RenderParams.h"
#include "../../../../LayaAir/RenderEngine/RenderEngine/WebGLEngine/GLEnum/WebGLMode.h"
#include "../../3D/JSCommandUniformMap.h"
#include "../../3D/JSRenderStateCommand.h"
#include "JSWebGLInternalTex.h"
#include "JCSystemConfig.h"

namespace laya
{

	JSWebGLEngine::JSWebGLEngine()
	{
		assert(true);
		//m_pEngine = new WebGLEngine((WebGLMode)webglMode);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchWebGLEngine", 4, this);
	}
	//------------------------------------------------------------------------------
	JSWebGLEngine::JSWebGLEngine(int webglMode)
	{
		m_pEngine = new WebGLEngine((WebGLMode)webglMode);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchWebGLEngine", 4, this);
	}
	//------------------------------------------------------------------------------
	void JSWebGLEngine::initRenderEngine()
	{
		m_pEngine->initRenderEngine();
	}
	//------------------------------------------------------------------------------
	JSWebGLEngine::~JSWebGLEngine()
	{
        if (!g_kSystemConfig.m_bConchWebGL)
        {
            if (m_pEngine)
            {
                delete m_pEngine;
                m_pEngine = nullptr;
            }
        }
		JCMemorySurvey::GetInstance()->releaseClass("conchWebGLEngine", this);
	}
	//------------------------------------------------------------------------------
	int JSWebGLEngine::getParams(int type)
	{
		return m_pEngine->getParams((RenderParams)type);
	}
	//------------------------------------------------------------------------------
	bool JSWebGLEngine::getCapable(int type)
	{
		return m_pEngine->getCapable((RenderCapable)type);
	}
	//------------------------------------------------------------------------------
	/*void JSWebGLEngine::applyRenderState(JSValueAsParam param)
	{
	}*/
	//------------------------------------------------------------------------------
	void JSWebGLEngine::applyRenderStateCommand(JSValueAsParam param)
	{
		JSRenderStateCommand* pJSRenderStateCommand = (JSRenderStateCommand*)Converter<JSRenderStateCommand*>::ToCpp(param);
		m_pEngine->applyRenderStateCMD(*pJSRenderStateCommand->m_pRenderStateCommand);
	}
	//------------------------------------------------------------------------------
	void JSWebGLEngine::viewport(int x, int y, int width, int height)
	{
		m_pEngine->viewport(x, y, width, height);
	}
	//------------------------------------------------------------------------------
	void JSWebGLEngine::scissor(int x, int y, int width, int height)
	{
		m_pEngine->scissor(x, y, width, height);
	}
	//------------------------------------------------------------------------------
	void JSWebGLEngine::scissorTest(bool value)
	{
		m_pEngine->scissorTest(value);
	}
	//------------------------------------------------------------------------------
	void JSWebGLEngine::colorMask(bool r, bool g, bool b, bool a)
	{
		m_pEngine->colorMask(r, g, b, a);
	}
	//------------------------------------------------------------------------------
	void JSWebGLEngine::clearRenderTexture(int clearFlag, bool hasClearcolor, float r, float g, float b, float a, float clearDepth)
	{
		Color c(r, g, b, a);
		m_pEngine->clearRenderTexture((uint32_t)clearFlag, hasClearcolor ? &c : nullptr, clearDepth);
	}
	//------------------------------------------------------------------------------
	bool JSWebGLEngine::isWebGL2()
	{
		return m_pEngine->isWebGL2();
	}
	int JSWebGLEngine::propertyNameToID(const char* name)
	{
		return m_pEngine->propertyNameToID(name);
	}
	int JSWebGLEngine::getUBOPointer(const char* name)
	{
		return m_pEngine->getUBOPointer(name);
	}
	void JSWebGLEngine::copySubFrameBuffertoTex(JSValueAsParam texture, int level, int xoffset, int yoffset, int x, int y, int width, int height)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		return m_pEngine->copySubFrameBuffertoTex(pJSInternalTex->m_texture.get(), level, xoffset, yoffset, x, y, width, height);
	}
	void JSWebGLEngine::addStatisticsInfo(int info, int value)
	{
		m_pEngine->addStatisticsInfo((RenderStatisticsInfo)info, value);
	}
	void JSWebGLEngine::clearStatisticsInfo(int value)
	{
		m_pEngine->clearStatisticsInfo((RenderStatisticsInfo)value);
	}
	int JSWebGLEngine::getStatisticsInfo(int value)
	{
		return m_pEngine->getStatisticsInfo((RenderStatisticsInfo)value);
	}
	void JSWebGLEngine::unbindVertexState()
	{
		return m_pEngine->unbindVertexState();
	}
	//------------------------------------------------------------------------------
	void JSWebGLEngine::exportJS(Context& context)
	{
        class_<JSWebGLEngine> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<int>();
		class_binding.property("isWebGL2", &JSWebGLEngine::isWebGL2);
		class_binding.function("clearRenderTexture", &JSWebGLEngine::clearRenderTexture);
		class_binding.function("viewport", &JSWebGLEngine::viewport);
		class_binding.function("scissor", &JSWebGLEngine::scissor);
		class_binding.function("scissorTest", &JSWebGLEngine::scissorTest);
		class_binding.function("colorMask", &JSWebGLEngine::colorMask);
		class_binding.function("getParams", &JSWebGLEngine::getParams);
		class_binding.function("getCapable", &JSWebGLEngine::getCapable);
		//JSP_ADD_METHOD("applyRenderState", JSWebGLEngine::applyRenderState);
		class_binding.function("applyRenderStateCommand", &JSWebGLEngine::applyRenderStateCommand);
		class_binding.function("initRenderEngine", &JSWebGLEngine::initRenderEngine);
		class_binding.function("propertyNameToID", &JSWebGLEngine::propertyNameToID);
		class_binding.function("getUBOPointer", &JSWebGLEngine::getUBOPointer);
		class_binding.function("copySubFrameBuffertoTex", &JSWebGLEngine::copySubFrameBuffertoTex);
		class_binding.function("addStatisticsInfo", &JSWebGLEngine::addStatisticsInfo);
		class_binding.function("clearStatisticsInfo", &JSWebGLEngine::clearStatisticsInfo);
		class_binding.function("getStatisticsInfo", &JSWebGLEngine::getStatisticsInfo);
		class_binding.function("unbindVertexState", &JSWebGLEngine::unbindVertexState);
		context.class_("conchWebGLEngine", class_binding);
	}
	//------------------------------------------------------------------------------
}

#include "JSGLESEngine.h"
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include <render/driver/gles/WebGLInternalTex.h>
#include <render/3D/design/renderEnum/RenderCapable.h>
#include <render/3D/design/renderEnum/RenderParams.h>
#include <render/driver/gles/GLEnum/WebGLMode.h>
#include "render/3D/temp/CommandUniformMap.h"
#include "JSWebGLInternalTex.h"
#include "JCSystemConfig.h"
#include "render/driver/gles/LayaGL.h"
namespace laya
{

	JSGLESEngine::JSGLESEngine()
	{
		assert(true);
		//m_pEngine = new GLESEngine((WebGLMode)webglMode);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchGLESEngine", 4, this);
	}
	//------------------------------------------------------------------------------
	JSGLESEngine::JSGLESEngine(int webglMode)
	{
		LayaGL::m_pWebglEngine = m_pEngine = new GLESEngine((WebGLMode)webglMode);
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchGLESEngine", 4, this);
	}
	//------------------------------------------------------------------------------
	void JSGLESEngine::initRenderEngine()
	{
		m_pEngine->initRenderEngine();
	}
	//------------------------------------------------------------------------------
	JSGLESEngine::~JSGLESEngine()
	{
        if (!g_kSystemConfig.m_bConchWebGL)
        {
            if (m_pEngine)
            {
                delete m_pEngine;
                m_pEngine = nullptr;
            }
        }
		JCMemorySurvey::GetInstance()->releaseClass("conchGLESEngine", this);
	}
	//------------------------------------------------------------------------------
	int JSGLESEngine::getParams(int type)
	{
		return m_pEngine->getParams((RenderParams)type);
	}
	//------------------------------------------------------------------------------
	bool JSGLESEngine::getCapable(int type)
	{
		return m_pEngine->getCapable((RenderCapable)type);
	}
	//------------------------------------------------------------------------------
	/*void JSGLESEngine::applyRenderState(JSValueAsParam param)
	{
	}*/
	//------------------------------------------------------------------------------
	void JSGLESEngine::applyRenderStateCommand(JSValueAsParam param)
	{
		JSRenderStateCommand* pJSRenderStateCommand = (JSRenderStateCommand*)Converter<JSRenderStateCommand*>::ToCpp(param);
		m_pEngine->applyRenderStateCMD(*pJSRenderStateCommand->m_pRenderStateCommand);
	}
	//------------------------------------------------------------------------------
	void JSGLESEngine::viewport(int x, int y, int width, int height)
	{
		m_pEngine->viewport(x, y, width, height);
	}
	//------------------------------------------------------------------------------
	void JSGLESEngine::scissor(int x, int y, int width, int height)
	{
		m_pEngine->scissor(x, y, width, height);
	}
	//------------------------------------------------------------------------------
	void JSGLESEngine::scissorTest(bool value)
	{
		m_pEngine->scissorTest(value);
	}
	//------------------------------------------------------------------------------
	void JSGLESEngine::colorMask(bool r, bool g, bool b, bool a)
	{
		m_pEngine->colorMask(r, g, b, a);
	}
	//------------------------------------------------------------------------------
	void JSGLESEngine::clearRenderTexture(int clearFlag, bool hasClearcolor, float r, float g, float b, float a, float clearDepth)
	{
		Color c(r, g, b, a);
		m_pEngine->clearRenderTexture((uint32_t)clearFlag, hasClearcolor ? &c : nullptr, clearDepth);
	}
	//------------------------------------------------------------------------------
	bool JSGLESEngine::isWebGL2()
	{
		return m_pEngine->isWebGL2();
	}
	int JSGLESEngine::propertyNameToID(const char* name)
	{
		return m_pEngine->propertyNameToID(name);
	}
	int JSGLESEngine::getUBOPointer(const char* name)
	{
		return m_pEngine->getUBOPointer(name);
	}
	void JSGLESEngine::copySubFrameBuffertoTex(JSValueAsParam texture, int level, int xoffset, int yoffset, int x, int y, int width, int height)
	{
		JSWebGLInternalTex* pJSInternalTex = (JSWebGLInternalTex*)Converter<JSWebGLInternalTex*>::ToCpp(texture);
		return m_pEngine->copySubFrameBuffertoTex(pJSInternalTex->m_texture.get(), level, xoffset, yoffset, x, y, width, height);
	}
	void JSGLESEngine::addStatisticsInfo(int info, int value)
	{
		m_pEngine->_addStatisticsInfo((RenderStatisticsInfo)info, value);
	}
	void JSGLESEngine::clearStatisticsInfo(int value)
	{
		m_pEngine->clearStatisticsInfo((RenderStatisticsInfo)value);
	}
	int JSGLESEngine::getStatisticsInfo(int value)
	{
		return m_pEngine->getStatisticsInfo((RenderStatisticsInfo)value);
	}
	void JSGLESEngine::unbindVertexState()
	{
		return m_pEngine->unbindVertexState();
	}
	//------------------------------------------------------------------------------
	void JSGLESEngine::exportJS(Context& context)
	{
        class_<JSGLESEngine> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<int>();
		class_binding.property("isWebGL2", &JSGLESEngine::isWebGL2);
		class_binding.function("clearRenderTexture", &JSGLESEngine::clearRenderTexture);
		class_binding.function("viewport", &JSGLESEngine::viewport);
		class_binding.function("scissor", &JSGLESEngine::scissor);
		class_binding.function("scissorTest", &JSGLESEngine::scissorTest);
		class_binding.function("colorMask", &JSGLESEngine::colorMask);
		class_binding.function("getParams", &JSGLESEngine::getParams);
		class_binding.function("getCapable", &JSGLESEngine::getCapable);
		//JSP_ADD_METHOD("applyRenderState", JSGLESEngine::applyRenderState);
		class_binding.function("applyRenderStateCommand", &JSGLESEngine::applyRenderStateCommand);
		class_binding.function("initRenderEngine", &JSGLESEngine::initRenderEngine);
		class_binding.function("propertyNameToID", &JSGLESEngine::propertyNameToID);
		class_binding.function("getUBOPointer", &JSGLESEngine::getUBOPointer);
		class_binding.function("copySubFrameBuffertoTex", &JSGLESEngine::copySubFrameBuffertoTex);
		class_binding.function("addStatisticsInfo", &JSGLESEngine::addStatisticsInfo);
		class_binding.function("clearStatisticsInfo", &JSGLESEngine::clearStatisticsInfo);
		class_binding.function("getStatisticsInfo", &JSGLESEngine::getStatisticsInfo);
		class_binding.function("unbindVertexState", &JSGLESEngine::unbindVertexState);
		context.class_("conchGLESEngine", class_binding);
	}
	//------------------------------------------------------------------------------
}

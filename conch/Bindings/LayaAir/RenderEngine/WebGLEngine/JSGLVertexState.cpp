#include "JSGLVertexState.h"
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "JSWebGLEngine.h"
#include "../../3D/JSIndexBuffer3D.h"
#include "../../3D/JSVertexBuffer3D.h"
#include <render/3D/temp/IndexBuffer.h>
#include <render/3D/temp/VertexBuffer.h>

namespace laya
{

	JSGLVertexState::JSGLVertexState()
	{
		assert(true);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchGLVertexState", 128, this);
	}
	JSGLVertexState::JSGLVertexState(JSValueAsParam engine)
	{
		JSWebGLEngine* pWebglEngine = (JSWebGLEngine*)Converter<JSWebGLEngine*>::ToCpp(engine);
		m_GLVertexState = new GLVertexState(pWebglEngine->m_pEngine);
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchGLVertexState", 128, this);
	}
	//------------------------------------------------------------------------------
	JSGLVertexState::~JSGLVertexState()
	{
		if (m_GLVertexState)
		{
			delete m_GLVertexState;
			m_GLVertexState = nullptr;
		}
		JCMemorySurvey::GetInstance()->releaseClass("conchGLVertexState", this);
	}
	void JSGLVertexState::bindVertexArray()
	{
		m_GLVertexState->bindVertexArray();
	}
	void JSGLVertexState::unbindVertexArray()
	{
		m_GLVertexState->unbindVertexArray();
	}
	void JSGLVertexState::applyVertexBuffer(JSValueAsParam pVertexBuffers)
	{
		std::vector<JSVertexBuffer3D*> vecJSVertexBuffer3Ds;
        Converter<std::vector<JSVertexBuffer3D*>>::ToCpp(pVertexBuffers, vecJSVertexBuffer3Ds);
		std::vector<VertexBuffer*> vecJCVertexBuffer3Ds;
		vecJCVertexBuffer3Ds.reserve(vecJSVertexBuffer3Ds.size());
		for (int i = 0, n = vecJSVertexBuffer3Ds.size(); i < n; i++)
		{
			assert(vecJSVertexBuffer3Ds[i] != nullptr);
			vecJCVertexBuffer3Ds.push_back(vecJSVertexBuffer3Ds[i]->m_pVertexBuffer3D);
		}
		m_GLVertexState->applyVertexBuffer(vecJCVertexBuffer3Ds);
	}
	void JSGLVertexState::applyIndexBuffer(JSValueAsParam pIndexBuffer)
	{
		JSIndexBuffer3D* pJSIndexBuffer = (JSIndexBuffer3D*)Converter<JSIndexBuffer3D*>::ToCpp(pIndexBuffer);
		if (pJSIndexBuffer && pJSIndexBuffer->m_pIndexBuffer3D)
		{
			m_GLVertexState->applyIndexBuffer(pJSIndexBuffer->m_pIndexBuffer3D);
		}
	}
	void JSGLVertexState::destroy()
	{
		m_GLVertexState->destroy();
	}
	//------------------------------------------------------------------------------
	void JSGLVertexState::exportJS(Context& context)
	{
		class_<JSGLVertexState> class_binding;
		class_binding.constructor<>();
		class_binding.constructor<JSValueAsParam>();
		class_binding.function("destroy", &JSGLVertexState::destroy);
		class_binding.function("applyIndexBuffer", &JSGLVertexState::applyIndexBuffer);
		class_binding.function("applyVertexBuffer", &JSGLVertexState::applyVertexBuffer);
		class_binding.function("bindVertexArray", &JSGLVertexState::bindVertexArray);
		class_binding.function("unbindVertexArray", &JSGLVertexState::unbindVertexArray);
		context.class_("conchGLVertexState", class_binding);
	}
}
//------------------------------------------------------------------------------

#include "JSRenderState.h"
#include <binder/JSInterface.h>
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include "JSRenderGeometryElement.h"
#include "JSShaderInstance.h"
#include "JSShaderData.h"
#include "JSRenderContext3D.h"
#include "JSTransform.h"

namespace laya
{
	JSRenderState::JSRenderState()
	{
		AdjustAmountOfExternalAllocatedMemory(128);
		JCMemorySurvey::GetInstance()->newClass("conchRenderState", 128, this);
	}
	//------------------------------------------------------------------------------
	JSRenderState::~JSRenderState()
	{	
		JCMemorySurvey::GetInstance()->releaseClass("conchRenderState", this);
	}
	float JSRenderState::getCull()
	{
		return *m_renderState.cull;
	}
	void JSRenderState::setCull(float value)
	{
		this->cull = value;
		m_renderState.cull = &this->cull;
	}
	int JSRenderState::getBlend()
	{
		return *m_renderState.blend;
	}
	void JSRenderState::setBlend(int value)
	{
		this->blend = value;
		m_renderState.blend = &this->blend;
	}
	int JSRenderState::getSrcBlend()
	{
		return *m_renderState.srcBlend;
	}
	void JSRenderState::setSrcBlend(int value)
	{
		this->srcBlend = value;
		m_renderState.srcBlend = &this->srcBlend;
	}
	int JSRenderState::getDstBlend()
	{
		return *m_renderState.dstBlend;
	}
	void JSRenderState::setDstBlend(int value)
	{
		this->dstBlend = value;
		m_renderState.dstBlend = &this->dstBlend;
	}
	int JSRenderState::getSrcBlendRGB()
	{
		return *m_renderState.srcBlendRGB;
	}
	void JSRenderState::setSrcBlendRGB(int value)
	{
		this->srcBlendRGB = value;
		m_renderState.srcBlendRGB = &this->srcBlendRGB;
	}
	int JSRenderState::getDstBlendRGB()
	{
		return *m_renderState.dstBlendRGB;
	}
	void JSRenderState::setDstBlendRGB(int value)
	{
		this->dstBlendRGB = value;
		m_renderState.dstBlendRGB = &this->dstBlendRGB;
	}
	int JSRenderState::getSrcBlendAlpha()
	{
		return *m_renderState.srcBlendAlpha;
	}
	void JSRenderState::setSrcBlendAlpha(int value)
	{
		this->srcBlendAlpha = value;
		m_renderState.srcBlendAlpha = &this->srcBlendAlpha;
	}
	int JSRenderState::getDstBlendAlpha()
	{
		return *m_renderState.dstBlendAlpha;
	}
	void JSRenderState::setDstBlendAlpha(int value)
	{
		this->dstBlendAlpha = value;
		m_renderState.dstBlendAlpha = &this->dstBlendAlpha;
	}
	int JSRenderState::getBlendEquation()
	{
		return *m_renderState.blendEquation;
	}
	void JSRenderState::setBlendEquation(int value)
	{
		this->blendEquation = value;
		m_renderState.blendEquation = &this->blendEquation;
	}
	int JSRenderState::getBlendEquationRGB()
	{
		return *m_renderState.blendEquationRGB;
	}
	void JSRenderState::setBlendEquationRGB(int value)
	{
		this->blendEquationRGB = value;
		m_renderState.blendEquationRGB = &this->blendEquationRGB;
	}
	int JSRenderState::getBlendEquationAlpha()
	{
		return *m_renderState.blendEquationAlpha;
	}
	void JSRenderState::setBlendEquationAlpha(int value)
	{
		this->blendEquationAlpha = value;
		m_renderState.blendEquationAlpha = &this->blendEquationAlpha;
	}
	int JSRenderState::getDepthTest()
	{
		return *m_renderState.depthTest;
	}
	void JSRenderState::setDepthTest(int value)
	{
		this->depthTest = value;
		m_renderState.depthTest = &this->depthTest;
	}
	bool JSRenderState::getDepthWrite()
	{
		return m_renderState.depthWrite;
	}
	void JSRenderState::setDepthWrite(bool value)
	{
		this->depthWrite = value;
		m_renderState.depthWrite = &this->depthWrite;
	}
	bool JSRenderState::getStencilWrite()
	{
		return m_renderState.stencilWrite;
	}
	void JSRenderState::setStencilWrite(bool value)
	{
		this->stencilWrite = value;
		m_renderState.stencilWrite = &this->stencilWrite;
	}
	int JSRenderState::getStencilTest()
	{
		return *m_renderState.stencilTest;
	}
	void JSRenderState::setStencilTest(int value)
	{
		this->stencilTest = value;
		m_renderState.stencilTest = &this->stencilTest;
	}
	int JSRenderState::getStencilRef()
	{
		return *m_renderState.stencilRef;
	}
	void JSRenderState::setStencilRef(int value)
	{
		this->stencilRef = value;
		m_renderState.stencilRef = &this->stencilRef;
	}
	/*void JSRenderState::setBlendConstColor(float r, float g, float b, float a)
	{
		m_renderState.blendConstColor.x = r;
		m_renderState.blendConstColor.y = g;
		m_renderState.blendConstColor.z = b;
		m_renderState.blendConstColor.w = a;
	}*/
	void JSRenderState::setStencilOp(float x, float y, float z)
	{
		this->stencilOpX = x;
		this->stencilOpY = y;
		this->stencilOpZ = z;

		m_renderState.stencilOpX = &this->stencilOpX;
		m_renderState.stencilOpY = &this->stencilOpY;
		m_renderState.stencilOpZ = &this->stencilOpZ;
	}
    void JSRenderState::setNull()
    {
		m_renderState.setNull();
    }
	//------------------------------------------------------------------------------
	void JSRenderState::exportJS(Context& context)
	{
		class_<JSRenderState> class_binding;
	    class_binding.constructor<>();
		class_binding.property("cull", &JSRenderState::getCull, &JSRenderState::setCull);
		class_binding.property("blend", &JSRenderState::getBlend, &JSRenderState::setBlend);
		class_binding.property("srcBlend", &JSRenderState::getSrcBlend, &JSRenderState::setSrcBlend);
		class_binding.property("dstBlend", &JSRenderState::getDstBlend, &JSRenderState::setDstBlend);
		class_binding.property("srcBlendRGB", &JSRenderState::getSrcBlendRGB, &JSRenderState::setSrcBlendRGB);
		class_binding.property("dstBlendRGB", &JSRenderState::getDstBlendRGB, &JSRenderState::setDstBlendRGB);
		class_binding.property("srcBlendAlpha", &JSRenderState::getSrcBlendAlpha, &JSRenderState::setSrcBlendAlpha);
		class_binding.property("dstBlendAlpha", &JSRenderState::getDstBlendAlpha, &JSRenderState::setDstBlendAlpha);
		//JSP_ADD_METHOD("blendConstColor", JSRenderState::setBlendConstColor);
		class_binding.property("blendEquation", &JSRenderState::getBlendEquation, &JSRenderState::setBlendEquation);
		class_binding.property("blendEquationRGB", &JSRenderState::getBlendEquationRGB, &JSRenderState::setBlendEquationRGB);
		class_binding.property("blendEquationAlpha", &JSRenderState::getBlendEquationAlpha, &JSRenderState::setBlendEquationAlpha);
		class_binding.property("depthTest", &JSRenderState::getDepthTest, &JSRenderState::setDepthTest);
		class_binding.property("depthWrite", &JSRenderState::getDepthWrite, &JSRenderState::setDepthWrite);
		class_binding.property("stencilWrite", &JSRenderState::getStencilWrite, &JSRenderState::setStencilWrite);
		class_binding.property("stencilTest", &JSRenderState::getStencilTest, &JSRenderState::setStencilTest);
		class_binding.property("stencilRef", &JSRenderState::getStencilRef, &JSRenderState::setStencilRef);
		class_binding.function("stencilOp", &JSRenderState::setStencilOp);
        class_binding.function("setNull", &JSRenderState::setNull);
		context.class_("conchRenderState", class_binding);
	}
}
//------------------------------------------------------------------------------

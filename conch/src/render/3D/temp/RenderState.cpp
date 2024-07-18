#include "RenderState.h"
#include <cassert>

namespace laya
{
	RenderState RenderState::Default;
	int32_t			RenderState::s_cull = RenderState::CULL_BACK;;
	int32_t			RenderState::s_blend = RenderState::BLEND_DISABLE;
	int32_t			RenderState::s_srcBlend = RenderState::BLENDPARAM_ONE;
	int32_t			RenderState::s_dstBlend = RenderState::BLENDPARAM_ZERO;
	int32_t			RenderState::s_srcBlendRGB = RenderState::BLENDPARAM_ONE;
	int32_t		    RenderState::s_dstBlendRGB = RenderState::BLENDPARAM_ZERO;
	int32_t			RenderState::s_srcBlendAlpha = RenderState::BLENDPARAM_ONE;
	int32_t			RenderState::s_dstBlendAlpha = RenderState::BLENDPARAM_ZERO;
	//Vector4			RenderState::s_blendConstColor;
	int32_t			RenderState::s_blendEquation = RenderState::BLENDEQUATION_ADD;
	int32_t			RenderState::s_blendEquationRGB = RenderState::BLENDEQUATION_ADD;
	int32_t			RenderState::s_blendEquationAlpha = RenderState::BLENDEQUATION_ADD;
	int32_t			RenderState::s_depthTest = RenderState::DEPTHTEST_LEQUAL;
	bool				RenderState::s_depthWrite = true;
	bool				RenderState::s_stencilWrite = false;
	int32_t			RenderState::s_stencilTest = RenderState::STENCILTEST_OFF;
	int32_t			RenderState::s_stencilRef = 1;
	Vector3			RenderState::s_stencilOp = Vector3(RenderState::STENCILOP_KEEP, RenderState::STENCILOP_KEEP, RenderState::STENCILOP_REPLACE);
	//double			RenderState::s_stencilOpX = RenderState::STENCILOP_KEEP;
	//double			RenderState::s_stencilOpY = RenderState::STENCILOP_KEEP;
	//double			RenderState::s_stencilOpZ = RenderState::STENCILOP_REPLACE;
	RenderState::RenderState()
	{
		this->cull = &s_cull;
		this->blend = &s_blend;
		this->srcBlend = &s_srcBlend;
		this->dstBlend = &s_dstBlend;
		this->srcBlendRGB = &s_srcBlendRGB;
		this->dstBlendRGB = &s_dstBlendRGB;
		this->srcBlendAlpha = &s_srcBlendAlpha;
		this->dstBlendAlpha = &s_dstBlendAlpha;
		//this->blendConstColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		this->blendEquation = &s_blendEquation;
		this->blendEquationRGB = &s_blendEquationRGB;
		this->blendEquationAlpha = &s_blendEquationAlpha;
		this->depthTest = &s_depthTest;
		this->depthWrite = &s_depthWrite;
		this->stencilRef = &s_stencilRef;
		this->stencilTest = &s_stencilTest;
		this->stencilWrite = &s_stencilWrite;
		this->stencilOp = &s_stencilOp;
		//this->stencilOpX = &s_stencilOpX;
		//this->stencilOpY = &s_stencilOpY;
		//this->stencilOpZ = &s_stencilOpZ;
	}
	RenderState::~RenderState()
	{
		
	}
	void RenderState::setNull()
	{
		this->cull = nullptr;
		this->blend = nullptr;
		this->srcBlend = nullptr;
		this->dstBlend = nullptr;
		this->srcBlendRGB = nullptr;
		this->dstBlendRGB = nullptr;
		this->srcBlendAlpha = nullptr;
		this->dstBlendAlpha = nullptr;
		this->blendEquation = nullptr;
		this->blendEquationRGB = nullptr;
		this->blendEquationAlpha = nullptr;
		this->depthTest = nullptr;
		this->depthWrite = nullptr;
		this->stencilRef = nullptr;
		this->stencilTest = nullptr;
		this->stencilWrite = nullptr;
		this->stencilOp = nullptr;
		//this->stencilOpX = this->stencilOpY = this->stencilOpZ = nullptr;
	}
	int32_t RenderState::getCull()
	{
		return *this->cull;
	}
	void RenderState::setCull(int32_t value)
	{
		this->m_cull = value;
		this->cull = &this->m_cull;
	}
	int32_t RenderState::getBlend()
	{
		return *this->blend;
	}
	void RenderState::setBlend(int32_t value)
	{
		this->m_blend = value;
		this->blend = &this->m_blend;
	}
	int32_t RenderState::getSrcBlend()
	{
		return *this->srcBlend;
	}
	void RenderState::setSrcBlend(int32_t value)
	{
		this->m_srcBlend = value;
		this->srcBlend = &this->m_srcBlend;
	}
	int32_t RenderState::getDstBlend()
	{
		return *this->dstBlend;
	}
	void RenderState::setDstBlend(int32_t value)
	{
		this->m_dstBlend = value;
		this->dstBlend = &this->m_dstBlend;
	}
	int32_t RenderState::getSrcBlendRGB()
	{
		return *this->srcBlendRGB;
	}
	void RenderState::setSrcBlendRGB(int32_t value)
	{
		this->m_srcBlendRGB = value;
		this->srcBlendRGB = &this->m_srcBlendRGB;
	}
	int32_t RenderState::getDstBlendRGB()
	{
		return *this->dstBlendRGB;
	}
	void RenderState::setDstBlendRGB(int32_t value)
	{
		this->m_dstBlendRGB = value;
		this->dstBlendRGB = &this->m_dstBlendRGB;
	}
	int32_t RenderState::getSrcBlendAlpha()
	{
		return *this->srcBlendAlpha;
	}
	void RenderState::setSrcBlendAlpha(int32_t value)
	{
		this->m_srcBlendAlpha = value;
		this->srcBlendAlpha = &this->m_srcBlendAlpha;
	}
	int32_t RenderState::getDstBlendAlpha()
	{
		return *this->dstBlendAlpha;
	}
	void RenderState::setDstBlendAlpha(int32_t value)
	{
		this->m_dstBlendAlpha = value;
		this->dstBlendAlpha = &this->m_dstBlendAlpha;
	}
	int32_t RenderState::getBlendEquation()
	{
		return *this->blendEquation;
	}
	void RenderState::setBlendEquation(int32_t value)
	{
		this->m_blendEquation = value;
		this->blendEquation = &this->m_blendEquation;
	}
	int32_t RenderState::getBlendEquationRGB()
	{
		return *this->blendEquationRGB;
	}
	void RenderState::setBlendEquationRGB(int32_t value)
	{
		this->m_blendEquationRGB = value;
		this->blendEquationRGB = &this->m_blendEquationRGB;
	}
	int32_t RenderState::getBlendEquationAlpha()
	{
		return *this->blendEquationAlpha;
	}
	void RenderState::setBlendEquationAlpha(int32_t value)
	{
		this->m_blendEquationAlpha = value;
		this->blendEquationAlpha = &this->m_blendEquationAlpha;
	}
	int32_t RenderState::getDepthTest()
	{
		return *this->depthTest;
	}
	void RenderState::setDepthTest(int32_t value)
	{
		this->m_depthTest = value;
		this->depthTest = &this->m_depthTest;
	}
	bool RenderState::getDepthWrite()
	{
		return this->depthWrite;
	}
	void RenderState::setDepthWrite(bool value)
	{
		this->m_depthWrite = value;
		this->depthWrite = &this->m_depthWrite;
	}
	bool RenderState::getStencilWrite()
	{
		return this->stencilWrite;
	}
	void RenderState::setStencilWrite(bool value)
	{
		this->m_stencilWrite = value;
		this->stencilWrite = &this->m_stencilWrite;
	}
	int32_t RenderState::getStencilTest()
	{
		return *this->stencilTest;
	}
	void RenderState::setStencilTest(int32_t value)
	{
		this->m_stencilTest = value;
		this->stencilTest = &this->m_stencilTest;
	}
	int32_t RenderState::getStencilRef()
	{
		return *this->stencilRef;
	}
	void RenderState::setStencilRef(int32_t value)
	{
		this->m_stencilRef = value;
		this->stencilRef = &this->m_stencilRef;
	}
	/*void RenderState::setBlendConstColor(float r, float g, float b, float a)
	{
		m_renderState.blendConstColor.x = r;
		m_renderState.blendConstColor.y = g;
		m_renderState.blendConstColor.z = b;
		m_renderState.blendConstColor.w = a;
	}*/
	void RenderState::setStencilOp(const Vector3& value)
	{
		this->m_stencilOp = value;

		this->stencilOp = &this->m_stencilOp;
	}

	Vector3* RenderState::getStencilOp() {
		return stencilOp;
	}
}
//------------------------------------------------------------------------------

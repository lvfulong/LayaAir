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
		this->blendEquation = &s_blendEquation;
		this->blendEquationRGB = &s_blendEquationRGB;
		this->blendEquationAlpha = &s_blendEquationAlpha;
		this->depthTest = &s_depthTest;
		this->depthWrite = &s_depthWrite;
		this->stencilRef = &s_stencilRef;
		this->stencilTest = &s_stencilTest;
		this->stencilOp = &s_stencilOp;
		setStencilWrite(false);
		setStencilWriteMask(0xFF);
		setStencilReadMask(0xFF);
		setDepthBias(false);
		setDepthBiasConstant(0.0);
		setDepthBiasSlopeScale(0.0);
		setDepthBiasClamp(0.0);
	}
	RenderState::~RenderState()
	{
		
	}
	void RenderState::setNull()
	{
		cull = nullptr;
		blend = nullptr;
		srcBlend = nullptr;
		dstBlend = nullptr;
		srcBlendRGB = nullptr;
		dstBlendRGB = nullptr;
		srcBlendAlpha = nullptr;
		dstBlendAlpha = nullptr;
		blendEquation = nullptr;
		blendEquationRGB = nullptr;
		blendEquationAlpha = nullptr;
		depthTest = nullptr;
		depthWrite = nullptr;
		stencilRef = nullptr;
		stencilTest = nullptr;
		stencilWrite = nullptr;
		stencilOp = nullptr;
		stencilWriteMask = nullptr;
		stencilReadMask = nullptr;
		depthBias = nullptr;
		depthBiasConstant = nullptr;
		depthBiasSlopeScale = nullptr;
		depthBiasClamp = nullptr;
	}
	int32_t RenderState::getCull()
	{
		return m_cull;
	}
	void RenderState::setCull(int32_t value)
	{
		m_cull = value;
		cull = &m_cull;
	}
	int32_t RenderState::getBlend()
	{
		return m_blend;
	}
	void RenderState::setBlend(int32_t value)
	{
		m_blend = value;
		blend = &m_blend;
	}
	int32_t RenderState::getSrcBlend()
	{
		return m_srcBlend;
	}
	void RenderState::setSrcBlend(int32_t value)
	{
		m_srcBlend = value;
		srcBlend = &m_srcBlend;
	}
	int32_t RenderState::getDstBlend()
	{
		return m_dstBlend;
	}
	void RenderState::setDstBlend(int32_t value)
	{
		m_dstBlend = value;
		dstBlend = &m_dstBlend;
	}
	int32_t RenderState::getSrcBlendRGB()
	{
		return m_srcBlendRGB;
	}
	void RenderState::setSrcBlendRGB(int32_t value)
	{
		m_srcBlendRGB = value;
		srcBlendRGB = &m_srcBlendRGB;
	}
	int32_t RenderState::getDstBlendRGB()
	{
		return m_dstBlendRGB;
	}
	void RenderState::setDstBlendRGB(int32_t value)
	{
		m_dstBlendRGB = value;
		dstBlendRGB = &m_dstBlendRGB;
	}
	int32_t RenderState::getSrcBlendAlpha()
	{
		return m_srcBlendAlpha;
	}
	void RenderState::setSrcBlendAlpha(int32_t value)
	{
		m_srcBlendAlpha = value;
		srcBlendAlpha = &m_srcBlendAlpha;
	}
	int32_t RenderState::getDstBlendAlpha()
	{
		return m_dstBlendAlpha;
	}
	void RenderState::setDstBlendAlpha(int32_t value)
	{
		m_dstBlendAlpha = value;
		dstBlendAlpha = &m_dstBlendAlpha;
	}
	int32_t RenderState::getBlendEquation()
	{
		return m_blendEquation;
	}
	void RenderState::setBlendEquation(int32_t value)
	{
		m_blendEquation = value;
		blendEquation = &m_blendEquation;
	}
	int32_t RenderState::getBlendEquationRGB()
	{
		return m_blendEquationRGB;
	}
	void RenderState::setBlendEquationRGB(int32_t value)
	{
		m_blendEquationRGB = value;
		blendEquationRGB = &m_blendEquationRGB;
	}
	int32_t RenderState::getBlendEquationAlpha()
	{
		return m_blendEquationAlpha;
	}
	void RenderState::setBlendEquationAlpha(int32_t value)
	{
		m_blendEquationAlpha = value;
		blendEquationAlpha = &m_blendEquationAlpha;
	}
	int32_t RenderState::getDepthTest()
	{
		return m_depthTest;
	}
	void RenderState::setDepthTest(int32_t value)
	{
		m_depthTest = value;
		depthTest = &m_depthTest;
	}
	bool RenderState::getDepthWrite()
	{
		return m_depthWrite;
	}
	void RenderState::setDepthWrite(bool value)
	{
		m_depthWrite = value;
		depthWrite = &m_depthWrite;
	}
	bool RenderState::getStencilWrite()
	{
		return stencilWrite;
	}
	void RenderState::setStencilWrite(bool value)
	{
		m_stencilWrite = value;
		stencilWrite = &m_stencilWrite;
	}
	int32_t RenderState::getStencilTest()
	{
		return m_stencilTest;
	}
	void RenderState::setStencilTest(int32_t value)
	{
		m_stencilTest = value;
		stencilTest = &m_stencilTest;
	}

	int32_t RenderState::getStencilRef()
	{
		return m_stencilRef;
	}

	void RenderState::setStencilRef(int32_t value)
	{
		m_stencilRef = value;
		stencilRef = &m_stencilRef;
	}

	void RenderState::setStencilOp(const Vector3& value)
	{
		m_stencilOp = value;

		stencilOp = &m_stencilOp;
	}

	Vector3* RenderState::getStencilOp() {
		return stencilOp;
	}

	int32_t RenderState::getStencilWriteMask() {
		return m_stencilWriteMask;
	}

	void RenderState::setStencilWriteMask(int32_t value) {
		m_stencilWriteMask = value;
		stencilWriteMask = &m_stencilWriteMask;
	}

	int32_t RenderState::getStencilReadMask() {
		return m_stencilReadMask;
	}

	void RenderState::setStencilReadMask(int32_t value) {
		m_stencilReadMask = value;
		stencilReadMask = &m_stencilReadMask;
	}

	/** 是否开启深度偏移 */
	bool RenderState::getDepthBias() {
		return m_depthBias;
	}

	void RenderState::setDepthBias(bool value) {
		m_depthBias = value;
		depthBias = &m_depthBias;
	}

	/** 深度偏移常量 */
	float RenderState::getDepthBiasConstant() {
		return m_depthBiasConstant;
	}

	void RenderState::setDepthBiasConstant(float value) {
		m_depthBiasConstant = value;
		depthBiasConstant = &m_depthBiasConstant;
	}

	/** 深度偏移斜率缩放 */
	float RenderState::getDepthBiasSlopeScale() {
		return m_depthBiasSlopeScale;
	}

	void RenderState::setDepthBiasSlopeScale(float value) {
		m_depthBiasSlopeScale = value;
		depthBiasSlopeScale = &m_depthBiasSlopeScale;
	}


	/** 深度偏移限制值 */
	float RenderState::getDepthBiasClamp() {
		return m_depthBiasClamp;
	}

	void RenderState::setDepthBiasClamp(float value) {
		m_depthBiasClamp = value;
		depthBiasClamp = &m_depthBiasClamp;
	}
}
//------------------------------------------------------------------------------

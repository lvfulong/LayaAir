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
}
//------------------------------------------------------------------------------

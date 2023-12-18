#ifndef __RenderStateType_H__
#define __RenderStateType_H__

namespace laya
{
	enum class RenderStateType
	{
		DepthTest,
		DepthMask,
		DepthFunc,
		StencilTest,
		StencilMask,
		StencilFunc,
		StencilOp,
		BlendType,
		BlendEquation,
		BlendEquationSeparate,
		BlendFunc,
		BlendFuncSeperate,
		CullFace,
		FrontFace,
	};
}
#endif
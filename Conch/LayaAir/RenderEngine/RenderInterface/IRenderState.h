#ifndef __IRenderState_H__
#define __IRenderState_H__

namespace laya
{
	class IRenderState
	{
	public:
		virtual ~IRenderState() {}
		virtual void setDepthTest(bool value) = 0;
		virtual void setDepthMask(bool value) = 0;
		virtual void setDepthFunc(CompareFunction compareFunction) = 0;
		virtual void setStencilTest(bool value) = 0;
		virtual void setStencilMask(bool value) = 0;
		virtual void setStencilFunc(CompareFunction compareFunction, int ref) = 0;
		virtual void setstencilOp(StencilOperation failFunction, StencilOperation zfailFunction, StencilOperation zpassFunction) = 0;
		virtual void setBlend(bool value) = 0;
		virtual void setBlendEquation(BlendEquationSeparate equation) = 0;
		virtual void setBlendEquationSeparate(BlendEquationSeparate blendEquationRGB, BlendEquationSeparate blendEquationAlpha) = 0;
		virtual void setBlendFunc(BlendFactor source, BlendFactor dest, bool force = false) = 0;
		virtual void setBlendFuncSeperate(BlendFactor sourceRGBFactor, BlendFactor destRGBFactor, BlendFactor srcAlphaFactor, BlendFactor dstAlphaFactor) = 0;
		virtual void setCullFace(bool value) = 0;
		virtual void setFrontFace(CullMode cullmode) = 0;
	};
}
#endif
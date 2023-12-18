#ifndef __RenderStateContext_H__
#define __RenderStateContext_H__

#include "RenderEnum/BlendEquationSeparate.h"
#include "RenderEnum/BlendFactor.h"
#include "RenderEnum/BlendType.h"
#include "RenderEnum/CompareFunction.h"
#include "RenderEnum/CullMode.h"
#include "RenderEnum/RenderStateType.h"
#include "RenderEnum/StencilOperation.h"
#include "RenderEngine/WebGLEngine/WebGLEngine.h"
#include "RenderInterface/IRenderState.h"

namespace laya
{
	class RenderStateContext
	{
	public:

		static void setDepthTest(WebGLEngine* pWebGLEngine, bool value)
		{
			pWebGLEngine->getRenderState()->setDepthTest(value);
		}

		static void setDepthMask(WebGLEngine* pWebGLEngine, bool value)
		{
			pWebGLEngine->getRenderState()->setDepthMask(value);
		}

		static void setDepthFunc(WebGLEngine* pWebGLEngine, CompareFunction value)
		{
			pWebGLEngine->getRenderState()->setDepthFunc(value);
		}

		static void setStencilTest(WebGLEngine* pWebGLEngine, bool value)
		{
			pWebGLEngine->getRenderState()->setStencilTest(value);
		}


		static void setStencilMask(WebGLEngine* pWebGLEngine, bool value)
		{
			pWebGLEngine->getRenderState()->setStencilMask(value);
		}


		static void setStencilFunc(WebGLEngine* pWebGLEngine, CompareFunction fun, int ref)
		{
			pWebGLEngine->getRenderState()->setStencilFunc(fun, ref);
		}

		static void setstencilOp(WebGLEngine* pWebGLEngine, StencilOperation fail, StencilOperation zfail, StencilOperation zpass)
		{
			pWebGLEngine->getRenderState()->setstencilOp(fail, zfail, zpass);
		}

		static void setBlend(WebGLEngine* pWebGLEngine, bool value)
		{
			pWebGLEngine->getRenderState()->setBlend(value);
		}

		static void setBlendEquation(WebGLEngine* pWebGLEngine, BlendEquationSeparate blendEquation)
		{
			pWebGLEngine->getRenderState()->setBlendEquation(blendEquation);
		}

		static void setBlendEquationSeparate(WebGLEngine* pWebGLEngine, BlendEquationSeparate blendEquationRGB, BlendEquationSeparate blendEquationAlpha)
		{
			pWebGLEngine->getRenderState()->setBlendEquationSeparate(blendEquationRGB, blendEquationAlpha);
		}

		static void setBlendFunc(WebGLEngine* pWebGLEngine, BlendFactor sFactor, BlendFactor dFactor, bool force = false)
		{
			pWebGLEngine->getRenderState()->setBlendFunc(sFactor, dFactor, force);
		}

		static void setBlendFuncSeperate(WebGLEngine* pWebGLEngine, BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha)
		{
			pWebGLEngine->getRenderState()->setBlendFuncSeperate(srcRGB, dstRGB, srcAlpha, dstAlpha);
		}

		static void setCullFace(WebGLEngine* pWebGLEngine, bool value)
		{
			pWebGLEngine->getRenderState()->setCullFace(value);
		}

		static void setFrontFace(WebGLEngine* pWebGLEngine, CullMode value)
		{
			pWebGLEngine->getRenderState()->setFrontFace(value);
		}
	};
}
#endif



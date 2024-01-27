#ifndef __RenderStateContext_H__
#define __RenderStateContext_H__

#include <render/3D/design/renderEnum/BlendEquationSeparate.h>
#include <render/3D/design/renderEnum/BlendFactor.h>
#include <render/3D/design/renderEnum/BlendType.h>
#include <render/3D/design/renderEnum/CompareFunction.h>
#include <render/3D/design/renderEnum/CullMode.h>
#include <render/3D/design/renderEnum/RenderStateType.h>
#include <render/3D/design/renderEnum/StencilOperation.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include "render/driver/gles/LayaGL.h"

namespace laya
{
	class RenderStateContext
	{
	public:

		static void setDepthTest(bool value)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setDepthTest(value);
		}

		static void setDepthMask(bool value)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setDepthMask(value);
		}

		static void setDepthFunc(CompareFunction value)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setDepthFunc(value);
		}

		static void setStencilTest(bool value)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setStencilTest(value);
		}


		static void setStencilMask(bool value)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setStencilMask(value);
		}


		static void setStencilFunc(CompareFunction fun, int ref)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setStencilFunc(fun, ref);
		}

		static void setstencilOp(StencilOperation fail, StencilOperation zfail, StencilOperation zpass)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setstencilOp(fail, zfail, zpass);
		}

		static void setBlend(bool value)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setBlend(value);
		}

		static void setBlendEquation(BlendEquationSeparate blendEquation)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setBlendEquation(blendEquation);
		}

		static void setBlendEquationSeparate(BlendEquationSeparate blendEquationRGB, BlendEquationSeparate blendEquationAlpha)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setBlendEquationSeparate(blendEquationRGB, blendEquationAlpha);
		}

		static void setBlendFunc(BlendFactor sFactor, BlendFactor dFactor, bool force = false)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setBlendFunc(sFactor, dFactor, force);
		}

		static void setBlendFuncSeperate(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setBlendFuncSeperate(srcRGB, dstRGB, srcAlpha, dstAlpha);
		}

		static void setCullFace(bool value)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setCullFace(value);
		}

		static void setFrontFace(CullMode value)
		{
			LayaGL::m_pWebglEngine->getRenderState()->setFrontFace(value);
		}
	};
}
#endif



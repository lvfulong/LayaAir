#ifndef __GLRenderState_H__
#define __GLRenderState_H__

#include <render/3D/design/renderEnum/BlendEquationSeparate.h>
#include <render/3D/design/renderEnum/BlendFactor.h>
#include <render/3D/design/renderEnum/BlendType.h>
#include <render/3D/design/renderEnum/CompareFunction.h>
#include <render/3D/design/renderEnum/CullMode.h>
#include <render/3D/design/renderEnum/RenderStateType.h>
#include <render/3D/design/renderEnum/StencilOperation.h>
#include <render/3D/design/renderInterface/IRenderState.h>
#include <render/3D/temp/RenderStateCommand.h>

#include <cassert>
#ifdef __APPLE__
#include <OpenGLES/ES3/GL.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
#include "JCSystemConfig.h"

namespace laya
{
	class WebGLEngine;
	class GLRenderState: public IRenderState
	{
	private:
		bool m_depthTest = true;
		bool m_depthMask = true;
		GLenum m_depthFunc;
		bool m_stencilTest = false;
		GLenum m_stencilFunc;
		bool m_stencilMask = false;
		GLint m_stencilRef;
		GLenum m_stencilOp_fail;
		GLenum m_stencilOp_zfail;
		GLenum m_stencilOp_zpass;
		bool m_blend = false;
		GLenum m_blendEquation;
		GLenum m_blendEquationRGB;
		GLenum m_blendEquationAlpha;
		GLenum m_sFactor;
		GLenum m_dFactor;
		GLenum m_sFactorRGB;
		GLenum m_dFactorRGB;
		GLenum m_sFactorAlpha;
		GLenum m_dFactorAlpha;
		bool m_cullFace = false;
		GLenum m_frontFace;

		WebGLEngine* m_engine;
	public:
		GLRenderState(WebGLEngine* engine)
		{
			m_engine = engine;
			_initState();
		}

		void _initState() 
		{
			//TODO:������ȫ
			setDepthFunc(CompareFunction::Less);
			setBlendEquationSeparate(BlendEquationSeparate::ADD, BlendEquationSeparate::ADD);
			m_blendEquation = GL_FUNC_ADD;
			m_sFactor = GL_ONE;
			m_dFactor = GL_ZERO;
			m_sFactorAlpha = GL_ONE;
			m_dFactorAlpha = GL_ONE;
		}
		GLenum _getBlendFactor(BlendFactor factor)
		{
			switch (factor) 
			{
			case BlendFactor::Zero:
				return GL_ZERO;
			case BlendFactor::One:
				return GL_ONE;
			case BlendFactor::SourceColor:
				return GL_SRC_COLOR;
			case BlendFactor::OneMinusSourceColor:
				return GL_ONE_MINUS_SRC_COLOR;
			case BlendFactor::DestinationColor:
				return GL_DST_COLOR;
			case BlendFactor::OneMinusDestinationColor:
				return GL_ONE_MINUS_DST_COLOR;
			case BlendFactor::SourceAlpha:
				return GL_SRC_ALPHA;
			case BlendFactor::OneMinusSourceAlpha:
				return GL_ONE_MINUS_SRC_ALPHA;
			case BlendFactor::DestinationAlpha:
				return GL_DST_ALPHA;
			case BlendFactor::OneMinusDestinationAlpha:
				return GL_ONE_MINUS_DST_ALPHA;
			case BlendFactor::SourceAlphaSaturate:
				return GL_SRC_ALPHA_SATURATE;
			case BlendFactor::BlendColor:
				return GL_CONSTANT_COLOR;
			case BlendFactor::OneMinusBlendColor:
				return GL_ONE_MINUS_CONSTANT_COLOR;
			};
		}
		GLenum _getBlendOperation(BlendEquationSeparate factor)
		{
			switch (factor)
			{
			case BlendEquationSeparate::ADD:
				return GL_FUNC_ADD;
			case BlendEquationSeparate::SUBTRACT:
				return GL_FUNC_SUBTRACT;
			case BlendEquationSeparate::REVERSE_SUBTRACT:
				return GL_FUNC_REVERSE_SUBTRACT;
			default:
				assert("Unknow type");
			};
		}

		//TODO �����Ż�
		GLenum _getGLCompareFunction(CompareFunction compareFunction)
		{
		switch (compareFunction) 
		{
		case CompareFunction::Never:
			return GL_NEVER;
		case CompareFunction::Less:
			return GL_LESS;
		case CompareFunction::Equal:
			return GL_EQUAL;
		case CompareFunction::LessEqual:
			return GL_LEQUAL;
		case CompareFunction::Greater:
			return GL_GREATER;
		case CompareFunction::NotEqual:
			return GL_NOTEQUAL;
		case CompareFunction::GreaterEqual:
			return GL_GEQUAL;
		case CompareFunction::Always:
			return GL_ALWAYS;
		}
		}

			//�����Ż�
		GLenum _getGLStencilOperation(StencilOperation compareFunction)
		{
		switch (compareFunction) {
		case StencilOperation::Keep:
			return GL_KEEP;
		case StencilOperation::Zero:
			return GL_ZERO;
		case StencilOperation::Replace:
			return GL_REPLACE;
		case StencilOperation::IncrementSaturate:
			return GL_INCR;
		case StencilOperation::DecrementSaturate:
			return GL_DECR;
		case StencilOperation::Invert:
			return GL_INVERT;
		case StencilOperation::IncrementWrap:
			return GL_INCR_WRAP;
		case StencilOperation::DecrementWrap:
			return GL_DECR_WRAP;
		}
		}

		GLenum _getGLFrontfaceFactor(CullMode cullmode) 
		{
			if (cullmode == CullMode::Front)
				return GL_CCW;
			else
				return GL_CW;
		}

		void setDepthTest(bool value) override
		{
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			}
			else
			{
				if (value != m_depthTest)
				{
					m_depthTest = value;
					value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
				}
			}
		}

		void setDepthMask(bool value) override
		{
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				glDepthMask(value);
			}
			else
			{
				if (value != m_depthMask)
				{
					m_depthMask = value;
					glDepthMask(value);
				}
			}
		}
		void setDepthFunc(CompareFunction compareFunction) override
		{
			GLenum value = _getGLCompareFunction(compareFunction);
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				glDepthFunc(value);
			}
			else
			{
				if (value != m_depthFunc)
				{
					m_depthFunc = value;
					glDepthFunc(value);
				}
			}
		}

		void setStencilTest(bool value) override
		{
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				value ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
			}
			else
			{
				if (value != m_stencilTest)
				{
					m_stencilTest = value;
					value ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
				}
			}
		}
		void setStencilMask(bool value) override;

		void setStencilFunc(CompareFunction compareFunction, int ref) override
		{
			GLenum fun = _getGLCompareFunction(compareFunction);
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				glStencilFunc(fun, ref, 0xff);
			}
			else
			{
				if (fun != m_stencilFunc || ref != m_stencilRef) 
				{
					m_stencilFunc = fun;
					m_stencilRef = ref;
					glStencilFunc(fun, ref, 0xff);
				}
			}
		} 
		void setstencilOp(StencilOperation failFunction, StencilOperation zfailFunction, StencilOperation zpassFunction) override
		{
			GLenum fail = _getGLStencilOperation(failFunction);
			GLenum zfail = _getGLStencilOperation(zfailFunction);
			GLenum zpass = _getGLStencilOperation(zpassFunction);
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				glStencilOp(fail, zfail, zpass);
			}
			else
			{
				if (m_stencilOp_fail != fail || m_stencilOp_zfail != zfail || m_stencilOp_zpass != zpass)
				{
					m_stencilOp_fail = fail;
					m_stencilOp_zfail = zfail;
					m_stencilOp_zpass = zpass;
					glStencilOp(fail, zfail, zpass);
				}
			}
		}
		void setBlend(bool value) override
		{
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				value ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
			}
			else
			{
				if (value != m_blend)
				{
					m_blend = value;
					value ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
				}
			}
		}
		void setBlendEquation(BlendEquationSeparate equation) override
		{
			GLenum blendEquation = _getBlendOperation(equation);
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				glBlendEquation(blendEquation);
			}
			else
			{
				if (blendEquation != m_blendEquation)
				{
					m_blendEquation = blendEquation;
					m_blendEquationRGB = m_blendEquationAlpha = GL_NONE;
					glBlendEquation(blendEquation);
				}
			}
		}
		void setBlendEquationSeparate(BlendEquationSeparate equationRGB, BlendEquationSeparate equationAlpha) override
		{
			GLenum blendEquationRGB = _getBlendOperation(equationRGB);
			GLenum blendEquationAlpha = _getBlendOperation(equationAlpha);
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				glBlendEquationSeparate(blendEquationRGB, blendEquationAlpha);
			}
			else
			{
				if (blendEquationRGB != m_blendEquationRGB || blendEquationAlpha != m_blendEquationAlpha)
				{
					m_blendEquationRGB = blendEquationRGB;
					m_blendEquationAlpha = blendEquationAlpha;
					m_blendEquation = GL_NONE;
					glBlendEquationSeparate(blendEquationRGB, blendEquationAlpha);
				}
			}
		}
		void setBlendFunc(BlendFactor source, BlendFactor dest, bool force = false) override
		{
			GLenum sFactor = _getBlendFactor(source);
			GLenum dFactor = _getBlendFactor(dest);
			// �и�iOS��bug����ԭ����д����ʱ������
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				glBlendFunc(sFactor, dFactor);
			}
			else
			{
				if (force || sFactor != m_sFactor || dFactor != m_dFactor) 
				{
					m_sFactor = sFactor;
					m_dFactor = dFactor;
					m_sFactorRGB = GL_NONE;
					m_dFactorRGB = GL_NONE;
					m_sFactorAlpha = GL_NONE;
					m_dFactorAlpha = GL_NONE;
					glBlendFunc(sFactor, dFactor);
				}
			}
		}
		void setBlendFuncSeperate(BlendFactor sourceRGBFactor, BlendFactor destRGBFactor, BlendFactor srcAlphaFactor, BlendFactor dstAlphaFactor) override
		{
			GLenum srcRGB = _getBlendFactor(sourceRGBFactor);
			GLenum dstRGB = _getBlendFactor(destRGBFactor);
			GLenum srcAlpha = _getBlendFactor(srcAlphaFactor);
			GLenum dstAlpha = _getBlendFactor(dstAlphaFactor);
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
			}
			else
			{
				if (srcRGB != m_sFactorRGB || dstRGB != m_dFactorRGB || srcAlpha != m_sFactorAlpha || dstAlpha != m_dFactorAlpha) 
				{
					m_sFactorRGB = srcRGB;
					m_dFactorRGB = dstRGB;
					m_sFactorAlpha = srcAlpha;
					m_dFactorAlpha = dstAlpha;
					m_sFactor = GL_NONE;
					m_dFactor = GL_NONE;
					glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
				}
			}
		}
		void setCullFace(bool value) override
		{
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				value ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
			}
			else
			{
				if (value != m_cullFace)
				{
					m_cullFace = value;
					value ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
				}
			}
		}
		void setFrontFace(CullMode cullmode) override
		{
			GLenum value = _getGLFrontfaceFactor(cullmode);
			if (laya::g_kSystemConfig.m_bConchWebGL)
			{
				glFrontFace(value);
			}
			else
			{
				if (value != m_frontFace)
				{
					m_frontFace = value;
					glFrontFace(value);
				}
			}
		}

		void applyRenderStateCommand(const RenderStateCommand& cmd);
	};
}

#endif

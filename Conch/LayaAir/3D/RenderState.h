#ifndef __RenderState_H__
#define __RenderState_H__

#include <stdio.h>
#ifdef __APPLE__
	#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
	#include <GLES3/gl3.h>
#endif
#include <unordered_map>
#include "../RenderEngine/RenderEnum/BlendType.h"
#include "../RenderEngine/RenderEnum/CullMode.h"
#include "../RenderEngine/RenderEnum/BlendEquationSeparate.h"
#include "../RenderEngine/RenderEnum/BlendFactor.h"
#include "../RenderEngine/RenderEnum/CompareFunction.h"
#include "../RenderEngine/RenderEnum/StencilOperation.h"
#include "./Math/Vector4.h"
#include "./Math/Vector3.h"

namespace laya
{

	class RenderState
	{
	public:
		enum
		{
			CULL_NONE = (int)CullMode::Off,
			CULL_FRONT = (int)CullMode::Front,
			CULL_BACK = (int)CullMode::Back,


			BLEND_DISABLE = (int)BlendType::BLEND_DISABLE,
			BLEND_ENABLE_ALL = (int)BlendType::BLEND_ENABLE_ALL,
			BLEND_ENABLE_SEPERATE = (int)BlendType::BLEND_ENABLE_SEPERATE,

			BLENDPARAM_ZERO = (int)BlendFactor::Zero,
			BLENDPARAM_ONE = (int)BlendFactor::One,
			BLENDPARAM_SRC_COLOR = (int)BlendFactor::SourceColor,
			BLENDPARAM_ONE_MINUS_SRC_COLOR = (int)BlendFactor::OneMinusSourceColor,
			BLENDPARAM_DST_COLOR = (int)BlendFactor::DestinationColor,
			BLENDPARAM_ONE_MINUS_DST_COLOR = (int)BlendFactor::OneMinusDestinationColor,
			BLENDPARAM_SRC_ALPHA = (int)BlendFactor::SourceAlpha,
			BLENDPARAM_ONE_MINUS_SRC_ALPHA = (int)BlendFactor::OneMinusSourceAlpha,
			BLENDPARAM_DST_ALPHA = (int)BlendFactor::DestinationAlpha,
			BLENDPARAM_ONE_MINUS_DST_ALPHA = (int)BlendFactor::OneMinusDestinationAlpha,
			BLENDPARAM_SRC_ALPHA_SATURATE = (int)BlendFactor::SourceAlphaSaturate,
			BLENDPARAM_BLENDCOLOR = (int)BlendFactor::BlendColor,
			BLENDPARAM_BLEND_ONEMINUS_COLOR = (int)BlendFactor::OneMinusBlendColor,

			BLENDEQUATION_ADD = (int)BlendEquationSeparate::ADD,
			BLENDEQUATION_SUBTRACT = (int)BlendEquationSeparate::SUBTRACT,
			BLENDEQUATION_REVERSE_SUBTRACT = (int)BlendEquationSeparate::REVERSE_SUBTRACT,
			BLENDEQUATION_MIN = (int)BlendEquationSeparate::MIN,
			BLENDEQUATION_MAX = (int)BlendEquationSeparate::MAX,

			DEPTHTEST_OFF = 0/*WebGLContext.NEVER*/,//TODO:ʲô��
			DEPTHTEST_NEVER = (int)CompareFunction::Never/*WebGLContext.NEVER*/,
			DEPTHTEST_LESS = (int)CompareFunction::Less/*WebGLContext.LESS*/,
			DEPTHTEST_EQUAL = (int)CompareFunction::Equal/*WebGLContext.EQUAL*/,
			DEPTHTEST_LEQUAL = (int)CompareFunction::LessEqual/*WebGLContext.LEQUAL*/,
			DEPTHTEST_GREATER = (int)CompareFunction::Greater/*WebGLContext.GREATER*/,
			DEPTHTEST_NOTEQUAL = (int)CompareFunction::NotEqual/*WebGLContext.NOTEQUAL*/,
			DEPTHTEST_GEQUAL = (int)CompareFunction::GreaterEqual/*WebGLContext.GEQUAL*/,
			DEPTHTEST_ALWAYS = (int)CompareFunction::Always/*WebGLContext.ALWAYS*/,


			STENCILTEST_OFF = 0,
			STENCILTEST_NEVER = (int)CompareFunction::Never/*WebGLContext.NEVER*/,
			STENCILTEST_LESS = (int)CompareFunction::Less/*WebGLContext.LESS*/,
			STENCILTEST_EQUAL = (int)CompareFunction::Equal/*WebGLContext.EQUAL*/,
			STENCILTEST_LEQUAL = (int)CompareFunction::LessEqual/*WebGLContext.LEQUAL*/,
			STENCILTEST_GREATER = (int)CompareFunction::Greater/*WebGLContext.GREATER*/,
			STENCILTEST_NOTEQUAL = (int)CompareFunction::NotEqual/*WebGLContext.NOTEQUAL*/,
			STENCILTEST_GEQUAL = (int)CompareFunction::GreaterEqual/*WebGLContext.GEQUAL*/,
			STENCILTEST_ALWAYS = (int)CompareFunction::Always/*WebGLContext.ALWAYS*/,
			STENCILOP_KEEP = (int)StencilOperation::Keep,
			STENCILOP_ZERO = (int)StencilOperation::Zero,
			STENCILOP_REPLACE = (int)StencilOperation::Replace,
			STENCILOP_INCR = (int)StencilOperation::IncrementSaturate,
			STENCILOP_INCR_WRAP = (int)StencilOperation::IncrementWrap,
			STENCILOP_DECR = (int)StencilOperation::DecrementSaturate,
			STENCILOP_DECR_WRAP = (int)StencilOperation::DecrementWrap,
			STENCILOP_INVERT = (int)StencilOperation::Invert,
		};
		static float		s_cull;
		static int			s_blend;
		static int			s_srcBlend;
		static int			s_dstBlend;
		static int			s_srcBlendRGB;
		static int		    s_dstBlendRGB;
		static int			s_srcBlendAlpha;
		static int			s_dstBlendAlpha;
		//Vector4			s_blendConstColor;
		static int			s_blendEquation;
		static int			s_blendEquationRGB;
		static int			s_blendEquationAlpha;
		static int			s_depthTest;
		static bool			s_depthWrite;
		static bool			s_stencilWrite;
		static int			s_stencilTest;
		static int			s_stencilRef;
		//Vector3			stencilOp;
		static int			s_stencilOpX;
		static int			s_stencilOpY;
		static int			s_stencilOpZ;
		RenderState();

		~RenderState();

		void setNull();
		static RenderState Default;
	public:
		float*			cull;
		int*			blend;
		int*			srcBlend;
		int*			dstBlend;
		int*			srcBlendRGB;
		int*		    dstBlendRGB;
		int*			srcBlendAlpha;
		int*			dstBlendAlpha;
		//Vector4		blendConstColor;
		int*			blendEquation;
		int*			blendEquationRGB;
		int*			blendEquationAlpha;
		int*			depthTest;
		bool*		depthWrite;
		bool*		stencilWrite;
		int*			stencilTest;
		int*			stencilRef;
		//Vector3		stencilOp;
		int*			stencilOpX;
		int*			stencilOpY;
		int*			stencilOpZ;
	};
}
#endif //__RenderState_H__

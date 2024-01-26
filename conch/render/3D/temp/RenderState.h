#ifndef __RenderState_H__
#define __RenderState_H__

#include <stdio.h>
#include <utils/Preprocessor.h>
#include <unordered_map>
#include <render/3D/design/renderEnum/BlendType.h>
#include <render/3D/design/renderEnum/CullMode.h>
#include <render/3D/design/renderEnum/BlendEquationSeparate.h>
#include <render/3D/design/renderEnum/BlendFactor.h>
#include <render/3D/design/renderEnum/CompareFunction.h>
#include <render/3D/design/renderEnum/StencilOperation.h>
#include <core/math/Vector4.h>
#include <core/math/Vector3.h>

namespace laya
{

	class RenderState
	{
	public:
		enum
		{
			CULL_NONE = (int32_t)CullMode::Off,
			CULL_FRONT = (int32_t)CullMode::Front,
			CULL_BACK = (int32_t)CullMode::Back,


			BLEND_DISABLE = (int32_t)BlendType::BLEND_DISABLE,
			BLEND_ENABLE_ALL = (int32_t)BlendType::BLEND_ENABLE_ALL,
			BLEND_ENABLE_SEPERATE = (int32_t)BlendType::BLEND_ENABLE_SEPERATE,

			BLENDPARAM_ZERO = (int32_t)BlendFactor::Zero,
			BLENDPARAM_ONE = (int32_t)BlendFactor::One,
			BLENDPARAM_SRC_COLOR = (int32_t)BlendFactor::SourceColor,
			BLENDPARAM_ONE_MINUS_SRC_COLOR = (int32_t)BlendFactor::OneMinusSourceColor,
			BLENDPARAM_DST_COLOR = (int32_t)BlendFactor::DestinationColor,
			BLENDPARAM_ONE_MINUS_DST_COLOR = (uint32_t)BlendFactor::OneMinusDestinationColor,
			BLENDPARAM_SRC_ALPHA = (int32_t)BlendFactor::SourceAlpha,
			BLENDPARAM_ONE_MINUS_SRC_ALPHA = (int32_t)BlendFactor::OneMinusSourceAlpha,
			BLENDPARAM_DST_ALPHA = (int32_t)BlendFactor::DestinationAlpha,
			BLENDPARAM_ONE_MINUS_DST_ALPHA = (int32_t)BlendFactor::OneMinusDestinationAlpha,
			BLENDPARAM_SRC_ALPHA_SATURATE = (int32_t)BlendFactor::SourceAlphaSaturate,
			BLENDPARAM_BLENDCOLOR = (int32_t)BlendFactor::BlendColor,
			BLENDPARAM_BLEND_ONEMINUS_COLOR = (int32_t)BlendFactor::OneMinusBlendColor,

			BLENDEQUATION_ADD = (int32_t)BlendEquationSeparate::ADD,
			BLENDEQUATION_SUBTRACT = (int32_t)BlendEquationSeparate::SUBTRACT,
			BLENDEQUATION_REVERSE_SUBTRACT = (int32_t)BlendEquationSeparate::REVERSE_SUBTRACT,
			BLENDEQUATION_MIN = (int32_t)BlendEquationSeparate::MIN,
			BLENDEQUATION_MAX = (int32_t)BlendEquationSeparate::MAX,

			DEPTHTEST_OFF = (int32_t)CompareFunction::Off/*WebGLContext.NEVER*/,//TODO:ʲô��
			DEPTHTEST_NEVER = (int32_t)CompareFunction::Never/*WebGLContext.NEVER*/,
			DEPTHTEST_LESS = (int32_t)CompareFunction::Less/*WebGLContext.LESS*/,
			DEPTHTEST_EQUAL = (int32_t)CompareFunction::Equal/*WebGLContext.EQUAL*/,
			DEPTHTEST_LEQUAL = (int32_t)CompareFunction::LessEqual/*WebGLContext.LEQUAL*/,
			DEPTHTEST_GREATER = (int32_t)CompareFunction::Greater/*WebGLContext.GREATER*/,
			DEPTHTEST_NOTEQUAL = (int32_t)CompareFunction::NotEqual/*WebGLContext.NOTEQUAL*/,
			DEPTHTEST_GEQUAL = (int32_t)CompareFunction::GreaterEqual/*WebGLContext.GEQUAL*/,
			DEPTHTEST_ALWAYS = (int32_t)CompareFunction::Always/*WebGLContext.ALWAYS*/,


			STENCILTEST_OFF = 0,
			STENCILTEST_NEVER = (int32_t)CompareFunction::Never/*WebGLContext.NEVER*/,
			STENCILTEST_LESS = (int32_t)CompareFunction::Less/*WebGLContext.LESS*/,
			STENCILTEST_EQUAL = (int32_t)CompareFunction::Equal/*WebGLContext.EQUAL*/,
			STENCILTEST_LEQUAL = (int32_t)CompareFunction::LessEqual/*WebGLContext.LEQUAL*/,
			STENCILTEST_GREATER = (int32_t)CompareFunction::Greater/*WebGLContext.GREATER*/,
			STENCILTEST_NOTEQUAL = (int32_t)CompareFunction::NotEqual/*WebGLContext.NOTEQUAL*/,
			STENCILTEST_GEQUAL = (int32_t)CompareFunction::GreaterEqual/*WebGLContext.GEQUAL*/,
			STENCILTEST_ALWAYS = (int32_t)CompareFunction::Always/*WebGLContext.ALWAYS*/,
			STENCILOP_KEEP = (int32_t)StencilOperation::Keep,
			STENCILOP_ZERO = (int32_t)StencilOperation::Zero,
			STENCILOP_REPLACE = (int32_t)StencilOperation::Replace,
			STENCILOP_INCR = (int32_t)StencilOperation::IncrementSaturate,
			STENCILOP_INCR_WRAP = (int32_t)StencilOperation::IncrementWrap,
			STENCILOP_DECR = (int32_t)StencilOperation::DecrementSaturate,
			STENCILOP_DECR_WRAP = (int32_t)StencilOperation::DecrementWrap,
			STENCILOP_INVERT = (int32_t)StencilOperation::Invert,
		};
		static int32_t			s_cull;
		static int32_t			s_blend;
		static int32_t			s_srcBlend;
		static int32_t			s_dstBlend;
		static int32_t			s_srcBlendRGB;
		static int32_t		    s_dstBlendRGB;
		static int32_t			s_srcBlendAlpha;
		static int32_t			s_dstBlendAlpha;
		//Vector4			s_blendConstColor;
		static int32_t			s_blendEquation;
		static int32_t			s_blendEquationRGB;
		static int32_t			s_blendEquationAlpha;
		static int32_t			s_depthTest;
		static bool				s_depthWrite;
		static bool				s_stencilWrite;
		static int32_t			s_stencilTest;
		static int32_t			s_stencilRef;
		static Vector3			s_stencilOp;
		//static double			s_stencilOpX;
		//static double			s_stencilOpY;
		//static double			s_stencilOpZ;
		RenderState();

		~RenderState();

		int32_t getCull();
		void setCull(int32_t value);
		int32_t getBlend();
		void setBlend(int32_t value);
		int32_t getSrcBlend();
		void setSrcBlend(int32_t value);
		int32_t getDstBlend();
		void setDstBlend(int32_t value);
		int32_t getSrcBlendRGB();
		void setSrcBlendRGB(int32_t value);
		int32_t getDstBlendRGB();
		void setDstBlendRGB(int32_t value);
		int32_t getSrcBlendAlpha();
		void setSrcBlendAlpha(int32_t value);
		int32_t getDstBlendAlpha();
		void setDstBlendAlpha(int32_t value);
		int32_t getBlendEquation();
		void setBlendEquation(int32_t value);
		int32_t getBlendEquationRGB();
		void setBlendEquationRGB(int32_t value);
		int32_t getBlendEquationAlpha();
		void setBlendEquationAlpha(int32_t value);
		int32_t getDepthTest();
		void setDepthTest(int32_t value);
		bool getDepthWrite();
		void setDepthWrite(bool value);
		bool getStencilWrite();
		void setStencilWrite(bool value);
		int32_t getStencilTest();
		void setStencilTest(int32_t value);
		int32_t getStencilRef();
		void setStencilRef(int32_t value);
		//void setBlendConstColor(float r, float g, float b, float a);
		void setStencilOp(const Vector3& value);
		void setNull();
		static RenderState Default;
	public:
		int32_t*			cull;
		int32_t*			blend;
		int32_t*			srcBlend;
		int32_t*			dstBlend;
		int32_t*			srcBlendRGB;
		int32_t*		    dstBlendRGB;
		int32_t*			srcBlendAlpha;
		int32_t*			dstBlendAlpha;
		//Vector4			blendConstColor;
		int32_t*			blendEquation;
		int32_t*			blendEquationRGB;
		int32_t*			blendEquationAlpha;
		int32_t*			depthTest;
		bool*				depthWrite;
		bool*				stencilWrite;
		int32_t*			stencilTest;
		int32_t*			stencilRef;
		Vector3*			stencilOp;
		//double*				stencilOpX;
		//double*				stencilOpY;
		//double*				stencilOpZ;//double? todo



		int32_t			m_cull;
		int32_t			m_blend;
		int32_t			m_srcBlend;
		int32_t			m_dstBlend;
		int32_t			m_srcBlendRGB;
		int32_t		    m_dstBlendRGB;
		int32_t			m_srcBlendAlpha;
		int32_t			m_dstBlendAlpha;
		//Vector4		blendConstColor;
		int32_t			m_blendEquation;
		int32_t			m_blendEquationRGB;
		int32_t			m_blendEquationAlpha;
		int32_t			m_depthTest;
		bool			m_depthWrite;
		bool			m_stencilWrite;
		int32_t			m_stencilTest;
		int32_t			m_stencilRef;
		Vector3			m_stencilOp;
		//int32_t			stencilOpX;
		//int32_t			stencilOpY;
		//int32_t			stencilOpZ;
	};
}
#endif //__RenderState_H__

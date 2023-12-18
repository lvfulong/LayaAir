#ifndef __JSRenderState_H__
#define __JSRenderState_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../../LayaAir/3D/RenderState.h"

namespace laya
{
	class JSRenderNode;
	class JSRenderState
	{
	public:
		static void exportJS(Context& context);
		JSRenderState();
		~JSRenderState();
		float getCull();
		void setCull(float value);
		int getBlend();
		void setBlend(int value);
		int getSrcBlend();
		void setSrcBlend(int value);
		int getDstBlend();
		void setDstBlend(int value);
		int getSrcBlendRGB();
		void setSrcBlendRGB(int value);
		int getDstBlendRGB();
		void setDstBlendRGB(int value);
		int getSrcBlendAlpha();
		void setSrcBlendAlpha(int value);
		int getDstBlendAlpha();
		void setDstBlendAlpha(int value);
		int getBlendEquation();
		void setBlendEquation(int value);
		int getBlendEquationRGB();
		void setBlendEquationRGB(int value);
		int getBlendEquationAlpha();
		void setBlendEquationAlpha(int value);
		int getDepthTest();
		void setDepthTest(int value);
		bool getDepthWrite();
		void setDepthWrite(bool value);
		bool getStencilWrite();
		void setStencilWrite(bool value);
		int getStencilTest();
		void setStencilTest(int value);
		int getStencilRef();
		void setStencilRef(int value);
		//void setBlendConstColor(float r, float g, float b, float a);
		void setStencilOp(float x, float y, float z);
        void setNull();
	public:
		RenderState m_renderState;

		float		cull;
		int			blend;
		int			srcBlend;
		int			dstBlend;
		int			srcBlendRGB;
		int		    dstBlendRGB;
		int			srcBlendAlpha;
		int			dstBlendAlpha;
		//Vector4		blendConstColor;
		int			blendEquation;
		int			blendEquationRGB;
		int			blendEquationAlpha;
		int			depthTest;
		bool		depthWrite;
		bool		stencilWrite;
		int			stencilTest;
		int			stencilRef;
		//Vector3		stencilOp;
		int			stencilOpX;
		int			stencilOpY;
		int			stencilOpZ;
	};
}
#endif //__JSRenderState_H__

#ifndef __SubmitTarget_H__
#define __SubmitTarget_H__

#include <stack>
#include "BlendMode.h"
#include "SubmitBase.h"
#include "Matrix.h"

namespace laya
{
	class Mesh2D;
	class RenderTexture2D;
	class Context2D;
	class WebGLInternalTex;
	class SubmitTarget : public SubmitBase
	{	
	public:
		static SubmitTarget* create(Mesh2D* mesh, BlendMode blendMode, RenderTexture2D* target, Context2D* ctx);
		static SubmitTarget* create(Mesh2D* mesh, BlendMode blendMode, WebGLInternalTex* pTexture, GLESEngine* pGLESEngine, bool gamma);
        static void onDestroy();
		SubmitTarget(GLESEngine* pWebglEngine);
		int renderSubmit() override;
		void recycle() override;
		float u_TexRange[4];
	private:
		friend class JSContext2D;
		static std::stack<SubmitTarget*>	POOL;
        int                                 m_textureID;
		float								m_colorAlpha[4];
		float								m_colorMat[16];
		float								m_blurinfo[2];
		float								m_strength_sig2_2sig2_gauss1[4];
		bool								m_bGamma = true;
	};
}
#endif //__SubmitTarget_H__
